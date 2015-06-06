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
#include <asplib/asplib_utils/strings/stdStringUtils.h>

using namespace asplib;
using namespace ADDON;
using namespace std;
using namespace PLATFORM;

#define DEFAULT_SOURCE_NAME_STR string("PortAudio")
#define DEFAULT_SEPERATOR_STR   string("::")


PortAudioSource::PortAudioSource()
{
  m_State = DEVICE_STOPPED;
}

PortAudioSource::PortAudioSource(asplib::CPaHostAPIVector_t &UsedHostAPIs) :
  IPortAudio(UsedHostAPIs)
{
  m_State = DEVICE_STOPPED;
}

PortAudioSource::~PortAudioSource()
{
  if(m_State != DEVICE_STOPPED)
  {
    StopCapturing();
  }
}

bool PortAudioSource::Create(unsigned int SampleFrequency, unsigned int FrameSize, string DeviceName)
{
  vector<string> tokens;
  strTokenizer(DeviceName, DEFAULT_SEPERATOR_STR, tokens);

  PaDeviceIndex deviceIdx = paNoDevice;
  if(tokens.size() == 3)
  {
    CPaDeviceInfoVector_t PaDevices;
    IPortAudio::get_AvailableDevices(PaDevices);
    for(unsigned int ii = 0; ii < PaDevices.size() && deviceIdx == paNoDevice; ii++)
    {
      if(PaDevices[ii].deviceName == tokens[2])
      {
        deviceIdx = PaDevices[ii].paDeviceIdx;
      }
    }
  }

  if(deviceIdx == paNoDevice)
  {
    KODI->Log(LOG_NOTICE, "Couldn't find capture device \"%s\" using default capture device", DeviceName.c_str());
  }

  // configure device
  long frameSize = -1;
  if(FrameSize > 0)
  {
    frameSize = FrameSize;
  }
  PaError paErr = IPortAudio::configure_Device(2, 0, SampleFrequency, deviceIdx, -1, paFloat32|paNonInterleaved, frameSize);
  if(paErr != paNoError)
  {
    KODI->Log(LOG_ERROR, "Couldn't configure PortAudio device PortAudio Error: %s", get_PortAudioErrStr(paErr).c_str());
    return false;
  }

  // create device
  paErr = IPortAudio::create_Device();
  if(paErr != paNoError)
  {
    KODI->Log(LOG_ERROR, "Couldn't create PortAudio device PortAudio Error: %s", get_PortAudioErrStr(paErr).c_str());
    return false;
  }

  // create buffer
  m_RingBuffer = new TRingBuffer<float>(get_InputFrameSize()*4);
  
  KODI->Log(LOG_DEBUG, "Successful configured and created PortAudio Device: %s", m_InputDeviceInfo.deviceName.c_str());
  KODI->Log(LOG_DEBUG, "  frameSize: %i",       IPortAudio::get_InputFrameSize());
  KODI->Log(LOG_DEBUG, "  channel amount: %i",  IPortAudio::get_InputChannelAmount());
  KODI->Log(LOG_DEBUG, "  sampleFrequency: %f", IPortAudio::get_InputSampleFrequency());
  return true;
}

int PortAudioSource::Get_Devices(CCaptureDeviceList_t &DeviceList)
{
  vector<uint> sampleFrequencies;
  return Get_Devices(DeviceList, sampleFrequencies);
}

