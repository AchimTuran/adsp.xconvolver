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

#define CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY  9003
#define BUTTON_START_CHIRP_SIGNAL                   3001
#define BUTTON_STOP_CHIRP_SIGNAL                    3002
#define LABEL_CURRENT_AUDIO_CHANNEL                 3003
#define SPINCONTROLEX_SAMPLE_FREQUENCY              3004
#define SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES     3005
#define LIST_SETTINGS_SELECTION                     900
#define GROUP_SETTINGS_SELECTION                    9000
#define SETTINGS_SELECTION_ELEMENT_ROOM_CORRECTION  9993


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

  m_SpincontrolexCaptureDevice = GUI->Control_getSpin(m_window, SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES);
  if(!m_SpincontrolexCaptureDevice)
  {
    KODI->Log(LOG_ERROR, "Couldn't find GUI \"m_SpincontrolexCaptureDevice\" with control ID=%i", SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES);
  }
  m_SpincontrolexSampleFrequency = GUI->Control_getSpin(m_window, SPINCONTROLEX_SAMPLE_FREQUENCY);

  // create signal player
  m_SignalRecorder  = new CSignalRecorder();
  m_SignalPlayer    = new CSignalPlayer();

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
    m_SpincontrolexCaptureDevice->SetValue(0);
    for(uint ii = 0; ii < m_CaptureDevices[0].sampleFrequencies.size(); ii++)
    {
      m_SpincontrolexSampleFrequency->AddLabel(toString<uint>(m_CaptureDevices[0].sampleFrequencies[ii]).c_str(), ii);
    }
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

      m_window->SetControlLabel(LABEL_CURRENT_AUDIO_CHANNEL, "measure IR");
      //m_SignalPlayer = new CSignalPlayer();
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
      }

      m_SignalPlayer->Create(sampleFrequency);
      m_SignalRecorder->Create(sampleFrequency, deviceName);
      m_SignalRecorder->StartRecording();
      m_SignalPlayer->StartPlaying();
    }
    break;

    case BUTTON_STOP_CHIRP_SIGNAL:
      if(m_SignalPlayer)
      {
        m_SignalPlayer->StopPlaying();
        while(m_SignalPlayer->IsRunning());
        //delete m_SignalPlayer;
        //m_SignalPlayer = NULL;
      }
      if(m_SignalRecorder)
      {
        m_SignalRecorder->StopRecording();
        while(m_SignalRecorder->IsRunning());
        //delete m_SignalPlayer;
        //m_SignalPlayer = NULL;
      }
      m_window->SetControlLabel(LABEL_CURRENT_AUDIO_CHANNEL, "stopped");
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
      }
    }
    break;

    case LABEL_CURRENT_AUDIO_CHANNEL:
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
