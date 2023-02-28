// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include <LuminoGraphicsRHI/ShaderCompiler/detail/D3DCompilerAPI.hpp>

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

} // namespace detail
} // namespace ln
