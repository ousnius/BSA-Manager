#pragma once

#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/treectrl.h>
#include <wx/tokenzr.h>
#include <wx/file.h>
#include <wx/filepicker.h>
#include <wx/dnd.h>
#include <wx/stdpaths.h>
#include <vector>
#include <unordered_set>

class BSAManager;

class BSAManagerApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	virtual void OnInitCmdLine(wxCmdLineParser& parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

	void InitBSA(wxArrayString);
	void LoadTree();
	wxTreeItemId FindItem(wxTreeItemId, const wxString&, const bool& = false);

	int ExportFile(const wxString& bsaName, const wxString& fileName, const wxString& targetPath);
	int ExportFolder(const wxString& bsaName, const wxString& folderName, const wxString& targetPath);

private:
	BSAManager* frame = nullptr;
	std::vector<std::string> tree;

	wxArrayString cmdFiles;
};

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
	{ wxCMD_LINE_PARAM, "f", "files", "loads the files on init", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
	{ wxCMD_LINE_NONE }
};


class BSAManager : public wxFrame
{
private:
	void bsaTreeOnTreeItemRightClick(wxTreeEvent& event);
	void bsaTreeOnTreeBeginDrag(wxTreeEvent& event);
	void bsaTreeOnContextMenu(wxCommandEvent& event);
	
	void menuFileClicked(wxCommandEvent& event);

public:
	wxMenuBar* menuBar = nullptr;
	wxStatusBar* statusBar = nullptr;
	wxTreeCtrl* bsaTree = nullptr;

	BSAManager(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "BSA Manager", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500, 300), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~BSAManager();
};
