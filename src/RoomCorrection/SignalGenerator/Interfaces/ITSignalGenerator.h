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
  ISignalGenerator()
  {
    m_MaxBufferedSamples = 0;
    m_MaxSampleByteSize = 0;
    m_SampleFrequency = 0;
    m_Samples = NULL;
  }

  ISignalGenerator(ulong MaxBufferedSamples, size_t MaxSampleByteSize, ulong SampleFrequency)
  {
    if(!Create(MaxBufferedSamples, MaxSampleByteSize, SampleFrequency))
    {
      // ToDo: throw error!
    }
  }

  virtual ~ISignalGenerator()
  {
    Destroy();
  }

  bool Create(ulong MaxBufferedSamples, size_t MaxSampleByteSize, ulong SampleFrequency)
  {
    if(!MaxBufferedSamples || MaxSampleByteSize <= 0 || !SampleFrequency)
    {
      return false;
    }

    if(MaxBufferedSamples > m_MaxBufferedSamples && m_Samples)
    {
      delete [] m_Samples;
      m_Samples = NULL;
    }

    m_MaxBufferedSamples = MaxBufferedSamples;
    m_MaxSampleByteSize = MaxSampleByteSize;
    m_SampleFrequency = SampleFrequency;
    m_Samples = new uchar[m_MaxBufferedSamples*m_MaxSampleByteSize];
    if(!m_Samples)
    {
      return false;
    }

    return true;
  }

  void Destroy()
  {
    if(m_Samples)
    {
      delete[] m_Samples;
      m_Samples = NULL;
    }
  }

  ulong get_BufferedSamples()
  {
    return m_MaxBufferedSamples;
  }

  size_t get_SampleByteSize()
  {
    return m_MaxSampleByteSize;
  }

  ulong get_SampleFrequency()
  {
    return m_SampleFrequency;
  }

  virtual ulong get_Samples(void *Buffer, ulong MaxSamples, ulong Offset=0) = 0;
  virtual ulong set_Samples(void *Buffer, ulong MaxSamples, ulong Offset=0) = 0;

protected:
  uchar   *m_Samples;

private:
  ulong   m_MaxBufferedSamples;
  size_t  m_MaxSampleByteSize;
  ulong   m_SampleFrequency;
};

template<typename T>
class TSignalGenerator : public ISignalGenerator
{
public:
  TSignalGenerator()
  {
  }

  TSignalGenerator(ulong MaxBufferedSamples, ulong SampleFrequency) :
    ISignalGenerator(MaxBufferedSamples, sizeof(T), SampleFrequency)
  {
  }

  bool Create(ulong MaxBufferedSamples, ulong SampleFrequency)
  {
    return ISignalGenerator::Create(MaxBufferedSamples, sizeof(T), SampleFrequency);
  }

  ulong get_Samples(void *Buffer, ulong MaxSamples, ulong Offset)
  {
    if(Offset >= get_BufferedSamples())
    {
      // show error!
      return 0;
    }

    ulong samplesCopy = MaxSamples;
    if(Offset + samplesCopy > get_BufferedSamples())
    {
      samplesCopy = get_BufferedSamples() - Offset;
    }

    if(!samplesCopy)
    {
      return 0;
    }

    memcpy(Buffer, m_Samples + Offset, get_SampleByteSize()*samplesCopy);
    return samplesCopy;
  }

  ulong set_Samples(void *Buffer, ulong MaxSamples, ulong Offset)
  {
    if(Offset >= get_BufferedSamples() || !Buffer)
    {
      // show error!
      return 0;
    }

    ulong samplesCopy = MaxSamples;
    if(Offset + samplesCopy > get_BufferedSamples())
    {
      samplesCopy = get_BufferedSamples() - Offset;
    }

    if(samplesCopy <= 0)
    {
      return 0;
    }

    memcpy(m_Samples + Offset, Buffer, get_SampleByteSize()*samplesCopy);
    return samplesCopy;
  }

  ulong get_Data(uint Offset, T* &Data)
  {
    if(Offset >= get_BufferedSamples())
    {
      Data = NULL;
      return 0;
    }

    Data = ((T*)m_Samples) + Offset;
    return this->get_BufferedSamples() - Offset;
  }
};