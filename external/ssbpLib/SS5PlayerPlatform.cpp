// 
//  SS5Platform.cpp
//
#include "SS5PlayerPlatform.h"
#include <set>
#include "../../src/Graphics/Internal.h"
#include "../../include/Lumino/Graphics/Color.h"
#include "../../include/Lumino/Graphics/Texture.h"
#include "../../src/Graphics/SpriteRenderer.h"

/**
* 各プラットフォームに合わせて処理を作成してください
* DXライブラリ用に作成されています。
*/
//#include "DxLib.h"

namespace ss
{
	::ln::detail::GraphicsManager*	g_graphicsManager = nullptr;
	::ln::detail::SpriteRenderer*		g_spriteRenderer = nullptr;
	std::set<::ln::Texture2D*>	g_texPtrSet;

	/**
	* ファイル読み込み
	*/
	unsigned char* SSFileOpen(const char* pszFileName, const char* pszMode, unsigned long * pSize)
	{
		unsigned char * pBuffer = NULL;
		SS_ASSERT2(pszFileName != NULL && pSize != NULL && pszMode != NULL, "Invalid parameters.");
		*pSize = 0;
		do
		{
		    // read the file from hardware
			FILE *fp = fopen(pszFileName, pszMode);
		    SS_BREAK_IF(!fp);
		    
		    fseek(fp,0,SEEK_END);
		    *pSize = ftell(fp);
		    fseek(fp,0,SEEK_SET);
		    pBuffer = new unsigned char[*pSize];
		    *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
		    fclose(fp);
		} while (0);
		if (! pBuffer)
		{

			std::string msg = "Get data from file(";
		    msg.append(pszFileName).append(") failed!");
		    
		    SSLOG("%s", msg.c_str());

		}
		return pBuffer;
	}

	/**
	* テクスチャの読み込み
	*/
	long SSTextureLoad(const char* pszFileName, SsTexWrapMode::_enum  wrapmode, SsTexFilterMode::_enum filtermode)
	{
		//return 0;

		ln::RefPtr<ln::Texture2D> tex(LN_NEW ln::Texture2D(), false);
		ln::String path = ln::String::FromNativeCharString(pszFileName);
		tex->Initialize(path.c_str(), ln::TextureFormat::R8G8B8A8, 1);
		
		ln::Texture2D* p = tex.DetachMove();
		g_texPtrSet.insert(p);
		return (long)p;
		
#if 0
		/**
		* テクスチャ管理用のユニークな値を返してください。
		* テクスチャの管理はゲーム側で行う形になります。
		* テクスチャにアクセスするハンドルや、テクスチャを割り当てたバッファ番号等になります。
		*
		* プレイヤーはここで返した値とパーツのステータスを引数に描画を行います。
		*/
		long rc = 0;

		rc = (long)LoadGraph(pszFileName);
		//SpriteStudioで設定されたテクスチャ設定を反映させるための分岐です。
		switch (wrapmode)
		{
		case SsTexWrapMode::clamp:	//クランプ
			break;
		case SsTexWrapMode::repeat:	//リピート
			break;
		case SsTexWrapMode::mirror:	//ミラー
			break;
		}
		switch (filtermode)
		{
		case SsTexFilterMode::nearlest:	//ニアレストネイバー
			break;
		case SsTexFilterMode::linear:	//リニア、バイリニア
			break;
		}

		return rc;
#endif
		return 0;
	}
	
	/**
	* テクスチャの解放
	*/
	bool SSTextureRelese(long handle)
	{
		//return true;

		ln::Texture2D* tex = (ln::Texture2D*)handle;
		auto itr = g_texPtrSet.find(tex);
		if (itr != g_texPtrSet.end())
		{
			tex->Release();
			g_texPtrSet.erase(itr);
			return true;
		}
		return false;

		///// 解放後も同じ番号で何度も解放処理が呼ばれるので、例外が出ないように作成してください。
		//bool rc = true;

		//if ( DeleteGraph((int)handle) == -1 )
		//{
		//	rc = false;
		//}

		//return rc ;
		return false;
	}

	/**
	* テクスチャのサイズを取得
	* テクスチャのUVを設定するのに使用します。
	*/
	bool SSGetTextureSize(long handle, int &w, int &h)
	{
		//w = 10;
		//h = 10;
		//return true;
		ln::Texture2D* tex = (ln::Texture2D*)handle;
		auto& size = tex->GetSize();
		w = size.width;
		h = size.height;
		return true;
	}

