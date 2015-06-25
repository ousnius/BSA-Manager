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
	
	bsaTree = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE|wxTR_TWIST_BUTTONS );
	sizer->Add( bsaTree, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
}

BSAManager::~BSAManager()
{
}
