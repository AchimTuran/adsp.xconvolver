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
#include "configuration/templateConfiguration.h"
#include "../template/include/MACROHelper.h"
#include "include/checkTemplateConfig.h"

#include <vector>
#include <string>
#include "include/client.h"
#include "xbmc_adsp_dll.h"
#include "util/util.h"

// include adsp template specific header files
#include "include/ADSPProcessorHandle.h"
#include "include/ADSPAddonHandler.h"
// includes your DSP Processor class
#include ADSP_PROCESSOR_HEADER_FILE

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#endif

int            m_iStreamsPresent  = 0;
bool           m_bCreated         = false;
ADDON_STATUS   m_CurStatus        = ADDON_STATUS_UNKNOWN;

/* User adjustable settings are saved here.
 * Default values are defined inside client.h
 * and exported to the other source files.
 */
std::string   g_strUserPath       = "";
std::string   g_strAddonPath      = "";


CHelper_libXBMC_addon *XBMC       = NULL;
CHelper_libXBMC_adsp  *ADSP       = NULL;
CHelper_libXBMC_gui   *GUI        = NULL;

/*
 *	ADSP Addon handling class
 */
CADSPAddonHandler g_AddonHandler;

extern "C" {

void ADDON_ReadSettings(void)
{
#ifdef ADSP_ADDON_USE_READSETTINGS
	g_AddonHandler.ReadSettings();
#endif
}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
	if (!hdl || !props)
	{
		return ADDON_STATUS_UNKNOWN;
	}

	AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

	XBMC = new CHelper_libXBMC_addon;
	if (!XBMC->RegisterMe(hdl))
	{
		SAFE_DELETE(XBMC);
		return ADDON_STATUS_PERMANENT_FAILURE;
	}

	GUI = new CHelper_libXBMC_gui;
	if (!GUI->RegisterMe(hdl))
	{
		SAFE_DELETE(GUI);
		SAFE_DELETE(XBMC);
		return ADDON_STATUS_PERMANENT_FAILURE;
	}

	ADSP = new CHelper_libXBMC_adsp;
	if (!ADSP->RegisterMe(hdl))
	{
		SAFE_DELETE(ADSP);
		SAFE_DELETE(GUI);
		SAFE_DELETE(XBMC);
		return ADDON_STATUS_PERMANENT_FAILURE;
	}
	XBMC->Log(LOG_DEBUG, "%s - Creating the Audio DSP add-on template", __FUNCTION__);

	m_CurStatus     = ADDON_STATUS_UNKNOWN;
	g_strUserPath   = adspprops->strUserPath;
	g_strAddonPath  = adspprops->strAddonPath;

	ADDON_ReadSettings();

	if(!g_AddonHandler.Init())
	{
		return m_CurStatus;
	}

	m_CurStatus = ADDON_STATUS_OK;
	m_bCreated = true;
	m_iStreamsPresent = 0;
	return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
	m_bCreated = false;
  m_iStreamsPresent = 0;

  g_AddonHandler.Destroy();

  SAFE_DELETE(ADSP);
  SAFE_DELETE(GUI);
  SAFE_DELETE(XBMC);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
#ifdef ADSP_ADDON_USE_SETTINGS
  return true;
#else
	return false;
#endif
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
#ifdef ADSP_ADDON_USE_GETSETTINGS
	return g_AddonHandler.GetSettings(sSet);
#else
	(void) sSet; // Remove compiler warning
	return 0;
#endif
}

ADDON_STATUS ADDON_SetSetting(const char *SettingName, const void *SettingValue)
{
	string settingName(SettingName);
	if(!SettingValue)
	{
		return ADDON_STATUS_PERMANENT_FAILURE;
	}

#ifdef ADSP_ADDON_USE_SETTINGS
	return g_AddonHandler.SetSetting( string(SettingName), SettingValue );
#else
	return ADDON_STATUS_OK;
#endif
}

void ADDON_Stop()
{
#ifdef ADSP_ADDON_USE_STOP
	g_AddonHandler.Stop();
#endif
}

void ADDON_FreeSettings()
{
#ifdef ADSP_ADDON_USE_FREESETTINGS
	g_AddonHandler.FreeSettings();
#endif
}

void ADDON_Announce(const char *Flag, const char *Sender, const char *Message, const void *Data)
{
#ifdef ADSP_ADDON_USE_ANNOUNCE
	g_AddonHandler.Announce( string(Flag), string(Sender), string(Message), Data );
#else
	// Remove compiler warnings
	(void) Flag;
	(void) Sender; 
	(void) Message; 
	(void) Data;
#endif
}


