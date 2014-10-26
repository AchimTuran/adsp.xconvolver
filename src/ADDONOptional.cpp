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


//! Example optional class method implementations.
/*!
 * \file ADDONOptional.cpp
 * ToDo: detailed description!
 */
#include "template/include/client.h"
using namespace ADDON;

#include "ADDONOptional.h"

#include "Dialogs/GUIDialogPreProcess.h"
#include "Dialogs/GUIDialogMasterProcess.h"
#include "Dialogs/GUIDialogPostProcess.h"
#include "Dialogs/GUIDialogOutputResample.h"

CADDONOptional::CADDONOptional()
{
}

CADDONOptional::~CADDONOptional()
{
}


ADDON_STATUS CADDONOptional::SetSetting(std::string SettingName, const void *SettingValue)
{
	return ADDON_STATUS_OK;
}

unsigned int CADDONOptional::GetSettings(ADDON_StructSetting ***sSet)
{
	return 0;
}

void CADDONOptional::Stop()
{
}

void CADDONOptional::FreeSettings()
{
}

void CADDONOptional::Announce(std::string Flag, std::string Sender, std::string Message, const void *Data)
{
}

void CADDONOptional::ReadSettings(void)
{
}

AE_DSP_ERROR CADDONOptional::CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
	switch(Menuhook.iHookId)
	{
	case ID_MENU_PRE_PROCESS:
		{
			CGUIDialogPreProcess menu;
			menu.DoModal();
		}
	break;
	
	case ID_MENU_MASTER_PROCESS:
		{
			CGUIDialogMasterProcess menu;
			menu.DoModal();
		}
	break;

	case ID_MENU_POST_PROCESS:
		{
			CGUIDialogPostProcess menu;
			menu.DoModal();
		}
	break;

	case ID_MENU_OUTPUT_RESAMPLE_PROCESS:
		{
			CGUIDialogOutputResample menu;
			menu.DoModal();
		}
	break;

	default:
		XBMC->Log(LOG_ERROR, "called unknown menu hook!" );
		return AE_DSP_ERROR_FAILED;
	break;
	};
	//if (menuhook.iHookId == ID_MENU_PRE_PROCESS)
	//{
	//	CGUIDialogSpeakerGain settings(item.data.iStreamId);
	//	settings.DoModal();
	//}
	//else if (menuhook.iHookId == ID_MENU_SPEAKER_DISTANCE_SETUP)
	//{
	//	CGUIDialogSpeakerDistance settings(item.data.iStreamId);
	//	settings.DoModal();
	//}
	//else if (menuhook.iHookId == ID_MASTER_PROCESS_FREE_SURROUND)
	//{
	//	CGUIDialogFreeSurround settings(item.data.iStreamId);
	//	settings.DoModal();
	//}
	//else if (menuhook.iHookId == ID_MENU_EXTENDED_SETTINGS)
	//{
	//	CGUIDialogSpeakerDistance settings(0);
	//	settings.DoModal();
	//}
	return AE_DSP_ERROR_NO_ERROR;
}

bool CADDONOptional::OptionalInit()
{
	// now we register out menu hooks
	AE_DSP_MENUHOOK hook;

	// register menu hook for the pre process
	hook.iHookId            = ID_MENU_PRE_PROCESS;
	hook.category           = AE_DSP_MENUHOOK_PRE_PROCESS;
    hook.iLocalizedStringId = 30021;
    hook.iRelevantModeId    = PRE_MODE1_ID;
    ADSP->AddMenuHook(&hook);

	// register menu hook for the master process
	hook.iHookId            = ID_MENU_MASTER_PROCESS;
	hook.category           = AE_DSP_MENUHOOK_MASTER_PROCESS;
    hook.iLocalizedStringId = 30022;
    hook.iRelevantModeId    = MA_MODE1_ID;
    ADSP->AddMenuHook(&hook);

	// register menu hook for the post process
	hook.iHookId            = ID_MENU_POST_PROCESS;
	hook.category           = AE_DSP_MENUHOOK_POST_PROCESS;
    hook.iLocalizedStringId = 30023;
    hook.iRelevantModeId    = POST_MODE1_ID;
    ADSP->AddMenuHook(&hook);

	// register menu hook for the output resample process
	hook.iHookId            = ID_MENU_PRE_PROCESS;
	hook.category           = AE_DSP_MENUHOOK_RESAMPLE;
    hook.iLocalizedStringId = 30024;
    hook.iRelevantModeId    = OUTPUT_RESAMPLE_MODE_ID;
    ADSP->AddMenuHook(&hook);

	return true;
}