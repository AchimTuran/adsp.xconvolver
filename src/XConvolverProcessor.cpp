/*
 *      Copyright (C) 2005-2014 Team KODI
 *      http://KODI.org
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
 *  along with KODI; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */


//! Example process class method implementations.
/*!
 * \file DSPProcessor.cpp
 * ToDo: detailed description!
 */
#include "XConvolverProcessor.h"
#include "channelUpmixer.h"
#include "filterManager.h"

#include "client.h"
using namespace ADDON;

LXC_OPTIMIZATION_MODULE g_LXC_optModule = LXC_OPT_NATIVE;
LXC_FFT_MODULE g_fftModule = LXC_fftModule_fftwf;

static uint l_popularSampleRates[] = {
  5512,
  8000,
  11025,
  16000,
  22050,
  32000,
  44100,
  48000,
  64000,
  88200,
  96000,
  176400,
  192000,
  384000,
  0
};

// set addon properties here
CXConvolverProcessor::CXConvolverProcessor()
{
  m_StreamFilter  = NULL;
  m_Ringbuffers   = NULL;
  m_ResultBuffers = NULL;
  m_fftPlans      = NULL;

  m_MaxBypassChannels     = 0;
  m_MaxProcessingChannels = 0;
}

// delete your buffers here
CXConvolverProcessor::~CXConvolverProcessor()
{
  if (m_StreamFilter)
  {
    if (m_Ringbuffers)
    {
      for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
      {
        m_StreamFilter->ringbufferCallbacks.LXC_Ringbuffer_destroy(&m_Ringbuffers[ii]);
      }

      delete[] m_Ringbuffers;
      m_Ringbuffers = NULL;
    }

    if (m_ResultBuffers)
    {
      for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
      {
        m_StreamFilter->bufferCallbacks.LXC_Buffer_destroy(&m_ResultBuffers[ii]);
      }

      delete[] m_ResultBuffers;
      m_ResultBuffers = NULL;
    }

    if (m_fftPlans)
    {
      for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
      {
        m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_destroy_fft(&m_fftPlans[ii]);
      }

      delete[] m_fftPlans;
      m_fftPlans = NULL;
    }
  }

  CFilterManager::DestroyStreamFilter(&m_StreamFilter);
}

