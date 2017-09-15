
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Lumino/Base/DllLoader.h>
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

class DX9Module
{
public:

	typedef IDirect3D9 * (WINAPI *MD_Direct3DCreate9)(UINT SDKVersion);

	typedef HRESULT(WINAPI *MD_D3DXCreateTextureFromFileInMemoryEx)(
		LPDIRECT3DDEVICE9         pDevice,
		LPCVOID                   pSrcData,
		UINT                      SrcDataSize,
		UINT                      Width,
		UINT                      Height,
		UINT                      MipLevels,
		DWORD                     Usage,
		D3DFORMAT                 Format,
		D3DPOOL                   Pool,
		DWORD                     Filter,
		DWORD                     MipFilter,
		D3DCOLOR                  ColorKey,
		D3DXIMAGE_INFO*           pSrcInfo,
		PALETTEENTRY*             pPalette,
		LPDIRECT3DTEXTURE9*       ppTexture);

	typedef HRESULT(WINAPI *MD_D3DXGetImageInfoFromFileInMemory)(
		LPCVOID pSrcData,
		UINT SrcDataSize,
		D3DXIMAGE_INFO* pSrcInfo);

	typedef HRESULT(WINAPI *MD_D3DXCheckTextureRequirements)(
		LPDIRECT3DDEVICE9 pDevice,
		UINT *pWidth,
		UINT *pHeight,
		UINT *pNumMipLevels,
		DWORD Usage,
		D3DFORMAT *pFormat,
		D3DPOOL Pool);

	typedef HRESULT(WINAPI *MD_D3DXCheckVolumeTextureRequirements)(
		LPDIRECT3DDEVICE9 pDevice,
		UINT *pWidth,
		UINT *pHeight,
		UINT *pDepth,
		UINT *pNumMipLevels,
		DWORD Usage,
		D3DFORMAT *pFormat,
		D3DPOOL Pool);

	typedef HRESULT(WINAPI *MD_D3DXCreateEffect)(
		LPDIRECT3DDEVICE9 pDevice,
		LPCVOID pSrcData,
		UINT SrcDataLen,
		CONST D3DXMACRO* pDefines,
		LPD3DXINCLUDE pInclude,
		DWORD Flags,
		LPD3DXEFFECTPOOL pPool,
		LPD3DXEFFECT* ppEffect,
		LPD3DXBUFFER *ppCompilationErrors);

	typedef HRESULT(WINAPI *MD_D3DXLoadMeshFromXInMemory)(
		LPCVOID Memory,
		DWORD SizeOfMemory,
		DWORD Options,
		LPDIRECT3DDEVICE9 pD3DDevice,
		LPD3DXBUFFER * ppAdjacency,
		LPD3DXBUFFER * ppMaterials,
		LPD3DXBUFFER * ppEffectInstances,
		DWORD * pNumMaterials,
		LPD3DXMESH * ppMesh);

	//typedef HRESULT(WINAPI *MD_D3DXCreateFontIndirectA)(
	//	LPDIRECT3DDEVICE9 pDevice,
	//	CONST D3DXFONT_DESCA * pDesc,
	//	LPD3DXFONT * ppFont);

	//typedef HRESULT(WINAPI *MD_D3DXCreateFontIndirectW)(
	//	LPDIRECT3DDEVICE9 pDevice,
	//	CONST D3DXFONT_DESCW * pDesc,
	//	LPD3DXFONT * ppFont);

	typedef HRESULT(WINAPI *MD_D3DXDeclaratorFromFVF)(
		DWORD FVF,
		D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE]);

	typedef HRESULT(WINAPI *MD_D3DXLoadMeshHierarchyFromXInMemory)(
		LPCVOID pMemory,
		DWORD SizeOfMemory,
		DWORD MeshOptions,
		LPDIRECT3DDEVICE9 pDevice,
		LPD3DXALLOCATEHIERARCHY pAlloc,
		LPD3DXLOADUSERDATA pUserDataLoader,
		LPD3DXFRAME* ppFrameHeirarchy,
		LPD3DXANIMATIONCONTROLLER* ppAnimController);

	typedef HRESULT(WINAPI *MD_D3DXFrameDestroy)(
		LPD3DXFRAME pFrameRoot,
		LPD3DXALLOCATEHIERARCHY pAlloc);

	typedef HRESULT(WINAPI *MD_D3DXComputeNormals)(
		LPD3DXBASEMESH pMesh,
		const DWORD *pAdjacency);

	typedef HRESULT(WINAPI *MD_D3DXCreateEffectPool)(
		LPD3DXEFFECTPOOL* ppPool);

	typedef HRESULT(WINAPI *MD_D3DXGetShaderConstantTable)(
		CONST DWORD* pFunction,
		LPD3DXCONSTANTTABLE* ppConstantTable);

