#include "BSAManager.h"
#include "FSEngine.h"
#include "FSManager.h"


IMPLEMENT_APP(BSAManagerApp)
wxDECLARE_APP(BSAManagerApp);

bool BSAManagerApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	frame = new BSAManager(nullptr);
	frame->bsaTree->SetDropTarget(new DnDBSA(frame->bsaTree));
	frame->Show(true);
	SetTopWindow(frame);

	InitBSA(cmdFiles);

	return true;
}

int BSAManagerApp::OnExit()
{
	FSManager::del();
	return 0;
}

void BSAManagerApp::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(cmdLineDesc);
}

bool BSAManagerApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
	cmdFiles.Clear();

	for (size_t i = 0; i < parser.GetParamCount(); i++)
		cmdFiles.Add(parser.GetParam(i));

	return true;
}

void BSAManagerApp::InitBSA(wxArrayString files)
{
	frame->bsaTree->Freeze();
	frame->statusBar->SetStatusText("Initializing BSAs...");

	frame->bsaTree->DeleteAllItems();
	tree.clear();
	FSManager::del();
	FSManager::addArchives(files);

	for (FSArchiveFile *archive : FSManager::archiveList())
	{
		if (archive)
		{
			archive->fileTree(tree);
		}
	}

	frame->statusBar->SetStatusText("Loading tree controls...");
	LoadTree();
	frame->statusBar->SetStatusText("Ready!");
	frame->bsaTree->Thaw();
}

