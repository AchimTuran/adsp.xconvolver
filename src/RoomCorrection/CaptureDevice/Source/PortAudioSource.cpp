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



#include "PortAudioSource.h"

#include "template/include/ADSPAddonMain.h"

using namespace asplib;
using namespace ADDON;

#define DEFAULT_SOURCE_NAME_STR "PortAudio"


PortAudioSource::PortAudioSource()
{
}

PortAudioSource::~PortAudioSource()
{
}

bool PortAudioSource::Create(uint SampleFrequency, uint FrameSize)
{
  PaError paErr = IPortAudio::create_Device();
  if(paErr != paNoError)
  {
    KODI->Log(LOG_ERROR, "Couldn't create PortAudio device PortAudio Error: %s", get_PortAudioErrStr(paErr).c_str());
    return false;
  }
  
  KODI->Log(LOG_DEBUG, "Successful created PortAudio Device: %s", m_InputDeviceInfo.deviceName.c_str());
  return true;
}

int PortAudioSource::get_Devices(CCaptureDeviceList_t &DeviceList)
{
  return 0;
}

void PortAudioSource::Destroy()
{
}

bool PortAudioSource::StartCapturing()
{
  return true;
}

bool PortAudioSource::StopCapturing()
{
  return true;
}

bool PortAudioSource::PauseCapturing()
{
  return true;
}

bool PortAudioSource::IsCapturing()
{
  return true;
}

ulong PortAudioSource::GetStoredSamples()
{
  return 0;
}

ulong PortAudioSource::GetSamples(float *Samples, ulong MaxSamples, ulong Offset = 0)
{
  return 0;
}

int PortAudioSource::AudioCallback(const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData)
{
  return paContinue;
}