public:

	/// 初期化
    static void initialize();

	static MD_Direct3DCreate9						Direct3DCreate9;
	static MD_D3DXCreateTextureFromFileInMemoryEx	D3DXCreateTextureFromFileInMemoryEx;
	static MD_D3DXGetImageInfoFromFileInMemory		D3DXGetImageInfoFromFileInMemory;
	static MD_D3DXCheckTextureRequirements			D3DXCheckTextureRequirements;
	static MD_D3DXCheckVolumeTextureRequirements	D3DXCheckVolumeTextureRequirements;
	static MD_D3DXCreateEffect						D3DXCreateEffect;
	static MD_D3DXLoadMeshFromXInMemory				D3DXLoadMeshFromXInMemory;
	//static MD_D3DXCreateFontIndirectW				D3DXCreateFontIndirectW;
	static MD_D3DXDeclaratorFromFVF					D3DXDeclaratorFromFVF;
	static MD_D3DXLoadMeshHierarchyFromXInMemory	D3DXLoadMeshHierarchyFromXInMemory;
	static MD_D3DXFrameDestroy						D3DXFrameDestroy;
	static MD_D3DXComputeNormals					D3DXComputeNormals;
	static MD_D3DXCreateEffectPool					D3DXCreateEffectPool;
	static MD_D3DXGetShaderConstantTable			D3DXGetShaderConstantTable;

	//static IDirect3D9* Direct3DCreate9(UINT SDKVersion);
	//static HRESULT D3DXCreateTextureFromFileInMemoryEx(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO* pSrcInfo, PALETTEENTRY* pPalette, LPDIRECT3DTEXTURE9* ppTexture);
	//static HRESULT D3DXGetImageInfoFromFileInMemory(LPCVOID pSrcData, UINT SrcDataSize, D3DXIMAGE_INFO* pSrcInfo);
	//static HRESULT D3DXCheckTextureRequirements(LPDIRECT3DDEVICE9 pDevice, UINT *pWidth, UINT *pHeight, UINT *pNumMipLevels, DWORD Usage, D3DFORMAT *pFormat, D3DPOOL Pool);
	//static HRESULT D3DXCreateEffect(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataLen, CONST D3DXMACRO* pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT* ppEffect, LPD3DXBUFFER *ppCompilationErrors);
	//static HRESULT D3DXLoadMeshFromXInMemory(LPCVOID Memory, DWORD SizeOfMemory, DWORD Options, LPDIRECT3DDEVICE9 pD3DDevice, LPD3DXBUFFER * ppAdjacency, LPD3DXBUFFER * ppMaterials, LPD3DXBUFFER * ppEffectInstances, DWORD * pNumMaterials, LPD3DXMESH * ppMesh);
	//static HRESULT D3DXCreateFontIndirect(LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC * pDesc, LPD3DXFONT * ppFont);
	//static HRESULT D3DXDeclaratorFromFVF(DWORD FVF, D3DVERTEXELEMENT9 pDeclarator[MAX_FVF_DECL_SIZE]);
	//static HRESULT D3DXLoadMeshHierarchyFromXInMemory(LPCVOID pMemory, DWORD SizeOfMemory, DWORD MeshOptions, LPDIRECT3DDEVICE9 pDevice, LPD3DXALLOCATEHIERARCHY pAlloc, LPD3DXLOADUSERDATA pUserDataLoader, LPD3DXFRAME* ppFrameHeirarchy, LPD3DXANIMATIONCONTROLLER* ppAnimController);
	//static HRESULT D3DXFrameDestroy(LPD3DXFRAME pFrameRoot, LPD3DXALLOCATEHIERARCHY pAlloc);
	//static HRESULT D3DXComputeNormals(LPD3DXBASEMESH pMesh, const DWORD *pAdjacency);
	//static HRESULT D3DXCreateEffectPool(LPD3DXEFFECTPOOL* ppPool);

public:

	/// D3DFORMAT の文字列表現の取得
    static const Char* GetDxFormatString( D3DFORMAT Format );

	/// VertexElemenst から D3DVERTEXELEMENT9 の要素に変換
	static void TranslateElementLNToDX(const VertexElement* element, BYTE* type, BYTE* offset, BYTE* usage);

	/// D3DVERTEXELEMENT9 の要素から VertexElemenst の要素に変換 (要 delete[])
	static VertexElement* TranslateElementArrayDXToLN(D3DVERTEXELEMENT9* dx_elm, int* count);

	/// TextureFormat から D3DFORMAT に変換
	static D3DFORMAT TranslateLNFormatToDxFormat(TextureFormat format);

	/// D3DFORMAT から TextureFormat に変換
	static TextureFormat TranslateFormatDxToLN(D3DFORMAT dx_format);

	/// デバッグ用。IUnknown の参照カウントの確認
	static int getReferenceCount( IUnknown* obj )
	{
		if ( obj == NULL ) {
			return 0;
		}
		obj->AddRef();
		return obj->Release();
	}

private:
	DX9Module() {}
	~DX9Module() {}

private:
	static DllLoader	m_D3D9Module;	///< "d3d9.dll"
	static DllLoader	m_D3Dx9Module;	///< "d3dx9.dll"
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
