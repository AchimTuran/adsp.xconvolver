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


#include <stdio.h>
#include <kodi/kodi_adsp_types.h>
#include <LibXConvolverCore/include/LXC_Core_types.h>
#include <LibXConvolverCore/LXC_fftHandles/LXC_fftHandles_types.h>

typedef struct
{
  float *filter;
  uint maxFilterLength;
  uint sampleFrequency;
}FILTER_SAMPLES;

typedef struct
{
  LXC_BUFFER                filters[AE_DSP_CH_MAX];
  LXC_BUFFER_CALLBACKS      bufferCallbacks;
  LXC_RINGBUFFER_CALLBACKS  ringbufferCallbacks;
  LXC_CALLBACKS             lxcCallbacks;
  LXC_FFT_HANDLE            fftHandle;
  LXC_CONFIG                lxcConfig;
} STREAM_FILTER;


// singleton implementation based on 
// http://de.wikibooks.org/wiki/C%2B%2B-Programmierung:_Entwurfsmuster:_Singleton#Normal_.28Heap.29
class CFilterManager
{
public:
  static CFilterManager* Get();
  bool LoadFilters();
  STREAM_FILTER *CreateStreamFilter(uint SampleFrequency, uint MaxInputFrameLength, LXC_FFT_MODULE fftModule, LXC_OPTIMIZATION_MODULE Module = LXC_OPT_NATIVE);
  static void DestroyStreamFilter(STREAM_FILTER **StreamFilter);

private:
	CFilterManager();                         // prevent object generation
  CFilterManager(const CFilterManager&);    // prevent object generation with copy constructor
	~CFilterManager();
  
  //  AE_DSP_STREAM_MAX_STREAMS
  FILTER_SAMPLES m_Filters[AE_DSP_CH_MAX];  // array with original filter samples
  static CFilterManager* m_Instance;        // filter manager instance

  //LXC_FFT_HANDLE m_fftHandle;
	
  class CFilterManagerGuard
  {
  public:
    ~CFilterManagerGuard()
    {
      if (CFilterManager::m_Instance)
      {
        delete CFilterManager::m_Instance;
        CFilterManager::m_Instance = NULL;
      }
    }
  };
  friend class CFilterManagerGuard;
};