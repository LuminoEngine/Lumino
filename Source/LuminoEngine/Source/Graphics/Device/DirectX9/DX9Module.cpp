
#include "DX9Module.h"
#include "../../../Resource.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// DX9Module
//==============================================================================

DX9Module::MD_Direct3DCreate9						DX9Module::Direct3DCreate9 = NULL;
DX9Module::MD_D3DXCreateTextureFromFileInMemoryEx	DX9Module::D3DXCreateTextureFromFileInMemoryEx = NULL;
DX9Module::MD_D3DXGetImageInfoFromFileInMemory		DX9Module::D3DXGetImageInfoFromFileInMemory = NULL;
DX9Module::MD_D3DXCheckTextureRequirements			DX9Module::D3DXCheckTextureRequirements = NULL;
DX9Module::MD_D3DXCheckVolumeTextureRequirements	DX9Module::D3DXCheckVolumeTextureRequirements = NULL;
DX9Module::MD_D3DXCreateEffect						DX9Module::D3DXCreateEffect = NULL;
DX9Module::MD_D3DXLoadMeshFromXInMemory				DX9Module::D3DXLoadMeshFromXInMemory = NULL;
//DX9Module::MD_D3DXCreateFontIndirect				DX9Module::D3DXCreateFontIndirect = NULL;
DX9Module::MD_D3DXDeclaratorFromFVF					DX9Module::D3DXDeclaratorFromFVF = NULL;
DX9Module::MD_D3DXLoadMeshHierarchyFromXInMemory	DX9Module::D3DXLoadMeshHierarchyFromXInMemory = NULL;
DX9Module::MD_D3DXFrameDestroy						DX9Module::D3DXFrameDestroy = NULL;
DX9Module::MD_D3DXComputeNormals					DX9Module::D3DXComputeNormals = NULL;
DX9Module::MD_D3DXCreateEffectPool					DX9Module::D3DXCreateEffectPool = NULL;
DX9Module::MD_D3DXGetShaderConstantTable			DX9Module::D3DXGetShaderConstantTable = NULL;
DllLoader	DX9Module::m_D3D9Module;
DllLoader	DX9Module::m_D3Dx9Module;

//------------------------------------------------------------------------------
void DX9Module::initialize()
{
	if (!Direct3DCreate9)
    {
        // モジュール読み込み
		if (!m_D3D9Module.tryLoad(_T("d3d9.dll")))
		{
			LN_THROW(0, RuntimeException, InternalResource::getString(InternalResource::DirectXNotInstalledError).c_str(), _T("d3d9.dll"));
		}
		Direct3DCreate9 = reinterpret_cast<MD_Direct3DCreate9>(m_D3D9Module.getProcAddress("Direct3DCreate9"));

        // モジュール読み込み
		Char name[64] = { 0 };
		_stprintf_s(name, 64, _T("d3dx9_%d.dll"), D3DX_SDK_VERSION);
		if (!m_D3Dx9Module.tryLoad(name))
		{
			LN_THROW(0, RuntimeException, InternalResource::getString(InternalResource::DirectXNotInstalledError).c_str(), name);
		}

		// TODO: d3dx9 が無ければ、"エンドユーザーランタイムをインストールしてください"

		D3DXCreateTextureFromFileInMemoryEx = reinterpret_cast< MD_D3DXCreateTextureFromFileInMemoryEx >(m_D3Dx9Module.getProcAddress("D3DXCreateTextureFromFileInMemoryEx"));
		D3DXGetImageInfoFromFileInMemory = reinterpret_cast< MD_D3DXGetImageInfoFromFileInMemory >(m_D3Dx9Module.getProcAddress("D3DXGetImageInfoFromFileInMemory"));
		D3DXCheckTextureRequirements = reinterpret_cast< MD_D3DXCheckTextureRequirements >(m_D3Dx9Module.getProcAddress("D3DXCheckTextureRequirements"));
		D3DXCheckVolumeTextureRequirements = reinterpret_cast< MD_D3DXCheckVolumeTextureRequirements >(m_D3Dx9Module.getProcAddress("D3DXCheckVolumeTextureRequirements"));
		D3DXCreateEffect = reinterpret_cast< MD_D3DXCreateEffect >(m_D3Dx9Module.getProcAddress("D3DXCreateEffect"));
		D3DXLoadMeshFromXInMemory = reinterpret_cast< MD_D3DXLoadMeshFromXInMemory >(m_D3Dx9Module.getProcAddress("D3DXLoadMeshFromXInMemory"));
		//D3DXCreateFontIndirectW = reinterpret_cast< MD_D3DXCreateFontIndirect >(m_D3Dx9Module.GetProcAddress("D3DXCreateFontIndirectW"));
		D3DXDeclaratorFromFVF = reinterpret_cast< MD_D3DXDeclaratorFromFVF >(m_D3Dx9Module.getProcAddress("D3DXDeclaratorFromFVF"));
		D3DXLoadMeshHierarchyFromXInMemory = reinterpret_cast< MD_D3DXLoadMeshHierarchyFromXInMemory >(m_D3Dx9Module.getProcAddress("D3DXLoadMeshHierarchyFromXInMemory"));
		D3DXFrameDestroy = reinterpret_cast< MD_D3DXFrameDestroy >(m_D3Dx9Module.getProcAddress("D3DXFrameDestroy"));
		D3DXComputeNormals = reinterpret_cast< MD_D3DXComputeNormals >(m_D3Dx9Module.getProcAddress("D3DXComputeNormals"));
		D3DXCreateEffectPool = reinterpret_cast< MD_D3DXCreateEffectPool >(m_D3Dx9Module.getProcAddress("D3DXCreateEffectPool"));
		D3DXGetShaderConstantTable = reinterpret_cast< MD_D3DXGetShaderConstantTable >(m_D3Dx9Module.getProcAddress("D3DXGetShaderConstantTable"));
	}
}

