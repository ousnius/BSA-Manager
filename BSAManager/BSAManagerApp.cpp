#include "BSAManager.h"
#include "BSAManagerApp.h"
#include "FSEngine.h"
#include "FSManager.h"
 
IMPLEMENT_APP(BSAManagerApp)
 
bool BSAManagerApp::OnInit()
{
	frame = new BSAManager(nullptr);
	frame->Show(true);
	SetTopWindow(frame);

	frame->Freeze();
	InitBSA();
	wxMessageBox("Scanned BSAs!");
	LoadTree();
	wxMessageBox("Loaded tree!");
	frame->Thaw();

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
	
	wxString currentSub;
	wxString bsaName;
	std::vector<wxString> existingItems;

	wxTreeItemId bsaRoot;
	wxTreeItemId currentRoot = frame->bsaTree->GetRootItem();
	for (auto it : tree)
	{
		if (wxString(it).EndsWith(".bsa"))
		{
			currentRoot = frame->bsaTree->GetRootItem();
			currentSub.clear();
			bsaRoot.Unset();
		}

		wxArrayString strArray = wxStringTokenize(it, "/");
		for (auto str : strArray)
		{
			wxString searchSub = currentSub;
			if (!searchSub.empty())
				searchSub += "/";
			searchSub += str;

			if (str == "cypress")
				int a = 1;

			if (std::find(existingItems.begin(), existingItems.end(), searchSub) == existingItems.end())
			{
				currentSub = searchSub;
				existingItems.push_back(currentSub);
				currentRoot = frame->bsaTree->AppendItem(currentRoot, str);
				if (!bsaRoot.IsOk())
				{
					bsaRoot = currentRoot;
					bsaName = currentSub;
				}
			}
			else
			{
				wxTreeItemId foundItem = FindItem(currentRoot, str);
				currentRoot = foundItem;
				currentSub = searchSub;
			}
		}

		if (bsaRoot.IsOk())
			currentRoot = bsaRoot;
		else
			currentRoot = frame->bsaTree->GetRootItem();

		currentSub = bsaName;
	}
}

wxTreeItemId BSAManagerApp::FindItem(wxTreeItemId root, const wxString& sSearchFor)
{
	wxTreeItemIdValue cookie;
	wxTreeItemId search;
	wxTreeItemId item = frame->bsaTree->GetFirstChild(root, cookie);
	wxTreeItemId child;
 
	while (item.IsOk())
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
