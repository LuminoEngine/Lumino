
#pragma once
#include "../Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		GUI イベントの引数の基本クラスです。
*/
class EventArgs
	: public CoreObject
{
public:
	EventArgs();
	virtual ~EventArgs();

protected:

};

/**
	@brief		
*/
class PooledEventArgs
	: public EventArgs
{
public:
	PooledEventArgs();
	virtual ~PooledEventArgs();

protected:
	static const int DataSize = 64;
	byte_t	m_data[DataSize];
};

/**
	@brief	マウスイベントの引数を表します。
*/
class MouseEventArgs
	: public PooledEventArgs
{
public:
	MouseEventArgs();
	virtual ~MouseEventArgs();

protected:
	struct Data
	{
		MouseButton	Button;		///< ボタン番号
		short		Delta;		///< マウスホイールの回転回数 (正または負の回数)
		short		X;			///< マウスイベント生成時のマウスの X 座標 (クライアント領域外は -1)
		short		Y;			///< マウスイベント生成時のマウスの Y 座標 (クライアント領域外は -1)
		//short		MoveX;	    ///< X 座標の移動量
		//short		MoveY;      ///< Y 座標の移動量
	};
};

/**
	@brief	キーボードイベントの引数を表します。	
*/
class KeyEventArgs
	: public PooledEventArgs
{
public:
	KeyEventArgs();
	virtual ~KeyEventArgs();

protected:
	struct Data
	{
		Key			KeyCode;	///< キーコード
		bool		IsAlt;		///< Alt キーが押されている場合 true
		bool		IsShift;	///< Shift キーが押されている場合 true
		bool		IsControl;	///< Ctrl キーが押されている場合 true
	};
};

class EventArgsPool
{
public:

private:
};

	//bool injectMouseMove(float delta_x, float delta_y);
	//bool injectMouseButtonDown(MouseButton button);
	//bool injectMouseButtonUp(MouseButton button);
	//bool injectKeyDown(Key::Scan scan_code);
	//bool injectKeyUp(Key::Scan scan_code);
	//bool injectChar(String::value_type code_point);
	//bool injectMouseWheelChange(float delta);


} // namespace GUI
} // namespace Lumino
