#include "FileDialogue.h"
#include "Game/GameWindow.h"
#include "glfw/glfw3native.h"
#include <commdlg.h>
#include <windows.h>
#include <shlobj.h>
#include <shobjidl_core.h>



std::string Fracture::FileDialogue::OpenFile(const char* filter)
{		
	//SDL_SysWMinfo 
	HWND hwnd = glfwGetWin32Window(GameWindow::Context());
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	std::string root = "C:/";
	ofn.lpstrInitialDir = root.c_str(); 
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return std::string();
}

std::string Fracture::FileDialogue::OpenFile(const char* filter,std::string& filename)
{
	HWND hwnd = glfwGetWin32Window(GameWindow::Context());
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR stFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrTitle = stFile;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;	
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		std::string name = ofn.lpstrFile;
		const size_t last_slash_idx = name.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
			name.erase(0, last_slash_idx + 1);
		}

		// Remove extension if present.
		const size_t period_idx = name.rfind('.');
		if (std::string::npos != period_idx)
		{
			name.erase(period_idx);
		}
		filename = name;
		return ofn.lpstrFile;
	}
	return std::string();
}

std::string Fracture::FileDialogue::SaveFile(const char* filter)
{
	HWND hwnd = glfwGetWin32Window(GameWindow::Context());
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return std::string();
}

std::string Fracture::FileDialogue::AddAsset()
{
	const char* filter = "Fracture Project (*.fracture)\0*.fracture\0)";
	HWND hwnd = glfwGetWin32Window(GameWindow::Context());
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return std::string();

}

std::string Fracture::FileDialogue::SelectDirectory()
{
	IFileDialog* pfd;
	int max_size = 512;
	char* out[256] = { 0 };

	if (SUCCEEDED(CoCreateInstance(
		CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem* psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				WCHAR* tmp; //SIGDN_DESKTOPABSOLUTEPARSING
				if (SUCCEEDED(psi->GetDisplayName(SIGDN_FILESYSPATH, &tmp)))
				{
					char ch[260];
					char DefChar = ' ';
					WideCharToMultiByte(CP_ACP, 0, tmp, -1, ch, 260, &DefChar, NULL);
					return std::string(ch);
				}
				psi->Release();
			}
		}
		pfd->Release();
	}
	
	return std::string();
}
