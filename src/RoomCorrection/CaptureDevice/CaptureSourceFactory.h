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

class ICaptureSource;

class CCaptureSourceFactory
{
public:
  static bool EnumerateSources(CCaptureDeviceList_t &AvailableSources);
  static bool EnumerateSourceDevices(CAPTURE_SOURCES SourceID, CCaptureDeviceList_t &AvailableDevices);

  static ICaptureSource* GetCaptureDevice(CAPTURE_SOURCES SourceID, std::string DeviceStr);
  static ICaptureSource* GetCaptureDevice(CAPTURE_SOURCES SourceID, int DeviceID);
  static void DestroyCaptureDevice(ICaptureSource **CaptureDevice);
};