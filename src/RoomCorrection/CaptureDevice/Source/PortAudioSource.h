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



#include "RoomCorrection/CaptureDevice/Interfaces/ICaptureSource.h"
#include "asplib_utils/audioInterfaces/IPortAudio.h"
#include "asplib_utils/buffers/TRingBuffer.h"
#include "kodi/threads/mutex.h"
#include <string>
#include <vector>

class PortAudioSource : public ICaptureSource, public asplib::IPortAudio
{
public:
  PortAudioSource();
  PortAudioSource(asplib::CPaHostAPIVector_t &UsedHostAPIs);
  ~PortAudioSource();

  virtual bool Create(unsigned int SampleFrequency, unsigned int FrameSize, std::string DeviceName="");
  int  Get_Devices(CCaptureDeviceList_t &DeviceList);
  virtual int  Get_Devices(CCaptureDeviceList_t &DeviceList, std::vector<uint> &SampleFrequencies);
  virtual void Destroy();

  virtual bool StartCapturing();
  virtual bool StopCapturing();
  virtual bool PauseCapturing();
  virtual bool IsCapturing();

  virtual ulong GetStoredSamples();
  virtual ulong GetSamples(float *Samples, ulong MaxSamples, ulong Offset = 0);

private:
  virtual int AudioCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

  typedef enum
  {
    DEVICE_STOPPED = 0,
    DEVICE_PAUSED,
    DEVICE_CAPTURING
  }DeviceStates_t;

  PLATFORM::CMutex m_Mutex;
  volatile DeviceStates_t m_State;
  asplib::TRingBuffer<float> *m_RingBuffer;
};