//------------------------------------------------------------------------------
const Char* DX9Module::GetDxFormatString(D3DFORMAT Format)
{
	switch ( Format )
	{
	case D3DFMT_UNKNOWN:			
	    return _LT( "UNKNOWN" );
	case D3DFMT_R8G8B8:				
	    return _LT( "R8G8B8" );
	case D3DFMT_A8R8G8B8:			
	    return _LT( "A8R8G8B8" );
	case D3DFMT_X8R8G8B8:			
	    return _LT( "X8R8G8B8" );
	case D3DFMT_R5G6B5:				
	    return _LT( "R5G6B5" );
	case D3DFMT_X1R5G5B5:			
	    return _LT( "X1R5G5B5" );
	case D3DFMT_A1R5G5B5:			
	    return _LT( "A1R5G5B5" );
	case D3DFMT_A4R4G4B4:			
	    return _LT( "A4R4G4B4" );
	case D3DFMT_R3G3B2:				
	    return _LT( "R3G3B2" );
	case D3DFMT_A8:					
	    return _LT( "A8" );
	case D3DFMT_A8R3G3B2:			
	    return _LT( "A8R3G3B2" );
	case D3DFMT_X4R4G4B4:			
	    return _LT( "X4R4G4B4" );
	case D3DFMT_A2B10G10R10:		
	    return _LT( "A2B10G10R10" );
	case D3DFMT_A8B8G8R8:			
	    return _LT( "A8B8G8R8" );
	case D3DFMT_X8B8G8R8:			
	    return _LT( "X8B8G8R8" );
	case D3DFMT_G16R16:				
	    return _LT( "G16R16" );
	case D3DFMT_A2R10G10B10:		
	    return _LT( "A2R10G10B10" );
	case D3DFMT_A16B16G16R16:		
	    return _LT( "A16B16G16R16" );
	case D3DFMT_A8P8:				
	    return _LT( "A8P8" );
	case D3DFMT_P8:					
	    return _LT( "P8" );
	case D3DFMT_L8:					
	    return _LT( "L8" );
	case D3DFMT_L16:				
	    return _LT( "L16" );
	case D3DFMT_A8L8:				
	    return _LT( "A8L8" );
	case D3DFMT_A4L4:				
	    return _LT( "A4L4" );
	case D3DFMT_V8U8:				
	    return _LT( "V8U8" );
	case D3DFMT_Q8W8V8U8:			
	    return _LT( "Q8W8V8U8" );
	case D3DFMT_V16U16:				
	    return _LT( "V16U16" );
	case D3DFMT_Q16W16V16U16:		
	    return _LT( "Q16W16V16U16" );
	case D3DFMT_CxV8U8:				
	    return _LT( "CxV8U8" );
	case D3DFMT_L6V5U5:				
	    return _LT( "L6V5U5" );
	case D3DFMT_X8L8V8U8:			
	    return _LT( "X8L8V8U8" );
	case D3DFMT_A2W10V10U10:		
	    return _LT( "A2W10V10U10" );
	case D3DFMT_G8R8_G8B8:			
	    return _LT( "G8R8_G8B8" );
	case D3DFMT_R8G8_B8G8:			
	    return _LT( "R8G8_B8G8" );
	case D3DFMT_DXT1:				
	    return _LT( "DXT1" );
	case D3DFMT_DXT2:				
	    return _LT( "DXT2" );
	case D3DFMT_DXT3:				
	    return _LT( "DXT3" );
	case D3DFMT_DXT4:				
	    return _LT( "DXT4" );
	case D3DFMT_DXT5:				
	    return _LT( "DXT5" );
	case D3DFMT_UYVY:				
	    return _LT( "UYVY" );
	case D3DFMT_YUY2:				
	    return _LT( "YUY2" );
	case D3DFMT_D16_LOCKABLE:		
	    return _LT( "D16_LOCKABLE" );
	case D3DFMT_D32:				
	    return _LT( "D32" );
	case D3DFMT_D15S1:				
	    return _LT( "D15S1" );
	case D3DFMT_D24S8:				
	    return _LT( "D24S8" );
	case D3DFMT_D24X8:				
	    return _LT( "D24X8" );
	case D3DFMT_D24X4S4:			
	    return _LT( "D24X4S4" );
	case D3DFMT_D32F_LOCKABLE:		
	    return _LT( "D32F_LOCKABLE" );
	case D3DFMT_D24FS8:				
	    return _LT( "D24FS8" );
	case D3DFMT_D16:				
	    return _LT( "D16" );
	case D3DFMT_VERTEXDATA:			
	    return _LT( "VERTEXDATA" );
	case D3DFMT_INDEX16:			
	    return _LT( "INDEX16" );
	case D3DFMT_INDEX32:			
	    return _LT( "INDEX32" );
	case D3DFMT_R16F:				
	    return _LT( "R16F" );
	case D3DFMT_G16R16F:			
	    return _LT( "G16R16F" );
	case D3DFMT_A16B16G16R16F:		
	    return _LT( "A16B16G16R16F" );
	case D3DFMT_R32F:				
	    return _LT( "R32F" );
	case D3DFMT_G32R32F:			
	    return _LT( "G32R32F" );
	case D3DFMT_A32B32G32R32F:		
	    return _LT( "A32B32G32R32F" );
	}

	return _LT( "-" );
}

