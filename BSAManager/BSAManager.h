#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/tokenzr.h>
#include <wx/file.h>
#include <wx/filepicker.h>
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
	wxTreeItemId FindItem(wxTreeItemId, const wxString&, const bool& = false);

	int ExportFile(const wxString& bsaName, const wxString& fileName, const wxString& targetPath);
	int ExportFolder(const wxString& bsaName, const wxString& folderName, const wxString& targetPath);

private:
	BSAManager* frame = nullptr;
	std::vector<std::string> tree;
};


class BSAManager : public wxFrame
{
private:
	void bsaTreeOnTreeItemRightClick(wxTreeEvent& event);
	void bsaTreeOnContextMenu(wxCommandEvent& event);

public:
	wxStatusBar* statusBar = nullptr;
	wxTreeCtrl* bsaTree = nullptr;

	BSAManager(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("BSA Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~BSAManager();
};
