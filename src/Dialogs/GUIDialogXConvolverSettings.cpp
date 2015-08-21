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



#include "GUIDialogXConvolverSettings.h"
#include <asplib/asplib_utils/strings/stdStringUtils.h>
#include <template/include/typedefs.h>
#include <string>

using namespace std;
using namespace ADDON;

CGUIDialogXConvolverSettings::CGUIDialogXConvolverSettings(int FocusedSettingsPage) :
  CGUIDialogBase("DialogXConvolverSettings.xml", false, true)
{
  if(FocusedSettingsPage >= 0)
  {
    m_FocusedSettingsPage = FocusedSettingsPage;
  }
  else
  {
    m_FocusedSettingsPage = -1;
  }

  m_SignalPlayer = NULL;
}

CGUIDialogXConvolverSettings::~CGUIDialogXConvolverSettings()
{
}

bool CGUIDialogXConvolverSettings::OnInit()
{
  //{
  //  string script = g_strAddonPath + string("\\python.scripts\\") + string("HelloWorld.py");
  //  std::vector<std::string> arguments;
  //  arguments.push_back("plugin://adsp.xconvolver/python.scripts/");
  //  arguments.push_back("1");
  //  arguments.push_back("");
  //  INTERFACES->ExecuteScriptAsync(script, "adsp.xconvolver", arguments);
  //}

  if (m_FocusedSettingsPage >= 0)
  { // ToDo: Set Focus control
    m_window->SetFocusId(LIST_SETTINGS_SELECTION);
    if(m_FocusedSettingsPage < m_window->GetListSize())
    {
      m_window->SetCurrentListPosition(m_FocusedSettingsPage);
    }
    else
    {
      m_window->SetCurrentListPosition(0);
    }
  }

  m_SpincontrolexCaptureDevice    = GUI->Control_getSpin(m_window, SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES);
  m_SpincontrolexSampleFrequency  = GUI->Control_getSpin(m_window, SPINCONTROLEX_SAMPLE_FREQUENCY);
  m_SpincontrolexChannels         = GUI->Control_getSpin(m_window, SPINCONTROLEX_CHANNELS);
  if(!m_SpincontrolexCaptureDevice || !m_SpincontrolexSampleFrequency || !m_SpincontrolexChannels)
  {
    KODI->Log(LOG_ERROR,  "Couldn't find a necessary control ID! Closing dialog. Please check your skin file. For ID={%i, %i, %i}.",
                          SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES, SPINCONTROLEX_SAMPLE_FREQUENCY, SPINCONTROLEX_CHANNELS);
    return false;
  }

  // create signal player
  m_SignalRecorder  = new CSignalRecorder("capture_");
  m_SignalPlayer    = new CSignalPlayer(m_SignalRecorder, this);

  // set capture device names
  vector<uint> sampleFrequencies;
  //sampleFrequencies.push_back(5512);
  sampleFrequencies.push_back(8000);
  sampleFrequencies.push_back(11025);
  sampleFrequencies.push_back(16000);
  sampleFrequencies.push_back(22050);
  sampleFrequencies.push_back(32000);
  sampleFrequencies.push_back(44100);
  sampleFrequencies.push_back(48000);
  sampleFrequencies.push_back(64000);
  sampleFrequencies.push_back(88200);
  sampleFrequencies.push_back(96000);
  sampleFrequencies.push_back(176400);
  sampleFrequencies.push_back(192000);
  sampleFrequencies.push_back(384000);

  m_SignalRecorder->Get_AvailableDevices(m_CaptureDevices, sampleFrequencies);
  
  for(uint ii = 0; ii < m_CaptureDevices.size(); ii++)
  {
    m_SpincontrolexCaptureDevice->AddLabel(m_CaptureDevices[ii].name.c_str(), ii);
  }

  if(m_CaptureDevices.size() <= 0)
  {
    // ToDo: Implement measurement disabling function
    KODI->Log(LOG_ERROR, "No capture devices found! Measuring function will be disabled.");
  }
  else
  {
    //m_SpincontrolexCaptureDevice->SetValue(0);
    m_SpincontrolexSampleFrequency->AddLabel(toString<uint>(m_CaptureDevices[0].defaultSampleFrequency).c_str(), 0);
    for(uint ii = 0; ii < m_CaptureDevices[0].sampleFrequencies.size(); ii++)
    {
      m_SpincontrolexSampleFrequency->AddLabel(toString<uint>(m_CaptureDevices[0].sampleFrequencies[ii]).c_str(), ii +1);
    }

    m_SpincontrolexChannels->AddLabel(toString<uint>(1).c_str(), 0);
    if(m_CaptureDevices[0].maxChannels > 1)
    {
      for(uint ch = 2; ch <= m_CaptureDevices[0].maxChannels; ch++)
      {
        m_SpincontrolexChannels->AddLabel(toString<uint>(ch).c_str(), ch -1);
      }
    }
    m_SpincontrolexChannels->SetValue(0);
  }

  return true;
}

