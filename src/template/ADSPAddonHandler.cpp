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

#include "include/client.h"

#include "include/ADSPAddonHandler.h"
#include "configuration/templateConfiguration.h"
#include "template/include/MACROHelper.h"

using namespace std;
using namespace ADDON;

extern std::string adspImageUserPath;


//Helper function prototypes
string GetSettingsFile();

CADSPAddonHandler::CADSPAddonHandler()
{
	for( unsigned int ii = 0; ii < AE_DSP_STREAM_MAX_STREAMS; ii++ )
	{
		m_ADSPProcessor[ii] = NULL;
	}
}

CADSPAddonHandler::~CADSPAddonHandler()
{
	for( unsigned int ii = 0; ii < AE_DSP_STREAM_MAX_STREAMS; ii++ )
	{
		if(m_ADSPProcessor[ii])
		{
			delete m_ADSPProcessor[ii];
		}
	}
}


bool CADSPAddonHandler::Init()
{
	AE_DSP_MODES::AE_DSP_MODE modeSettings;
	string imagePath = g_strUserPath + "\\" + adspImageUserPath + "\\";
	string temp;

#ifdef ADSP_ADDON_USE_INPUTRESAMPLE
	//for(unsigned int ii = 0; ii < ADSP_MAX_INRES_MODES; ii++)
	//{
		modeSettings.iUniqueDBModeId = adspInResampleUniqueDdId;
		modeSettings.iModeType = AE_DSP_MODE_TYPE_INPUT_RESAMPLE;
		strcpy(modeSettings.strModeName, adspInResampleStrNames);

		modeSettings.iModeNumber = adspInResampleModeNum;
		modeSettings.iModeSupportTypeFlags = adspInResampleTypeFlags;
		modeSettings.bHasSettingsDialog = adspInResampleSettings;
		modeSettings.bIsDisabled = adspInResampleDisabled;

		modeSettings.iModeName = adspInResampleName;
		modeSettings.iModeSetupName = adspInResampleSetupName;
		modeSettings.iModeDescription = adspInResampleDescription;
		modeSettings.iModeHelp = adspInResampleHelp;

		temp = imagePath + adspInResampleOwnImage;
		strcpy(modeSettings.strOwnModeImage, temp.c_str());
		strcpy(modeSettings.strOverrideModeImage, "");//adspInResampleOverrideImage[ii]);

		ADSP->RegisterMode(&modeSettings);
	//}
#endif

#ifdef ADSP_ADDON_USE_PREPROCESSING
	for(unsigned int ii = 0; ii < ADSP_MAX_PRE_MODES; ii++)
	{
		modeSettings.iUniqueDBModeId = adspPreUniqueDbId[ii];
		modeSettings.iModeType = AE_DSP_MODE_TYPE_PRE_PROCESS;
		strcpy_s(modeSettings.strModeName, AE_DSP_ADDON_STRING_LENGTH, adspPreStrNames[ii]);

		modeSettings.iModeNumber = adspPreModeNum[ii];
		modeSettings.iModeSupportTypeFlags = adspPreTypeFlags[ii];
		modeSettings.bHasSettingsDialog = adspPreSettings[ii];
		modeSettings.bIsDisabled = adspPreDisabled[ii];

		modeSettings.iModeName = adspPreName[ii];
		modeSettings.iModeSetupName = adspPreSetupName[ii];
		modeSettings.iModeDescription = adspPreDescription[ii];
		modeSettings.iModeHelp = adspPreHelp[ii];

		temp = imagePath + adspPreOwnImage[ii];
		strcpy_s(modeSettings.strOwnModeImage, AE_DSP_ADDON_STRING_LENGTH, temp.c_str());
		strcpy_s(modeSettings.strOverrideModeImage, AE_DSP_ADDON_STRING_LENGTH, "");//adspPreOverrideImage[ii]);

		ADSP->RegisterMode(&modeSettings);
	}
#endif

#ifdef ADSP_ADDON_USE_MASTERPROCESS
	for(unsigned int ii = 0; ii < ADSP_MAX_MASTER_MODES; ii++)
	{
		modeSettings.iUniqueDBModeId = adspMaUniqueDdId[ii];
		modeSettings.iModeType = AE_DSP_MODE_TYPE_MASTER_PROCESS;
		strcpy_s(modeSettings.strModeName, AE_DSP_ADDON_STRING_LENGTH, adspMaStrNames[ii]);

		modeSettings.iModeNumber = adspMaModeNum[ii];
		modeSettings.iModeSupportTypeFlags = adspMaTypeFlags[ii];
		modeSettings.bHasSettingsDialog = adspMasterSettings[ii];
		modeSettings.bIsDisabled = adspMaDisabled[ii];

		modeSettings.iModeName = adspMaName[ii];
		modeSettings.iModeSetupName = adspMaSetupName[ii];
		modeSettings.iModeDescription = adspMaDescription[ii];
		modeSettings.iModeHelp = adspMaHelp[ii];

		temp = imagePath + adspMaOwnImage[ii];
		strcpy_s(modeSettings.strOwnModeImage, AE_DSP_ADDON_STRING_LENGTH, temp.c_str());
		temp = imagePath + adspMaOverrideImage[ii];
		strcpy_s(modeSettings.strOverrideModeImage, AE_DSP_ADDON_STRING_LENGTH, temp.c_str());

		ADSP->RegisterMode(&modeSettings);
	}
#endif

#ifdef ADSP_ADDON_USE_POSTPROCESS
	for(unsigned int ii = 0; ii < ADSP_MAX_POST_MODES; ii++)
	{
		modeSettings.iUniqueDBModeId = adspPostUniqueDbId[ii];
		modeSettings.iModeType = AE_DSP_MODE_TYPE_POST_PROCESS;
		strcpy_s(modeSettings.strModeName, AE_DSP_ADDON_STRING_LENGTH, adspPostStrNames[ii]);

		modeSettings.iModeNumber = adspPostModeNum[ii];
		modeSettings.iModeSupportTypeFlags = adspPostTypeFlags[ii];
		modeSettings.bHasSettingsDialog = adspPostSettings[ii];
		modeSettings.bIsDisabled = adspPostDisabled[ii];

		modeSettings.iModeName = adspPostName[ii];
		modeSettings.iModeSetupName = adspPostSetupName[ii];
		modeSettings.iModeDescription = adspPostDescription[ii];
		modeSettings.iModeHelp = adspPostHelp[ii];

		strcpy_s(modeSettings.strOwnModeImage, AE_DSP_ADDON_STRING_LENGTH, adspPostOwnImage[ii]);
		strcpy_s(modeSettings.strOverrideModeImage, AE_DSP_ADDON_STRING_LENGTH, "");//adspPostOverrideImage[ii]);

		ADSP->RegisterMode(&modeSettings);
	}
#endif

#ifdef ADSP_ADDON_USE_OUTPUTRESAMPLE
	//for(unsigned int ii = 0; ii < ADSP_MAX_OUTRES_MODES; ii++)
	//{
		modeSettings.iUniqueDBModeId = adspOutResampleUniqueDdId;
		modeSettings.iModeType = AE_DSP_MODE_TYPE_OUTPUT_RESAMPLE;
		strcpy_s(modeSettings.strModeName, AE_DSP_ADDON_STRING_LENGTH, adspOutResampleStrNames);

		modeSettings.iModeNumber = adspOutResampleModeNum;
		modeSettings.iModeSupportTypeFlags = adspOutResampleTypeFlags;
		modeSettings.bHasSettingsDialog = adspOutResampleSettings;
		modeSettings.bIsDisabled = adspOutResampleDisabled;

		modeSettings.iModeName = adspOutResampleName;
		modeSettings.iModeSetupName = adspOutResampleSetupName;
		modeSettings.iModeDescription = adspOutResampleDescription;
		modeSettings.iModeHelp = adspOutResampleHelp;

		strcpy_s(modeSettings.strOwnModeImage, AE_DSP_ADDON_STRING_LENGTH, adspOutResampleOwnImage);
		strcpy_s(modeSettings.strOverrideModeImage, AE_DSP_ADDON_STRING_LENGTH, "");//adspOutResampleOverrideImage[ii]);

		ADSP->RegisterMode(&modeSettings);
	//}
#endif

	//now we try to initialize the addon processor class
#ifdef ADSP_ADDON_USE_OPTIONAL_INIT
	return OptionalInit();
#else
	return true;
#endif
}


