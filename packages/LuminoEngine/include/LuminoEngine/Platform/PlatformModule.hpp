#pragma once
#include <LuminoEngine/Engine/Module.hpp>
#include "Common.hpp"

namespace ln {

struct PlatformModuleSettings {
    WindowCreationSettings mainWindowSettings;
    WindowSystem windowSystem = WindowSystem::Native;
};

class PlatformModule : public Module
{
public:
	/**
	 * Initialize PlatformModule.
	 * 
	 * mainWindowSettings を設定することで、このモジュールの初期化と同時に PlatformWindow を作成することができます。
	 * この PlatformWindow は Platform::mainWindow() で取得できます。
	 * 
	 * mainWindowSettings はオプションです。
	 * ただし Graphcis モジュールのバックエンドに OpenGL を使う場合、モジュールの初期化時点で
	 * アクティブな OpenGL コンテキストが必要になるため、あらかじめウィンドウを作成しておく必要があります。
	 * 
	 * なお、Lumino はデスクトップ環境での OpenGL の利用は非推奨です。
	 * WebGL との互換性のために残されています。
	 */
    static PlatformModule* initialize(const PlatformModuleSettings& settings = {});

	/**
	 * Terminate PlatformModule.
	 */
	static void terminate();
};

} // namespace ln
