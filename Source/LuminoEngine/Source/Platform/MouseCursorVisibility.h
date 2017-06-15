
#pragma once
#include <Lumino/Base/Environment.h>

LN_NAMESPACE_BEGIN
namespace detail
{

class MouseCursorVisibility
{
public:
	MouseCursorVisibility()
		: m_visibleMouseCursor(true)
		, m_mouseVisibleTime(0)
		, m_startTime(0)
		, m_requestForced(false)
	{}

	~MouseCursorVisibility()
	{}

public:

	/// この関数が false を返している間は非表示にする (ワーカースレッドでチェック)
	bool checkVisible() const
	{
		if (m_requestForced) return true;
		if (m_visibleMouseCursor) return true;
		if (m_mouseVisibleTime == 0) return false;
		return ((Environment::getTickCount() - m_startTime) < m_mouseVisibleTime);
	}

	/// マウスカーソルの表示設定 (count は マウスカーソル放置時、消すまでの時間(ms)。0 で常に消す)
	void setMouseCursorVisibleState(bool visible, int time)
	{
		m_visibleMouseCursor = visible;
		m_mouseVisibleTime = std::max(0, time);
		m_startTime = Environment::getTickCount();
		m_requestForced = false;
	}

	/// マウスカーソルの操作後に呼ぶこと
	void onMoveCursor(bool on_client_area)
	{
		// 表示しない場合
		//if (!m_visibleMouseCursor && m_mouseVisibleTime == 0)
		//{
		//	m_requestForced = false;
		//}
		//else
		{
			if (on_client_area)
			{
				m_startTime = Environment::getTickCount();
				m_requestForced = false;
			}
			else
			{
				m_requestForced = true;
			}
		}
	}

private:
	bool		m_visibleMouseCursor;
	uint64_t	m_mouseVisibleTime;
	uint64_t	m_startTime;
	bool		m_requestForced;

};

} // namespace detail
LN_NAMESPACE_END
