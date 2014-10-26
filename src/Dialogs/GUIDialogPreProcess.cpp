#include "GUIDialogPreProcess.h"


CGUIDialogPreProcess::CGUIDialogPreProcess() :
	CGUIDialogBase(	"DialogPreProcess.xml", false, true )
{
}

CGUIDialogPreProcess::~CGUIDialogPreProcess()
{
}

bool CGUIDialogPreProcess::OnInit()
{
	return true;
}

bool CGUIDialogPreProcess::OnClick(int controlId)
{
	return true;
}

bool CGUIDialogPreProcess::OnFocus(int controlId)
{
	return true;
}

bool CGUIDialogPreProcess::OnAction(int actionId)
{
	if(	actionId == ADDON_ACTION_CLOSE_DIALOG ||
		actionId == ADDON_ACTION_PREVIOUS_MENU )
	{
		this->Close();
		return true;
	}
	else
	{
		return false;
	}
}