bool CXConvolverProcessor::Init()
{
  if (m_StreamFilter)
  {
    CFilterManager::DestroyStreamFilter(&m_StreamFilter);
  }

  m_StreamFilter = CFilterManager::Get()->CreateStreamFilter(m_StreamSettings.iProcessSamplerate, m_StreamSettings.iProcessFrames, g_fftModule, g_LXC_optModule);
  if (!m_StreamFilter)
  {
    KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
    return false;
  }

  // count processing and bypass channels
  m_MaxProcessingChannels = 0;
  m_MaxBypassChannels = 0;
  for (uint ii = 0; ii < AE_DSP_CH_MAX; ii++)
  {
    if((m_StreamSettings.lOutChannelPresentFlags & 1 << ii) && m_StreamFilter->filters[ii].buffer)
    {
      m_ProcessingChannels[m_MaxProcessingChannels] = ii;
      m_MaxProcessingChannels++;
    }
    else if(m_StreamSettings.lOutChannelPresentFlags & 1 << ii)
    {
      m_BypassChannels[m_MaxBypassChannels] = ii;
      m_MaxBypassChannels++;
    }
  }

  // set rest of channels to AE_DSP_CH_INVALID
  for (uint ii = m_MaxBypassChannels; ii < AE_DSP_CH_MAX; ii++)
  {
    m_BypassChannels[ii] = AE_DSP_CH_INVALID;
  }

  for (uint ii = m_MaxProcessingChannels; ii < AE_DSP_CH_MAX; ii++)
  {
    m_ProcessingChannels[ii] = AE_DSP_CH_INVALID;
  }

  // create fft plans for each processing channel
  m_fftPlans = new LXC_FFT_PLAN[m_MaxProcessingChannels];
  if (!m_fftPlans)
  {
    // ToDo: throw some error message
    KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
    return false;
  }

  // create fft plans
  for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
  {
    m_fftPlans[ii].LXC_fftSize = 0;
    m_fftPlans[ii].LXC_fftZeros = 0;
    m_fftPlans[ii].LXC_maxInputFrameLength = 0;
    m_fftPlans[ii].LXC_scaleFactor = 0;
    m_fftPlans[ii].LXC_specific_fftPlan = NULL;

    LXC_ERROR_CODE err = m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_create_fft(&(m_fftPlans[ii]),
                                                                                   m_StreamFilter->fftHandle.LXC_fftPlan.LXC_fftSize,
                                                                                   m_StreamFilter->fftHandle.LXC_fftPlan.LXC_maxInputFrameLength);
    if (err != LXC_NO_ERR)
    {
      // ToDo: show some error message
      return false;
    }
  }
  // destroy unused fftPlan
  m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_destroy_fft(&(m_StreamFilter->fftHandle.LXC_fftPlan));

  // ToDo: pack input channels to use one fft/ifft

  // create input ringbuffers
  m_Ringbuffers = new LXC_RINGBUFFER[m_MaxProcessingChannels];
  if (!m_Ringbuffers)
  {
    // ToDo: throw some error message
    KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
    return false;
  }

  // create result buffers
  m_ResultBuffers = new LXC_BUFFER[m_MaxProcessingChannels];
  if (!m_ResultBuffers)
  {
    // ToDo: throw some error message
    KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
    return false;
  }

  // create input buffers
  for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
  {
    // create ringbuffer
    m_Ringbuffers[ii].buffer = NULL;
    m_Ringbuffers[ii].sampleFrequency = m_StreamFilter->filters[m_ProcessingChannels[ii]].sampleFrequency;
    m_Ringbuffers[ii].maxPartLength   = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterPartLength;
    m_Ringbuffers[ii].maxElements     = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterParts;

    LXC_ERROR_CODE err = m_StreamFilter->ringbufferCallbacks.LXC_Ringbuffer_create(&m_Ringbuffers[ii]);
    if (err != LXC_NO_ERR)
    {
      // ToDo: throw some error message
      KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
      return false;
    }

    // create result buffer
    m_ResultBuffers[ii].buffer = NULL;
    m_ResultBuffers[ii].maxFilterLength             = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterLength;
    m_ResultBuffers[ii].maxFilterPartLength         = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterPartLength;
    m_ResultBuffers[ii].maxFilterPartLength_NonZero = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterPartLength_NonZero;
    m_ResultBuffers[ii].maxFilterParts              = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterParts;
    m_ResultBuffers[ii].sampleFrequency             = m_StreamFilter->filters[m_ProcessingChannels[ii]].sampleFrequency;
    
    err = m_StreamFilter->bufferCallbacks.LXC_Buffer_create(&m_ResultBuffers[ii]);
    if (err != LXC_NO_ERR)
    {
      // ToDo: throw some error message
      KODI->Log(LOG_ERROR, "%s: Could not create stream filter! Not enough free memory?", __FUNCTION__);
      return false;
    }
  }

  return true;
}

// for future LFE Processing
bool CXConvolverProcessor::InputProcess(const float **Array_in, unsigned int Samples)
{
	return true;
}

//unsigned int CXConvolverProcessor::MasterProcess(float **Array_in, float **Array_out, unsigned int Samples)
//{
//	ADSP_CHANNEL_LAYOUT sourceChannelLayout = getChannelLayout(m_StreamSettings.lInChannelPresentFlags);
//	ADSP_CHANNEL_LAYOUT sinkChannelLayout	= getChannelLayout(m_StreamSettings.lOutChannelPresentFlags);
//
//	switch(sinkChannelLayout)
//	{
//		case channellayout_stereo:
//			from_X_to_stereo( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_2_1:
//			from_X_to_2_1( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_3_0:
//			from_X_to_3_0( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//		
//		case channellayout_quad_side:
//			from_X_to_quad_side( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_3_1:			
//			from_X_to_3_1( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_5_0:
//			from_X_to_5_0_side( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_4_1_side:
//			from_X_to_4_1_side( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_5_1_side:
//			from_X_to_5_1_side( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_7_0:
//			from_X_to_7_0( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//
//		case channellayout_7_1:
//			from_X_to_7_1( Array_in, Array_out, Samples, sourceChannelLayout );
//		break;
//	}
//
//	return Samples;
//
//  return 0;
//}

