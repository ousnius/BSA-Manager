///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "BSAManager.h"

///////////////////////////////////////////////////////////////////////////

BSAManager::BSAManager( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );
	
	bsaTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxTR_TWIST_BUTTONS );
	sizer->Add( bsaTree, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	bsaTree->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( BSAManager::bsaTreeOnTreeItemActivated ), NULL, this );
}

BSAManager::~BSAManager()
{
	// Disconnect Events
	bsaTree->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( BSAManager::bsaTreeOnTreeItemActivated ), NULL, this );
	
}

void BSAManager::bsaTreeOnTreeItemActivated(wxTreeEvent& event)
{
	wxString text;
	wxTreeItemId id = event.GetItem();
	while (id.IsOk())
	{
		text.Prepend("/" + bsaTree->GetItemText(id));
		wxTreeItemId parent = bsaTree->GetItemParent(id);
		if (parent.IsOk() && parent != bsaTree->GetRootItem())
		{
			id = parent;
		}
		else
		{
			break;
		}
	}
	text.Remove(0, 1);

	int bsaNamePos = text.Find(".bsa");
	if (bsaNamePos != wxNOT_FOUND)
	{
		wxString bsaName = text.SubString(0, bsaNamePos + 3);
		wxMessageBox("BSA name of selected item: " + bsaName);

		wxString fileName = text.AfterFirst('/');
		wxMessageBox("File name of selected item: " + fileName);
	}
}