//------------------------------------------------------------------------------
void DX9Module::TranslateElementLNToDX(const VertexElement* element, BYTE* type, BYTE* offset, BYTE* usage)
{
	*type = 0;
	*offset = 0;
	*usage = 0;

	switch (element->Type)
	{
	case VertexElementType_Float1:
		*type = D3DDECLTYPE_FLOAT1;
		*offset = sizeof(float);
		break;

	case VertexElementType_Float2:
		*type = D3DDECLTYPE_FLOAT2;
		*offset = sizeof(float) * 2;
		break;

	case VertexElementType_Float3:
		*type = D3DDECLTYPE_FLOAT3;
		*offset = sizeof(float) * 3;
		break;

	case VertexElementType_Float4:
		*type = D3DDECLTYPE_FLOAT4;
		*offset = sizeof(float) * 4;
		break;

	case VertexElementType_Ubyte4:
		*type = D3DDECLTYPE_UBYTE4;
		*offset = sizeof(unsigned char) * 4;
		break;

	case VertexElementType_Color4:
		*type = D3DDECLTYPE_D3DCOLOR;
		*offset = sizeof(unsigned char) * 4;
		break;

	case VertexElementType_Short2:
		*type = D3DDECLTYPE_SHORT2;
		*offset = sizeof(short) * 2;
		break;

	case VertexElementType_Short4:
		*type = D3DDECLTYPE_SHORT4;
		*offset = sizeof(short) * 4;
		break;
	}

	switch (element->Usage)
	{
	case VertexElementUsage_Position:
		*usage = D3DDECLUSAGE_POSITION;
		break;

	case VertexElementUsage_Normal:
		*usage = D3DDECLUSAGE_NORMAL;
		break;

	case VertexElementUsage_Color:
		*usage = D3DDECLUSAGE_COLOR;
		break;

	case VertexElementUsage_TexCoord:
		*usage = D3DDECLUSAGE_TEXCOORD;
		break;

	case VertexElementUsage_PointSize:
		*usage = D3DDECLUSAGE_PSIZE;
		break;

	case VertexElementUsage_BlendIndices:
		*usage = D3DDECLUSAGE_BLENDINDICES;
		break;

	case VertexElementUsage_BlendWeight:
		*usage = D3DDECLUSAGE_BLENDWEIGHT;
		break;
	}
}

