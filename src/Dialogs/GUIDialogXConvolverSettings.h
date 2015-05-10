#pragma once

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


#include "template/include/GUIDialogBase.h"

class CGUIDialogXConvolverSettings : public CGUIDialogBase
{
public:
  CGUIDialogXConvolverSettings(int FocusedControl=-1);
  ~CGUIDialogXConvolverSettings();

private:
  virtual bool OnInit();
  virtual bool OnClick(int controlId);
  virtual bool OnFocus(int controlId);
  virtual bool OnAction(int actionId);

  int m_FocusedControl;
};