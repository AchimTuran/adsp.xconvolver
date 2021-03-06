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



#include "RoomCorrection/typedefs.h"

class ICaptureSource
{
public:
  virtual bool Create(uint SampleFrequency, uint FrameSize=0, uint MaxCaptureChannels=1, std::string DeviceName="") = 0;
  virtual void Destroy() = 0;

  virtual bool StartCapturing() = 0;
  virtual bool StopCapturing() = 0;
  virtual bool PauseCapturing() = 0;
  virtual bool IsCapturing() = 0;
  
  virtual int   Get_Devices(CCaptureDeviceList_t &DeviceList) = 0;
  virtual ulong GetStoredSamples() = 0;
  virtual ulong GetSamples(float *Samples, ulong MaxSamples, ulong Offset = 0) = 0;
};