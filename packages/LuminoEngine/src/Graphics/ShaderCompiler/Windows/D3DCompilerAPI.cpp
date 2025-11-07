// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include <LuminoEngine/Graphics/ShaderCompiler/detail/D3DCompilerAPI.hpp>

namespace ln {
namespace detail {

//==============================================================================
// D3DCompilerAPI

D3DCompilerAPI::PFN_D3DCreateBlob D3DCompilerAPI::D3DCreateBlob;
D3DCompilerAPI::PFN_D3DCompileFromFile D3DCompilerAPI::D3DCompileFromFile;
D3DCompilerAPI::PFN_D3DReflect D3DCompilerAPI::D3DReflect;
HMODULE D3DCompilerAPI::s_hD3DCompilerDLL;

bool D3DCompilerAPI::Initialize()
{
    if (!s_hD3DCompilerDLL) {
        s_hD3DCompilerDLL = ::LoadLibrary(D3DCOMPILER_DLL_W);
        if (s_hD3DCompilerDLL) {
            D3DCreateBlob = reinterpret_cast<PFN_D3DCreateBlob>(::GetProcAddress(s_hD3DCompilerDLL, "D3DCreateBlob"));
            D3DCompileFromFile = reinterpret_cast<PFN_D3DCompileFromFile>(::GetProcAddress(s_hD3DCompilerDLL, "D3DCompileFromFile"));;
            D3DReflect = reinterpret_cast<PFN_D3DReflect>(::GetProcAddress(s_hD3DCompilerDLL, "D3DReflect"));
        }
        else {
            LN_LOG_ERROR("{} not found.", D3DCOMPILER_DLL_A);
            return false;
        }
    }
    return true;
}

// LoadLibrary で読み込み可能な dxcompiler.dll が保存されているフォルダのパスを検索する
std::filesystem::path D3DCompilerAPI::findDXCompilerDLLPath() {
    // LoadLibrary してみて、成功したらそのモジュールが保存されているパスを返す
    const wchar_t* dllNames[] = {
        L"dxcompiler.dll",
    };
    for (const auto& dllName : dllNames) {
        HMODULE hModule = ::LoadLibraryW(dllName);
        if (hModule) {
            wchar_t path[MAX_PATH];
            ::GetModuleFileNameW(hModule, path, MAX_PATH);
            ::FreeLibrary(hModule);
            return std::filesystem::path(path).parent_path();
        }
    }
    return std::filesystem::path();
}

} // namespace detail
} // namespace ln
