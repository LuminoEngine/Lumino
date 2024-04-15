
#pragma once
#include <Shobjidl.h>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "Win32PlatformWindowManager.hpp"

namespace ln {

class Win32PlatformFileSystemDialog
{
public:
    enum class Type
    {
        OpenFile,
        SaveFile,
        SelectFolder,
    };

    Win32PlatformFileSystemDialog()
        : m_fileDialog(NULL)
    {
    }

    ~Win32PlatformFileSystemDialog()
    {
        dispose();
    }

    // http://eternalwindows.jp/installer/originalinstall/originalinstall02.html
    void init(Type type)
    {
        if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))	// multi-threaded is not supported
        {
            HRESULT hr;
            if (type == Type::OpenFile)
            {
                hr = CoCreateInstance(
                    CLSID_FileOpenDialog,
                    NULL,
                    CLSCTX_INPROC_SERVER,
                    IID_PPV_ARGS(&m_fileDialog));
            }
            else if (type == Type::SaveFile)
            {
                hr = CoCreateInstance(
                    CLSID_FileSaveDialog,
                    NULL,
                    CLSCTX_INPROC_SERVER,
                    IID_PPV_ARGS(&m_fileDialog));
            }
            else if (type == Type::SelectFolder)
            {
                hr = CoCreateInstance(
                    CLSID_FileOpenDialog,
                    NULL,
                    CLSCTX_INPROC_SERVER,
                    IID_PPV_ARGS(&m_fileDialog));
                DWORD dwOptions = 0;
                m_fileDialog->GetOptions(&dwOptions);
                m_fileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);
            }

            if (LN_ENSURE(SUCCEEDED(hr))) return;
        }
    }

    void dispose()
    {
        if (m_fileDialog) {
            m_fileDialog->Release();
            m_fileDialog = NULL;
        }
        CoUninitialize();
    }

    bool showDialog(PlatformWindow* parent)
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
                m_path = String::fromCString(lpszPath);
                CoTaskMemFree(lpszPath);
                psi->Release();
            }

            return true;
        }
        return false;
    }

    Path getPath() const
    {
        return m_path;
    }

private:
    IFileDialog* m_fileDialog;
    Path m_path;
};

class Win32PlatformOpenFileDialog
	: public PlatformOpenFileDialog
{
public:
    virtual ~Win32PlatformOpenFileDialog() { m_dialog.dispose(); }
    void init() { m_dialog.init(Win32PlatformFileSystemDialog::Type::OpenFile); }
    virtual bool showDialog(PlatformWindow* parent) override { return m_dialog.showDialog(parent); }
    virtual Path getPath() override { return m_dialog.getPath(); }

private:
    Win32PlatformFileSystemDialog m_dialog;
};

class Win32PlatformSelectFolderDialog
    : public PlatformSelectFolderDialog
{
public:
    virtual ~Win32PlatformSelectFolderDialog() { m_dialog.dispose(); }
    void init() { m_dialog.init(Win32PlatformFileSystemDialog::Type::SelectFolder); }
    virtual bool showDialog(PlatformWindow* parent) override { return m_dialog.showDialog(parent); }
    virtual Path getPath() override { return m_dialog.getPath(); }

private:
    Win32PlatformFileSystemDialog m_dialog;
};

} // namespace ln

