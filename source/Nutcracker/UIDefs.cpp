#include "NutcrackerPCH.h"
#include "UIDefs.h"
#include "Interpreter.h"

namespace nutcracker
{

wxImageList gImageListSmall(16, 16);
wxImageList gImageList32x32(32,32);
class MainWnd* gMainWnd;
bool gShuttingDown;
class WorkspaceWnd* gWorkspaceWnd;
class FileEditorGroupWnd* gFileEditorGroupWnd;
class FindResultsWnd* gFindResultsWnd;
std::shared_ptr<Workspace> gWorkspace;

std::unique_ptr<cz::Parameters> gParameters;

cz::UTF8String wxStringToUtf8(const wxString& str)
{
	return cz::UTF8String(str.ToUTF8().data());
}

void showError(const char* title, const char* fmt, ...)
{
	char buf[2048];
	va_list vp;
	va_start(vp, fmt);

	vsnprintf(buf, sizeof(buf), fmt, vp);
	czWarning(ID_Log, buf);
	wxMessageBox(wxString::FromUTF8(buf), wxString::FromUTF8(title ? title : "Error"), wxOK|wxICON_ERROR);
	va_end(vp);
}

void showMsg(const char* title, const char* fmt, ...)
{
	char buf[2048];
	va_list vp;
	va_start(vp, fmt);

	vsnprintf(buf, sizeof(buf), fmt, vp);
	wxMessageBox(wxString::FromUTF8(buf), wxString::FromUTF8(title ? title : "Error"), wxOK|wxICON_INFORMATION);
	va_end(vp);
}

void showException(std::exception& e)
{
	showError("Exception", "%s", e.what());
}

} // namespace nutcracker
