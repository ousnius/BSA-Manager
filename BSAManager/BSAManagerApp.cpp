#include "BSAManager.h"
#include "BSAManagerApp.h"
#include "FSEngine.h"
#include "FSManager.h"
 
IMPLEMENT_APP(BSAManagerApp)
 
bool BSAManagerApp::OnInit()
{
	BSAManager *frame = new BSAManager(nullptr);
	frame->Show(true);
	SetTopWindow(frame);

	wxString bsaNames;
	for (FSArchiveFile *archive : FSManager::archiveList())
	{
		if (archive)
		{
			bsaNames += archive->name() + "\n";
		}
	}

	wxMessageBox(bsaNames);

	return true;
}
