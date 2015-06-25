///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __BSAMANAGER_H__
#define __BSAMANAGER_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class BSAManager
///////////////////////////////////////////////////////////////////////////////
class BSAManager : public wxFrame 
{
	private:
	
	protected:
		wxTreeCtrl* bsaTree;
	
	public:
		
		BSAManager( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("BSA Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~BSAManager();
	
};

#endif //__BSAMANAGER_H__
