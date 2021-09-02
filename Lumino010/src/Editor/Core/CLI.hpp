#pragma once

/**
 * ターミナル入出力。
 * 色を付けたりする。
 * ログ目的ではない点に注意。CLI としてユーザーに通知したいものを出力するために使うこと。
 *
 * stderr への出力系は、GUI アプリでリダイレクトしつつ作業ログを表示するために、適宜使用してもよい。
 */
class CLI
{
public:
	/** コマンドのメイン出力。stdout に出力される。 */
	static void output(const ln::String& str);

	/** verbose. stderr に出力される。 */
	static void verbose(const ln::String& str);

	/** 進捗状況やログの出力。stderr に出力される。 */
	static void info(const ln::String& str);

	/** ワーニング出力。stderr に出力される。 */
	static void warning(const ln::String& str);

	/** エラー出力。stderr に出力される。 */
	static void error(const ln::String& str);

    /** エラー出力。stderr に出力され、例外を投げる。 */
    static void fatal(const ln::String& str);
};

