#ifndef __BSAMANAGERAPP_H__
#define __BSAMANAGERAPP_H__

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/treectrl.h>
#include <vector>

class BSAManagerApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	void InitBSA();
	void LoadTree();
	wxTreeItemId FindItem(wxTreeItemId, const wxString&);

private:
	BSAManager* frame;
	std::vector<std::string> tree;
};

#endif //__BSAMANAGERAPP_H__
