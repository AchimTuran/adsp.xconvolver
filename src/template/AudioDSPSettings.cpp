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

#include "libXBMC_addon.h"
#include "libXBMC_adsp.h"
#include "libXBMC_gui.h"

#include "util/XMLUtils.h"
#include "util/util.h"
#include "util/StdString.h"

#include "AudioDSPSettings.h"
#include "GUIDialogSpeakerGain.h"

using namespace std;
using namespace ADDON;

CDSPSettings::CDSPSettings()
{
  for (int i = 0; i < MAX_CHANNEL; i++)
  {
    m_Settings.m_channels[i].iChannelNumber = -1;
    m_Settings.m_channels[i].iVolumeCorrection = 0;
    m_Settings.m_channels[i].iOldVolumeCorrection = 0;
    m_Settings.m_channels[i].iDistanceCorrection = 0;
    m_Settings.m_channels[i].iOldDistanceCorrection = 0;
    m_Settings.m_channels[i].ptrSpinControl = NULL;
  }

  m_Settings.m_FreeSurround.fInputGain = 1.0f;
  m_Settings.m_FreeSurround.fCircularWrap = 90.0f;
  m_Settings.m_FreeSurround.fShift = 0.0f;
  m_Settings.m_FreeSurround.fDepth = 1.0f;
  m_Settings.m_FreeSurround.fCenterImage = 1.0f;
  m_Settings.m_FreeSurround.fFocus = 0.0f;
  m_Settings.m_FreeSurround.fFrontSeparation = 1.0;
  m_Settings.m_FreeSurround.fRearSeparation = 1.0;
  m_Settings.m_FreeSurround.bLFE = false;
  m_Settings.m_FreeSurround.fLowCutoff = 40.0;
  m_Settings.m_FreeSurround.fHighCutoff = 90.0;
}

int CDSPSettings::TranslateChannelIdToStringId(int channel)
{
  int labelId = -1;
  switch (channel)
  {
    case AE_DSP_CH_FL:    labelId = 30031; break;
    case AE_DSP_CH_FR:    labelId = 30032; break;
    case AE_DSP_CH_FC:    labelId = 30037; break;
    case AE_DSP_CH_LFE:   labelId = 30038; break;
    case AE_DSP_CH_BL:    labelId = 30035; break;
    case AE_DSP_CH_BR:    labelId = 30036; break;
    case AE_DSP_CH_FLOC:  labelId = 30052; break;
    case AE_DSP_CH_FROC:  labelId = 30053; break;
    case AE_DSP_CH_BC:    labelId = 30039; break;
    case AE_DSP_CH_SL:    labelId = 30033; break;
    case AE_DSP_CH_SR:    labelId = 30034; break;
    case AE_DSP_CH_TFL:   labelId = 30054; break;
    case AE_DSP_CH_TFR:   labelId = 30055; break;
    case AE_DSP_CH_TFC:   labelId = 30056; break;
    case AE_DSP_CH_TC:    labelId = 30057; break;
    case AE_DSP_CH_TBL:   labelId = 30063; break;
    case AE_DSP_CH_TBR:   labelId = 30064; break;
    case AE_DSP_CH_TBC:   labelId = 30060; break;
    case AE_DSP_CH_BLOC:  labelId = 30061; break;
    case AE_DSP_CH_BROC:  labelId = 30062; break;
    default: break;
  }
  return labelId;
}

AE_DSP_CHANNEL CDSPSettings::TranslateGUIIdToChannelId(int controlId)
{
  switch (controlId)
  {
    case SPIN_CONTROL_SPEAKER_CH_FL:      return AE_DSP_CH_FL;
    case SPIN_CONTROL_SPEAKER_CH_FR:      return AE_DSP_CH_FR;
    case SPIN_CONTROL_SPEAKER_CH_FC:      return AE_DSP_CH_FC;
    case SPIN_CONTROL_SPEAKER_CH_LFE:     return AE_DSP_CH_LFE;
    case SPIN_CONTROL_SPEAKER_CH_BL:      return AE_DSP_CH_BL;
    case SPIN_CONTROL_SPEAKER_CH_BR:      return AE_DSP_CH_BR;
    case SPIN_CONTROL_SPEAKER_CH_FLOC:    return AE_DSP_CH_FLOC;
    case SPIN_CONTROL_SPEAKER_CH_FROC:    return AE_DSP_CH_FROC;
    case SPIN_CONTROL_SPEAKER_CH_BC:      return AE_DSP_CH_BC;
    case SPIN_CONTROL_SPEAKER_CH_SL:      return AE_DSP_CH_SL;
    case SPIN_CONTROL_SPEAKER_CH_SR:      return AE_DSP_CH_SR;
    case SPIN_CONTROL_SPEAKER_CH_TFL:     return AE_DSP_CH_TFL;
    case SPIN_CONTROL_SPEAKER_CH_TFR:     return AE_DSP_CH_TFR;
    case SPIN_CONTROL_SPEAKER_CH_TFC:     return AE_DSP_CH_TFC;
    case SPIN_CONTROL_SPEAKER_CH_TC:      return AE_DSP_CH_TC;
    case SPIN_CONTROL_SPEAKER_CH_TBL:     return AE_DSP_CH_TBL;
    case SPIN_CONTROL_SPEAKER_CH_TBR:     return AE_DSP_CH_TBR;
    case SPIN_CONTROL_SPEAKER_CH_TBC:     return AE_DSP_CH_TBC;
    case SPIN_CONTROL_SPEAKER_CH_BLOC:    return AE_DSP_CH_BLOC;
    case SPIN_CONTROL_SPEAKER_CH_BROC:    return AE_DSP_CH_BROC;
    default: break;
  }
  return AE_DSP_CH_MAX;
}