void CADSPAddonHandler::Destroy()
{
	// ToDo: what should we process here?!?
}


AE_DSP_ERROR CADSPAddonHandler::StreamCreate(const AE_DSP_SETTINGS *addonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties)
{
	const unsigned int iStreamID = addonSettings->iStreamID;
	if(iStreamID >= AE_DSP_STREAM_MAX_STREAMS)
	{
		XBMC->Log(LOG_ERROR, "StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!");
		return AE_DSP_ERROR_UNKNOWN;
	}

	if(m_ADSPProcessor[iStreamID])
	{
		delete m_ADSPProcessor[iStreamID];
		m_ADSPProcessor[iStreamID] = NULL;
	}

	m_ADSPProcessor[iStreamID] = new CADSPProcessorHandle(addonSettings, pProperties);
	if( m_ADSPProcessor[iStreamID] )
	{
		return AE_DSP_ERROR_NO_ERROR;
	}
	else
	{
		XBMC->Log(LOG_ERROR, "Couldn't create new ADSP-Stream! Not enough free memory?");
		return AE_DSP_ERROR_FAILED;
	}
}


AE_DSP_ERROR CADSPAddonHandler::StreamDestroy(unsigned int Id)
{
	if(Id >= AE_DSP_STREAM_MAX_STREAMS)
	{
		XBMC->Log(LOG_ERROR, "StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!");
		return AE_DSP_ERROR_UNKNOWN;
	}

	if(m_ADSPProcessor[Id])
	{
		delete m_ADSPProcessor[Id];
		m_ADSPProcessor[Id] = NULL;
	}
	else
	{
		XBMC->Log(LOG_ERROR, "Couldn't destroy Stream: %i! It was not created!", Id);
		return AE_DSP_ERROR_UNKNOWN;
	}

	return AE_DSP_ERROR_NO_ERROR;
}

