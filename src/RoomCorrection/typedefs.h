#pragma once
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



#include <map>
#include <string>
#include <vector>

typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;

template<typename T> class TSamplesBuffer;
typedef TSamplesBuffer<float> CSamplesBuffer;

typedef enum CAPTURE_SOURCES
{
  CAPTURE_SOURCE_INVALID = -1,
  CAPTURE_SOURCE_PORTAUDIO,
  CAPTURE_SOURCE_MAX
};

typedef std::map<std::string, CAPTURE_SOURCES> CCaptureDeviceSourceList_t;
typedef std::vector<std::string> CCaptureDeviceList_t;

typedef enum DRC_ERROR
{
  DRC_ERROR_INVALID_INPUT = -1000,
  DRC_ERROR_NO_ERR = 0
};
