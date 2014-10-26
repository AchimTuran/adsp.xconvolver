#include "GUIDialogMasterProcess.h"


CGUIDialogMasterProcess::CGUIDialogMasterProcess() :
	CGUIDialogBase(	"DialogMasterProcess.xml", false, true )
{
}

CGUIDialogMasterProcess::~CGUIDialogMasterProcess()
{
}

bool CGUIDialogMasterProcess::OnInit()
{
	return true;
}

bool CGUIDialogMasterProcess::OnClick(int controlId)
{
	return true;
}

bool CGUIDialogMasterProcess::OnFocus(int controlId)
{
	return true;
}

bool CGUIDialogMasterProcess::OnAction(int actionId)
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