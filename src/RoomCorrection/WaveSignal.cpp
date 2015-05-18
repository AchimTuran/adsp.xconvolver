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



#include <stdio.h>
using namespace std;

#include "WaveSignal.h"

#include <LibXConvolverUtils/LXC_Signal/LXC_WavLoader.h>

CWaveSignal::CWaveSignal(string File)
{
  if(File.empty() || File == "")
  {
    // ToDo: throw error!
  }
  LXC_WavStruct waveFile;
  LXC_CWavLoader::openWavFile(File, &waveFile);

  if(waveFile.maxChannels > 1)
  {
    LXC_CWavLoader::reorderChannels(&waveFile);
  }

  if(!m_Signal.Create(waveFile.maxSamples, waveFile.sampleFrequency))
  {
    // ToDo: throw error
  }
 
  if(!m_Signal.set_Samples(waveFile.samples, waveFile.maxSamples, waveFile.sampleFrequency))
  {
    // ToDo: throw error
  }
}

CWaveSignal::~CWaveSignal()
{
}

ulong CWaveSignal::get_Samples(void *Buffer, ulong Samples, ulong Offset)
{
  return m_Signal.get_Samples(Buffer, Samples, Offset);
}