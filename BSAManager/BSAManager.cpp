#include "BSAManager.h"
#include "FSEngine.h"
#include "FSManager.h"


IMPLEMENT_APP(BSAManagerApp)

bool BSAManagerApp::OnInit()
{
	frame = new BSAManager(this, nullptr);
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

int BSAManagerApp::OnExit()
{
	FSManager::del();
	return 0;
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

	std::string currentSub;
	std::string bsaName;
	std::unordered_set<std::string> existingItems;

	wxTreeItemId bsaRoot;
	wxTreeItemId currentRoot = frame->bsaTree->GetRootItem();
	for (auto it : tree)
	{
		wxString iter(it);
		if (iter.EndsWith(".bsa"))
		{
			currentRoot = frame->bsaTree->GetRootItem();
			currentSub.clear();
			bsaRoot.Unset();
		}

		wxArrayString strArray = wxStringTokenize(it, "/");
		for (auto str : strArray)
		{
			std::string searchSub = currentSub;
			if (!searchSub.empty())
				searchSub += "/";
			searchSub += str;

			if (existingItems.find(searchSub) == existingItems.end())
			{
				currentSub = searchSub;
				existingItems.insert(currentSub);
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
		item = frame->bsaTree->GetNextChild(root, cookie);
	}

	/* Not found */
	wxTreeItemId dummy;
	return dummy;
}


BSAManager::BSAManager(BSAManagerApp* appRef, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer(wxVERTICAL);

	bsaTree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS);
	sizer->Add(bsaTree, 1, wxALL | wxEXPAND, 5);


	this->SetSizer(sizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	bsaTree->Connect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler(BSAManager::bsaTreeOnTreeItemActivated), nullptr, this);
}

BSAManager::~BSAManager()
{
	// Disconnect Events
	bsaTree->Disconnect(wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler(BSAManager::bsaTreeOnTreeItemActivated), nullptr, this);
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
