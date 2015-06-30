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


#include <template/include/GUIDialogBase.h>
#include "RoomCorrection/SignalPlayer.h"
#include "RoomCorrection/SignalRecorder.h"

#define CONTROL_BUTTON_SELECT_FIR_FITLER_DIRECTORY  9003
#define BUTTON_START_CHIRP_SIGNAL                   3001
#define BUTTON_STOP_CHIRP_SIGNAL                    3002
#define LABEL_CURRENT_AUDIO_CHANNEL                 3003
#define SPINCONTROLEX_SAMPLE_FREQUENCY              3004
#define SPINCONTROLEX_AVAILABLE_CAPTURE_DEVICES     3005
#define LIST_SETTINGS_SELECTION                     900
#define GROUP_SETTINGS_SELECTION                    9000
#define SETTINGS_SELECTION_ELEMENT_ROOM_CORRECTION  9993

class CGUIDialogXConvolverSettings : public CGUIDialogBase
{
  friend class CSignalPlayer;
public:
  CGUIDialogXConvolverSettings(int FocusedSettingsPage=-1);
  ~CGUIDialogXConvolverSettings();

private:
  virtual bool OnInit();
  virtual bool OnClick(int controlId);
  virtual bool OnFocus(int controlId);
  virtual bool OnAction(int actionId);
  virtual void OnClose();

  int                   m_FocusedSettingsPage;
  CSignalPlayer         *m_SignalPlayer;
  CSignalRecorder       *m_SignalRecorder;
  CCaptureDeviceList_t  m_CaptureDevices;
  CAddonGUISpinControl  *m_SpincontrolexCaptureDevice;
  CAddonGUISpinControl  *m_SpincontrolexSampleFrequency;
};
