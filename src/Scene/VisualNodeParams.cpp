
#pragma once

#include "../Internal.h"
#include "VisualNodeParams.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// VisualNodeParams
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeParams::VisualNodeParams()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeParams::~VisualNodeParams()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNodeParams::Create(int subsetCount)
{
	LN_THROW(subsetCount >= 1, ArgumentException);

	m_combinedSubsetParams.Resize(subsetCount);

	// もし 1 より多ければサブセット単位のパラメータを各個作成する。
	// (1つの場合はメモリ使用量を抑えるため、m_globalParams だけを使う)
	if (subsetCount > 1) {
		m_subsetParams.Resize(subsetCount);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VisualNodeSubsetParams& VisualNodeParams::GetSubsetParams(int index)
{
	// -1 の時は m_globalParams を返す。
	// また、サブセット数が 1 の時はメモリ使用量を押させるため m_subsetParams を確保していない。m_globalParams を共有する。
	if (index == -1 || m_subsetParams.IsEmpty()) {
		return m_globalParams;
	}
	return m_subsetParams[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const VisualNodeSubsetParams& VisualNodeParams::GetSubsetParams(int index) const
{
	// ※↑と同じ
	if (index == -1 || m_subsetParams.IsEmpty()) {
		return m_globalParams;
	}
	return m_subsetParams[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VisualNodeParams::UpdateSubsetRenderParam(const VisualNodeParams* parentParams)
{
	//---------------------------------------------------------
	// グローバル設定の更新

	m_combinedGlobalParams = m_globalParams;

	// 親設定がある場合は親のグローバル設定を掛け合わせる
	if (parentParams != NULL) {
		m_combinedGlobalParams.Multiply(parentParams->m_combinedGlobalParams);
	}


	//---------------------------------------------------------
	// 各サブセット設定の更新

	if (m_combinedSubsetParams.GetCount() == 1) {
		m_combinedSubsetParams[0] = m_combinedGlobalParams;	// サブセット数が 1 の時はメモリ使用量を押させるため m_globalParams だけを使用する。
	}
	else
	{
		for (int i = 0; i < m_combinedSubsetParams.GetCount(); i++) {
			m_combinedSubsetParams[i] = m_subsetParams[i];
			m_combinedSubsetParams[i].Multiply(m_combinedGlobalParams);	// グローバル設定を掛け合わせる
		}
	}
}

} // namespace Scene
} // namespace Lumino