CADSPProcessorHandle *CADSPAddonHandler::GetStream(AE_DSP_STREAM_ID Id)
{
	if(Id >= AE_DSP_STREAM_MAX_STREAMS)
	{
		XBMC->Log(LOG_ERROR, "StreamID was equal or greater than AE_DSP_STREAM_MAX_STREAMS!");
		return NULL;
	}

	return (m_ADSPProcessor[Id]);
}

AE_DSP_ERROR CADSPAddonHandler::StreamInitialize(const AE_DSP_SETTINGS *Settings)
{
	return AE_DSP_ERROR_NO_ERROR;
}

/*
 * ADSP-Addon support functions
 */
bool CADSPAddonHandler::SupportsInputProcess()
{
#ifdef ADSP_ADDON_USE_INPUTPROCESS
	return true;
#else
	return false;
#endif
}

bool CADSPAddonHandler::SupportsPreProcess()
{
#ifdef ADSP_ADDON_USE_PREPROCESSING
	return true;
#else
	return false;
#endif
}

bool CADSPAddonHandler::SupportsMasterProcess()
{
#ifdef ADSP_ADDON_USE_MASTERPROCESS
	return true;
#else
	return false;
#endif
}

bool CADSPAddonHandler::SupportsPostProcess()
{
#ifdef ADSP_ADDON_USE_POSTPROCESS
	return true;
#else
	return false;
#endif
}

bool CADSPAddonHandler::SupportsInputResample()
{
#ifdef ADSP_ADDON_USE_INPUTRESAMPLE
	return true;
#else
	return false;
#endif
}

bool CADSPAddonHandler::SupportsOutputResample()
{
#ifdef ADSP_ADDON_USE_OUTPUTRESAMPLE
	return true;
#else
	return false;
#endif
}


//Helper function
string GetSettingsFile()
{
  string settingFile = g_strAddonPath;
  if (settingFile.at(settingFile.size() - 1) == '\\' || settingFile.at(settingFile.size() - 1) == '/')
  {
    settingFile.append(ADSP_SETTINGS_FILE);
  }
  else
  {
    settingFile.append(string("/") + string(ADSP_SETTINGS_FILE));
  }

  return settingFile;
}