// olf bypass function
//unsigned int currentChannelFlag = AE_DSP_PRSNT_CH_FL;
//for( unsigned int ii = 0; ii < AE_DSP_CH_MAX; ii++ )
//{
//	if(m_StreamSettings.lOutChannelPresentFlags & currentChannelFlag)
//	{
//		memcpy( Array_out[ii], Array_in[ii], Samples*sizeof(float) );
//	}

//	currentChannelFlag <<= 1;
//}
unsigned int CXConvolverProcessor::PostProcess(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
  // first process bypass channels
  for (uint ii = 0; ii < m_MaxBypassChannels; ii++)
  {
    memcpy(Array_out[m_BypassChannels[ii]], Array_in[m_BypassChannels[ii]], Samples*sizeof(float));
  }


  // convolve channels with FIR-Filters
  for (uint ii = 0; ii < m_MaxProcessingChannels; ii++)
  {    
    //memcpy(Array_out[m_ProcessingChannels[ii]], Array_in[m_ProcessingChannels[ii]], Samples*sizeof(float));
    LXC_FFT_PLAN *fftPlan = &(m_fftPlans[ii]);
    //const uint timeFrameLength = fftPlan->maxInputFrameLength;
    const uint freqFrameLength = fftPlan->LXC_fftSize;

    // convert format external, fft, convert format internal
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_fmtc_external_TO_fft(&(Array_in[m_ProcessingChannels[ii]][0]), fftPlan, Samples);
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_fft(fftPlan);
    void *X = m_StreamFilter->ringbufferCallbacks.LXC_Ringbuffer_getNextPart(&(m_Ringbuffers[ii]));
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_fmtc_fft_TO_internal(fftPlan, X, freqFrameLength);

    // convolution in frequency domain is only a complex multiplication
    // convolve channel1
    uint maxParts = m_StreamFilter->filters[m_ProcessingChannels[ii]].maxFilterParts;
    for (uint jj = 0; jj < maxParts; jj++)
    {
      void* G = m_StreamFilter->bufferCallbacks.LXC_Buffer_getPart(&(m_StreamFilter->filters[m_ProcessingChannels[ii]]), jj);
      void* X = m_StreamFilter->ringbufferCallbacks.LXC_Ringbuffer_getPart(&(m_Ringbuffers[ii]), jj);
      void* Z = m_StreamFilter->bufferCallbacks.LXC_Buffer_getPart(&(m_ResultBuffers[ii]), jj);
      if (G && X && Z)
      {
        m_StreamFilter->lxcCallbacks.LXC_CpxMul(freqFrameLength, X, G, Z);
      }
    }

    // sum all samples
    m_StreamFilter->lxcCallbacks.LXC_CpxAdd(&(m_ResultBuffers[ii]), m_fftPlans[ii].LXC_scaleFactor);

    void* Z = m_StreamFilter->bufferCallbacks.LXC_Buffer_getPart(&(m_ResultBuffers[ii]), 0);
    if (!Z)
    {
      return LXC_ERR_INVALID_INPUT;
    }

    // do inverse fast fourier transform
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_fmtc_internal_TO_fft(Z, fftPlan, freqFrameLength);
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_ifft(fftPlan);
    m_StreamFilter->fftHandle.LXC_fftCallbacks.LXC_fmtc_fft_TO_external(fftPlan, &(Array_out[m_ProcessingChannels[ii]][0]), Samples);
  }

	return Samples;
}

//unsigned int CXConvolverProcessor::PostProcessNeededSamplesize(unsigned int Mode_id)
//{
//  return 4096;
//}

int CXConvolverProcessor::MasterProcessGetOutChannels(unsigned long &Out_channel_present_flags)
{
	Out_channel_present_flags = m_StreamSettings.lInChannelPresentFlags;
	return m_StreamSettings.iInChannels;
}

AE_DSP_ERROR CXConvolverProcessor::Create()
{
  return AE_DSP_ERROR_NO_ERROR;
}

AE_DSP_ERROR CXConvolverProcessor::send_Message(CADSPModeMessage &Message)
{
  return AE_DSP_ERROR_NO_ERROR;
}
