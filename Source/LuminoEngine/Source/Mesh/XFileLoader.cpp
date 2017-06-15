
#include "../Internal.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Mesh/Mesh.h>
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Device/DirectX9/DX9Module.h"
#include "../Graphics/Device/DirectX9/DX9GraphicsDevice.h"
#include "ModelManager.h"
#include "XFileLoader.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// AllocateHierarchy
//==============================================================================

//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
AllocateHierarchy::AllocateHierarchy()
    : mFrameNum         ( 0 )
    , mAllMaterialNum   ( 0 )
{

}

//------------------------------------------------------------------------------
// フレーム作成コールバック
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateFrame( const char* name_, D3DXFRAME** out_new_frame_ )
{
	HRESULT hr = S_OK;

    // まずは空のフレームを作成する
	DerivedD3DXFrame* dx_frame = LN_NEW DerivedD3DXFrame();
	if ( dx_frame == NULL )
	{
		hr = E_OUTOFMEMORY;
		goto ERR_EXIT;
	}

    // フレームの名前をフレームに格納
	hr = _allocateName( name_, &dx_frame->Name );
	if ( FAILED( hr ) ) { 
		goto ERR_EXIT; }

	// 出来上がったフレームを返す
	*out_new_frame_ = dx_frame;

	// 番号をつける
	dx_frame->Index = mFrameNum;
	++mFrameNum;

    mDerivedD3DXFrameArray.add( dx_frame );
	return S_OK;

ERR_EXIT:
	DestroyFrame( dx_frame );
	return hr;
}

//------------------------------------------------------------------------------
// メッシュコンテナ作成コールバック
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::CreateMeshContainer(
	const char* name_,
	const D3DXMESHDATA* dx_mesh_data_,
    const D3DXMATERIAL* dx_materials,
	const D3DXEFFECTINSTANCE* dx_effect_instances,
	DWORD material_num_,
    const DWORD* adjacency_,
	ID3DXSkinInfo* dx_skin_info_,
    D3DXMESHCONTAINER** new_mesh_container_ )
{
    HRESULT hr = S_OK;
	DerivedD3DXMeshContainer* mesh_container = NULL;

    // パッチ・プログレッシブメッシュは扱わない
	if ( dx_mesh_data_->Type != D3DXMESHTYPE_MESH ) goto ERR_EXIT;

    // FVF互換でないメッシュは扱わない
	if ( dx_mesh_data_->pMesh->GetFVF() == 0 ) goto ERR_EXIT;
		
	// メッシュデータ構造体中の LPD3DXMESH へのポインタを得る
	ID3DXMesh* dx_mesh = dx_mesh_data_->pMesh;

    // メッシュコンテナを作成する
	mesh_container = LN_NEW DerivedD3DXMeshContainer();
	if ( !mesh_container )
	{
		hr = E_OUTOFMEMORY;
		goto ERR_EXIT;
	}

	// 引数で受け取ったメッシュコンテナの名前を保存
	hr = _allocateName( name_, &mesh_container->Name );
	if ( FAILED( hr ) ) goto ERR_EXIT;

    // メッシュに法線・頂点色があるか調べて、無ければ追加する
    mesh_container->MeshData.Type = D3DXMESHTYPE_MESH;
	//bool converted;
 //   ID3DXMesh* new_mesh;
	//hr = _checkMeshFormat( dx_mesh, &converted, &new_mesh );
	//if ( FAILED( hr ) ) goto ERR_EXIT;
	//// 法線または頂点色が追加されなかった場合
	//if ( !converted )
	//{
        mesh_container->MeshData.pMesh = dx_mesh;
		dx_mesh->AddRef();
	//}
 //   else
 //   {
 //       mesh_container->MeshData.pMesh = new_mesh;
 //   }
		
    // 引数で受け取ったマテリアルの情報をメッシュコンテナに格納
	hr = _registMaterialData( dx_mesh->GetNumFaces(), adjacency_, material_num_, dx_materials, mesh_container );
	if ( FAILED( hr ) ) { 
		goto ERR_EXIT; }
		
    // スキニングの情報がある場合
	if ( dx_skin_info_ != NULL )
	{
		// 元のメッシュをコンテナに格納して覚えておく
		mesh_container->OriginalMesh = dx_mesh;
		dx_mesh->AddRef();

		// スキニング情報をコンテナに登録する
		_registSkinInfo( dx_skin_info_, mesh_container );
	}

    // 作成したメッシュコンテナのポインタを渡す
    mDerivedD3DXMeshContainerArray.add( mesh_container );
	*new_mesh_container_ = mesh_container;
	mesh_container = NULL;

	return S_OK;

ERR_EXIT:

	if ( mesh_container )
	{
		DestroyMeshContainer( mesh_container );
	}
	return hr;
}

