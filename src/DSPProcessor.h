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


//! Example header file for a processing class.
/*!
 * \file DSPProcessor.h
 * ToDo: detailed description!
 */
#include <string>
#include "template/include/IADSPProcessor.h"

//!	In this class you can define your processing modes.
/*! 
 * All processing modes can be enabled or disabled in the templateConfiguration.h.
 */
class CDSPProcessor : public IADSPProcessor
{
public:
	//! Here for example you can create global buffers of your Addon
	CDSPProcessor();

	//! Here for example you can delete global buffers from your Addon
	~CDSPProcessor();

	//!	Can be used to have unchanged stream.
	/*!
	 * @param Array_in Pointer to data memory
	 * @param Samples Amount of samples inside Array_in
	 * @return true if work was OK
	 * @remarks All ADSP add-ons allowed to-do this. This Mode is optional and can be enabled if you define ADSP_ADDON_USE_INPUTPROCESS in templateConfiguration.h.
	 */
	virtual bool InputProcess(const float **Array_in, unsigned int Samples);

	//! This mode is processed before a mastermode and can be used to have a high quality resample.
	/*!
	 * @param id The stream id
	 * @param Array_in Pointer to input data memory
	 * @param Array_out Pointer to output data memory
	 * @param Samples Amount of samples inside Array_in
	 * @return Amount of samples processed
	 * @remarks Only one ADSP add-on is allowed to-do this! This Mode is optional and can be enabled if you define ADSP_ADDON_USE_INPUTRESAMPLE in templateConfiguration.h.
	 */
	virtual unsigned int InputResampleProcess(float **Array_in, float **Array_out, unsigned int Samples);

	//! ADSP preprocessing.
	/*!
	 * @param Mode_id The mode inside add-on which must be performed on call. 
	 * Id is set from add-on and can be modified templateConfiguration.cpp by editing adspPreModeNum array.
	 * Best practise is to use a global enum to have only a mode id once. For example see adspProcessingModeIDs in templateConfiguration.h
	 * @param Array_in Pointer to input data memory
	 * @param Array_out Pointer to output data memory
	 * @param Samples Amount of samples inside Array_in
	 * @return Amount of samples processed
	 * @remarks All ADSP add-ons allowed to-do this. This Mode is optional and can be enabled if you define ADSP_ADDON_USE_PREPROCESSING in templateConfiguration.h.
	 */
	virtual unsigned int PreProcess(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples);

	//! Master processing becomes performed with it Here a channel up-mix/down-mix 
	//! for stereo surround sound can be performed. Only one master mode can be selected
	//! in listening mode.
	/*!
	 * @param Array_in Pointer to input data memory
	 * @param Array_out Pointer to output data memory
	 * @param Samples Amount of samples inside Array_in
	 * @return Amount of samples processed
	 * @remarks Optional. Is set by AE_DSP_ADDON_CAPABILITIES and asked with GetAddonCapabilities
	 */
	virtual unsigned int MasterProcess(float **Array_in, float **Array_out, unsigned int Samples);

	/*!
	 * @brief DSP post processing
	 * On the post processing can be things performed with additional channel upmix like 6.1 to 7.1
	 * or frequency/volume corrections, speaker distance handling, equalizer... .
	 * All DSP add-ons allowed to-do this.
	 * @param mode_id The mode inside add-on which must be performed on call. Id is set from add-on by iModeNumber on AE_DSP_MODE structure during RegisterMode callback,
	 * and can be defined from add-on as a structure pointer or anything else what is needed to find it.
	 * @param array_in Pointer to input data memory
	 * @param array_out Pointer to output data memory
	 * @param samples Amount of samples inside array_in
	 * @return Amount of samples processed
	 * @remarks Optional. Is set by AE_DSP_ADDON_CAPABILITIES and asked with GetAddonCapabilities
	 */
	virtual unsigned int PostProcess(unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples);

	/*!
	 * @brief Re-sampling after master processing becomes performed with it if needed, only
	 * one add-on can perform it.
	 * @param Array_in Pointer to input data memory
	 * @param Array_out Pointer to output data memory
	 * @param Samples Amount of samples inside Array_in
	 * @return Amount of samples processed
	 * @remarks Optional. Is set by AE_DSP_ADDON_CAPABILITIES and asked with GetAddonCapabilities
	 */
	virtual unsigned int OutputResampleProcess(float **Array_in, float **Array_out, unsigned int Samples);

	/*! 
	 * @brief	Overloading the function and set output channels flags to the same value as input flags.
	 *			So it will be possible to copy all channels with its own audio samples to XBMC's dsp system.
	 * @retval	Out_channel_present_flags the exact channel present flags after performed up-/downmix 
	 *			for availible flags see AE_DSP_CHANNEL_PRESENT in xbmc_adsp_types.h
	 * @return	The amount channels, when no up- or downmix should be performed always return -1. 
	 *			This method returns always -1 by default.
	 * @remarks Optional. Must be used and set if a channel up- or downmix is processed from the active master mode
	 */
	virtual int MasterProcessGetOutChannels(unsigned long &Out_channel_present_flags);
};