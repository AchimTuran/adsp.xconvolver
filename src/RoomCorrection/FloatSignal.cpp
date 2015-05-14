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
#include "FloatSignal.h"

CFloatSignal::CFloatSignal(float *Samples, ulong SamplesAmount, ulong SampleFrequency) :
  TSignalGenerator<float>(SamplesAmount, SampleFrequency)
{
  if(!Samples)
  {
    // ToDo: throw error!
  }

  memcpy(m_Samples, Samples, m_SampleByteSize*SamplesAmount);
}

ulong CFloatSignal::get_Samples(void *Buffer, ulong Samples, ulong Offset)
{
  if(Offset >= m_BufferedSamples)
  {
    // show error!
    return 0;
  }

  ulong samplesCopy = Samples;

  if(Offset + samplesCopy > m_BufferedSamples)
  {  
    samplesCopy = m_BufferedSamples - Offset;
  }

  if(samplesCopy)
  {
    memcpy(Buffer, m_Samples + Offset, m_SampleByteSize*samplesCopy);
  }

  return samplesCopy;
}