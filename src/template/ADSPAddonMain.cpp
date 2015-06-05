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

#include <vector>
#include <string>

// include template configuration header files
#include "configuration/templateConfiguration.h"
#include "../template/include/MACROHelper.h"
#include "include/checkTemplateConfig.h"

// include kodi platform header files
#include <kodi/kodi_adsp_dll.h>
#include <kodi/util/util.h>

// include adsp template specific header files
#include <template/include/ADSPAddonMain.h>
#include "include/ADSPProcessorHandle.h"
#include "include/ADSPAddonHandler.h"
#include <template/include/typedefs.h>

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

CHelper_libXBMC_addon       *KODI         = NULL;
CHelper_libKODI_adsp        *ADSP         = NULL;
CHelper_libKODI_guilib      *GUI          = NULL;
CHelper_libKODI_audioengine *AUDIOENGINE  = NULL;

/*
 *	ADSP Addon handling class
 */
CADSPAddonHandler g_AddonHandler;

extern "C" {

void ADDON_ReadSettings(void)
{
#ifdef ADSP_ADDON_USE_READSETTINGS
  try
  {
    g_AddonHandler.ReadSettings();
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif
}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  try
  {
    if (!hdl || !props)
    {
      return ADDON_STATUS_UNKNOWN;
    }

    AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

    KODI = new CHelper_libXBMC_addon;
    if (!KODI->RegisterMe(hdl))
    {
      SAFE_DELETE(KODI);
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    GUI = new CHelper_libKODI_guilib;
    if (!GUI->RegisterMe(hdl))
    {
      SAFE_DELETE(GUI);
      SAFE_DELETE(KODI);
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    ADSP = new CHelper_libKODI_adsp;
    if (!ADSP->RegisterMe(hdl))
    {
      SAFE_DELETE(ADSP);
      SAFE_DELETE(GUI);
      SAFE_DELETE(KODI);
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    AUDIOENGINE = new CHelper_libKODI_audioengine;
    if(!AUDIOENGINE->RegisterMe(hdl))
    {
      SAFE_DELETE(ADSP);
      SAFE_DELETE(GUI);
      SAFE_DELETE(KODI);
      SAFE_DELETE(AUDIOENGINE);
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    KODI->Log(LOG_DEBUG, "%s - Creating Audio DSP add-on template", __FUNCTION__);

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
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    if(KODI)
    {
      ADDON_STRING_EXCEPTION_TO_LOG(e);
    }

    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);
    SAFE_DELETE(AUDIOENGINE);
  }

  return ADDON_STATUS_PERMANENT_FAILURE;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  try
  {
    m_bCreated = false;
    m_iStreamsPresent = 0;

    g_AddonHandler.Destroy();

    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(KODI);

    m_CurStatus = ADDON_STATUS_UNKNOWN;
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
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
  try
  {
    return g_AddonHandler.GetSettings(sSet);
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#else
	(void) sSet; // Remove compiler warning
#endif

	return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *SettingName, const void *SettingValue)
{
  try
  {
    if(!SettingValue || !SettingName)
    {
      return ADDON_STATUS_PERMANENT_FAILURE;
    }
    string settingName(SettingName);

  #ifdef ADSP_ADDON_USE_SETTINGS
    return g_AddonHandler.SetSetting(string(SettingName), SettingValue);
  #else
    return ADDON_STATUS_OK;
  #endif
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return ADDON_STATUS_PERMANENT_FAILURE;
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
    try
    {
      g_AddonHandler.FreeSettings();
    }
    catch(CAddonStringException &e)
    {
      m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
      ADDON_STRING_EXCEPTION_TO_LOG(e);
    }
  #endif
}

void ADDON_Announce(const char *Flag, const char *Sender, const char *Message, const void *Data)
{
#ifdef ADSP_ADDON_USE_ANNOUNCE
  try
  {
    if(!Sender || !Message || !Data)
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Invalid input parameters!");
    }

    g_AddonHandler.Announce(string(Flag), string(Sender), string(Message), Data);
  }
	catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
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
  return KODI_AE_DSP_API_VERSION;
}

const char* GetMinimumAudioDSPAPIVersion(void)
{
  return KODI_AE_DSP_MIN_API_VERSION;
}

const char* GetGUIAPIVersion(void)
{
  return KODI_GUILIB_API_VERSION;
}

const char* GetMinimumGUIAPIVersion(void)
{
  return KODI_GUILIB_MIN_API_VERSION;
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
  try
  {
    if(pCapabilities == NULL)
    {
      return AE_DSP_ERROR_FAILED;
    }

    pCapabilities->bSupportsInputProcess    = g_AddonHandler.SupportsInputProcess();
    pCapabilities->bSupportsPreProcess      = g_AddonHandler.SupportsPreProcess();
    pCapabilities->bSupportsMasterProcess   = g_AddonHandler.SupportsMasterProcess();
    pCapabilities->bSupportsPostProcess     = g_AddonHandler.SupportsPostProcess();
    pCapabilities->bSupportsInputResample   = g_AddonHandler.SupportsInputResample();
    pCapabilities->bSupportsOutputResample  = g_AddonHandler.SupportsOutputResample();

    return AE_DSP_ERROR_NO_ERROR;
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}

const char* GetDSPName(void)
{
  return ADSP_ADDON_NAME;
}

const char* GetDSPVersion(void)
{
  return ADSP_ADDON_VERSION;
}

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
#ifdef ADSP_ADDON_USE_MENUHOOK
  try
  {
    return g_AddonHandler.CallMenuHook(Menuhook, Item);
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
#else
	return AE_DSP_ERROR_NOT_IMPLEMENTED;
#endif
}


/*!
 * Control function for start and stop of dsp processing.
 */
AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *AddonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties, ADDON_HANDLE handle)
{
  try
  {
    return g_AddonHandler.StreamCreate( AddonSettings, pProperties, handle );
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}

AE_DSP_ERROR StreamDestroy(const ADDON_HANDLE handle)
{
  try
  {
    return g_AddonHandler.StreamDestroy(handle->dataIdentifier);
  }
	catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}

AE_DSP_ERROR StreamInitialize(const ADDON_HANDLE handle, const AE_DSP_SETTINGS *AddonSettings)
{
  try
  {
    if(!AddonSettings)
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Invalid input parameters!");
    }

    return g_AddonHandler.StreamInitialize(handle, AddonSettings);
  }
	catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}


/*!
 * Pre processing related functions
 */
unsigned int PreProcessNeededSamplesize(const ADDON_HANDLE handle, unsigned int Mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->PreProcessNeededSamplesize(Mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("PreProcessNeededSamplesize(...): uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}

float PreProcessGetDelay(const ADDON_HANDLE handle, unsigned int Mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->PreProcessGetDelay(Mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("PreProcessGetDelay(...): uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0.0f;
}

/*!
 * Resampling related functions before master processing.
 * only one dsp addon is allowed to do this
 */
unsigned int InputResampleProcessNeededSamplesize(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->InputResampleProcessNeededSamplesize();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("InputResampleProcessNeededSamplesize(...): uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}

int InputResampleSampleRate(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->InputResampleSampleRate();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("InputResampleSampleRate(...): uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}
  
float InputResampleGetDelay(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->InputResampleGetDelay();
    }
    else
    {
      KODI->Log(LOG_ERROR, "InputResampleGetDelay(...): uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0.0f;
}

/*!
 * Master processing functions
 * only one during playback selectable dsp addon is allowed to do this
 */
AE_DSP_ERROR MasterProcessSetMode(const ADDON_HANDLE handle, AE_DSP_STREAMTYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return  p->MasterProcessSetMode(Type, Mode_id, Unique_db_mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}

unsigned int MasterProcessNeededSamplesize(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->MasterProcessNeededSamplesize();
    }
    else
    {
      throw ("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

	return 0;
}

float MasterProcessGetDelay(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->MasterProcessGetDelay();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0.0f;
}

int MasterProcessGetOutChannels(const ADDON_HANDLE handle, unsigned long &Out_channel_present_flags)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->MasterProcessGetOutChannels(Out_channel_present_flags);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}

const char *MasterProcessGetStreamInfoString(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->MasterProcessGetStreamInfoString();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return "";
}


/*!
 * Post processing related functions
 * all enabled addons allowed todo this
 */
unsigned int PostProcessNeededSamplesize(const ADDON_HANDLE handle, unsigned int Mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return  p->PostProcessNeededSamplesize(Mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

	return 0;
}

float PostProcessGetDelay(const ADDON_HANDLE handle, unsigned int Mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->PostProcessGetDelay(Mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0.0f;
}

/*!
 * Resampling related functions after final processing.
 * only one dsp addon is allowed to do this
 */
unsigned int OutputResampleProcessNeededSamplesize(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->OutputResampleProcessNeededSamplesize();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}

int OutputResampleSampleRate(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->OutputResampleSampleRate();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0;
}

float OutputResampleGetDelay(const ADDON_HANDLE handle)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->OutputResampleGetDelay();
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return 0.0f;
}

/*!
 *	Addon Processing functions
 */
bool InputProcess(const ADDON_HANDLE handle, const float **Array_in, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_INPUTPROCESS
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->InputProcess(Array_in, Samples);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return true;
}

unsigned int InputResampleProcess(const ADDON_HANDLE handle, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_INPUTRESAMPLE
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->InputResampleProcess(Array_in, Array_out, Samples);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return 0;
}

unsigned int PreProcess(const ADDON_HANDLE handle, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_PREPROCESSING
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->PreProcess(Mode_id, Array_in, Array_out, Samples);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return 0;
}

unsigned int MasterProcess(const ADDON_HANDLE handle, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_MASTERPROCESS
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->MasterProcess(Array_in, Array_out, Samples);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return 0;
}

unsigned int PostProcess(const ADDON_HANDLE handle, unsigned int Mode_id, float **Array_in, float **Array_out, unsigned int Samples)
{
#ifdef ADSP_ADDON_USE_POSTPROCESS
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->PostProcess(Mode_id, Array_in, Array_out, Samples);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return 0;
}

unsigned int OutputResampleProcess(const ADDON_HANDLE handle, float **array_in, float **array_out, unsigned int samples)
{
#ifdef ADSP_ADDON_USE_OUTPUTRESAMPLE
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->OutputResampleProcess(array_in,  array_out, samples);
    }
    else
    {
      throw ADDON_STRING_XCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }
#endif

	return 0;
}

AE_DSP_ERROR StreamIsModeSupported(const ADDON_HANDLE handle, AE_DSP_MODE_TYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
  try
  {
    CADSPProcessorHandle *p = g_AddonHandler.GetStream(handle->dataIdentifier);
    if(p)
    {
      return p->StreamIsModeSupported(Type, Mode_id, Unique_db_mode_id);
    }
    else
    {
      throw ADDON_STRING_EXCEPTION_HANDLER("Uninitialized Stream was requested!");
    }
  }
  catch(CAddonStringException &e)
  {
    m_CurStatus = ADDON_STATUS_PERMANENT_FAILURE;
    ADDON_STRING_EXCEPTION_TO_LOG(e);
  }

  return AE_DSP_ERROR_FAILED;
}

}
