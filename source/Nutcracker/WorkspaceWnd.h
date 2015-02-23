/********************************************************************
	CrazyGaze (http://www.crazygaze.com)
	Author : Rui Figueira
	Email  : rui@crazygaze.com
	
	purpose:
	
*********************************************************************/

#pragma once

#include "TreeCtrlUtilCode.h"
#include "AppEvents.h"
#include "Workspace.h"
#include "AutoUI/WorkspaceWnd_Auto.h"

namespace nutcracker
{

class WorkspaceWnd : public WorkspaceWnd_Auto, public AppEventListener
{
public:
	WorkspaceWnd(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
	virtual ~WorkspaceWnd();

	void locateFile(FileId fileId);

private:
	DECLARE_EVENT_TABLE()
	virtual void OnTreeItemActivated(wxTreeEvent& event) override;
	virtual void OnTreeItemMenu(wxTreeEvent& event) override;
	void OnOpenContainingFolder(wxCommandEvent& event);
	void OnRunScriptFile(wxCommandEvent& event);
	virtual void OnRefreshClick(wxCommandEvent& event) override;

	std::shared_ptr<TreeCtrlUtil::TreeItemData> updateState(const std::shared_ptr<TreeCtrlUtil::TreeItemData>& parent,
															const std::shared_ptr<const BaseItem>& item);
	void updateState();
	wxTreeItemId findFileTreeItem(FileId fileId);

	virtual void onAppEvent(const AppEvent& evt) override;
	TreeCtrlUtil::TreeCtrlData m_treeData;
	FileId m_selectedFileId;
};

} // namespace nutcracker
