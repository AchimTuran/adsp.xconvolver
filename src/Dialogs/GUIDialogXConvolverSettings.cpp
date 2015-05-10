/*
*		Copyright (C) 2014 Achim Turan, Achim.Turan@o2online.de
*		https://github.com/AchimTuran/LibXConvolver
*
*	This file is part of LibXConvolver
*
*	LibXConvolver is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	LibXConvolver is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with LibXConvolver.  If not, see <http://www.gnu.org/licenses/>.
*
*/



#include "GUIDialogXConvolverSettings.h"

#define CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY  9003

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

}