	//DXライブラリ用頂点バッファ作成関数
	//VERTEX_3D vertex3Dfrom(const ss::SSV3F_C4B_T2F &vct)
	//{
	//	VERTEX_3D v = {
	//		{ vct.vertices.x, vct.vertices.y, vct.vertices.z },
	//		vct.colors.b, vct.colors.g, vct.colors.r, vct.colors.a,
	//		vct.texCoords.u, vct.texCoords.v
	//	};
	//	return v;
	//}
	/**
	* スプライトの表示
	*/
	void SSDrawSprite(State state)
	{
		//printf("%f\n", state.scaleY);

		float x = state.mat[12];	/// 表示座標はマトリクスから取得します。
		float y = state.mat[13];	/// 表示座標はマトリクスから取得します。

		g_spriteRenderer->SetTransform(
			ln::Matrix::MakeAffineTransformation(
				ln::Vector3(state.scaleX, state.scaleY, 0),
				ln::Vector3::Zero,
				ln::Quaternion::MakeFromEulerAngles(ln::Vector3(state.rotationX, state.rotationY, state.rotationZ)),
				ln::Vector3(x, y, 0)));
		

		if (0)
		{
			g_spriteRenderer->DrawRequest2D(
				ln::Vector3(0, 0, 0),
				ln::Vector2(state.rect.size.width, state.rect.size.height),
				ln::Vector2(0, 0),
				(ln::Texture2D*)state.texture.handle,
				ln::RectF(state.rect.origin.x, state.rect.origin.y, state.rect.size.width, state.rect.size.height),
				ln::Color::White);
		}
		else
		{
			g_spriteRenderer->DrawRequest(
				ln::Vector3(0, 0, 0),
				ln::Vector2(state.rect.size.width, state.rect.size.height),
				ln::Vector2(0, 0),
				(ln::Texture2D*)state.texture.handle,
				ln::RectF(state.rect.origin.x, state.rect.origin.y, state.rect.size.width, state.rect.size.height),
				ln::Color::White,
				ln::SpriteBaseDirection::ZMinus);
		}


		//DrawRectRotaGraph(
		//	(int)x, (int)y,	//この座標が画像の中心になります。
		//	(int)state.rect.origin.x, (int)state.rect.origin.y, (int)state.rect.size.width, (int)state.rect.size.height,
		//	scaleX, SSRadianToDegree(rotationZ),
		//	state.texture.handle, TRUE, state.flipX
		//	);
#if 0
		//未対応機能
		//ステータスから情報を取得し、各プラットフォームに合わせて機能を実装してください。
		//X回転、Y回転、上下反転、カラーブレンド（一部のみ）
		//頂点変形、Xサイズ、Yサイズ
		float x = state.mat[12];	/// 表示座標はマトリクスから取得します。
		float y = state.mat[13];	/// 表示座標はマトリクスから取得します。
		float rotationZ = state.rotationZ;		/// 回転値
		float scaleX = state.scaleX;							/// 拡大率
		float scaleY = state.scaleY;							/// 拡大率

		//描画ファンクション
		//
		switch (state.blendfunc)
		{
			case BLEND_MIX:		///< 0 ブレンド（ミックス）
				if (state.opacity == 255)
				{
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, state.opacity);
				}
				else
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, state.opacity);
				}
				break;
			case BLEND_MUL:		///< 1 乗算
				SetDrawBlendMode(DX_BLENDMODE_MULA, state.opacity);
				break;
			case BLEND_ADD:		///< 2 加算
				SetDrawBlendMode(DX_BLENDMODE_ADD, state.opacity);
				break;
			case BLEND_SUB:		///< 3 減算
				SetDrawBlendMode(DX_BLENDMODE_SUB, state.opacity);
				break;

		}

		if (state.flags & PART_FLAG_COLOR_BLEND)
		{
			//RGBのカラーブレンドを設定
			//厳密に再現するには専用のシェーダーを使い、テクスチャにカラー値を合成する必要がある
			//作成する場合はssShader_frag.h、CustomSpriteのコメントとなってるシェーダー処理を参考にしてください。
			if (state.colorBlendType == VERTEX_FLAG_ONE)
			{
				//単色カラーブレンド
			}
			else
			{
				//頂点カラーブレンド
				//未対応
			}
			switch (state.colorBlendFunc)
			{
			case BLEND_MIX:
				break;
			case BLEND_MUL:		///< 1 乗算
				// ブレンド方法は乗算以外未対応
				// とりあえず左上の色を反映させる
				SetDrawBright(state.quad.tl.colors.r, state.quad.tl.colors.g, state.quad.tl.colors.b);
				break;
			case BLEND_ADD:		///< 2 加算
				break;
			case BLEND_SUB:		///< 3 減算
				break;
			}
//			DrawModiGraph

		}

