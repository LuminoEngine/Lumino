
#pragma once
#include "Common.hpp"

namespace ln {
class InputGesture;

/**
 * ゲームパッドやキーボードなどの入力デバイスを透過的に扱うためのインターフェイスです。
 */
LN_CLASS(Static)
class Input
{
public:
    /**
     * ボタンが現在押されているかを判定します。
     */
    LN_METHOD()
    static bool isPressed(const StringView& buttonName);

    /**
     * ボタンが新しく押された瞬間を判定します。
     */
    LN_METHOD()
    static bool isTriggered(const StringView& buttonName);

    /**
     * ボタンが離された瞬間を判定します。
     */
    LN_METHOD()
    static bool isTriggeredOff(const StringView& buttonName);

    /**
     * ボタンが新しく押された瞬間とリピート状態を判定します。     
     */
    LN_METHOD()
    static bool isRepeated(const StringView& buttonName);

    /**
     * 指定した軸のアナログ値を取得します。      
     */
    LN_METHOD()
    static float getAxisValue(const StringView& buttonName);

    /**
     * ボタンのアクションマッピングを追加します。
     */
    LN_METHOD()
    static void addBinding(const StringView& buttonName, InputGesture* gesture);

    /**
     * アクションマッピングを除外します。
    */
    LN_METHOD()
    static void removeBinding(InputGesture* gesture);

    /**
     * 指定したボタンのアクションマッピングをすべて削除します。
     */
    LN_METHOD()
    static void clearBindings(const StringView& buttonName);

    /**
     * 全てのアクションマッピングを削除します。       
     */
    LN_METHOD()
    static void clearAllBindings();

    /**
     * サポートしているすべての入力のうち、現在入力中であるものを InputGesture として返します。
     * 
     * この機能はキーコンフィグ用のヘルパーです。新しく押された瞬間を判定します。それ以外は nullptr を返します。  
     */
    //LN_METHOD()
    static InputGesture* getAnyActiveTriggered();

    /** すべてのボタンが離されるまで入力を無効にします。 */
    static void disableUntilIdle();

private:
};

/**
 * デフォルトでマッピングされる仮想ボタンの名前を表します。
 */
class InputButtons
{
public:
    /** 左方向を表す仮想ボタンの名前 */
    static const String Left;

    /** 右方向を表す仮想ボタンの名前 */
    static const String Right;

    /** 上方向を表す仮想ボタンの名前 */
    static const String Up;

    /** 下方向を表す仮想ボタンの名前 */
    static const String Down;

    /** 決定操作を表す仮想ボタンの名前 */
    static const String Submit;

    /** キャンセル操作を表す仮想ボタンの名前 */
    static const String Cancel;

    /** メニュー操作を表す仮想ボタンの名前 */
    static const String Menu;

    /** 補助操作を表す仮想ボタンの名前 */
    static const String Shift;

    /** 前ページの表示操作を表す仮想ボタンの名前 */
    static const String PageUp;

    /** 次ページの表示操作を表す仮想ボタンの名前 */
    static const String PageDown;

    /** マッピングされているいずれかの操作を表す仮想ボタンの名前 */
    static const String Any;
};

} // namespace ln