//------------------------------------------------------------------------------
// フレーム解放コールバック
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyFrame( D3DXFRAME* frame_to_free_ )
{
    LN_SAFE_DELETE_ARRAY( frame_to_free_->Name );
	LN_SAFE_DELETE( frame_to_free_ );
	return S_OK;
}

//------------------------------------------------------------------------------
// メッシュコンテナ解放コールバック
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::DestroyMeshContainer( D3DXMESHCONTAINER* mesh_container_base_ )
{
    DerivedD3DXMeshContainer* mesh_container = (DerivedD3DXMeshContainer*)mesh_container_base_;

	LN_SAFE_DELETE_ARRAY( mesh_container->Name );
	LN_SAFE_DELETE_ARRAY( mesh_container->pAdjacency );
	LN_SAFE_DELETE_ARRAY( mesh_container->pMaterials );

	LN_COM_SAFE_RELEASE( mesh_container->OriginalMesh );
	LN_SAFE_DELETE_ARRAY( mesh_container->BoneOffsetMatrices );

    //SAFE_DELETE_ARRAY( mesh_container->pAttributeTable );

	LN_COM_SAFE_RELEASE( mesh_container->MeshData.pMesh );
	LN_COM_SAFE_RELEASE( mesh_container->pSkinInfo );

	LN_SAFE_DELETE( mesh_container );
	return S_OK;
}

//---------------------------------------------------------------------------------
// メモリを確保して文字列を格納する ( フレーム等の名前格納用 )
//---------------------------------------------------------------------------------
HRESULT AllocateHierarchy::_allocateName( const char* name_, char** out_name_ )
{
	if ( name_ != NULL )
	{
		size_t size = strlen( name_ ) + 1;
		*out_name_ = LN_NEW char[ size ];
		if ( *out_name_ == NULL )
		{
			// メモリ確保失敗
			return E_OUTOFMEMORY;
		}
		memcpy( *out_name_, name_, size * sizeof(char) );
	}
	else
	{
		*out_name_ = NULL;
	}
	return S_OK;
}

