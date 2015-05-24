#pragma once
/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */


#include <new>
#include <RoomCorrection/typedefs.h>
#include <template/include/typedefs.h>

template<typename T>
class TSamplesBuffer
{
public:
  TSamplesBuffer()
  {
    m_Buffer = NULL;
    m_MaxSamples = 0;
  }
  ~TSamplesBuffer();

  T operator [] (ulong Idx)
  {
    if(Idx >= m_MaxSamples)
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Invalid index! Out of range!");
    }

    return static_cast<T*>(m_Buffer)[Idx];
  }

  void Destroy()
  {
    if(m_Buffer)
    {
      delete [] m_Buffer;
      m_Buffer = NULL;
    }
  }

  void Create(ulong MaxSamples)
  {
    if(!MaxSamples)
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Invalid input size!");
    }

    if(MaxSamples != m_MaxSamples)
    {
      Destroy();

      m_MaxSamples = MaxSamples;
      m_Buffer = new T[m_MaxSamples];
    }

    memset(m_Buffer, 0, sizeof(T)*m_MaxSamples);
  }

private:
  uchar *m_Buffer;
  ulong m_MaxSamples;
};
