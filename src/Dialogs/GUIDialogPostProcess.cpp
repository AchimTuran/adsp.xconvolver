#include "GUIDialogPostProcess.h"


CGUIDialogPostProcess::CGUIDialogPostProcess() :
	CGUIDialogBase(	"DialogMasterProcess.xml", false, true )
{
}

CGUIDialogPostProcess::~CGUIDialogPostProcess()
{
}

bool CGUIDialogPostProcess::OnInit()
{
	return true;
}

bool CGUIDialogPostProcess::OnClick(int controlId)
{
	return true;
}

bool CGUIDialogPostProcess::OnFocus(int controlId)
{
	return true;
}

bool CGUIDialogPostProcess::OnAction(int actionId)
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