//------------------------------------------------------------------------------
VertexElement* DX9Module::TranslateElementArrayDXToLN(D3DVERTEXELEMENT9* dx_elm, int* count)
{
    // 要素数チェック
    int num = 0;
    while ( true )
    {
        if ( dx_elm[ num ].Stream == 0xff && dx_elm[ num ].Type == D3DDECLTYPE_UNUSED )
        {
            break;
        }
        ++num;
    }
    ++num;

	VertexElement* ln_elm = LN_NEW VertexElement[num];
	for (int i = 0; i < num; ++i)
	{
		ln_elm[i].StreamIndex = dx_elm[i].Stream;
		ln_elm[i].UsageIndex = dx_elm[i].UsageIndex;

		switch (dx_elm[i].Type)
		{
		case D3DDECLTYPE_FLOAT1:
			ln_elm[i].Type = VertexElementType_Float1;
			break;

		case D3DDECLTYPE_FLOAT2:
			ln_elm[i].Type = VertexElementType_Float2;
			break;

		case D3DDECLTYPE_FLOAT3:
			ln_elm[i].Type = VertexElementType_Float3;
			break;

		case D3DDECLTYPE_FLOAT4:
			ln_elm[i].Type = VertexElementType_Float4;
			break;

		case D3DDECLTYPE_UBYTE4:
			ln_elm[i].Type = VertexElementType_Ubyte4;
			break;

		case D3DDECLTYPE_D3DCOLOR:
			ln_elm[i].Type = VertexElementType_Color4;
			break;

		case D3DDECLTYPE_SHORT2:
			ln_elm[i].Type = VertexElementType_Short2;
			break;

		case D3DDECLTYPE_SHORT4:
			ln_elm[i].Type = VertexElementType_Short4;
			break;

		default:
			ln_elm[i].Type = VertexElementType_Unknown;
			break;
		};

		switch (dx_elm[i].Usage)
		{
		case D3DDECLUSAGE_POSITION:
			ln_elm[i].Usage = VertexElementUsage_Position;
			break;

		case D3DDECLUSAGE_NORMAL:
			ln_elm[i].Usage = VertexElementUsage_Normal;
			break;

		case D3DDECLUSAGE_COLOR:
			ln_elm[i].Usage = VertexElementUsage_Color;
			break;

		case D3DDECLUSAGE_TEXCOORD:
			ln_elm[i].Usage = VertexElementUsage_TexCoord;
			break;

		case D3DDECLUSAGE_PSIZE:
			ln_elm[i].Usage = VertexElementUsage_PointSize;
			break;

		case D3DDECLUSAGE_BLENDINDICES:
			ln_elm[i].Usage = VertexElementUsage_BlendIndices;
			break;

		case D3DDECLUSAGE_BLENDWEIGHT:
			ln_elm[i].Usage = VertexElementUsage_BlendWeight;
			break;

		default:
			ln_elm[i].Usage = VertexElementUsage_Unknown;
			break;
		};
	}

	*count = num;
    return ln_elm;
}

//------------------------------------------------------------------------------
D3DFORMAT DX9Module::TranslateLNFormatToDxFormat(TextureFormat format)
{
	/*
		本ライブラリのピクセルフォーマット定数名はバイトシーケンスを表している。
		対して、D3DFMT はリトルエンディアンのu32をビットシフトしたときの並びで表している。
		https://msdn.microsoft.com/ja-jp/library/windows/apps/dn166864.aspx
	*/
	static const std::array<D3DFORMAT, 10> d3dFormats =
	{
		D3DFMT_UNKNOWN,			// TextureFormat_Unknown

		D3DFMT_A8B8G8R8,		// TextureFormat_R8G8B8A8
		D3DFMT_X8B8G8R8,		// TextureFormat_R8G8B8X8
		D3DFMT_A8R8G8B8,		// TextureFormat_B8G8R8A8 (いつも使ってた D3DFMT_A8R8G8B8)
		D3DFMT_X8R8G8B8,		// TextureFormat_B8G8R8X8

		D3DFMT_A16B16G16R16F,	// TextureFormat_R16G16B16A16_Float
		D3DFMT_A32B32G32R32F,	// TextureFormat_R32G32B32A32_Float
		D3DFMT_R16F,			// TextureFormat_R16_Float
		D3DFMT_R32F,			// TextureFormat_R32_Float

		D3DFMT_D24S8,			// TextureFormat_D24S8
	};
	//assert(LN_ARRAY_SIZE_OF(d3dFormats) == (int)TextureFormat::_Count);
	return d3dFormats[(int)format];
}

//------------------------------------------------------------------------------
TextureFormat DX9Module::TranslateFormatDxToLN(D3DFORMAT dx_format)
{
	switch (dx_format)
    {
	case D3DFMT_A8B8G8R8:		return TextureFormat::R8G8B8A8;
	case D3DFMT_X8B8G8R8:		return TextureFormat::R8G8B8X8;
	case D3DFMT_A8R8G8B8:		return TextureFormat::B8G8R8A8;
	case D3DFMT_X8R8G8B8:		return TextureFormat::B8G8R8X8;

	case D3DFMT_A16B16G16R16F:	return TextureFormat::R16G16B16A16_Float;
	case D3DFMT_A32B32G32R32F:	return TextureFormat::R32G32B32A32_Float;
	case D3DFMT_D24S8:			return TextureFormat::D24S8;
	case D3DFMT_R16F:			return TextureFormat::R16_Float;
	case D3DFMT_R32F:			return TextureFormat::R32_Float;
    }

	return TextureFormat::Unknown;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