//------------------------------------------------------------------------------
// マテリアル情報・隣接情報をコンテナに登録する
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::_registMaterialData(
	int face_num_,
	const DWORD* adjacency_,
	int material_num_,
	const D3DXMATERIAL* materials_,
	DerivedD3DXMeshContainer* out_ )
{
    // マテリアル数 (最低1つ作成)
    out_->NumMaterials = (((1) > (material_num_)) ? (1) : (material_num_));

    mAllMaterialNum += out_->NumMaterials;

	// メモリ確保
    out_->pAdjacency = LN_NEW DWORD[ face_num_ * 3 ];
    out_->pMaterials = LN_NEW D3DXMATERIAL[ out_->NumMaterials ];
    if( ( out_->pAdjacency == NULL ) || ( out_->pMaterials == NULL ) )
    {
        return E_OUTOFMEMORY;
    }

    // 隣接情報をコピー
    memcpy( out_->pAdjacency, adjacency_, sizeof( DWORD ) * face_num_ * 3 );

    // 受け取ったマテリアルをコピーする
	if ( material_num_ > 0 )
	{
        // まずは丸ごとコピー
        memcpy( out_->pMaterials, materials_, sizeof( D3DXMATERIAL ) * material_num_ );

        // テクスチャ名を保存する
        for (int i = 0; i < material_num_; ++i )
        {
            // アンビエントにはディフューズを使う
			//out_->pMaterials[ i ].Ambient = out_->pMaterials[ i ].Diffuse;

            if ( out_->pMaterials[ i ].pTextureFilename != NULL )
            {
                out_->TextureNames.add(String::fromNativeCharString( out_->pMaterials[ i ].pTextureFilename ) );
                out_->pMaterials[ i ].pTextureFilename = NULL;
            }
            else
            {
                // テクスチャ名がない場合でも、空の文字列を入れておく
                // そうしないと、マテリアル数とテクスチャ名数が合わなくなる
                out_->TextureNames.add(String::getEmpty());
            }
        }
	}
    else
    {
        out_->pMaterials[0].pTextureFilename = NULL;
        memset( &out_->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
        out_->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        out_->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        out_->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        out_->pMaterials[0].MatD3D.Specular = out_->pMaterials[0].MatD3D.Diffuse;
    }
	return S_OK;
}

//------------------------------------------------------------------------------
// スキニング情報をコンテナに登録する
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::_registSkinInfo( ID3DXSkinInfo* dx_skin_info_, DerivedD3DXMeshContainer* out_ )
{
	// スキン情報を格納
	out_->pSkinInfo = dx_skin_info_;
	dx_skin_info_->AddRef();

	// 物体のローカル空間からボーン空間に頂点を移動するためのオフセット行列用の配列を用意する
	DWORD bone_num_ = dx_skin_info_->GetNumBones();
	out_->BoneOffsetMatrices = LN_NEW D3DXMATRIX[ bone_num_ ];
	if ( !out_->BoneOffsetMatrices ) { 
		return E_OUTOFMEMORY; }

	// それぞれのボーンのオフセット行列を取り出して保存する
	for (DWORD i = 0; i < bone_num_; ++i )
	{
		out_->BoneOffsetMatrices[ i ] = *( dx_skin_info_->GetBoneOffsetMatrix( i ) );
	}

	// スキニングに都合のよい一般的なスキニング情報と座標変換を生成する
	return _generateSkinnedMesh( out_ );
}

//------------------------------------------------------------------------------
// スキンメッシュを作成する
//------------------------------------------------------------------------------
HRESULT AllocateHierarchy::_generateSkinnedMesh( DerivedD3DXMeshContainer* out_ )
{
	HRESULT hr = S_OK;

	IDirect3DDevice9* dx_device;
	out_->MeshData.pMesh->GetDevice( &dx_device );
	dx_device->Release();

	//SAFE_RELEASE( out_->MeshData.pMesh );
	//SAFE_RELEASE( out_->mBoneCombinationBuf );


    // if software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    //if( g_SkinningMethod == SOFTWARE )
    if ( 1 )
    {
        /*
        hr = out_->OriginalMesh->CloneMeshFVF(
            D3DXMESH_MANAGED, out_->OriginalMesh->GetFVF(),
            dx_device, &out_->MeshData.pMesh );
        if( FAILED( hr ) )
            goto ERR_EXIT;

        // 属性数取得
        hr = out_->MeshData.pMesh->GetAttributeTable( NULL, &out_->NumAttributeGroups );
        if( FAILED( hr ) )
            goto ERR_EXIT;

        // 属性のメモリ確保
        //delete[] pMeshContainer->pAttributeTable;
        SAFE_DELETE_ARRAY( out_->pAttributeTable );
        out_->pAttributeTable = LN_NEW D3DXATTRIBUTERANGE[ out_->NumAttributeGroups ];
        if( out_->pAttributeTable == NULL )
        {
            hr = E_OUTOFMEMORY;
            goto ERR_EXIT;
        }

        // 属性をコピー
        hr = out_->MeshData.pMesh->GetAttributeTable( out_->pAttributeTable, NULL );
        if( FAILED( hr ) )
            goto ERR_EXIT;
            */

        //_p( out_->OriginalMesh->GetFVF() );

        //_p(out_->pSkinInfo->GetBoneName(0));

        //_p(out_->pSkinInfo->GetNumBones());

        /*
        // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
        if( g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
        {
            g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space for blend matrices
            delete[] g_pBoneMatrices;
            g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
            if( g_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto ERR_EXIT;
            }
        }
        */
    }












#if 0



	//if( ( g_SkinningMethod == D3DINDEXEDVS ) || ( g_SkinningMethod == D3DINDEXEDHLSLVS ) )
	if ( 1 )
	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		// 一度に使うボーン行列の数かも
		uint32_t max_matrices = 26;
		out_->mPaletteEntriesNum = min( max_matrices, out_->pSkinInfo->GetNumBones() );

		// メッシュ最適化の種類を決める
		// ( DirectX のサンプルでは、hlsl 1.1 より前のバージョンでは
		//   ソフトウェア処理も兼ねるようにしてるけど、今回はカット )
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;	// キャッシュヒット率アップのために面順番並べ替え + MANAGED
			
		// 元のメッシュはこの関数終了後は使わないので、
		// スキンメッシュ作成後に解放されるように参照カウントを減らしておく
		SAFE_RELEASE( out_->MeshData.pMesh );

		// 頂点単位のブレンドの重みとインデックスとボーンの組み合わせテーブルを適用した新しいメッシュを作る
		hr = out_->pSkinInfo->ConvertToIndexedBlendedMesh(
			out_->mOrigMesh,
			Flags,
			out_->mPaletteEntriesNum,
			out_->pAdjacency,
			NULL, NULL, NULL,
			&out_->mInflNum,			// ボーンの影響の 1 面あたりの最大数
			&out_->mAttributeGroupsNum,	// ボーンの組み合わせテーブルに含まれるボーンの数へのポインタ
			&out_->mBoneCombinationBuf,	// ボーンの組み合わせテーブルへのポインタ
			&out_->MeshData.pMesh );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }


		// 法線があるかどうか調べる ( 無い場合は次の FVF 変換後に法線を再計算するフラグを true に )
		bool req_compute_normals = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_NORMAL ) == 0 );

		// 頂点色があるかどうか調べる
		bool use_vertex_color = ( ( out_->MeshData.pMesh->GetFVF() & D3DFVF_DIFFUSE ) != 0 );

		// 必要があれば最適な FVF に変換
		DWORD new_fvf =
			( out_->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK ) |
			D3DFVF_NORMAL |
			D3DFVF_DIFFUSE |
			D3DFVF_TEX1 |
			D3DFVF_LASTBETA_UBYTE4;
		if ( new_fvf != out_->MeshData.pMesh->GetFVF() )
		{
			ID3DXMesh* dx_mesh;
			hr = out_->MeshData.pMesh->CloneMeshFVF(
				out_->MeshData.pMesh->GetOptions(),
				new_fvf, dx_device, &dx_mesh );

			if ( SUCCEEDED( hr ) )
			{
				// 変換前に法線がなかった場合は法線を計算する
				if ( req_compute_normals )
				{
					D3DXComputeNormals( dx_mesh, NULL );
				}
				// 変換前に頂点色がなかった場合は白で埋める
				//if ( !use_vertex_color )
				//{
				//	//LVertex
				//	//LockVertexBuffer
				//	printf( "use_vertex_color\n" );
				//}

				// 変換前に頂点色がなかった場合は白で埋める
				if ( !use_vertex_color )
				{
					IDirect3DVertexBuffer9* vb;
					out_->MeshData.pMesh->GetVertexBuffer( &vb );

					LVertex* v;
					uint32_t num = out_->MeshData.pMesh->GetNumVertices();
					//hr = t_mesh->LockVertexBuffer( 0, (void**)&v );
					vb->Lock( 0, 0, (void**)&v, 0 );

					for ( uint32_t i = 0; i < num; ++i )
					{
						v[ i ].mColor = 0xffffffff;
					}
					//t_mesh->UnlockVertexBuffer();
					vb->Unlock();
					SAFE_RELEASE( vb );
				}


				out_->MeshData.pMesh->Release();
				out_->MeshData.pMesh = dx_mesh;
				dx_mesh = NULL;
			}
		}
			
		// メッシュの頂点宣言を取得
		D3DVERTEXELEMENT9 decl[ MAX_FVF_DECL_SIZE ];
			
		hr = out_->MeshData.pMesh->GetDeclaration( decl );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }

		/*
		printf( "--------------------------------------\n" );
		int ii = 0;
		while ( 1 )
		{
			if ( decl[ ii ].Stream == 0xff ) { break; }

			printf( "-------------------\n" );
			wprintf( L"Stream:%d Offset:%d Type:%s Method:%s Usage:%s UsageIndex:%d\n",
				decl[ ii ].Stream,
				decl[ ii ].Offset,
				DXUTTraceD3DDECLTYPEtoString( decl[ ii ].Type ),
				DXUTTraceD3DDECLMETHODtoString( decl[ ii ].Method ),
				DXUTTraceD3DDECLUSAGEtoString( decl[ ii ].Usage ),
				decl[ ii ].UsageIndex );
			printf( "-------------------\n" );

			++ii;
		}
		printf( "--------------------------------------\n" );
		*/
			

		// 最初の D3DDECLUSAGE_BLENDINDICES は D3DDECLTYPE_D3DCOLOR として扱うみたいな
		// ( これがないとモデルが崩れる )
		D3DVERTEXELEMENT9* cur_decl = decl;
		while ( cur_decl->Stream != 0xff )
		{
			if ( ( cur_decl->Usage == D3DDECLUSAGE_BLENDINDICES ) && ( cur_decl->UsageIndex == 0 ) )
			{
				cur_decl->Type = D3DDECLTYPE_D3DCOLOR;
			}
			++cur_decl;
		}

		hr = out_->MeshData.pMesh->UpdateSemantics( decl );
		if ( FAILED( hr ) ) { 
			goto ERR_EXIT; }

		// ボーンの最大数を覚えておく
		if ( mBoneMatricesMaxNum < out_->pSkinInfo->GetNumBones() )
		{
			mBoneMatricesMaxNum = out_->pSkinInfo->GetNumBones();
		}
	}
