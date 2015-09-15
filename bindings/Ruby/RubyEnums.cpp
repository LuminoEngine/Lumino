#include "LuminoRuby.h"

VALUE g_enum_Result;
VALUE g_enum_VariantType;
VALUE g_enum_FileAccessPriority;
VALUE g_enum_FileType;
VALUE g_enum_FileMode;
VALUE g_enum_FileAccess;
VALUE g_enum_SeekOrigin;
VALUE g_enum_AsyncIOState;
VALUE g_enum_RotationOrder;
VALUE g_enum_KeyCode;
VALUE g_enum_AcceleratorKey;
VALUE g_enum_Button;
VALUE g_enum_DeviceButtonType;
VALUE g_enum_DeviceButton;
VALUE g_enum_DirectMusicMode;
VALUE g_enum_SoundPlayingMode;
VALUE g_enum_SoundPlayingState;
VALUE g_enum_SoundFadeBehavior;
VALUE g_enum_InternalGameSound;
VALUE g_enum_BackbufferResizeMode;
VALUE g_enum_TextureFormat;
VALUE g_enum_BlendMode;
VALUE g_enum_CullingMode;
VALUE g_enum_TextAlign;
VALUE g_enum_PlacementType;
VALUE g_enum_FrontAxis;
VALUE g_enum_MeshCreateFlags;
VALUE g_enum_CameraType;
VALUE g_enum_Coord;
VALUE g_enum_ObjectType;


