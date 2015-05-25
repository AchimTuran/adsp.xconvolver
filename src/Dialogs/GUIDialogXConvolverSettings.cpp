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
#include <template/include/typedefs.h>
#include <string>
using namespace std;

#define CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY  9003
#define BUTTON_START_CHIRP_SIGNAL                   3001
#define BUTTON_STOP_CHIRP_SIGNAL                    3002
#define LABEL_CURRENT_AUDIO_CHANNEL                 3003

CGUIDialogXConvolverSettings::CGUIDialogXConvolverSettings(int FocusedControl) :
  CGUIDialogBase("DialogXConvolverSettings.xml", false, true)
{
  if (FocusedControl >= 0)
  {
    m_FocusedControl = FocusedControl;
  }
}

CGUIDialogXConvolverSettings::~CGUIDialogXConvolverSettings()
{
  if(m_SignalPlayer)
  {
    m_SignalPlayer->StopPlaying();
    while(m_SignalPlayer->IsRunning());
    delete m_SignalPlayer;
  }
}

bool CGUIDialogXConvolverSettings::OnInit()
{
  if (m_FocusedControl >= 0)
  { // ToDo: Set Focus control
    m_window->SetFocusId(900);
    m_window->SetCurrentListPosition(m_FocusedControl);
  }

  return true;
}

bool CGUIDialogXConvolverSettings::OnClick(int controlId)
{
  switch(controlId)
  {
    case BUTTON_START_CHIRP_SIGNAL:
      if(m_SignalPlayer)
      {
        m_SignalPlayer->StopPlaying();
        while(m_SignalPlayer->IsRunning());
        delete m_SignalPlayer;
        m_SignalPlayer = NULL;
      }

      m_SignalPlayer = new CSignalPlayer();
      m_SignalPlayer->Create();
      m_SignalPlayer->StartPlaying();
    break;

    case BUTTON_STOP_CHIRP_SIGNAL:
      if(m_SignalPlayer)
      {
        m_SignalPlayer->StopPlaying();
        while(m_SignalPlayer->IsRunning());
        delete m_SignalPlayer;
        m_SignalPlayer = NULL;
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
}
