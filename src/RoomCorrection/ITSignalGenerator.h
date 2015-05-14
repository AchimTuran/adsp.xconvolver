#pragma once

/*
 *      Copyright (C) 2014-2015 Team KODI
 *      http://kodi.tv
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */



#include <memory.h>
#include <stdio.h>

// typedefs
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

class ISignalGenerator
{
public:
  ISignalGenerator(ulong BufferedSamples, size_t SampleByteSize, ulong SampleFrequency)
  {
    if(!BufferedSamples || SampleByteSize <= 0 || !SampleFrequency)
    {
      // ToDo: throw error!
    }

    m_BufferedSamples = BufferedSamples;
    m_SampleByteSize = SampleByteSize;
    m_SampleFrequency = SampleFrequency;
    m_Samples = new uchar[m_BufferedSamples*m_SampleByteSize];
    if(!m_Samples)
    {
      // ToDo: throw error
    }
  }

  virtual ~ISignalGenerator()
  {
    if(m_Samples)
    {
      delete [] m_Samples;
      m_Samples = NULL;
    }
  }

  ulong get_BufferedSamples()
  {
    return m_BufferedSamples;
  }

  virtual ulong get_Samples(void *Buffer, ulong Samples, ulong Offset=0) = 0;

protected:
  ulong   m_BufferedSamples;
  size_t  m_SampleByteSize;
  ulong   m_SampleFrequency;
  uchar   *m_Samples;
};

template<typename T>
class TSignalGenerator : public ISignalGenerator
{
public:
  TSignalGenerator(ulong BufferedSamples, ulong SampleFrequency) :
    ISignalGenerator(BufferedSamples, sizeof(T), SampleFrequency)
  {
  }
};