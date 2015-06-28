#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/tokenzr.h>
#include <vector>
#include <unordered_set>

class BSAManager;

class BSAManagerApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	void InitBSA();
	void LoadTree();
	wxTreeItemId FindItem(wxTreeItemId, const wxString&);

private:
	BSAManager* frame = nullptr;
	std::vector<std::string> tree;
};


class BSAManager : public wxFrame
{
private:
	BSAManagerApp* appRef = nullptr;

protected:
	virtual void bsaTreeOnTreeItemActivated(wxTreeEvent& event);

public:
	wxTreeCtrl* bsaTree = nullptr;

	BSAManager(BSAManagerApp* appRef, wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("BSA Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~BSAManager();
};
