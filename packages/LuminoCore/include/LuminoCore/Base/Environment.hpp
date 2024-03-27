// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include "String.hpp"
#include "../IO/Path.hpp"

namespace ln {

/**
 * システムの特別なフォルダパスを示す値です。
 * 
 * 次の表は、各 OS 上のパスの例です。
 * 
 * SpecialFolder   | Windows                              | OS X            | Linux
 * ----------------|--------------------------------------|-----------------|---------
 * ApplicationData | C:/Users/\<USER\>/AppData/Roaming    |                 | ~/.local/share, /usr/local/share, /usr/share/
 * Temporary       | C:/Users/\<USER\>/AppData/Local/Temp |                 | /tmp
 * Home            | C:/Users/\<USER\>/AppData/Local/Temp | /Users/\<USER\> | ~
 */
enum class SpecialFolder {
    ApplicationData,
    Temporary,
    Home,
};

/** システムの特別なフォルダパス を取得する際の動作 */
enum class SpecialFolderOption {
    /** フォルダパスが存在しなければ、空の文字列を返します。 */
    None,

    /** フォルダパスが存在しなければ、フォルダを作成します。 */
    Create,

    /** フォルダパスの存在を確認せずにパスを返します。 */
    DoNotVerify,
};

/** データのバイトオーダー (エンディアン) */
enum class ByteOrder {
    /** リトルエンディアン */
    LittleEndian,

    /** ビッグエンディアン */
    BigEndian,
};

/** Represents an runtime system platform. */
enum class RuntimePlatform {
    /** Linux operating system. */
    Linux,

    /** OSX operating system. */
    OSX,

    /** Windows operating system. */
    Windows,

    /** Web browser system. */
    Web,
};

/** 現在の動作環境やプラットフォームに関する情報へのアクセス手段を提供します。 */
class Environment {
public:
    /** カレントディレクトリのパスを取得します。 */
    static Path currentDirectory();

    /** カレントディレクトリを変更します。 */
    static void setCurrentDirectory(const Path& path);

    /** アプリケーションを開始した実行ファイルのパスを取得します。*/
    static Path executablePath();

    /** アプリケーションを開始した実行ファイルが保存されているディレクトリパスを取得します。*/
    static Path executableDirectory();

    /** パス文字列の大文字と小文字を区別するかを確認します。 */
    static CaseSensitivity pathCaseSensitivity();

    /** システムの特別なフォルダのパスを取得します。 */
    static Path specialFolderPath(SpecialFolder specialFolder);

    /** システムの特別なフォルダのパスを取得します。 */
    static Path specialFolderPath(SpecialFolder specialFolder, const StringView& relativeDirPath, SpecialFolderOption option);

    /** 環境変数の値を取得します。 */
    static Optional_deprecated<String> getEnvironmentVariable(const StringView& variableName);

    /** 現在のプロセスの環境変数を設定します。(システム環境変数や他のプロセスの環境変数へ影響を及ぼしません) */
    static void setEnvironmentVariable(const StringView& variableName, const StringView& value);

    /** 現在の環境のバイトオーダー (エンディアン) を確認します。 */
    static ByteOrder byteOrder();

    /** システム起動からの時間を ms 単位で取得します。 */
    static uint64_t getTickCount();

    /**
     * Indicates whether the current application is running on the specified platform.
     *
     * 主に、特定の OS でのみ使用可能な API やセキュリティ要件を区別するために使用します。
     * ファイルシステムやディレクトリセパレータを区別するためのものではない点に注意してください。
     */
    static bool isRuntimePlatform(RuntimePlatform value);
};

} // namespace ln