bool CGUIDialogXConvolverSettings::OnClick(int controlId)
{
  switch(controlId)
  {
    case BUTTON_START_CHIRP_SIGNAL:
    {
      if(m_SignalPlayer->IsRunning())
      {
        m_SignalPlayer->StopPlaying();
        while(m_SignalPlayer->IsRunning());
      }

      if(m_SignalRecorder->IsRunning())
      {
        m_SignalRecorder->StopRecording();
        while(m_SignalRecorder->IsRunning());
      }

      SetMeasurementStatus(string("started measurement process"));
      string deviceName = "";
      int deviceIdx = m_SpincontrolexCaptureDevice->GetValue();
      uint sampleFrequency = 44100;
      if(deviceIdx < (int)m_CaptureDevices.size() && deviceIdx >= 0)
      {
        deviceName = m_CaptureDevices[deviceIdx].name;
        int sampleFrequencyIdx = m_SpincontrolexSampleFrequency->GetValue();
        if(sampleFrequencyIdx < (int)m_CaptureDevices[deviceIdx].sampleFrequencies.size() && sampleFrequencyIdx >= 0)
        {
          sampleFrequency = m_CaptureDevices[deviceIdx].sampleFrequencies[sampleFrequencyIdx];
        }
        else
        {
          sampleFrequency = m_CaptureDevices[deviceIdx].defaultSampleFrequency;
        }
      }
      
      uint captureChannels = m_SpincontrolexChannels->GetValue() + 1;
      m_SignalRecorder->Create(sampleFrequency, captureChannels, deviceName);
      m_SignalPlayer->Create(sampleFrequency);
      
      m_SignalPlayer->StartPlaying();
    }
    break;

    case BUTTON_STOP_CHIRP_SIGNAL:
      if(m_SignalPlayer)
      {
        m_SignalPlayer->StopPlaying();
        while(m_SignalPlayer->IsRunning());
      }
      SetMeasurementStatus(string("Measuring stopped"));

      //{
      //  string script = g_strAddonPath + string("\\python.scripts\\") + string("HelloWorld.py");
      //  std::vector<std::string> arguments;
      //  arguments.push_back("plugin://adsp.xconvolver/python.scripts/");
      //  arguments.push_back("1");
      //  arguments.push_back("");
      //  INTERFACES->ExecuteScriptAsync(script, "adsp.xconvolver", arguments);
      //}

    break;

    case SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES:
    {
      m_SpincontrolexSampleFrequency->Clear();
      int deviceIdx = m_SpincontrolexCaptureDevice->GetValue();
      if(deviceIdx < (int)m_CaptureDevices.size() && deviceIdx >= 0)
      {
        for(uint ii = 0; ii < m_CaptureDevices[deviceIdx].sampleFrequencies.size(); ii++)
        {
          m_SpincontrolexSampleFrequency->AddLabel(toString<uint>(m_CaptureDevices[deviceIdx].sampleFrequencies[ii]).c_str(), ii);
        }

        if(m_CaptureDevices[deviceIdx].sampleFrequencies.size() > 0)
        {
          m_SpincontrolexSampleFrequency->SetValue(0);
        }

        m_SpincontrolexChannels->Clear();
        m_SpincontrolexChannels->AddLabel(toString<uint>(1).c_str(), 0);
        if(m_CaptureDevices[deviceIdx].maxChannels > 1)
        {
          for(uint ch = 2; ch <= m_CaptureDevices[deviceIdx].maxChannels; ch++)
          {
            m_SpincontrolexChannels->AddLabel(toString<uint>(ch).c_str(), ch - 1);
          }
        }
        m_SpincontrolexChannels->SetValue(0);
      }
    }
    break;

    case LABEL_CURRENT_AUDIO_CHANNEL:
    {
    }
    break;

    default:
      return false;
    break;
  }

  return true;
}

bool CGUIDialogXConvolverSettings::OnFocus(int controlId)
{
  switch(controlId)
  {
    case SETTINGS_SELECTION_ELEMENT_ROOM_CORRECTION:
    break;

    default:
      return false;
    break;
  }

  return true;
}

bool CGUIDialogXConvolverSettings::OnAction(int actionId)
{
  if (actionId == ADDON_ACTION_CLOSE_DIALOG ||
      actionId == ADDON_ACTION_NAV_BACK     ||
      actionId == ADDON_ACTION_PREVIOUS_MENU)
  {
    this->Close();
    return true;
  }
  else
  {
    return false;
  }
  //if(actionId == ADDON_ACTION_CLOSE_DIALOG ||
  //   actionId == ADDON_ACTION_PREVIOUS_MENU ||
  //   actionId == ADDON_ACTION_NAV_BACK)
  //{
  //  return OnClick(BUTTON_CANCEL);
  //}
  //else
  //{
  //  return false;
  //}
}

void CGUIDialogXConvolverSettings::OnClose()
{
  if(m_SignalPlayer)
  {
    m_SignalPlayer->StopPlaying();
    while(m_SignalPlayer->IsRunning());
    delete m_SignalPlayer;
    m_SignalPlayer = NULL;
  }

  if(m_SignalRecorder)
  {
    m_SignalRecorder->StopRecording();
    while(m_SignalRecorder->IsRunning());
    delete m_SignalRecorder;
    m_SignalRecorder = NULL;
  }

  if(m_SpincontrolexCaptureDevice)
  {
    GUI->Control_releaseSpin(m_SpincontrolexCaptureDevice);
    m_SpincontrolexCaptureDevice = NULL;
  }

  if(m_SpincontrolexSampleFrequency)
  {
    GUI->Control_releaseSpin(m_SpincontrolexSampleFrequency);
    m_SpincontrolexSampleFrequency = NULL;
  }
}

bool CGUIDialogXConvolverSettings::SetMeasurementStatus(std::string &Message)
{
  if(!m_window)
  {
    return false;
  }
  
  m_window->SetControlLabel(LABEL_CURRENT_AUDIO_CHANNEL, Message.c_str());
  return true;
}