void InitEnums()
{
    g_enum_Result = rb_define_module_under(g_luminoModule, "Result");
    rb_define_const(g_enum_Result, "OK", INT2FIX(0));
    rb_define_const(g_enum_Result, "UNKNOWN", INT2FIX(-1));
    rb_define_const(g_enum_Result, "OUT_OF_MEMORY", INT2FIX(-2));
    rb_define_const(g_enum_Result, "IO", INT2FIX(-3));
    rb_define_const(g_enum_Result, "END_OF_STREAM", INT2FIX(-4));
    rb_define_const(g_enum_Result, "ARGUMENT", INT2FIX(-5));
    rb_define_const(g_enum_Result, "INVALID_OPERATION", INT2FIX(-6));
    rb_define_const(g_enum_Result, "NOT_SUPPORTED", INT2FIX(-7));
    rb_define_const(g_enum_Result, "FILE_NOT_FOUND", INT2FIX(-8));
    rb_define_const(g_enum_Result, "DIRECTORY_NOT_FOUND", INT2FIX(-9));
    rb_define_const(g_enum_Result, "INVALID_FORMAT", INT2FIX(-10));
    rb_define_const(g_enum_Result, "NOT_IMPLEMENTED", INT2FIX(-11));
    rb_define_const(g_enum_Result, "RUNTIME", INT2FIX(-12));
    rb_define_const(g_enum_Result, "ENCODING_FALLBACK", INT2FIX(-13));
    rb_define_const(g_enum_Result, "WIN32", INT2FIX(-14));
    rb_define_const(g_enum_Result, "COM", INT2FIX(-15));

    g_enum_VariantType = rb_define_module_under(g_luminoModule, "VariantType");
    rb_define_const(g_enum_VariantType, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_VariantType, "BOOL", INT2FIX(1));
    rb_define_const(g_enum_VariantType, "INT", INT2FIX(2));
    rb_define_const(g_enum_VariantType, "FLOAT", INT2FIX(3));
    rb_define_const(g_enum_VariantType, "STRING", INT2FIX(4));
    rb_define_const(g_enum_VariantType, "LIST", INT2FIX(5));
    rb_define_const(g_enum_VariantType, "OBJECT", INT2FIX(6));
    rb_define_const(g_enum_VariantType, "SIZE_F", INT2FIX(7));

    g_enum_FileAccessPriority = rb_define_module_under(g_luminoModule, "FileAccessPriority");
    rb_define_const(g_enum_FileAccessPriority, "ARCHIVE_FIRST", INT2FIX(0));
    rb_define_const(g_enum_FileAccessPriority, "DIRECTORY_FIRST", INT2FIX(1));
    rb_define_const(g_enum_FileAccessPriority, "ARCHIVE_ONLY", INT2FIX(2));

    g_enum_FileType = rb_define_module_under(g_luminoModule, "FileType");
    rb_define_const(g_enum_FileType, "BINARY", INT2FIX(0));
    rb_define_const(g_enum_FileType, "TEXT", INT2FIX(1));

    g_enum_FileMode = rb_define_module_under(g_luminoModule, "FileMode");
    rb_define_const(g_enum_FileMode, "OPEN", INT2FIX(0));
    rb_define_const(g_enum_FileMode, "CREATE", INT2FIX(1));
    rb_define_const(g_enum_FileMode, "APPEND", INT2FIX(2));

    g_enum_FileAccess = rb_define_module_under(g_luminoModule, "FileAccess");
    rb_define_const(g_enum_FileAccess, "READ", INT2FIX(0));
    rb_define_const(g_enum_FileAccess, "READWRITE", INT2FIX(1));
    rb_define_const(g_enum_FileAccess, "WRITE", INT2FIX(2));

    g_enum_SeekOrigin = rb_define_module_under(g_luminoModule, "SeekOrigin");
    rb_define_const(g_enum_SeekOrigin, "BEGIN", INT2FIX(0));
    rb_define_const(g_enum_SeekOrigin, "CURRENT", INT2FIX(1));
    rb_define_const(g_enum_SeekOrigin, "END", INT2FIX(2));

    g_enum_AsyncIOState = rb_define_module_under(g_luminoModule, "AsyncIOState");
    rb_define_const(g_enum_AsyncIOState, "IDLE", INT2FIX(0));
    rb_define_const(g_enum_AsyncIOState, "READY", INT2FIX(1));
    rb_define_const(g_enum_AsyncIOState, "PROCESSING", INT2FIX(2));
    rb_define_const(g_enum_AsyncIOState, "COMPLETED", INT2FIX(3));
    rb_define_const(g_enum_AsyncIOState, "FAILED", INT2FIX(4));

    g_enum_RotationOrder = rb_define_module_under(g_luminoModule, "RotationOrder");
    rb_define_const(g_enum_RotationOrder, "XYZ", INT2FIX(0));
    rb_define_const(g_enum_RotationOrder, "XZY", INT2FIX(1));
    rb_define_const(g_enum_RotationOrder, "YXZ", INT2FIX(2));
    rb_define_const(g_enum_RotationOrder, "YZX", INT2FIX(3));
    rb_define_const(g_enum_RotationOrder, "ZXY", INT2FIX(4));
    rb_define_const(g_enum_RotationOrder, "ZYX", INT2FIX(5));

    g_enum_KeyCode = rb_define_module_under(g_luminoModule, "KeyCode");
    rb_define_const(g_enum_KeyCode, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_KeyCode, "A", INT2FIX(1));
    rb_define_const(g_enum_KeyCode, "B", INT2FIX(2));
    rb_define_const(g_enum_KeyCode, "C", INT2FIX(3));
    rb_define_const(g_enum_KeyCode, "D", INT2FIX(4));
    rb_define_const(g_enum_KeyCode, "E", INT2FIX(5));
    rb_define_const(g_enum_KeyCode, "F", INT2FIX(6));
    rb_define_const(g_enum_KeyCode, "G", INT2FIX(7));
    rb_define_const(g_enum_KeyCode, "H", INT2FIX(8));
    rb_define_const(g_enum_KeyCode, "I", INT2FIX(9));
    rb_define_const(g_enum_KeyCode, "J", INT2FIX(10));
    rb_define_const(g_enum_KeyCode, "K", INT2FIX(11));
    rb_define_const(g_enum_KeyCode, "L", INT2FIX(12));
    rb_define_const(g_enum_KeyCode, "M", INT2FIX(13));
    rb_define_const(g_enum_KeyCode, "N", INT2FIX(14));
    rb_define_const(g_enum_KeyCode, "O", INT2FIX(15));
    rb_define_const(g_enum_KeyCode, "P", INT2FIX(16));
    rb_define_const(g_enum_KeyCode, "Q", INT2FIX(17));
    rb_define_const(g_enum_KeyCode, "R", INT2FIX(18));
    rb_define_const(g_enum_KeyCode, "S", INT2FIX(19));
    rb_define_const(g_enum_KeyCode, "T", INT2FIX(20));
    rb_define_const(g_enum_KeyCode, "U", INT2FIX(21));
    rb_define_const(g_enum_KeyCode, "V", INT2FIX(22));
    rb_define_const(g_enum_KeyCode, "W", INT2FIX(23));
    rb_define_const(g_enum_KeyCode, "X", INT2FIX(24));
    rb_define_const(g_enum_KeyCode, "Y", INT2FIX(25));
    rb_define_const(g_enum_KeyCode, "Z", INT2FIX(26));
    rb_define_const(g_enum_KeyCode, "F10", INT2FIX(46));
    rb_define_const(g_enum_KeyCode, "F11", INT2FIX(47));
    rb_define_const(g_enum_KeyCode, "F12", INT2FIX(48));
    rb_define_const(g_enum_KeyCode, "SPACE", INT2FIX(49));
    rb_define_const(g_enum_KeyCode, "ESCAPE", INT2FIX(50));
    rb_define_const(g_enum_KeyCode, "DOWN", INT2FIX(52));
    rb_define_const(g_enum_KeyCode, "LEFT", INT2FIX(53));
    rb_define_const(g_enum_KeyCode, "RIGHT", INT2FIX(54));
    rb_define_const(g_enum_KeyCode, "LSHIFT", INT2FIX(55));
    rb_define_const(g_enum_KeyCode, "RSHIFT", INT2FIX(56));
    rb_define_const(g_enum_KeyCode, "LCTRL", INT2FIX(57));
    rb_define_const(g_enum_KeyCode, "RCTRL", INT2FIX(58));
    rb_define_const(g_enum_KeyCode, "LALT", INT2FIX(59));
    rb_define_const(g_enum_KeyCode, "RALT", INT2FIX(60));
    rb_define_const(g_enum_KeyCode, "TAB", INT2FIX(61));
    rb_define_const(g_enum_KeyCode, "ENTER", INT2FIX(62));
    rb_define_const(g_enum_KeyCode, "BACKSPACE", INT2FIX(63));
    rb_define_const(g_enum_KeyCode, "INSERT", INT2FIX(64));
    rb_define_const(g_enum_KeyCode, "DELETE", INT2FIX(65));
    rb_define_const(g_enum_KeyCode, "PAGEUP", INT2FIX(66));
    rb_define_const(g_enum_KeyCode, "PAGEDOWN", INT2FIX(67));
    rb_define_const(g_enum_KeyCode, "HOME", INT2FIX(68));
    rb_define_const(g_enum_KeyCode, "END", INT2FIX(69));
    rb_define_const(g_enum_KeyCode, "COLON", INT2FIX(70));
    rb_define_const(g_enum_KeyCode, "SEMICOLON", INT2FIX(71));
    rb_define_const(g_enum_KeyCode, "COMMA", INT2FIX(72));
    rb_define_const(g_enum_KeyCode, "PERIOD", INT2FIX(73));
    rb_define_const(g_enum_KeyCode, "SLASH", INT2FIX(74));
    rb_define_const(g_enum_KeyCode, "MINUS", INT2FIX(75));
    rb_define_const(g_enum_KeyCode, "BACKSLASH", INT2FIX(76));
    rb_define_const(g_enum_KeyCode, "YEN", INT2FIX(77));
    rb_define_const(g_enum_KeyCode, "CARET", INT2FIX(78));
    rb_define_const(g_enum_KeyCode, "LBRACKET", INT2FIX(79));
    rb_define_const(g_enum_KeyCode, "RBRACKET", INT2FIX(80));
    rb_define_const(g_enum_KeyCode, "ANDROID_PLUS", INT2FIX(81));

    g_enum_AcceleratorKey = rb_define_module_under(g_luminoModule, "AcceleratorKey");
    rb_define_const(g_enum_AcceleratorKey, "TOGGLE_FULLSCREEN", INT2FIX(0));
    rb_define_const(g_enum_AcceleratorKey, "SHOW_DEBUG_STATISTICS", INT2FIX(1));
/*
    g_enum_Button = rb_define_module_under(g_luminoModule, "Button");
    rb_define_const(g_enum_Button, "0", INT2FIX(0));
    rb_define_const(g_enum_Button, "1", INT2FIX(1));
    rb_define_const(g_enum_Button, "2", INT2FIX(2));
    rb_define_const(g_enum_Button, "3", INT2FIX(3));
    rb_define_const(g_enum_Button, "4", INT2FIX(4));
    rb_define_const(g_enum_Button, "5", INT2FIX(5));
    rb_define_const(g_enum_Button, "6", INT2FIX(6));
    rb_define_const(g_enum_Button, "7", INT2FIX(7));
    rb_define_const(g_enum_Button, "8", INT2FIX(8));
    rb_define_const(g_enum_Button, "9", INT2FIX(9));
    rb_define_const(g_enum_Button, "10", INT2FIX(10));
    rb_define_const(g_enum_Button, "11", INT2FIX(11));
    rb_define_const(g_enum_Button, "12", INT2FIX(12));
    rb_define_const(g_enum_Button, "13", INT2FIX(13));
    rb_define_const(g_enum_Button, "14", INT2FIX(14));
    rb_define_const(g_enum_Button, "15", INT2FIX(15));
    rb_define_const(g_enum_Button, "LEFT", INT2FIX(16));
    rb_define_const(g_enum_Button, "RIGHT", INT2FIX(17));
    rb_define_const(g_enum_Button, "UP", INT2FIX(18));
    rb_define_const(g_enum_Button, "DOWN", INT2FIX(19));
    rb_define_const(g_enum_Button, "AXIS_1X_MINUIS", INT2FIX(20));
    rb_define_const(g_enum_Button, "AXIS_1X_PLUS", INT2FIX(21));
    rb_define_const(g_enum_Button, "AXIS_1Y_MINUIS", INT2FIX(22));
    rb_define_const(g_enum_Button, "AXIS_1Y_PLUS", INT2FIX(23));
    rb_define_const(g_enum_Button, "AXIS_2X_MINUIS", INT2FIX(24));
    rb_define_const(g_enum_Button, "AXIS_2X_PLUS", INT2FIX(25));
    rb_define_const(g_enum_Button, "AXIS_2Y_MINUIS", INT2FIX(26));
    rb_define_const(g_enum_Button, "AXIS_2Y_PLUS", INT2FIX(27));
*/
    g_enum_DeviceButtonType = rb_define_module_under(g_luminoModule, "DeviceButtonType");
    rb_define_const(g_enum_DeviceButtonType, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_DeviceButtonType, "KEYBOARD", INT2FIX(1));
    rb_define_const(g_enum_DeviceButtonType, "MOUSE", INT2FIX(2));
    rb_define_const(g_enum_DeviceButtonType, "JOYPAD_BUTTON", INT2FIX(3));
    rb_define_const(g_enum_DeviceButtonType, "JOYPAD_POV", INT2FIX(4));
    rb_define_const(g_enum_DeviceButtonType, "JOYPAD_AXIS", INT2FIX(5));

    g_enum_DeviceButton = rb_define_module_under(g_luminoModule, "DeviceButton");
    rb_define_const(g_enum_DeviceButton, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_DeviceButton, "MOUSE_1", INT2FIX(256));
    rb_define_const(g_enum_DeviceButton, "MOUSE_2", INT2FIX(257));
    rb_define_const(g_enum_DeviceButton, "MOUSE_3", INT2FIX(258));
    rb_define_const(g_enum_DeviceButton, "MOUSE_4", INT2FIX(259));
    rb_define_const(g_enum_DeviceButton, "MOUSE_5", INT2FIX(260));
    rb_define_const(g_enum_DeviceButton, "MOUSE_6", INT2FIX(261));
    rb_define_const(g_enum_DeviceButton, "MOUSE_7", INT2FIX(262));
    rb_define_const(g_enum_DeviceButton, "MOUSE_8", INT2FIX(263));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_1", INT2FIX(264));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_2", INT2FIX(265));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_3", INT2FIX(266));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_4", INT2FIX(267));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_5", INT2FIX(268));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_6", INT2FIX(269));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_7", INT2FIX(270));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_8", INT2FIX(271));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_9", INT2FIX(272));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_10", INT2FIX(273));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_11", INT2FIX(274));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_12", INT2FIX(275));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_13", INT2FIX(276));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_14", INT2FIX(277));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_15", INT2FIX(278));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_BUTTON_16", INT2FIX(279));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_POV_UP", INT2FIX(280));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_POV_RIGHT", INT2FIX(281));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_POV_DOWN", INT2FIX(282));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_POV_LEFT", INT2FIX(283));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_1_MINUIS", INT2FIX(284));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_1_PLUS", INT2FIX(285));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_2_MINUIS", INT2FIX(286));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_2_PLUS", INT2FIX(287));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_3_MINUIS", INT2FIX(288));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_3_PLUS", INT2FIX(289));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_4_MINUIS", INT2FIX(290));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_4_PLUS", INT2FIX(291));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_5_MINUIS", INT2FIX(292));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_5_PLUS", INT2FIX(293));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_6_MINUIS", INT2FIX(294));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_6_PLUS", INT2FIX(295));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_7_MINUIS", INT2FIX(296));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_7_PLUS", INT2FIX(297));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_8_MINUIS", INT2FIX(298));
    rb_define_const(g_enum_DeviceButton, "JOYPAD_AXIS_8_PLUS", INT2FIX(299));

    g_enum_DirectMusicMode = rb_define_module_under(g_luminoModule, "DirectMusicMode");
    rb_define_const(g_enum_DirectMusicMode, "NOT_USE", INT2FIX(0));
    rb_define_const(g_enum_DirectMusicMode, "NORMAL", INT2FIX(1));
    rb_define_const(g_enum_DirectMusicMode, "THREAD_WAIT", INT2FIX(2));
    rb_define_const(g_enum_DirectMusicMode, "THREAD_REQUEST", INT2FIX(3));

    g_enum_SoundPlayingMode = rb_define_module_under(g_luminoModule, "SoundPlayingMode");
    rb_define_const(g_enum_SoundPlayingMode, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_SoundPlayingMode, "AUTO", INT2FIX(1));
    rb_define_const(g_enum_SoundPlayingMode, "ONMEMORY", INT2FIX(2));
    rb_define_const(g_enum_SoundPlayingMode, "STREAMING", INT2FIX(3));
    rb_define_const(g_enum_SoundPlayingMode, "MIDI", INT2FIX(4));

    g_enum_SoundPlayingState = rb_define_module_under(g_luminoModule, "SoundPlayingState");
    rb_define_const(g_enum_SoundPlayingState, "STOPPED", INT2FIX(0));
    rb_define_const(g_enum_SoundPlayingState, "PLAYING", INT2FIX(1));
    rb_define_const(g_enum_SoundPlayingState, "PAUSING", INT2FIX(2));

    g_enum_SoundFadeBehavior = rb_define_module_under(g_luminoModule, "SoundFadeBehavior");
    rb_define_const(g_enum_SoundFadeBehavior, "CONTINUE", INT2FIX(0));
    rb_define_const(g_enum_SoundFadeBehavior, "STOP", INT2FIX(1));
    rb_define_const(g_enum_SoundFadeBehavior, "STOP_RESET", INT2FIX(2));
    rb_define_const(g_enum_SoundFadeBehavior, "PAUSE", INT2FIX(3));
    rb_define_const(g_enum_SoundFadeBehavior, "PAUSE_RESET", INT2FIX(4));

    g_enum_InternalGameSound = rb_define_module_under(g_luminoModule, "InternalGameSound");
    rb_define_const(g_enum_InternalGameSound, "BGM", INT2FIX(0));
    rb_define_const(g_enum_InternalGameSound, "BGS", INT2FIX(1));
    rb_define_const(g_enum_InternalGameSound, "ME", INT2FIX(2));

    g_enum_BackbufferResizeMode = rb_define_module_under(g_luminoModule, "BackbufferResizeMode");
    rb_define_const(g_enum_BackbufferResizeMode, "SCALING", INT2FIX(0));
    rb_define_const(g_enum_BackbufferResizeMode, "SCALING_WITH_LETTER_BOX", INT2FIX(1));
    rb_define_const(g_enum_BackbufferResizeMode, "RESIZE", INT2FIX(2));

    g_enum_TextureFormat = rb_define_module_under(g_luminoModule, "TextureFormat");
    rb_define_const(g_enum_TextureFormat, "UNKNOWN", INT2FIX(0));
    rb_define_const(g_enum_TextureFormat, "A8R8G8B8", INT2FIX(1));
    rb_define_const(g_enum_TextureFormat, "X8R8G8B8", INT2FIX(2));
    rb_define_const(g_enum_TextureFormat, "A16B16G16R16F", INT2FIX(3));
    rb_define_const(g_enum_TextureFormat, "A32B32G32R32F", INT2FIX(4));
    rb_define_const(g_enum_TextureFormat, "D24S8", INT2FIX(5));

    g_enum_BlendMode = rb_define_module_under(g_luminoModule, "BlendMode");
    rb_define_const(g_enum_BlendMode, "NORMAL", INT2FIX(0));
    rb_define_const(g_enum_BlendMode, "ALPHA", INT2FIX(1));
    rb_define_const(g_enum_BlendMode, "ADD", INT2FIX(2));
    rb_define_const(g_enum_BlendMode, "ADD_ALPHA_DISABLE", INT2FIX(3));
    rb_define_const(g_enum_BlendMode, "SUB", INT2FIX(4));
    rb_define_const(g_enum_BlendMode, "SUB_ALPHA_DISABLE", INT2FIX(5));
    rb_define_const(g_enum_BlendMode, "MUL", INT2FIX(6));
    rb_define_const(g_enum_BlendMode, "SCREEN", INT2FIX(7));
    rb_define_const(g_enum_BlendMode, "REVERSE", INT2FIX(8));

    g_enum_CullingMode = rb_define_module_under(g_luminoModule, "CullingMode");
    rb_define_const(g_enum_CullingMode, "FRONT", INT2FIX(0));
    rb_define_const(g_enum_CullingMode, "BACK", INT2FIX(1));
    rb_define_const(g_enum_CullingMode, "DOUBLE", INT2FIX(2));

    g_enum_TextAlign = rb_define_module_under(g_luminoModule, "TextAlign");
    rb_define_const(g_enum_TextAlign, "LEFT", INT2FIX(0));
    rb_define_const(g_enum_TextAlign, "CENTER", INT2FIX(1));
    rb_define_const(g_enum_TextAlign, "RIGHT", INT2FIX(2));

    g_enum_PlacementType = rb_define_module_under(g_luminoModule, "PlacementType");
    rb_define_const(g_enum_PlacementType, "NORMAL", INT2FIX(0));
    rb_define_const(g_enum_PlacementType, "BILLBOARD", INT2FIX(1));
    rb_define_const(g_enum_PlacementType, "Y_FIXED", INT2FIX(2));

    g_enum_FrontAxis = rb_define_module_under(g_luminoModule, "FrontAxis");
    rb_define_const(g_enum_FrontAxis, "X", INT2FIX(0));
    rb_define_const(g_enum_FrontAxis, "Y", INT2FIX(1));
    rb_define_const(g_enum_FrontAxis, "Z", INT2FIX(2));
    rb_define_const(g_enum_FrontAxis, "RX", INT2FIX(3));
    rb_define_const(g_enum_FrontAxis, "RY", INT2FIX(4));
    rb_define_const(g_enum_FrontAxis, "RZ", INT2FIX(5));

    g_enum_MeshCreateFlags = rb_define_module_under(g_luminoModule, "MeshCreateFlags");
    rb_define_const(g_enum_MeshCreateFlags, "SOFTWARE_SKINNING", INT2FIX(0x01));
    rb_define_const(g_enum_MeshCreateFlags, "COLLISION_MESH", INT2FIX(0x02));
    rb_define_const(g_enum_MeshCreateFlags, "IGNORE_TEXTURE_NOT_FOUND", INT2FIX(0x04));

    g_enum_CameraType = rb_define_module_under(g_luminoModule, "CameraType");
    rb_define_const(g_enum_CameraType, "POSITION_AND_LOOKAT", INT2FIX(0));
    rb_define_const(g_enum_CameraType, "POSITION_AND_ANGLE", INT2FIX(1));
    rb_define_const(g_enum_CameraType, "TRANSFORM_MATRIX", INT2FIX(2));

    g_enum_Coord = rb_define_module_under(g_luminoModule, "Coord");
    rb_define_const(g_enum_Coord, "SPACE_2D", INT2FIX(0));
    rb_define_const(g_enum_Coord, "SPACE_3D", INT2FIX(1));

    g_enum_ObjectType = rb_define_module_under(g_luminoModule, "ObjectType");
    rb_define_const(g_enum_ObjectType, "SOUND", INT2FIX(0));


}

