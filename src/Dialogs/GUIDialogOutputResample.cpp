#include "GUIDialogOutputResample.h"

CGUIDialogOutputResample::CGUIDialogOutputResample() :
	CGUIDialogBase(	"DialogMasterProcess.xml", false, true )
{
}

CGUIDialogOutputResample::~CGUIDialogOutputResample()
{
}

bool CGUIDialogOutputResample::OnInit()
{
	return true;
}

bool CGUIDialogOutputResample::OnClick(int controlId)
{
	return true;
}

bool CGUIDialogOutputResample::OnFocus(int controlId)
{
	return true;
}

bool CGUIDialogOutputResample::OnAction(int actionId)
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