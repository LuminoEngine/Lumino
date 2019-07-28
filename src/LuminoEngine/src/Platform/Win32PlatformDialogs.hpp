
#pragma once
#include <Shobjidl.h>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "Win32PlatformWindowManager.hpp"

namespace ln {

class Win32PlatformFileOpenDialog
	: public PlatformFileOpenDialog
{
public:
	enum class Type
	{
		Open,
		Save,
	};

	Win32PlatformFileOpenDialog()
		: m_fileDialog(NULL)
	{
	}

	virtual ~Win32PlatformFileOpenDialog()
	{
		dispose();
	}

    // http://eternalwindows.jp/installer/originalinstall/originalinstall02.html
	void init(Type type)
	{
		if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))	// multi-threaded is not supported
		{
			HRESULT hr;
			if (type == Type::Open)
			{
				hr = CoCreateInstance(
					CLSID_FileOpenDialog,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&m_fileDialog));
			}
			else
			{
				hr = CoCreateInstance(
					CLSID_FileSaveDialog,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&m_fileDialog));

			}
			
			if (LN_ENSURE(SUCCEEDED(hr))) return;
		}
	}

	void dispose()
	{
		if (m_fileDialog != NULL) m_fileDialog->Release();
		CoUninitialize();
	}

	virtual bool showDialog(detail::PlatformWindow* parent) override
	{
		if (LN_REQUIRE(parent != nullptr)) return false;
		HWND hParent = (HWND)PlatformSupport::getWin32WindowHandle(parent);
		HRESULT hr = m_fileDialog->Show(hParent);
		if (SUCCEEDED(hr))
		{
            IShellItem *psi;
            hr = m_fileDialog->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                LPWSTR lpszPath;
                psi->GetDisplayName(SIGDN_FILESYSPATH, &lpszPath);
                m_filePath = String::fromCString(lpszPath);
                CoTaskMemFree(lpszPath);
                psi->Release();
            }

			return true;
		}
		return false;
	}

	virtual Path getFilePath() override
	{
		return m_filePath;
	}

private:
	IFileDialog*	m_fileDialog;
    Path m_filePath;
};

} // namespace ln

