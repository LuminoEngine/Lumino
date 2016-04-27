
#include "../../Internal.h"
#include "MMEShader.h"
#include "MMEShaderErrorInfo.h"
#include "MMEShaderTechnique.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

#define LN_SET_SHADER_ERR_MSG( fmt_, ... ) \
{ \
    LRefTString err; \
    err.format( fmt_, __VA_ARGS__ ); \
    mSceneShader->setErrorMessage( err ); \
}

//=============================================================================
// MMEShaderTechnique
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique::MMEShaderTechnique()
	: m_errorInfo(NULL)
	, m_ownerShader(NULL)
	, m_coreShader(NULL)
	, m_technique(NULL)
	, m_mmdPass(MMD_PASS_object)
	, m_isUseTexture()
	, m_isUseSphereMap()
	, m_isUseToon()
	, m_isUseSelfShadow()
	, m_scriptCommandList()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique::~MMEShaderTechnique()
{
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses) {
		pass->Release();
	}

	//ln_foreach( DrawingCommandList* list, mPassDrawingCommandListArray )
	//{
	//	SAFE_DELETE( list );
	//}
	//mPassDrawingCommandListArray.clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::Initialize(MMEShader* shader, ShaderTechnique* tech, MMEShaderErrorInfo* errorInfo)
{
	m_errorInfo = errorInfo;
	m_ownerShader = shader;
	m_coreShader = m_ownerShader->GetCoreShader();
	m_technique = tech;

	m_scriptCommandList.Initialize(m_ownerShader);

    //-------------------------------------------
    // MMD パスとアノテーションを調べる

    bool use_texture = false;
    bool use_sphere_map = false;
    bool use_toon = false;
    ShaderVariable* anno = NULL;

	anno = m_technique->FindAnnotation(_T("MMDPass"));
    if (anno != NULL)
    {
		const TCHAR* mmd_pass = anno->GetString();
		if (mmd_pass != NULL)
		{
			if (StringTraits::tcsicmp(mmd_pass, _T("object")) == 0) {
				m_mmdPass = MMD_PASS_object;
			}
			else if (StringTraits::tcsicmp(mmd_pass, _T("zplot")) == 0) {
				m_mmdPass = MMD_PASS_zplot;
			}
			else if (StringTraits::tcsicmp(mmd_pass, _T("object_ss")) == 0) {
				m_mmdPass = MMD_PASS_object_ss;
			}
			else if (StringTraits::tcsicmp(mmd_pass, _T("shadow")) == 0) {
				m_mmdPass = MMD_PASS_shadow;
			}
			else if (StringTraits::tcsicmp(mmd_pass, _T("edge")) == 0) {
				m_mmdPass = MMD_PASS_edge;
			}
		}
    }

	anno = m_technique->FindAnnotation(_T("UseTexture"));
	if (anno != NULL) {
		m_isUseTexture = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseSphereMap"));
	if (anno != NULL) {
		m_isUseSphereMap = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseToon"));
	if (anno != NULL) {
		m_isUseToon = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseSelfShadow"));
	if (anno != NULL) {
		m_isUseSelfShadow = anno->GetBool();
	}


    //-------------------------------------------
    // 描画するSubset番号の指定を調べる

    const TCHAR* subsetText = NULL;

	anno = m_technique->FindAnnotation(_T("Subset"));
	if (anno != NULL) {
		subsetText = anno->GetString();
	}
	if (subsetText != NULL) {
		ParseSubsetIndexArray(subsetText, &m_subsetIndexArray, &m_subsetIndexEndToRange);
    }

	//m_scriptCommandList.SetDrawingSubsetNumbers(subsetIndexArray);
        
    //-------------------------------------------
    // Script

	// 先に全ての Pass のコマンドリスト (入れ物) を作っておく。
	// (スクリプト解析で "Pass" を見つけたときに検索できるようにするため)
	LN_FOREACH(ShaderPass* pass, m_technique->GetPasses())
	{
		MMEShaderPass* mmePass = LN_NEW MMEShaderPass();
		mmePass->m_pass = pass;
		mmePass->m_scriptCommandList.Initialize(m_ownerShader);
		m_mmeShaderPasses.Add(mmePass);

		m_scriptCommandList.AddChildPassCommandList(&mmePass->m_scriptCommandList);
	}

    // テクニックに "Script" アノテーションがあるかチェック 
    ShaderVariable* scriptAnno;
	scriptAnno = m_technique->FindAnnotation(_T("Script"));
	if (scriptAnno != NULL && scriptAnno->GetType() == ShaderVariableType_String)
    {
		ParseScriptCommandString(scriptAnno, NULL, &m_scriptCommandList);
    }
    // 省略されている場合は全てのパスを実行するコマンドリストを作る
    else
    {
		LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses) {
			m_scriptCommandList.Add_Pass(pass->m_pass, &pass->m_scriptCommandList);
		}
    }

	// 最後に全てのパスのスクリプトを解析する
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses)
	{
		// "Script" アノテーションがあれば解析し、省略されている場合は "DrawGeometry" だけ追加する
		scriptAnno = pass->m_pass->FindAnnotation(_T("Script"));
		if (scriptAnno != NULL && scriptAnno->GetType() == ShaderVariableType_String) {
			ParseScriptCommandString(scriptAnno, pass->m_pass, &pass->m_scriptCommandList);
		}
		else {
			pass->m_scriptCommandList.Add_DrawGeometry(pass->m_pass);
		}
	}


    //-------------------------------------------
    // 中身チェック
#if 0
    printf( "◆ Script\n" );
    MMEScriptCommandList::iterator it = mScriptCommandList.begin();
    for ( ; it != mScriptCommandList.end(); ++it )
    {
        MMEScriptCommand* c = &(*it);
        switch ( c->Type )
        {
            case MME_SCRIPTCOM_RenderColorTarget:           
                printf( "RenderColorTarget index:%d var:%p\n", c->Index, c->Variable ); break;
            case MME_SCRIPTCOM_RenderDepthStencilTarget:    
                printf( "RenderDepthStencilTarget\n" ); break;
            case MME_SCRIPTCOM_ClearSetColor:               
                printf( "ClearSetColor\n" ); break;
            case MME_SCRIPTCOM_ClearSetDepth:               
                printf( "ClearSetDepth\n" ); break;
            case MME_SCRIPTCOM_Clear_Color:                 
                printf( "Clear_Color        %X\n", (it)->Color ); break;
            case MME_SCRIPTCOM_Clear_Depth:                 
                printf( "Clear_Depth        %f\n", (it)->Depth ); break;
            case MME_SCRIPTCOM_ScriptExternal_Color:        
                printf( "ScriptExternal_Color\n" ); break;
            case MME_SCRIPTCOM_Pass:                        
                printf( "Pass\n" ); break;
            case MME_SCRIPTCOM_BeginPass:                   
                printf( "BeginPass\n" ); break;
            case MME_SCRIPTCOM_EndPass:                     
                printf( "EndPass\n" ); break;
            case MME_SCRIPTCOM_LoopByCount:                 
                printf( "LoopByCount\n" ); break;
            case MME_SCRIPTCOM_LoopEnd:                     
                printf( "LoopEnd\n" ); break;
            case MME_SCRIPTCOM_LoopGetIndex:                
                printf( "LoopGetIndex\n" ); break;
            case MME_SCRIPTCOM_Draw_Geometry:               
                printf( "Draw_Geometry\n" ); break;
            case MME_SCRIPTCOM_Draw_Buffer:                 
                printf( "Draw_Buffer\n" ); break;
            case MME_SCRIPTCOM_RenderPort:                  
                printf( "RenderPort\n" ); break;
        }  
    }
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MMEShaderTechnique::ContainsSubsetIndex(int subsetIndex) const
{	
	// "Subset" が省略されている。全部描画できる。
	if (m_subsetIndexArray.IsEmpty()) {
		return true;
	}
	// 範囲の終端が '-' になっている。インデックス範囲外は全て描画可能。
	if (subsetIndex >= m_subsetIndexArray.GetCount()) {
		return m_subsetIndexEndToRange;
	}
	return m_subsetIndexArray.GetAt(subsetIndex);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderPass* MMEShaderTechnique::FindMMEShaderPass(const TCHAR* passName)
{
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses)
	{
		if (pass->m_pass->GetName().Compare(passName)) {
			return pass;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseScriptCommandString(ShaderVariable* scriptAnno, ShaderPass* thisPass, ShaderScriptCommandList* commandList)
{
    TCHAR* str = NULL;
    //char* s;
    //char* tp;
    //Graphics::ShaderVariable* tv;

	const TCHAR* script = scriptAnno->GetString();
    if ( script )
	{
		// 区切り文字(;)を終端文字に置換して処理をするため、一度書き換え可能バッファにs全部コピーする
		ByteBuffer tmpBuf(script);
		TCHAR* cmdLine = (TCHAR*)tmpBuf.GetData();
		while (true)
		{
			// ; を探して '\0' に置換。無ければ終端ということで終了
			int nextSemiColonIdx = StringTraits::IndexOf(cmdLine, -1, _T(";"), 1);
			if (nextSemiColonIdx == -1) { return; }

			cmdLine[nextSemiColonIdx] = _T('\0');

			// 置換した '\0' まで、(文字列の部分一致でだけど) キーワードを探していく
			switch (cmdLine[0])
            {
                /////////////////////////////
                // RenderColorTarget_0～3
                // RenderDepthStencilTarget
                // RenderPort
                case _T('R'):
                {
                    ///////////////////////////// RenderColorTarget_0～3
					if (cmdLine[6] == _T('C'))
                    {
                        // 設定インデックスを調べる
                        uint32_t index;
						TCHAR lc = cmdLine[17];
                        if ( '0' <= lc && lc <= '3' ) {
                            index = lc - '0';
                        }
                        else {
                            index = 0;
                        }

                        // = の後ろがテクスチャ型変数名になっているはず
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx == -1) {									// テクスチャ変数名が省略されている
							commandList->Add_RenderColorTarget(index, NULL);// デフォルトに戻すコマンド
						}
						else
						{
							// 変数を検索し、コマンド化
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_DeviceTexture) {
								commandList->Add_RenderColorTarget(index, v);
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidRenderTargetVariable, &cmdLine[idx + 1]);
								return;
							}
						}
                    }
                    ///////////////////////////// RenderDepthStencilTarget
					else if (cmdLine[6] == _T('D'))
                    {
						// = の後ろがテクスチャ型変数名になっているはず
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx == -1) {									// テクスチャ変数名が省略されている
							commandList->Add_RenderDepthStencilTarget(NULL);// デフォルトに戻すコマンド
						}
						else
						{
							// 変数を検索し、コマンド化
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_DeviceTexture) {
								commandList->Add_RenderDepthStencilTarget(v);
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidRenderDepthStencilTargetVariable, &cmdLine[idx + 1]);
								return;
							}
						}
                    }
                    ///////////////////////////// RenderPort
					else if (cmdLine[6] == _T('P'))
                    {
                        // MME 自体も未実装のコマンド。無視。
                    }
                    break;
                }

                /////////////////////////////
                // ClearSetColor
                // ClearSetDepth
                // Clear_Color
                // Clear_Depth
                case _T('C'):
                {
                    ///////////////////////////// ClearSetColor
					if (cmdLine[8] == _T('C'))
					{
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx != -1)
						{
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_Vector)
							{
								commandList->Add_ClearSetColor(v->GetVector());
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetColorVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: 変数名が見つからなかった
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetColorVariable);
							return;
						}
                    }
                    ///////////////////////////// ClearSetDepth
					else if (cmdLine[8] == _T('D'))
					{
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx != -1)
						{
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_Float)
							{
								commandList->Add_ClearSetDepth(v->GetFloat());
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: 変数名が見つからなかった
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable);
							return;
						}
                    }
                    ///////////////////////////// Clear_Color
					else if (cmdLine[6] == _T('C'))
                    {
						commandList->Add_ClearColor();
                    }
                    ///////////////////////////// Clear_Depth
					else if (cmdLine[6] == _T('D'))
                    {
						commandList->Add_ClearDepth();
                    }
                    break;
                }

                /////////////////////////////
                // ScriptExternal_Color
                case _T('S'):
                {
					commandList->Add_ScriptExternal_Color();
                    break;
                }

                /////////////////////////////
                // Pass
                case _T('P'):
				{
					int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
					if (idx != -1)
					{
						MMEShaderPass* mmeShaderPass = FindMMEShaderPass(&cmdLine[idx + 1]);
						if (mmeShaderPass != NULL) {
							commandList->Add_Pass(mmeShaderPass->m_pass, &mmeShaderPass->m_scriptCommandList);
						}
						else {
							// Error: 無効なパス名
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidPassName, &cmdLine[idx + 1]);
							return;
						}
					}
					else {
						// Error: 無効なパス名
						m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidPassName);
						return;
					}
                    break;
                }

                /////////////////////////////
                // LoopByCount
                // LoopEnd
                // LoopGetIndex
                case _T('L'):
                {
                    ///////////////////////////// LoopByCount
					if (cmdLine[4] == _T('B'))
                    {
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx != -1)
						{
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_Int)
							{
								commandList->Add_LoopByCount(v->GetInt());
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopByCountVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: 変数名が見つからなかった
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopByCountVariable);
							return;
						}
                    }
                    ///////////////////////////// LoopEnd
					else if (cmdLine[4] == _T('E'))
                    {
						commandList->Add_LoopEnd();
                    }
                    ///////////////////////////// LoopGetIndex
					else if (cmdLine[4] == _T('G'))
                    {
						int idx = StringTraits::IndexOf(cmdLine, -1, _T("="), 1);
						if (idx != -1)
						{
							ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == ShaderVariableType_Int)
							{
								commandList->Add_LoopGetIndex(v);
							}
							else {
								// Error: 変数名が見つからなかった
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: 変数名が見つからなかった
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable);
							return;
						}
                    }
                    break;
                }

                /////////////////////////////
                // Draw_Geometry
                // Draw_Buffer
                case _T('D'):
				{
					///////////////////////////// Draw_Geometry
					if (cmdLine[5] == _T('G'))
					{
						commandList->Add_DrawGeometry(thisPass);
					}
                    ///////////////////////////// Draw_Buffer
					else if (cmdLine[5] == _T('B'))
                    {
						commandList->Add_DrawBuffer(thisPass);
                    }
                    break;
                }
            }

			cmdLine += nextSemiColonIdx + 1;	// ; の次を指す
		}
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseSubsetIndexArray(const String& subsetText, Array<bool>* subsetIndexArray, bool* endToRange)
{
	*endToRange = false;
	Array<String> tokens = StringTraits::Split(String(subsetText), _T(","), StringSplitOptions::RemoveEmptyEntries);

	LN_FOREACH(String& token, tokens)
	{
		if (*endToRange) {
			// TODO: 文法エラー。最後が '-' であるトークンは最後のトークンでなければならない
			return;
		}

		int rangeMarkPos = token.IndexOf(_T("-"));

		// '-' の無い普通の数値だった
		if (rangeMarkPos == -1)
		{
			int n = token.ToInt32();//_ttoi(token.c_str());
			while (subsetIndexArray->GetCount() < n - 1) {	// サブセット番号 n までに足りない要素を詰めていく
				subsetIndexArray->Add(false);
			}
			subsetIndexArray->Add(true);					// n 番のサブセットは描画できる
		}
		// トークンの最後が '-' だった。以降全てのサブセットを描画することを示す
		else if (rangeMarkPos == token.GetLength() - 1)
		{
			int n = token.Left(rangeMarkPos).ToInt32(); //_ttoi(token.Left(rangeMarkPos));
			while (subsetIndexArray->GetCount() < n - 1) {	// サブセット番号 n までに足りない要素を詰めていく
				subsetIndexArray->Add(false);
			}
			subsetIndexArray->Add(true);					// n 番のサブセットは描画できる
			*endToRange = true;								// 以降、全てのサブセットを描画することを示す
		}
		// "2-5" のようなトークン
		else
		{
			int start = token.Left(rangeMarkPos).ToInt32();//_ttoi(token.Left(rangeMarkPos));
			int end = token.Right((token.GetLength() - rangeMarkPos) - 1).ToInt32();// _ttoi(token.Right((token.GetLength() - rangeMarkPos) - 1));
			while (subsetIndexArray->GetCount() < start - 1) {	// サブセット番号 start までに足りない要素を詰めていく
				subsetIndexArray->Add(false);
			}
			while (subsetIndexArray->GetCount() < end) {		// サブセット end 番まで描画できる
				subsetIndexArray->Add(true);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseSubsetIndexArray(const TCHAR* subsetText, Array<int>* subsetIndexArray)
{
	struct SubsetIndexOrder
	{
		int	Index;
		int	RangeState;    ///< 範囲の指定 (0=なし 1=つぎのSubsetIndexの値まで 2=サブセット数の最大まで)
	};
	typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;


	const TCHAR* c;
	const TCHAR* lc; // 最後に見つかった ',' まはた '-' の次の文字
	int   num;
	int   last_num = 0;
	bool  is_range = false;     // - によって範囲指定がある場合 true
	TCHAR	temp[64];
	uint32_t   size = 0;             // 実際に展開される配列の要素数
	SubsetIndexOrderList    subse_index_order_list;

	SubsetIndexOrder si;
	SubsetIndexOrder last_si;
	last_si.Index = 0;
	last_si.RangeState = 0;

	c = subsetText;
	lc = c;

	// コンマか終端まで探すループ
	while (true)
	{
		if (*c == _T(',') || *c == _T('\0'))
		{
			StringTraits::StrNCpy(temp, 64, lc, c - lc);

			temp[c - lc] = _T('\0');
            num = StringTraits::ToInt32(temp);
			lc = c + 1;

			si.Index = num;

			// ひとつ前の文字が - の場合はサブセット終端までの範囲指定
			if (*(c - 1) == _T('-'))
			{
				si.RangeState = 2;
				size += 2;          // 数値と、終端までを表す値 0xffffffff を格納する分
			}
			else
			{
				si.RangeState = 0;

				// ひとつ前からの範囲指定の場合
				if (last_si.RangeState == 1)
				{
					size += (si.Index - last_si.Index) + 1;   // 後ろの数字も含むので + 1  ( i < num ではなく i <= num )
				}
				// ひとつの数値の場合
				else
				{
					++size;
				}
			}

			subse_index_order_list.push_back(si);
			last_si = si;   // いらないかも

			// 終端か、サブセットの終端まで読むものが見つかった場合はここで終了
			if (*c == _T('\0') || si.RangeState == 2)
			{
				break;
			}
		}
		// - が見つかった場合はひとつ前の , (または先頭) からを数値にしておく
		else if (*c == _T('-'))
		{
			StringTraits::StrNCpy(temp, 64, lc, c - lc);
			temp[c - lc] = _T('\0');
			last_num = StringTraits::ToInt32(temp);
			is_range = true;
			lc = c + 1;

			SubsetIndexOrder si;
			si.Index = last_num;
			si.RangeState = true;
			subse_index_order_list.push_back(si);

			last_si = si;
		}
		++c;
	}



	// 作成に失敗した場合
	if (subse_index_order_list.empty())
	{
		// 空にする
		subsetIndexArray->Clear();
	}
	else
	{
		subsetIndexArray->Resize(size);

		uint32_t idx = 0;
		SubsetIndexOrderList::iterator  it1 = subse_index_order_list.begin();
		SubsetIndexOrderList::iterator end1 = subse_index_order_list.end();
		for (; it1 != end1;)
		{
			// 次の値までの範囲指定
			if (it1->RangeState == 1)
			{
				int i = it1->Index;
				++it1;

				// 次がサブセット数までの範囲指定の場合
				if (it1->RangeState == 2)
				{
					(*subsetIndexArray)[idx] = i;
					++idx;

					(*subsetIndexArray)[idx] = 0xffffffff;
					// 誤作動防止のため、ここで終了。it1->RangeState == 2 のとき、終端は必ず 0xffffffff になる。
					break;
				}
				else
				{
					for (; (i <= it1->Index); ++i)
					{
						(*subsetIndexArray)[idx] = i;
						++idx;
					}
					++it1;
				}
			}
			// 普通の値
			else
			{
				(*subsetIndexArray)[idx] = it1->Index;
				++idx;
				++it1;
			}
		}
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