/***********************************************************
 * Audio DSP Client AddOn specific public library functions
 ***********************************************************/
const char* GetAudioDSPAPIVersion(void)
{
  static string s_ApiVersion = XBMC_AE_DSP_API_VERSION;
  return s_ApiVersion.c_str();
}

const char* GetMinimumAudioDSPAPIVersion(void)
{
  static string s_MinApiVersion = XBMC_AE_DSP_MIN_API_VERSION;
  return s_MinApiVersion.c_str();
}

const char* GetGUIAPIVersion(void)
{
  static string s_GuiApiVersion = XBMC_GUI_API_VERSION;
  return s_GuiApiVersion.c_str();
}

const char* GetMinimumGUIAPIVersion(void)
{
  static string s_MinGuiApiVersion = XBMC_GUI_MIN_API_VERSION;
  return s_MinGuiApiVersion.c_str();
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
	if(pCapabilities == NULL)
	{
		return AE_DSP_ERROR_FAILED;
	}

	pCapabilities->bSupportsInputProcess		= g_AddonHandler.SupportsInputProcess();
	pCapabilities->bSupportsPreProcess			= g_AddonHandler.SupportsPreProcess();
	pCapabilities->bSupportsMasterProcess		= g_AddonHandler.SupportsMasterProcess();
	pCapabilities->bSupportsPostProcess			= g_AddonHandler.SupportsPostProcess();
	pCapabilities->bSupportsInputResample		= g_AddonHandler.SupportsInputResample();
	pCapabilities->bSupportsOutputResample		= g_AddonHandler.SupportsOutputResample();

	return AE_DSP_ERROR_NO_ERROR;
}

const char* GetDSPName(void)
{
	static string s_DSPName(ADSP_ADDON_NAME);
	return s_DSPName.c_str();
}

const char* GetDSPVersion(void)
{
	static string s_ADSPVersion(ADSP_ADDON_VERSION);
	return s_ADSPVersion.c_str();
}

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
#ifdef ADSP_ADDON_USE_MENUHOOK
	return g_AddonHandler.CallMenuHook(Menuhook, Item);
#else
	return AE_DSP_ERROR_NOT_IMPLEMENTED;
#endif
}


/*!
 * Control function for start and stop of dsp processing.
 */
AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *AddonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties)
{
	return g_AddonHandler.StreamCreate( AddonSettings, pProperties );
}

AE_DSP_ERROR StreamDestroy(AE_DSP_STREAM_ID Id)
{
	return g_AddonHandler.StreamDestroy(Id);
}

AE_DSP_ERROR StreamInitialize(const AE_DSP_SETTINGS *AddonSettings)
{
	if( !AddonSettings )
	{
		XBMC->Log(LOG_ERROR, "Null pointer were given!");
		return AE_DSP_ERROR_UNKNOWN;
	}

	return g_AddonHandler.StreamInitialize(AddonSettings);
}


/*!
 * Pre processing related functions
 */
unsigned int PreProcessNeededSamplesize(AE_DSP_STREAM_ID Id, unsigned int Mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->PreProcessNeededSamplesize(Mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PreProcessNeededSamplesize(...): uninitialized Stream was requested!");
		return 0;
	}
}

float PreProcessGetDelay(AE_DSP_STREAM_ID Id, unsigned int Mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->PreProcessGetDelay(Mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PreProcessGetDelay(...): uninitialized Stream was requested!");
		return 0.0f;
	}
}

/*!
 * Resampling related functions before master processing.
 * only one dsp addon is allowed to do this
 */
unsigned int InputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->InputResampleProcessNeededSamplesize();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "InputResampleProcessNeededSamplesize(...): uninitialized Stream was requested!");
		return 0;
	}
}

int InputResampleSampleRate(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->InputResampleSampleRate();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "InputResampleSampleRate(...): uninitialized Stream was requested!");
		return 0;
	}
}
  
float InputResampleGetDelay(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->InputResampleGetDelay();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "InputResampleGetDelay(...): uninitialized Stream was requested!");
		return 0;
	}
}

/*!
 * Master processing functions
 * only one during playback selectable dsp addon is allowed to do this
 */
AE_DSP_ERROR MasterProcessSetMode(AE_DSP_STREAM_ID Id, AE_DSP_STREAMTYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return  p->MasterProcessSetMode(Type, Mode_id, Unique_db_mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcessSetMode(...): uninitialized Stream was requested!");
		return AE_DSP_ERROR_UNKNOWN;
	}
}