bool CDSPSettings::LoadSettingsData(int settingId, bool initial)
{
  TiXmlDocument xmlDoc;
  string strSettingsFile = GetSettingsFile();

  if (!xmlDoc.LoadFile(strSettingsFile))
  {
    if (initial)
    {
      if (!SaveSettingsData())
      {
        XBMC->Log(LOG_ERROR, "failed to create initial settings data file at '%s')", strSettingsFile.c_str());
        return false;
      }
      return true;
    }
    else
      XBMC->Log(LOG_ERROR, "invalid settings data (no/invalid data file found at '%s')", strSettingsFile.c_str());
    return false;
  }

  TiXmlElement *pRootElement = xmlDoc.RootElement();
  if (strcmp(pRootElement->Value(), "demo") != 0)
  {
    if (!initial)
      XBMC->Log(LOG_ERROR, "invalid settings data (no <demo> tag found)");
    return false;
  }

  TiXmlElement *pElement = NULL;
  if (settingId < 0 || settingId == ID_MENU_SPEAKER_GAIN_SETUP || settingId == ID_MENU_SPEAKER_DISTANCE_SETUP)
  {
    pElement = pRootElement->FirstChildElement("channels");
    if (pElement)
    {
      TiXmlNode *pChannelNode = NULL;
      while ((pChannelNode = pElement->IterateChildren(pChannelNode)) != NULL)
      {
        CStdString strTmp;
        sDSPSettings::sDSPChannel channel;

        if (!XMLUtils::GetInt(pChannelNode, "number", channel.iChannelNumber))
          continue;

        if (XMLUtils::GetString(pChannelNode, "name", strTmp))
          channel.strName = strTmp;
        else
          channel.strName = "";

        if (!XMLUtils::GetInt(pChannelNode, "volume", channel.iVolumeCorrection))
          channel.iVolumeCorrection = 0;

        if (!XMLUtils::GetInt(pChannelNode, "distance", channel.iDistanceCorrection))
          channel.iDistanceCorrection = 0;

        m_Settings.m_channels[channel.iChannelNumber].iChannelNumber          = channel.iChannelNumber;
        m_Settings.m_channels[channel.iChannelNumber].iVolumeCorrection       = channel.iVolumeCorrection;
        m_Settings.m_channels[channel.iChannelNumber].iOldVolumeCorrection    = channel.iVolumeCorrection;
        m_Settings.m_channels[channel.iChannelNumber].strName                 = channel.strName;
        m_Settings.m_channels[channel.iChannelNumber].iDistanceCorrection     = channel.iDistanceCorrection;
        m_Settings.m_channels[channel.iChannelNumber].iOldDistanceCorrection  = channel.iDistanceCorrection;
      }
    }
  }

  if (settingId < 0 || settingId == ID_MASTER_PROCESS_FREE_SURROUND)
  {
    pElement = pRootElement->FirstChildElement("freesurround");
    if (pElement)
    {
      if (!XMLUtils::GetFloat(pElement, "inputgain", m_Settings.m_FreeSurround.fInputGain))
        m_Settings.m_FreeSurround.fInputGain = 0.70794576;
      if (!XMLUtils::GetFloat(pElement, "circularwrap", m_Settings.m_FreeSurround.fCircularWrap))
        m_Settings.m_FreeSurround.fCircularWrap = 90;
      if (!XMLUtils::GetFloat(pElement, "shift", m_Settings.m_FreeSurround.fShift))
        m_Settings.m_FreeSurround.fShift = 0.0f;
      if (!XMLUtils::GetFloat(pElement, "depth", m_Settings.m_FreeSurround.fDepth))
        m_Settings.m_FreeSurround.fDepth = 1.0f;
      if (!XMLUtils::GetFloat(pElement, "centerimage", m_Settings.m_FreeSurround.fCenterImage))
        m_Settings.m_FreeSurround.fCenterImage = 1.0f;
      if (!XMLUtils::GetFloat(pElement, "focus", m_Settings.m_FreeSurround.fFocus))
        m_Settings.m_FreeSurround.fFocus = 0.0f;
      if (!XMLUtils::GetFloat(pElement, "frontseparation", m_Settings.m_FreeSurround.fFrontSeparation))
        m_Settings.m_FreeSurround.fFrontSeparation = 1.0;
      if (!XMLUtils::GetFloat(pElement, "rearseparation", m_Settings.m_FreeSurround.fRearSeparation))
        m_Settings.m_FreeSurround.fRearSeparation = 1.0;
      if (!XMLUtils::GetBoolean(pElement, "bassredirection", m_Settings.m_FreeSurround.bLFE))
        m_Settings.m_FreeSurround.bLFE = false;
      if (!XMLUtils::GetFloat(pElement, "lowcutoff", m_Settings.m_FreeSurround.fLowCutoff))
        m_Settings.m_FreeSurround.fLowCutoff = 40.0;
      if (!XMLUtils::GetFloat(pElement, "highcutoff", m_Settings.m_FreeSurround.fHighCutoff))
        m_Settings.m_FreeSurround.fHighCutoff = 90.0;
    }
  }

  return true;
}

