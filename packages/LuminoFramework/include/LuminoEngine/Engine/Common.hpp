
#pragma once

namespace ln {
class World;
class Application;

typedef void (*ConfigureAppFunc)();
typedef Application* (*CreateAppInstanceFunc)();

enum class EngineFeature
{
    Application = 0x0001,
    Audio = 0x0002,
    Input = 0x0004,
    Graphics = 0x0008,
    Rendering = 0x0010,



    Public = Application | Graphics,
    Experimental = 0xFFFF,
};
LN_FLAGS_OPERATORS(EngineFeature);

/** Application running mode. */
enum class ApplicationMode
{
    /** ゲームアプリケーションとして実行します。 */
    Game,

    /**
     * エディタアプリケーションとして実行します。
     *
     * エディタアプリケーションは
     */
    Editor,
};

} // namespace ln