#ifdef UP_MINUS
		/**
		* DXライブラリのスプライト表示機能ではXとY同時拡大なので、とりあえずXスケールを使用する
		* Y反転できないので未対応
		* DrawRectRotaGraphはxとyが中心になるように、テクスチャの矩形を表示します。
		* DXライブラリのスプライト表示機能は上方向がマイナスになります。
		*/
		SetDrawBright(state.quad.bl.colors.r, state.quad.bl.colors.g, state.quad.bl.colors.b);
		DrawRectRotaGraph(
			(int)x, (int)y,	//この座標が画像の中心になります。
			(int)state.rect.origin.x, (int)state.rect.origin.y, (int)state.rect.size.width, (int)state.rect.size.height,
			scaleX, SSRadianToDegree(rotationZ),
			state.texture.handle, TRUE, state.flipX
			);
		SetDrawBright(255, 255, 255);
#else
		/**
		* DXライブラリの3D機能を使用してスプライトを表示します。
		* DXライブラリの3D機能は上方向がプラスになります。
		* 3Dを使用する場合頂点情報を使用して再現すると頂点変形やUV系のアトリビュートを反映させる事ができます。
		*/
		//描画用頂点情報を作成
		SSV3F_C4B_T2F_Quad quad;
		quad = state.quad;
		quad.tl.vertices.x *= scaleX;
		quad.tl.vertices.y *= scaleY;
		quad.tr.vertices.x *= scaleX;
		quad.tr.vertices.y *= scaleY;
		quad.bl.vertices.x *= scaleX;
		quad.bl.vertices.y *= scaleY;
		quad.br.vertices.x *= scaleX;
		quad.br.vertices.y *= scaleY;

		//頂点の回転、3D描画はY方向が逆なので角度をマイナスで計算する
		get_uv_rotation(&quad.tl.vertices.x, &quad.tl.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.tr.vertices.x, &quad.tr.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.bl.vertices.x, &quad.bl.vertices.y, 0, 0, rotationZ);
		get_uv_rotation(&quad.br.vertices.x, &quad.br.vertices.y, 0, 0, rotationZ);

		quad.tl.vertices.x += x;
		quad.tl.vertices.y += y;
		quad.tr.vertices.x += x;
		quad.tr.vertices.y += y;
		quad.bl.vertices.x += x;
		quad.bl.vertices.y += y;
		quad.br.vertices.x += x;
		quad.br.vertices.y += y;

		//頂点カラーにアルファを設定
		quad.tl.colors.a = quad.bl.colors.a * state.opacity / 255;
		quad.tr.colors.a = quad.bl.colors.a * state.opacity / 255;
		quad.bl.colors.a = quad.bl.colors.a * state.opacity / 255;
		quad.br.colors.a = quad.bl.colors.a * state.opacity / 255;

		//DXライブラリ用の頂点バッファを作成する
		VERTEX_3D vertex[4] = {
			vertex3Dfrom(quad.tl),
			vertex3Dfrom(quad.bl),
			vertex3Dfrom(quad.tr),
			vertex3Dfrom(quad.br)
		};
		//3Dプリミティブの表示
		DrawPolygon3DBase(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP, state.texture.handle, true);
#endif

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドステートを通常へ戻す
#endif
	}

	/**
	* ユーザーデータの取得
	*/
	void SSonUserData(Player *player, UserData *userData)
	{
		//ゲーム側へユーザーデータを設定する関数を呼び出してください。
	}

	/**
	* ユーザーデータの取得
	*/
	void SSPlayEnd(Player *player)
	{
		//ゲーム側へアニメ再生終了を設定する関数を呼び出してください。
	}


	///**
	//* 文字コード変換
	//*/ 
	//std::string utf8Togbk(const char *src)
	//{
	//	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	//	unsigned short * wszGBK = new unsigned short[len + 1];
	//	memset(wszGBK, 0, len * 2 + 2);
	//	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

	//	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	//	char *szGBK = new char[len + 1];
	//	memset(szGBK, 0, len + 1);
	//	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//	std::string strTemp(szGBK);
	//	if (strTemp.find('?') != std::string::npos)
	//	{
	//		strTemp.assign(src);
	//	}
	//	delete[]szGBK;
	//	delete[]wszGBK;
	//	return strTemp;
	//}

	///**
	//* windows用パスチェック
	//*/ 
	bool isAbsolutePath(const std::string& strPath)
	{
		return ln::PathTraits::IsAbsolutePath(strPath.c_str());
		//std::string strPathAscii = utf8Togbk(strPath.c_str());
		//if (strPathAscii.length() > 2
		//	&& ((strPathAscii[0] >= 'a' && strPathAscii[0] <= 'z') || (strPathAscii[0] >= 'A' && strPathAscii[0] <= 'Z'))
		//	&& strPathAscii[1] == ':')
		//{
		//	return true;
		//}
		return false;
	}

};
