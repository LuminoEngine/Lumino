// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

namespace ln {
namespace detail {

class D3DCompilerAPI
{
public:
	typedef HRESULT(WINAPI* PFN_D3DCreateBlob)(SIZE_T Size, ID3DBlob** ppBlob);
	typedef HRESULT(WINAPI* PFN_D3DCompileFromFile)(LPCWSTR pFileName,
			CONST D3D_SHADER_MACRO* pDefines,
			ID3DInclude* pInclude,
			LPCSTR pEntrypoint,
			LPCSTR pTarget,
			UINT Flags1,
			UINT Flags2,
			ID3DBlob** ppCode,
			ID3DBlob** ppErrorMsgs);
	typedef HRESULT(WINAPI* PFN_D3DReflect)(LPCVOID pSrcData,
			SIZE_T SrcDataSize,
			REFIID pInterface,
			void** ppReflector);

	static bool Initialize();
	static PFN_D3DCreateBlob D3DCreateBlob;
	static PFN_D3DCompileFromFile D3DCompileFromFile;
	static PFN_D3DReflect D3DReflect;
	static HMODULE s_hD3DCompilerDLL;
};

} // namespace detail
} // namespace ln
