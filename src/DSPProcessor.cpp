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


//! Example process class method implementations.
/*!
 * \file DSPProcessor.cpp
 * ToDo: detailed description!
 */
#include "DSPProcessor.h"
#include "channelUpmixer.h"

// set addon properties here
CDSPProcessor::CDSPProcessor()
{
}

// delete your buffers here
CDSPProcessor::~CDSPProcessor()
{
}

bool CDSPProcessor::InputProcess(const float **Array_in, unsigned int Samples)
{
	return true;
}

unsigned int CDSPProcessor::InputResampleProcess(float **Array_in, float **Array_out, unsigned int Samples)
{
	unsigned int currentChannelFlag = AE_DSP_PRSNT_CH_FL;
	for( unsigned int ii = 0; ii < AE_DSP_CH_MAX; ii++ )
	{
		if(m_StreamSettings.lInChannelPresentFlags & currentChannelFlag)
		{
			memcpy( Array_out[ii], Array_in[ii], Samples*sizeof(float) );
		}

		currentChannelFlag <<= 1;
	}

	return Samples;
}

unsigned int CDSPProcessor::PreProcess(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
	unsigned int currentChannelFlag = AE_DSP_PRSNT_CH_FL;
	for( unsigned int ii = 0; ii < AE_DSP_CH_MAX; ii++ )
	{
		if(m_StreamSettings.lInChannelPresentFlags & currentChannelFlag)
		{
			memcpy( Array_out[ii], Array_in[ii], Samples*sizeof(float) );
		}

		currentChannelFlag <<= 1;
	}

	return Samples;
}
	
unsigned int CDSPProcessor::MasterProcess(float **Array_in, float **Array_out, unsigned int Samples)
{
	ADSP_CHANNEL_LAYOUT sourceChannelLayout = getChannelLayout(m_StreamSettings.lInChannelPresentFlags);
	ADSP_CHANNEL_LAYOUT sinkChannelLayout	= getChannelLayout(m_StreamSettings.lOutChannelPresentFlags);

	switch(sinkChannelLayout)
	{
		case channellayout_stereo:
			from_X_to_stereo( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_2_1:
			from_X_to_2_1( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_3_0:
			from_X_to_3_0( Array_in, Array_out, Samples, sourceChannelLayout );
		break;
		
		case channellayout_quad_side:
			from_X_to_quad_side( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_3_1:			
			from_X_to_3_1( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_5_0:
			from_X_to_5_0_side( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_4_1_side:
			from_X_to_4_1_side( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_5_1_side:
			from_X_to_5_1_side( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_7_0:
			from_X_to_7_0( Array_in, Array_out, Samples, sourceChannelLayout );
		break;

		case channellayout_7_1:
			from_X_to_7_1( Array_in, Array_out, Samples, sourceChannelLayout );
		break;
	}

	return Samples;
}
	
unsigned int CDSPProcessor::PostProcess(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
	unsigned int currentChannelFlag = AE_DSP_PRSNT_CH_FL;
	for( unsigned int ii = 0; ii < AE_DSP_CH_MAX; ii++ )
	{
		if(m_StreamSettings.lOutChannelPresentFlags & currentChannelFlag)
		{
			memcpy( Array_out[ii], Array_in[ii], Samples*sizeof(float) );
		}

		currentChannelFlag <<= 1;
	}
	return Samples;
}

unsigned int CDSPProcessor::OutputResampleProcess(float **Array_in, float **Array_out, unsigned int Samples)
{
	unsigned int currentChannelFlag = AE_DSP_PRSNT_CH_FL;
	for( unsigned int ii = 0; ii < AE_DSP_CH_MAX; ii++ )
	{
		if(m_StreamSettings.lOutChannelPresentFlags & currentChannelFlag)
		{
			memcpy( Array_out[ii], Array_in[ii], Samples*sizeof(float) );
		}

		currentChannelFlag <<= 1;
	}
	return Samples;
}

int CDSPProcessor::MasterProcessGetOutChannels(unsigned long &Out_channel_present_flags)
{
	Out_channel_present_flags = m_StreamSettings.lInChannelPresentFlags;
	return m_StreamSettings.iInChannels;
}