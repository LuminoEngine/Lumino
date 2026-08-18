import { LogLevel } from "./types";

/**
 * ログレベルの 1 文字省略形。LogLevel の値をそのまま添字にする。
 * C++ 側 (LuminoBase/src/Logger.cpp の kLevelChars) と同じ並び。
 */
const LEVEL_CHARS = "TDVIWEF";

/** レベル → console メソッドの対応表。LogLevel の値をそのまま添字にする。 */
const CONSOLE_FNS: readonly ("debug" | "info" | "warn" | "error")[] =
    ["debug", "debug", "debug", "info", "warn", "error", "error"];

/**
 * ネイティブ (WASM) 側のログ行の先頭プレフィックス。
 *
 * LuminoC の luminoLogToStdout が `[Lumino][I] file.cpp:123 message` の形式で
 * stdout へ printf しており、その先頭のレベル文字を取り出す。
 * この書式は LuminoC と luminojs の間の暗黙の契約になっているため、
 * 変更するときは packages/LuminoC/src/LuminoAPI.cpp と同時に変更すること。
 */
const NATIVE_LOG_PREFIX = /^\[Lumino\]\[([TDVIWEF])\] /;

/** ログの出力先。`Logger.setSink` で差し替える。 */
export type LogSink = (level: LogLevel, message: string) => void;

/**
 * luminojs のログ出力を制御します。
 */
export class Logger {
    private static _level: LogLevel = LogLevel.INFO;
    private static _sink: LogSink | undefined;

    /**
     * JS 側のログ出力レベルフィルタを設定する。設定レベル未満のログは破棄される。
     * ネイティブ側にも反映するには `Runtime.setLogLevel` を使うこと。
     */
    static setLevel(level: LogLevel): void {
        this._level = level;
    }

    /** 現在のログ出力レベルフィルタを取得する。 */
    static getLevel(): LogLevel {
        return this._level;
    }

    /** 指定レベルのログを出力すべきか判定する。 */
    static shouldLog(level: LogLevel): boolean {
        return level >= this._level;
    }

    /**
     * 出力先を差し替える。`undefined` を渡すと console へのデフォルト出力に戻る。
     * 登録後は console への出力は行われない。
     */
    static setSink(sink?: LogSink): void {
        this._sink = sink;
    }

    static trace(...args: unknown[]): void { this._write(LogLevel.TRACE, args); }
    static debug(...args: unknown[]): void { this._write(LogLevel.DEBUG, args); }
    static verbose(...args: unknown[]): void { this._write(LogLevel.VERBOSE, args); }
    static info(...args: unknown[]): void { this._write(LogLevel.INFO, args); }
    static warning(...args: unknown[]): void { this._write(LogLevel.WARNING, args); }
    static error(...args: unknown[]): void { this._write(LogLevel.ERROR, args); }
    static fatal(...args: unknown[]): void { this._write(LogLevel.FATAL, args); }

    /**
     * @internal
     * ネイティブ (WASM) 側の stdout 行を Logger へ流す。
     * `[Lumino][X] ` プレフィックスを持つ行はそのレベルで、それ以外 (Lumino の
     * Logger を経由しない printf など) は Info として扱う。
     * 行は既に整形済みなので、プレフィックスを付け直さずそのまま出力する。
     */
    static writeNativeLine(text: string): void {
        const m = NATIVE_LOG_PREFIX.exec(text);
        if (!m) {
            this.info(text);
            return;
        }
        const level = LEVEL_CHARS.indexOf(m[1]) as LogLevel;
        if (!this.shouldLog(level)) return;
        this._emit(level, text, []);
    }

    private static _write(level: LogLevel, args: unknown[]): void {
        if (!this.shouldLog(level)) return;
        this._emit(level, `[Lumino][${LEVEL_CHARS[level]}]`, args);
    }

    private static _emit(level: LogLevel, head: string, args: unknown[]): void {
        if (this._sink) {
            // sink は 1 本の文字列を受け取るため、可変引数は空白区切りで連結する。
            this._sink(level, args.length > 0 ? `${head} ${args.map(String).join(" ")}` : head);
            return;
        }
        console[CONSOLE_FNS[level]](head, ...args);
    }
}