bool CDSPSettings::SaveSettingsData()
{
  TiXmlDocument xmlDoc;
  TiXmlElement xmlRootElement("demo");
  TiXmlNode *pRoot = xmlDoc.InsertEndChild(xmlRootElement);
  if (pRoot == NULL)
    return false;

  TiXmlElement xmlChannelsSetting("channels");
  TiXmlNode* pChannelsNode = pRoot->InsertEndChild(xmlChannelsSetting);
  if (pChannelsNode)
  {
    for (int i = 0; i < AE_DSP_CH_MAX; i++)
    {
      TiXmlElement xmlSetting("channel");
      TiXmlNode* pChannelNode = pChannelsNode->InsertEndChild(xmlSetting);
      if (pChannelNode)
      {
        XMLUtils::SetInt(pChannelNode, "number", i);
        XMLUtils::SetString(pChannelNode, "name", m_Settings.m_channels[i].strName.c_str());
        XMLUtils::SetInt(pChannelNode, "volume", m_Settings.m_channels[i].iVolumeCorrection);
        XMLUtils::SetInt(pChannelNode, "distance", m_Settings.m_channels[i].iDistanceCorrection);
      }
    }
  }

  TiXmlElement xmlFreeSurroundSetting("freesurround");
  TiXmlNode* pNode = pRoot->InsertEndChild(xmlFreeSurroundSetting);
  if (pNode)
  {
    XMLUtils::SetFloat(pNode, "inputgain", m_Settings.m_FreeSurround.fInputGain);
    XMLUtils::SetFloat(pNode, "circularwrap", m_Settings.m_FreeSurround.fCircularWrap);
    XMLUtils::SetFloat(pNode, "shift", m_Settings.m_FreeSurround.fShift);
    XMLUtils::SetFloat(pNode, "depth", m_Settings.m_FreeSurround.fDepth);
    XMLUtils::SetFloat(pNode, "centerimage", m_Settings.m_FreeSurround.fCenterImage);
    XMLUtils::SetFloat(pNode, "focus", m_Settings.m_FreeSurround.fFocus);
    XMLUtils::SetFloat(pNode, "frontseparation", m_Settings.m_FreeSurround.fFrontSeparation);
    XMLUtils::SetFloat(pNode, "rearseparation", m_Settings.m_FreeSurround.fRearSeparation);
    XMLUtils::SetBoolean(pNode, "bassredirection", m_Settings.m_FreeSurround.bLFE);
    XMLUtils::SetFloat(pNode, "lowcutoff", m_Settings.m_FreeSurround.fLowCutoff);
    XMLUtils::SetFloat(pNode, "highcutoff", m_Settings.m_FreeSurround.fHighCutoff);
  }

  if (!xmlDoc.SaveFile(GetSettingsFile()))
  {
    XBMC->Log(LOG_ERROR, "failed to write speaker settings data");
    return false;
  }

  return true;
}
