#pragma once
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


#include <string>
#include <template/AddonExceptions/TAddonException.h>

//--- typedefs input resample ---
typedef int ADSP_INRESMOD_UNIQUEDBID;
typedef char ADSP_INRESMOD_STR_NAMES;

typedef unsigned int ADSP_INRESMOD_MODE_NUM;
typedef unsigned int ADSP_INRESMOD_TYPE_FLAGS;
typedef bool ADSP_INRESMOD_SETTINGS_DIALOG;
typedef bool ADSP_INRESMOD_DISABLED;
//typedefs for g_localizeStrings
typedef unsigned int ADSP_INRESMOD_NAME;
typedef unsigned int ADSP_INRESMOD_SETUP_NAME;
typedef unsigned int ADSP_INRESMOD_DESCRIPTION;
typedef unsigned int ADSP_INRESMOD_HELP;

typedef char ADSP_INRESMOD_OWN_IMAGE;
typedef char ADSP_INRESMOD_OVERRIDE_IMAGE;


//--- typedefs preprocessing modes ---
typedef int ADSP_PREMOD_UNIQUEDBID;
typedef char ADSP_PREMOD_STR_NAMES;

typedef unsigned int ADSP_PREMOD_MODE_NUM;
typedef unsigned int ADSP_PREMOD_TYPE_FLAGS;
typedef bool ADSP_PREMOD_SETTINGS_DIALOG;
typedef bool ADSP_PREMOD_DISABLED;
//typedefs for g_localizeStrings
typedef unsigned int ADSP_PREMOD_NAME;
typedef unsigned int ADSP_PREMOD_SETUP_NAME;
typedef unsigned int ADSP_PREMOD_DESCRIPTION;
typedef unsigned int ADSP_PREMOD_HELP;

typedef char ADSP_PREMOD_OWN_IMAGE;
typedef char ADSP_PREMOD_OVERRIDE_IMAGE;


//--- typedefs master modes ---
typedef int ADSP_MAMOD_UNIQUEDBID;
typedef char ADSP_MAMOD_STR_NAMES;

typedef unsigned int ADSP_MAMOD_MODE_NUM;
typedef unsigned int ADSP_MAMOD_TYPE_FLAGS;
typedef bool ADSP_MAMOD_SETTINGS_DIALOG;
typedef bool ADSP_MAMOD_DISABLED;
//typedefs for g_localizeStrings
typedef unsigned int ADSP_MAMOD_NAME;
typedef unsigned int ADSP_MAMOD_SETUP_NAME;
typedef unsigned int ADSP_MAMOD_DESCRIPTION;
typedef unsigned int ADSP_MAMOD_HELP;

typedef char ADSP_MAMOD_OWN_IMAGE;
typedef char ADSP_MAMOD_OVERRIDE_IMAGE;


//--- typedefs postrocessing modes ---
typedef int ADSP_POSTMOD_UNIQUEDBID;
typedef char ADSP_POSTMOD_STR_NAMES;

typedef unsigned int ADSP_POSTMOD_MODE_NUM;
typedef unsigned int ADSP_POSTMOD_TYPE_FLAGS;
typedef bool ADSP_POSTMOD_SETTINGS_DIALOG;
typedef bool ADSP_POSTMOD_DISABLED;
//typedefs for g_localizeStrings
typedef unsigned int ADSP_POSTMOD_NAME;
typedef unsigned int ADSP_POSTMOD_SETUP_NAME;
typedef unsigned int ADSP_POSTMOD_DESCRIPTION;
typedef unsigned int ADSP_POSTMOD_HELP;

typedef char ADSP_POSTMOD_OWN_IMAGE;
typedef char ADSP_POSTMOD_OVERRIDE_IMAGE;


//--- typedefs postrocessing modes ---
typedef int ADSP_OUTRESMOD_UNIQUEDBID;
typedef char ADSP_OUTRESMOD_STR_NAMES;

typedef unsigned int ADSP_OUTRESMOD_MODE_NUM;
typedef unsigned int ADSP_OUTRESMOD_TYPE_FLAGS;
typedef bool ADSP_OUTRESMOD_SETTINGS_DIALOG;
typedef bool ADS_OUTRESMOD_DISABLED;
//typedefs for g_localizeStrings
typedef unsigned int ADSP_OUTRESMOD_NAME;
typedef unsigned int ADSP_OUTRESMOD_SETUP_NAME;
typedef unsigned int ADSP_OUTRESMOD_DESCRIPTION;
typedef unsigned int ADSP_OUTRESMOD_HELP;

typedef char ADSP_OUTRESMOD_OWN_IMAGE;
typedef char ADSP_OUTRESMOD_OVERRIDE_IMAGE;

// general typedefs
typedef TAddonException<std::string> CAddonStringException;

#define ADDON_STRING_EXCEPTION_HANDLER(ErrorStr)  CAddonStringException(ErrorStr, __func__, __FILE__, __LINE__)
#define ADDON_STRING_MODULE_EXCEPTION_HANDLER(ErrorStr, Module)  CAddonStringException(ErrorStr, __func__, __FILE__, __LINE__, Module)
#define ADDON_STRING_EXCEPTION_TO_LOG(Exception) KODI->Log(LOG_ERROR, "Function:%s File:%s: Line:%i ErrorMessage:%s", Exception.Function().c_str(), Exception.Filename().c_str(), Exception.what().c_str())

#if defined(TARGET_WINDOWS)
  #define PATH_SEPARATOR_SYMBOL std::string("\\")
#else
  #define PATH_SEPARATOR_SYMBOL std::string("/")
#endif

//#define ADSP_PROCESSING_MODE_IDS typedef enum
