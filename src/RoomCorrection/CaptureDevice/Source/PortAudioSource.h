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

class PortAudioSource : public ICaptureSource, public asplib::IPortAudio
{
public:
  PortAudioSource();
  ~PortAudioSource();

  virtual bool Create(uint SampleFrequency, uint FrameSize);
  virtual int  get_Devices(CCaptureDeviceList_t &DeviceList);
  virtual void Destroy() = 0;

  virtual bool StartCapturing();
  virtual bool StopCapturing();
  virtual bool PauseCapturing();
  virtual bool IsCapturing();

  virtual ulong GetStoredSamples();
  virtual ulong GetSamples(float *Samples, ulong MaxSamples, ulong Offset = 0);

protected:
  virtual int AudioCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);
};