unsigned int MasterProcessNeededSamplesize(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->MasterProcessNeededSamplesize();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcessNeededSamplesize(...): uninitialized Stream was requested!");
		return 0;
	}
}

float MasterProcessGetDelay(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->MasterProcessGetDelay();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcessGetDelay(...): uninitialized Stream was requested!");
		return 0.0f;
	}
}

int MasterProcessGetOutChannels(AE_DSP_STREAM_ID Id, unsigned long &Out_channel_present_flags)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->MasterProcessGetOutChannels(Out_channel_present_flags);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcessGetOutChannels(...): uninitialized Stream was requested!");
		return 0;
	}
}

const char *MasterProcessGetStreamInfoString(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->MasterProcessGetStreamInfoString();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcessGetStreamInfoString(...): uninitialized Stream was requested!");
		return "";
	}
}


/*!
 * Post processing related functions
 * all enabled addons allowed todo this
 */
unsigned int PostProcessNeededSamplesize(AE_DSP_STREAM_ID Id, unsigned int Mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return  p->PostProcessNeededSamplesize(Mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PostProcessNeededSamplesize(...): uninitialized Stream was requested!");
		return 0;
	}
}

float PostProcessGetDelay(AE_DSP_STREAM_ID Id, unsigned int Mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->PostProcessGetDelay(Mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PostProcessGetDelay(...): uninitialized Stream was requested!");
		return 0.0f;
	}
}

/*!
 * Resampling related functions after final processing.
 * only one dsp addon is allowed to do this
 */
unsigned int OutputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->OutputResampleProcessNeededSamplesize();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "OutputResampleProcessNeededSamplesize(...): uninitialized Stream was requested!");
		return 0;
	}
}

int OutputResampleSampleRate(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->OutputResampleSampleRate();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "OutputResampleSampleRate(...): uninitialized Stream was requested!");
		return 0;
	}
}

float OutputResampleGetDelay(AE_DSP_STREAM_ID Id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->OutputResampleGetDelay();
	}
	else
	{
		XBMC->Log(LOG_ERROR, "OutputResampleGetDelay(...): uninitialized Stream was requested!");
		return 0.0f;
	}
}

/*!
 *	Addon Processing functions
 */
bool InputProcess(AE_DSP_STREAM_ID Id, const float **Array_in, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_INPUTPROCESS
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->InputProcess(Array_in, Samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "InputProcess(...): uninitialized Stream was requested!");
		return false;
	}
#else
	return true;
#endif
}

unsigned int InputResampleProcess(AE_DSP_STREAM_ID Id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_INPUTRESAMPLE
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->InputResampleProcess(Array_in, Array_out, Samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "InputResampleProcess(...): uninitialized Stream was requested!");
		return 0;
	}
#else
	return 0;
#endif
}

unsigned int PreProcess(AE_DSP_STREAM_ID Id, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_PREPROCESSING
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->PreProcess(Mode_id, Array_in, Array_out, Samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PreProcess(...): uninitialized Stream was requested!");
		return 0;
	}
#else
	return 0;
#endif
}

unsigned int MasterProcess(AE_DSP_STREAM_ID Id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_MASTERPROCESS
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->MasterProcess(Array_in, Array_out, Samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "MasterProcess(...): uninitialized Stream was requested!");
		return 0;
	}
#else
	return 0;
#endif
}

unsigned int PostProcess(AE_DSP_STREAM_ID Id, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_POSTPROCESS
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->PostProcess(Mode_id, Array_in, Array_out, Samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "PostProcess(...): uninitialized Stream was requested!");
		return 0;
	}
#else
	return 0;
#endif
}

unsigned int OutputResampleProcess(AE_DSP_STREAM_ID Id, float **array_in, float **array_out, unsigned int samples)
{
#ifdef ADSP_ADDON_USE_OUTPUTRESAMPLE
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->OutputResampleProcess(array_in,  array_out, samples);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "OutputResampleProcess(...): uninitialized Stream was requested!");
		return 0;
	}
#else
	return 0;
#endif
}

AE_DSP_ERROR StreamIsModeSupported(AE_DSP_STREAM_ID Id, AE_DSP_MODE_TYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
	CADSPProcessorHandle *p = g_AddonHandler.GetStream(Id);
	if(p)
	{
		return p->StreamIsModeSupported(Type, Mode_id, Unique_db_mode_id);
	}
	else
	{
		XBMC->Log(LOG_ERROR, "StreamIsModeSupported(...): uninitialized Stream was requested!");
		return AE_DSP_ERROR_UNKNOWN;
	}
}

}