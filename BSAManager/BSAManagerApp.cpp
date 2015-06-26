#include "BSAManager.h"
#include "BSAManagerApp.h"
#include "FSEngine.h"
#include "FSManager.h"
 
IMPLEMENT_APP(BSAManagerApp)
 
bool BSAManagerApp::OnInit()
{
	frame = new BSAManager(nullptr);
	
	InitBSA();
	LoadTree();

	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

void BSAManagerApp::InitBSA()
{
	for (FSArchiveFile *archive : FSManager::archiveList())
	{
		if (archive)
		{
			archive->fileTree(tree);
		}
	}
}

void BSAManagerApp::LoadTree()
{
	if (frame->bsaTree)
		frame->bsaTree->AddRoot("BSA");

	wxTreeItemId bsaRoot;
	wxTreeItemId currentRoot = frame->bsaTree->GetRootItem();
	for (auto it : tree)
	{
		if (wxString(it).EndsWith(".bsa"))
		{
			currentRoot = frame->bsaTree->GetRootItem();
			bsaRoot.Unset();
		}

		wxArrayString strArray = wxStringTokenize(it, "/");
		for (auto str : strArray)
		{
			wxTreeItemId foundItem = FindItem(currentRoot, str);
			if (!foundItem.IsOk())
			{
				currentRoot = frame->bsaTree->AppendItem(currentRoot, str);
				if (!bsaRoot.IsOk())
					bsaRoot = currentRoot;
			}
			else
			{
				currentRoot = foundItem;
			}
		}

		if (bsaRoot.IsOk())
			currentRoot = bsaRoot;
		else
			currentRoot = frame->bsaTree->GetRootItem();
	}
}

wxTreeItemId BSAManagerApp::FindItem(wxTreeItemId root, const wxString& sSearchFor)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId search;
	wxTreeItemId item = frame->bsaTree->GetFirstChild(root, cookie);
	wxTreeItemId child;
 
	while(item.IsOk())
	{
		wxString sData = frame->bsaTree->GetItemText(item);
		if (sSearchFor.CompareTo(sData) == 0)
		{
			return item;
		}
		if (frame->bsaTree->ItemHasChildren(item))
		{
			wxTreeItemId search = FindItem(item, sSearchFor);
			if (search.IsOk())
			{
				return search;
			}
		}
		item = frame->bsaTree->GetNextChild(root, cookie);
	}
 
	/* Not found */
	wxTreeItemId dummy;
	return dummy;
}