int PortAudioSource::Get_Devices(CCaptureDeviceList_t &DeviceList, vector<uint> &SampleFrequencies)
{
  DeviceList.clear();

  CPaDeviceInfoVector_t PaDevices;
  IPortAudio::get_AvailableDevices(PaDevices);

  for(unsigned int ii = 0; ii < PaDevices.size(); ii++)
  {
    if(PaDevices[ii].deviceInfo->maxInputChannels > 0)
    {
      string PaHostAPIStr = Pa_GetHostApiInfo(PaDevices[ii].deviceInfo->hostApi)->name;
      CaptureDevice_t device;
      device.name = DEFAULT_SOURCE_NAME_STR + DEFAULT_SEPERATOR_STR + PaHostAPIStr + DEFAULT_SEPERATOR_STR + PaDevices[ii].deviceName;

      PaStreamParameters inParam;
      inParam.channelCount = PaDevices[ii].deviceInfo->maxInputChannels;
      inParam.device = PaDevices[ii].paDeviceIdx;
      inParam.sampleFormat = paFloat32 | paNonInterleaved;
      inParam.suggestedLatency = PaDevices[ii].deviceInfo->defaultLowInputLatency;
      inParam.hostApiSpecificStreamInfo = NULL;

      for(uint jj = 0; jj < SampleFrequencies.size(); jj++)
      {
        if(Pa_IsFormatSupported(&inParam, NULL, SampleFrequencies[jj]) == paNoError)
        {
          device.sampleFrequencies.push_back(SampleFrequencies[jj]);
        }
      }

      if(device.sampleFrequencies.size() <= 0)
      {
        device.sampleFrequencies.push_back((uint)PaDevices[ii].deviceInfo->defaultSampleRate);
      }

      DeviceList.push_back(device);
    }
  }

  return DeviceList.size();
}

void PortAudioSource::Destroy()
{
  if(m_State != DEVICE_STOPPED)
  {
    StopCapturing();
  }
}

bool PortAudioSource::StartCapturing()
{
  if(m_State == DEVICE_CAPTURING)
  {
    StopCapturing();
  }

  if(m_State == DEVICE_PAUSED)
  {
    CLockObject singleLock(m_Mutex);
    m_State = DEVICE_CAPTURING;
    return true;
  }


  PaError paErr = IPortAudio::start_Device();
  if(paErr != paNoError)
  {
    KODI->Log(LOG_ERROR, "Couldn't start captureDevice! PortAudio Error: %s.", IPortAudio::get_PortAudioErrStr(paErr).c_str());
    return false;
  }

  {
    CLockObject singleLock(m_Mutex);
    m_State = DEVICE_CAPTURING;
  }

  KODI->Log(LOG_DEBUG, "Started PortAudio captureDevice: %s.", IPortAudio::m_InputDeviceInfo.deviceName.c_str());
  return true;
}

bool PortAudioSource::StopCapturing()
{
  if(m_State == DEVICE_STOPPED)
  {
    return false;
  }

  PaError paErr = IPortAudio::stop_Device();
  if(paErr != paNoError)
  {
    KODI->Log(LOG_ERROR, "During stopping capture device a PortAudio Error occurred: %s.", IPortAudio::get_PortAudioErrStr(paErr).c_str());
    return false;
  }

  {
    CLockObject singleLock(m_Mutex);
    m_State = DEVICE_STOPPED;
  }

  KODI->Log(LOG_DEBUG, "Stopped PortAudio captureDevice: %s.", IPortAudio::m_InputDeviceInfo.deviceName.c_str());
  return true;
}

bool PortAudioSource::PauseCapturing()
{
  if(m_State == DEVICE_CAPTURING)
  {
    CLockObject singleLock(m_Mutex);
    m_State = DEVICE_PAUSED;
  }
  else
  {
    return false;
  }

  return true;
}

bool PortAudioSource::IsCapturing()
{
  CLockObject singleLock(m_Mutex);
  return m_State == DEVICE_CAPTURING;
}

ulong PortAudioSource::GetStoredSamples()
{
  return 0;
}

ulong PortAudioSource::GetSamples(float *Samples, ulong MaxSamples, ulong Offset)
{
  return 0;
}

int PortAudioSource::AudioCallback(const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData)
{
  if(m_State == DEVICE_CAPTURING)
  {
    float **captureInput = (float**)inputBuffer;
    if(m_RingBuffer->get_FreeSamples() < framesPerBuffer)
    {
      KODI->Log(LOG_ERROR, "Capture device buffer overflow! Aborting capturing.");
      m_State = DEVICE_STOPPED;
      return paAbort;
    }
    
    m_RingBuffer->write(captureInput[0], framesPerBuffer);
  }
  else if(m_State == DEVICE_STOPPED)
  {
    return paAbort;
  }

  return paContinue;
}