void BSAManagerApp::LoadTree()
{
	if (frame->bsaTree && !frame->bsaTree->GetRootItem().IsOk())
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

		wxArrayString strArray = wxStringTokenize(iter, "/");
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
				currentRoot = frame->bsaTree->AppendItem(
					currentRoot, str, -1, -1, new BSATreeItemData(currentSub));

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

wxTreeItemId BSAManagerApp::FindItem(wxTreeItemId root, const wxString& sSearchFor, const bool& withChildren)
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

		if (withChildren && frame->bsaTree->ItemHasChildren(item))
		{
			wxTreeItemId search = FindItem(item, sSearchFor, withChildren);
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

int BSAManagerApp::ExportFile(const wxString& bsaName, const wxString& fileName, const wxString& targetPath)
{
	wxFileName itInfo(targetPath);
	if (!wxFileName::Mkdir(itInfo.GetPath(), wxPosixPermissions::wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
		return 3;

	for (FSArchiveFile *archive : FSManager::archiveList())
	{
		if (archive && archive->name() == bsaName)
		{
			wxMemoryBuffer data;
			archive->fileContents(fileName, data);
			if (!data.IsEmpty())
			{
				wxFile file(targetPath, wxFile::OpenMode::write);
				if (file.IsOpened())
				{
					file.Write(data.GetData(), data.GetBufSize());
					file.Close();
				}
				else
				{
					return 2;
				}
			}
			else
			{
				return 1;
			}
			break;
		}
	}
	return 0;
}

int BSAManagerApp::ExportFolder(const wxString& bsaName, const wxString& folderName, const wxString& targetPath)
{
	std::vector<std::string> bsaTree;
	for (FSArchiveFile *archive : FSManager::archiveList())
	{
		if (archive && archive->name() == bsaName)
		{
			archive->addFilesOfFolders(folderName, bsaTree);
		}
	}

	for (auto it : bsaTree)
	{
		if (it.compare(folderName))
		{
			wxFileName itInfo(it);
			if (!wxFileName::Mkdir(targetPath + "/" + itInfo.GetPath(), wxPosixPermissions::wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
				continue;

			if (ExportFile(bsaName, it, targetPath + "/" + it))
				continue;
		}
	}
	return 0;
}


BSAManager::BSAManager(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	SetSizeHints(wxDefaultSize, wxDefaultSize);
	
	menuBar = new wxMenuBar();

	wxMenu* menuFile = new wxMenu();
	menuFile->Append(0, "Open BSA...");
	menuBar->Append(menuFile, "File");

	SetMenuBar(menuBar);
	statusBar = CreateStatusBar();

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer(wxVERTICAL);

	bsaTree = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxTR_MULTIPLE);
	sizer->Add(bsaTree, 1, wxALL | wxEXPAND, 0);

	SetSizer(sizer);
	Layout();

	Centre(wxBOTH);

	// Bind Events
	bsaTree->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &BSAManager::bsaTreeOnTreeItemRightClick, this);

	menuFile->Bind(wxEVT_MENU, &BSAManager::menuFileClicked, this);
}

BSAManager::~BSAManager()
{
	// Unbind Events
	bsaTree->Unbind(wxEVT_TREE_ITEM_RIGHT_CLICK, &BSAManager::bsaTreeOnTreeItemRightClick, this);
}

void BSAManager::bsaTreeOnTreeItemRightClick(wxTreeEvent& WXUNUSED(event))
{
	wxMenu* menu = new wxMenu();
	if (menu)
	{
		menu->Append(0, "Export File(s)");
		menu->Bind(wxEVT_MENU, &BSAManager::bsaTreeOnContextMenu, this);
		PopupMenu(menu);
		menu->Unbind(wxEVT_MENU, &BSAManager::bsaTreeOnContextMenu, this);
		delete menu;
	}
}

void BSAManager::bsaTreeOnContextMenu(wxCommandEvent& event)
{
	switch (event.GetId())
	{
		// Export selected
		case 0:
			statusBar->SetStatusText("Exporting file(s)...");
			wxString targetFolderStr;
			wxString targetFileStr;
			wxArrayTreeItemIds selections;
			bsaTree->GetSelections(selections);

			for (size_t i = 0; i < selections.Count(); i++)
			{
				wxTreeItemId id = selections[i];
				wxString text;
				if (id.IsOk())
					text = ((BSATreeItemData*)bsaTree->GetItemData(id))->GetPath();

				int bsaNamePos = text.Find(".bsa");
				if (bsaNamePos != wxNOT_FOUND)
				{
					wxString bsaName(text.SubString(0, bsaNamePos + 3));
					wxString filePath(text.AfterFirst('/'));
					wxFileName fileInfo(filePath);
					wxString fileExt(fileInfo.GetExt());
					
					// Export folder or multiple files
					if (fileExt.empty() || selections.Count() > 1)
					{
						// Ignore BSA root
						if (filePath.empty())
							break;

						// Only show dialog once
						if (targetFolderStr.empty())
						{
							wxDirDialog targetFolder(this, "Select a target folder", wxEmptyString, wxDIRP_DEFAULT_STYLE);
							if (targetFolder.ShowModal() == wxID_CANCEL)
								break;

							targetFolderStr = targetFolder.GetPath();
						}

						if (!fileExt.empty())
						{
							if (wxGetApp().ExportFile(bsaName, filePath, targetFolderStr + "/" + filePath))
							{
								wxMessageBox("Could not export file.", "Error", wxICON_ERROR);
								break;
							}
						}
						else
						{
							if (wxGetApp().ExportFolder(bsaName, filePath, targetFolderStr))
							{
								wxMessageBox(wxString::Format("Could not export folder: %s", filePath), "Error", wxICON_ERROR);
								break;
							}
						}
					}
					// Export file
					else
					{
						// Only show dialog once
						if (targetFileStr.empty())
						{
							wxFileDialog targetFile(this, "Select a target file", wxEmptyString, fileInfo.GetName(), wxString::Format("*.%s", fileExt), wxFD_SAVE);
							if (targetFile.ShowModal() == wxID_CANCEL)
								break;

							targetFileStr = targetFile.GetPath();
						}

						if (wxGetApp().ExportFile(bsaName, filePath, targetFileStr))
						{
							wxMessageBox("Could not export file.", "Error", wxICON_ERROR);
							break;
						}
					}
				}
			}
			break;
	}
	statusBar->SetStatusText("Ready!");
}

void BSAManager::menuFileClicked(wxCommandEvent& event)
{
	switch (event.GetId())
	{
		// Open
		case 0:
			wxFileDialog file(this, "Choose one or more archives...", wxEmptyString, wxEmptyString, "BSA|*.bsa", wxFD_DEFAULT_STYLE | wxFD_MULTIPLE);
			file.ShowModal();

			wxArrayString files;
			file.GetPaths(files);

			wxGetApp().InitBSA(files);
			break;
	}
}


bool DnDBSA::OnDropFiles(wxCoord, wxCoord, const wxArrayString& fileNames)
{
    if (owner)
    {
		wxArrayString bsaFileNames;
		for (auto &fn : fileNames)
			if (fn.EndsWith(".bsa"))
				bsaFileNames.Add(fn);

		if (!bsaFileNames.IsEmpty())
			wxGetApp().InitBSA(bsaFileNames);
    }
	else
		return false;

    return true;
}
