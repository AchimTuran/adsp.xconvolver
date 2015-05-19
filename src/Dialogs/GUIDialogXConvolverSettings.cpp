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
#include <string>
using namespace std;

#if defined(TARGET_WINDOWS)
  define PATH_SEPARATOR_SMYBOL string("\\")
#else
  #define PATH_EPARATOR_SYMBOL string("/")
#endif

#define CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY  9003

CGUIDialogXConvolverSettings::CGUIDialogXConvolverSettings(int FocusedControl) :
  CGUIDialogBase("DialogXConvolverSettings.xml", false, true)
{
  if (FocusedControl >= 0)
  {
    m_FocusedControl = FocusedControl;
  }
  m_WaveSignal = NULL;
}

CGUIDialogXConvolverSettings::~CGUIDialogXConvolverSettings()
{
}

bool CGUIDialogXConvolverSettings::OnInit()
{
  if (m_FocusedControl >= 0)
  { // ToDo: Set Focus control
    m_window->SetFocusId(900);
    m_window->SetCurrentListPosition(m_FocusedControl);
  }

  m_WaveSignal = new CWaveSignal(g_strAddonPath + PATH_EPARATOR_SYMBOL + string("measurement.signals") + PATH_EPARATOR_SYMBOL + string("ess_10_20000_fs44100_15s.wav"));
  if(!m_WaveSignal)
  {
    // ToDo: throw error!
  }

  return true;
}

bool CGUIDialogXConvolverSettings::OnClick(int controlId)
{
  //if (controlId == CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY)
  //{
  //  GUI->control_get
  //}

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
}

void CGUIDialogXConvolverSettings::OnClose()
{
  if(!m_WaveSignal)
  {
    delete m_WaveSignal;
    m_WaveSignal = NULL;
  }
}
