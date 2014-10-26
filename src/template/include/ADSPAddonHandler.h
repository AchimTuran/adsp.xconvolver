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

#include <string>
#include "xbmc_adsp_types.h"

#include "../configuration/templateConfiguration.h"
#include "ADSPProcessorHandle.h"

#include ADSP_PROCESSOR_HEADER_FILE

#ifdef ADSP_ADDON_OPTIONAL_CLASS_NAME
#include ADSP_ADDON_OPTIONAL_HEADER_FILE
#endif

extern std::string g_strAddonPath;

class CADSPAddonHandler
//#if USE_ADDONOPTIONAL
#ifdef ADSP_ADDON_OPTIONAL_CLASS_NAME
	: public ADSP_ADDON_OPTIONAL_CLASS_NAME
#endif
{
public:
	CADSPAddonHandler();			//! Constructor: here you can define global settings of the Addon
	virtual ~CADSPAddonHandler();	//! Destructor: here you can delete all buffers from the Addon

	/*!
	 * Control function for start and stop of dsp processing.
	 */
	AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *Settings, const AE_DSP_STREAM_PROPERTIES *pProperties);
	AE_DSP_ERROR StreamDestroy(unsigned int Id);
	AE_DSP_ERROR StreamInitialize(const AE_DSP_SETTINGS *Settings);

	/*!
	 * initialize or destroy methods for the AddonHandler
	 */
	void Destroy();
	bool Init();

	/*!
	 * Supported processing methods
	 */
	bool SupportsInputProcess();
	bool SupportsPreProcess();
	bool SupportsMasterProcess();
	bool SupportsPostProcess();
	bool SupportsInputResample();
	bool SupportsOutputResample();

	/*!
	 * Get Stream
	*/
	CADSPProcessorHandle *GetStream(AE_DSP_STREAM_ID Id);

private:
	//AE_DSP_SETTINGS           m_Settings;           /*!< @brief (required) the active XBMC audio settings */
	//AE_DSP_STREAM_PROPERTIES  m_Properties;
	//int                       m_iStreamType;
	//int                       m_iBaseType;
	//std::string               m_strName;            /*!< @brief (required) the audio stream name */
	//std::string               m_strCodecId;         /*!< @brief (required) codec id string of the audio stream */
	//std::string               m_strLanguage;        /*!< @brief (required) language id of the audio stream */
	//int                       m_iIdentifier;        /*!< @brief (required) audio stream id inside player */
	//int                       m_iChannels;          /*!< @brief (required) amount of basic channels */
	//int                       m_iSampleRate;        /*!< @brief (required) input sample rate */

	/*!
	 * Pointer array for active dsp processing classes, for this reason the
	 * stream id is never more as AE_DSP_STREAM_MAX_STREAMS and can be used as pointer to this array.
	 */
	CADSPProcessorHandle *m_ADSPProcessor[AE_DSP_STREAM_MAX_STREAMS];
};