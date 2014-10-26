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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <string>

//! Channel layout as an enum.
//! Standard channel layouts from http://trac.ffmpeg.org/wiki/AudioChannelManipulation
typedef enum 
{
	channellayout_mono = 0,			// FC	(supported from ActiveAE)
	channellayout_stereo,			// FL, FR	(supported from ActiveAE)
	channellayout_2_1,				// FL, FR, LFE	(supported from ActiveAE)
	channellayout_3_0,				// FL, FR, FC	(supported from ActiveAE)
	channellayout_3_0_back,			// FL, FR, BC
	channellayout_4_0,				// FL, FR, FC, BC
	channellayout_quad,				// FL, FR, BL, BR
	channellayout_quad_side,		// FL, FR, SL, SR	(supported from ActiveAE)
	channellayout_3_1,				// FL, FR, FC, LFE	(supported from ActiveAE)
	channellayout_5_0,				// FL, FR, FC, BL, BR
	channellayout_5_0_side,			// FL, FR, FC, SL, SR	(supported from ActiveAE)
	channellayout_4_1,				// FL, FR, FC, LFE, BC
	channellayout_4_1_side,			// FL, FR, LFE, SL, SR	(supported from ActiveAE)
	channellayout_5_1,				// FL, FR, FC, LFE, BL, BR
	channellayout_5_1_side,			// FL, FR, FC, LFE, SL, SR	(supported from ActiveAE)
	channellayout_6_0,				// FL, FR, FC, BC, SL, SR
	//channellayout_6_0_front,		// FL, FR, FLC, FRC, SL, SR --> not supported from ADSP Addons, it's only here for future implementations
	channellayout_hexagonal,		// FL, FR, FC, BL, BR, BC
	channellayout_6_1,				// FL, FR, FC, LFE, BC, SL, SR
	channellayout_6_1_back,			// FL, FR, FC, LFE, BL, BR, BC
	//channellayout_6_1_front,		// FL, FR, LFE, FLC, FRC, SL, SR --> not supported from ADSP Addons, it's only here for future implementations
	channellayout_7_0,				// FL, FR, FC, BL, BR, SL, SR	(supported from ActiveAE)
	//channellayout_7_0_front,		// FL, FR, FC, FLC, FRC, SL, SR
	channellayout_7_1,				// FL, FR, FC, LFE, BL, BR, SL, SR	(supported from ActiveAE)
	//channellayout_7_1_wide,		// FL, FR, FC, LFE, BL, BR, FLC, FRC
	//channellayout_7_1_wide_side,	// FL, FR, FC, LFE, FLC, FRC, SL, SR
	channellayout_octagonal,		// FL, FR, FC, BL, BR, BC, SL, SR
	// channellayout_downmix,		// DL, DR --> not supported from ADSP Addons, it's only here for future implementations
	channellayout_unknown
}ADSP_CHANNEL_LAYOUT;

//!	This function is used to get a standart channel layout as an enum. 
//! Standart channel layouts are taken from ffmpeg: http://trac.ffmpeg.org/wiki/AudioChannelManipulation
/*!
 * @param ChannelFlags Variable with AE_DSP_PRSNT_CH_X flags. For details see AE_DSP_CHANNEL_PRESENT.
 * @return Enum as channel layout. For details see ADSP_CHANNEL_LAYOUT.
 */
ADSP_CHANNEL_LAYOUT getChannelLayout(unsigned int ChannelFlags);

//!	This function is used to get a standart channel layout as an std::string. 
/*!
 * @param ChannelLayout Enum with channel layout. For details see ADSP_CHANNEL_LAYOUT.
 * @return String as channel layout. For details see ADSP_CHANNEL_LAYOUT.
 */
std::string getChannelLayoutString(ADSP_CHANNEL_LAYOUT channellayout_strings);


// functions for upmixing
void from_X_to_stereo( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_2_1( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_3_0( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_quad_side( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_3_1( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_7_0( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );
void from_X_to_7_1( float **Array_in, float **Array_out, unsigned int Samples, ADSP_CHANNEL_LAYOUT UpmixFrom );

// functions for upmixing to mono stereo (2.0)
void from_mono_to_stereo( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 2.1
void from_mono_to_2_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_2_1( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 3.0
void from_mono_to_3_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_3_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_3_0( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to quad_side
void from_mono_to_quad_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_quad_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_quad_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_quad_side( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 3_1
void from_mono_to_3_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_3_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_3_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_3_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_3_1( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 5_0_side
void from_mono_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_1_to_5_0_side( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 4_1_side
void from_mono_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_1_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_0_side_to_4_1_side( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 5_1_side
void from_mono_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_1_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_0_side_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );
void from_4_1_side_to_5_1_side( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 7_0_side
void from_mono_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_1_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_0_side_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_4_1_side_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_1_to_7_0( float **Array_in, float **Array_out, unsigned int Samples );

// functions for upmixing to 7_1
void from_mono_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_stereo_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_2_1_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_0_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_quad_side_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_3_1_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_0_side_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_4_1_side_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_5_1_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );
void from_7_0_to_7_1( float **Array_in, float **Array_out, unsigned int Samples );