ERR_EXIT:
#endif

	return hr;
}


//------------------------------------------------------------------------------
template<typename TIndex>
static void FlipTriangleFronts(TIndex* indices, int count)
{
	for (int i = 0; i < count; i += 3)
	{
		std::swap(indices[i + 1], indices[i + 2]);
	}
}

//==============================================================================
// XFileLoader
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<StaticMeshModel> XFileLoader::load(ModelManager* manager, Stream* stream, const PathName& parentDir, bool isDynamic, ModelCreationFlag flags)
{
	Driver::DX9GraphicsDevice* device = dynamic_cast<Driver::DX9GraphicsDevice*>(manager->getGraphicsManager()->getGraphicsDevice());
	LN_THROW(device != nullptr, ArgumentException);

	AllocateHierarchy allocate_hierarchy;
	D3DXFRAME* root_frame = NULL;
	ID3DXAnimationController* dx_anim_controller = NULL;

	ByteBuffer data((size_t)stream->getLength());
	stream->read(data.getData(), data.getSize());
	LN_COMCALL(DX9Module::D3DXLoadMeshHierarchyFromXInMemory(
		data.getConstData(),
		data.getSize(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,	// まずは 32bit インデックスで作る
		device->getIDirect3DDevice9(),
		&allocate_hierarchy,
		NULL,
		&root_frame,
		&dx_anim_controller));

	//---------------------
	class finally {
	public:
		finally(AllocateHierarchy* ah_, D3DXFRAME* rf_, ID3DXAnimationController* ac_)
			: mAH(ah_), mRF(rf_), mAC(ac_) { }
		~finally()
		{
			if (mRF) DX9Module::D3DXFrameDestroy(mRF, mAH);
			LN_COM_SAFE_RELEASE(mAC);
		}
		AllocateHierarchy* mAH;
		D3DXFRAME* mRF;
		ID3DXAnimationController* mAC;
	}
	finally(&allocate_hierarchy, root_frame, dx_anim_controller);
	//---------------------

	try
	{
		auto meshRes = RefPtr<MeshResource>::makeRef();
		meshRes->initialize(manager->getGraphicsManager(), MeshCreationFlags::None);
		auto mesh1 = newObject<StaticMeshModel>(manager->getGraphicsManager(), meshRes);

		// スキンメッシュではない場合
		if (!dx_anim_controller)
		{
			//-----------------------------------------------------
			// 頂点・インデックスの数を調べる

			uint32_t all_vertex_num = 0;
			uint32_t all_index_num = 0;

			DerivedD3DXMeshContainerArray& dx_mesh_containers = allocate_hierarchy.getDerivedD3DXMeshContainerArray();
			LN_THROW(dx_mesh_containers.getCount() == 1, InvalidFormatException, "XFile error: Mesh container is 2 or more.");
			
			for (DerivedD3DXMeshContainer* c : dx_mesh_containers)
			{
				// とりあえずこのフォーマットでのみ受け付ける
				all_vertex_num += c->MeshData.pMesh->GetNumVertices();
				all_index_num += c->MeshData.pMesh->GetNumFaces() * 3;
			}
			LN_THROW(all_vertex_num > 0, InvalidFormatException, "XFile error: fvf invalid.");

			//-----------------------------------------------------
			// 頂点バッファ、インデックスバッファ作成

			// VertexDeclaration
			meshRes->m_vertexDeclaration = manager->getGraphicsManager()->getDefaultVertexDeclaration();

			// VertexBuffer
			meshRes->m_vertexBufferInfos[0].buffer.attach(LN_NEW VertexBuffer());
			meshRes->m_vertexBufferInfos[0].buffer->initialize(
				manager->getGraphicsManager(),
				sizeof(Vertex) * all_vertex_num,
				nullptr,
				(isDynamic) ? ResourceUsage::Dynamic : ResourceUsage::Static, true);

			LN_NOTIMPLEMENTED();
#if 0
			// IndexBuffer
			IDirect3DIndexBuffer9* dx_indexbuffer;
			D3DINDEXBUFFER_DESC dx_indexbuffer_desc;
			dx_mesh_containers[0]->MeshData.pMesh->GetIndexBuffer(&dx_indexbuffer);
			dx_indexbuffer->GetDesc(&dx_indexbuffer_desc);
			dx_indexbuffer->Release();
			mesh->m_indexBuffer.Attach(LN_NEW IndexBuffer());
			mesh->m_indexBuffer->initialize(
				manager->GetGraphicsManager(),
				all_index_num,
				nullptr,
				(dx_indexbuffer_desc.Format == D3DFMT_INDEX16) ? IndexBufferFormat_UInt16 : IndexBufferFormat_UInt32,
				(isDynamic) ? ResourceUsage::Dynamic : ResourceUsage::Static);

			// 書き込み開始
			ScopedVertexBufferLock lockVertexBuffer(mesh->m_vertexBufferInfos[0].buffer);
			ScopedIndexBufferLock lockIndexBuffer(mesh->m_indexBuffer);
			Vertex* vertices = (Vertex*)lockVertexBuffer.GetData();
			void* indices = lockIndexBuffer.GetData();

			for (DerivedD3DXMeshContainer* c : dx_mesh_containers)	// TODO: 現在1つしか想定していない
		{
				// 頂点読み取り開始
				void* dx_vertices;
				c->MeshData.pMesh->LockVertexBuffer(0, (void**)&dx_vertices);
				uint32_t vertexCount = c->MeshData.pMesh->GetNumVertices();
				switch (c->MeshData.pMesh->GetFVF())
				{
				case D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1:
					ConvertVertex_Pos_Normal_UV(vertices, (XFileVertex_Pos_Normal_UV*)dx_vertices, vertexCount);
					break;
				case D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1:
					ConvertVertex_Pos_Normal_Color_UV(vertices, (XFileVertex_Pos_Normal_Color_UV*)dx_vertices, vertexCount);
					break;
				default:
					break;
				}
					



				// 頂点読み取り終了
				c->MeshData.pMesh->UnlockVertexBuffer();

				// インデックス読み取り開始
				void* dx_indices;
				c->MeshData.pMesh->LockIndexBuffer(0, (void**)&dx_indices);
				uint32_t i_num = c->MeshData.pMesh->GetNumFaces() * 3;

				// TODO: 複数メッシュコンテナ対応させるなら考えないとダメ
				memcpy(indices, dx_indices, mesh->m_indexBuffer->GetIndexStride() * i_num);

				// インデックス読み取り終了
				c->MeshData.pMesh->UnlockIndexBuffer();
			}

			// Lumino と X ファイルでは面の正面方向が違う。反転する。
			if ((dx_indexbuffer_desc.Format == D3DFMT_INDEX16))
				FlipTriangleFronts<uint16_t>((uint16_t*)indices, all_index_num);
			else
				FlipTriangleFronts<uint32_t>((uint32_t*)indices, all_index_num);

			// 書き込み終了
			mesh->m_vertexBufferInfos[0].buffer->Unlock();
			mesh->m_indexBuffer->Unlock();
#endif
			//-----------------------------------------------------
			// マテリアル、属性

			// マテリアル数取得
			uint32_t attr_num = allocate_hierarchy.getAllMaterialNum();
			mesh1->addMaterials(attr_num);

			// マテリアル、属性配列
			MaterialList* materials = mesh1->m_materials;
			MeshAttributeList& attributes = meshRes->m_attributes;
			uint32_t mi = 0;
			for (DerivedD3DXMeshContainer* c : dx_mesh_containers)
			{
				static const DWORD fvf = D3DFVF_XYZ | D3DFVF_NORMAL/* | D3DFVF_DIFFUSE*/ | D3DFVF_TEX1;
				if (c->MeshData.pMesh->GetFVF() == fvf)
				{
					// 属性テーブルを取得する
					DWORD   attr_num = 0;
					c->MeshData.pMesh->GetAttributeTable(NULL, &attr_num);
					List<D3DXATTRIBUTERANGE> dx_attrib_table;
					dx_attrib_table.resize(attr_num);
					c->MeshData.pMesh->GetAttributeTable(&dx_attrib_table[0], &attr_num);

					for (uint32_t i = 0; i < c->NumMaterials; ++i, ++mi)
					{
						DxMaterialToLnMaterial(c->pMaterials[i].MatD3D, materials->getAt(mi));

						// テクスチャ名がある場合はテクスチャ作成
						if (!c->TextureNames.isEmpty() && !c->TextureNames[i].isEmpty())
						{
							materials->getAt(mi)->setTextureParameter(
								Material::MaterialTextureParameter,
								manager->createTexture(parentDir, c->TextureNames[i], flags));
						}

						// 属性
						attributes[mi].MaterialIndex = i;
						attributes[mi].StartIndex = dx_attrib_table[i].FaceStart * 3;
						attributes[mi].PrimitiveNum = dx_attrib_table[i].FaceCount;
					}
				}
				else
				{
					LN_NOTIMPLEMENTED();
				}
			}
		}

		return mesh1;
	}
	catch (Exception&)
	{
		throw;
	}

	return nullptr;
#if 0
	//analyzeFrame( (DerivedD3DXFrame*)root_frame, 0 );

	//-----------------------------------------------------
	// 必要なボーンの名前の一覧を作る
	//	  D3DXFRAME の階層構造には、ボーンではなく
	//	  メッシュコンテナを格納しているフレームがある。
	//	  混乱を避けるため、ボーンとして扱わないフレームは
	//	  Model のフレーム構造からはじきたい。
	//	  そのため、本当に必要なボーンの名前一覧を作って
	//	  必要なフレームだけを作るようにする。

	std::map< LRefString, int > necessary_bone_names;

	DerivedD3DXMeshContainerArray& dx_mesh_containers = allocate_hierarchy.getDerivedD3DXMeshContainerArray();
	ln_foreach(DerivedD3DXMeshContainer* c, dx_mesh_containers)
	{
		uint32_t n = c->pSkinInfo->GetNumBones();
		for (uint32_t i = 0; i < n; ++i)
		{
			necessary_bone_names.insert(
				std::pair< LRefTString, int >(LRefString(c->pSkinInfo->GetBoneName(i)), i));
		}
	}

	//-----------------------------------------------------
	// 必要なフレームを bone_frames に格納

	DerivedD3DXFrameArray& dx_frames = allocate_hierarchy.getDerivedD3DXFrameArray();
	DerivedD3DXFrameArray  bone_frames;	 // 本当に必要なフレーム
	ln_foreach(DerivedD3DXFrame* frame, dx_frames)
	{
		if (frame->pMeshContainer)
		{
			std::map< LRefString, int >::iterator itr = necessary_bone_names.find(frame->Name);
			if (itr != necessary_bone_names.end())
			{
				bone_frames.push_back(frame);
			}
		}
		else
		{
			bone_frames.push_back(frame);
		}
	}

	//-----------------------------------------------------
	// フレーム作成

	ModelFrame* model_frames = LN_NEW ModelFrame[bone_frames.size()];

	ln_foreach(DerivedD3DXFrame* frame, bone_frames)
	{
		//_p( frame->Name );
		//dx_frames[ i ]->

		// printf( "Frame:%s Sibling:%p Child:%p\n", dx_frames[ i ]->Name, dx_frames[ i ]->pFrameSibling, dx_frames[ i ]->pFrameFirstChild );

	}

#endif
}

//------------------------------------------------------------------------------
void XFileLoader::DxMaterialToLnMaterial(const D3DMATERIAL9& dx_material, Material* material)
{
	material->setColorParameter(
		Material::DiffuseParameter,
		dx_material.Diffuse.r,
		dx_material.Diffuse.g,
		dx_material.Diffuse.b,
		dx_material.Diffuse.a);

	material->setColorParameter(
		Material::AmbientParameter,
		dx_material.Ambient.r,
		dx_material.Ambient.g,
		dx_material.Ambient.b,
		dx_material.Ambient.a);

	material->setColorParameter(
		Material::SpecularParameter,
		dx_material.Specular.r,
		dx_material.Specular.g,
		dx_material.Specular.b,
		dx_material.Specular.a);

	material->setColorParameter(
		Material::EmissiveParameter,
		dx_material.Emissive.r,
		dx_material.Emissive.g,
		dx_material.Emissive.b,
		dx_material.Emissive.a);

	material->setFloatParameter(
		Material::PowerParameter,
		dx_material.Power);
}

//------------------------------------------------------------------------------
void XFileLoader::ConvertVertex_Pos_Normal_UV(Vertex* lnVB, XFileVertex_Pos_Normal_UV* dxVB, uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		lnVB[i].position = dxVB[i].position;
		lnVB[i].normal = dxVB[i].normal;
		lnVB[i].uv = dxVB[i].uv;
		lnVB[i].color = Color::White;
	}
}

//------------------------------------------------------------------------------
void XFileLoader::ConvertVertex_Pos_Normal_Color_UV(Vertex* lnVB, XFileVertex_Pos_Normal_Color_UV* dxVB, uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		lnVB[i].position = dxVB[i].position;
		lnVB[i].normal = dxVB[i].normal;
		lnVB[i].uv = dxVB[i].uv;
		Color32 c(
			(dxVB[i].color >> 24) & 0xFF,
			(dxVB[i].color >> 16) & 0xFF,
			(dxVB[i].color >> 8) & 0xFF,
			(dxVB[i].color) & 0xFF);
		lnVB[i].color = Color(c);
	}
}

} // namespace detail
LN_NAMESPACE_END
