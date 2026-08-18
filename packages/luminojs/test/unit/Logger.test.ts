import { afterEach, beforeEach, describe, expect, it, vi } from "vitest";
import { Logger } from "../../src/Logger";
import { LogLevel } from "../../src/types";

// Logger は static 状態を持つため、各テストで初期状態 (Info / sink なし) に戻す。

const CONSOLE_METHODS = ["debug", "info", "warn", "error"] as const;
type ConsoleMethod = (typeof CONSOLE_METHODS)[number];

let spies: Record<ConsoleMethod, ReturnType<typeof vi.spyOn>>;

beforeEach(() => {
    Logger.setLevel(LogLevel.INFO);
    Logger.setSink(undefined);
    spies = {} as typeof spies;
    for (const m of CONSOLE_METHODS) {
        spies[m] = vi.spyOn(console, m).mockImplementation(() => {});
    }
});

afterEach(() => {
    vi.restoreAllMocks();
});

/** どの console メソッドが何回呼ばれたかをまとめて取り出す。 */
function callCounts(): Record<ConsoleMethod, number> {
    const counts = {} as Record<ConsoleMethod, number>;
    for (const m of CONSOLE_METHODS) counts[m] = spies[m].mock.calls.length;
    return counts;
}

describe("Logger のレベルフィルタ", () => {
    it("設定レベル未満のログは破棄される", () => {
        Logger.setLevel(LogLevel.WARNING);
        Logger.info("dropped");
        Logger.debug("dropped");
        expect(callCounts()).toEqual({ debug: 0, info: 0, warn: 0, error: 0 });

        Logger.warning("kept");
        expect(spies.warn).toHaveBeenCalledTimes(1);
    });

    it("既定レベルは Info", () => {
        Logger.verbose("dropped");
        Logger.info("kept");
        expect(callCounts()).toEqual({ debug: 0, info: 1, warn: 0, error: 0 });
    });

    it("LogLevel.DISABLE ですべて破棄される", () => {
        Logger.setLevel(LogLevel.DISABLE);
        Logger.trace("x");
        Logger.info("x");
        Logger.warning("x");
        Logger.error("x");
        Logger.fatal("x");
        Logger.writeNativeLine("[Lumino][F] Foo.cpp:1 fatal");
        expect(callCounts()).toEqual({ debug: 0, info: 0, warn: 0, error: 0 });
    });
});

describe("Logger のレベルと console メソッドの対応", () => {
    it.each([
        [LogLevel.TRACE, "debug"],
        [LogLevel.DEBUG, "debug"],
        [LogLevel.VERBOSE, "debug"],
        [LogLevel.INFO, "info"],
        [LogLevel.WARNING, "warn"],
        [LogLevel.ERROR, "error"],
        [LogLevel.FATAL, "error"],
    ] as const)("level %i は console.%s へ出力される", (level, method) => {
        Logger.setLevel(LogLevel.TRACE);
        const fn = [Logger.trace, Logger.debug, Logger.verbose, Logger.info,
            Logger.warning, Logger.error, Logger.fatal][level];
        fn.call(Logger, "message");
        expect(spies[method]).toHaveBeenCalledTimes(1);
    });

    it("レベル文字入りのプレフィックスが付く", () => {
        Logger.warning("hello", 42);
        expect(spies.warn).toHaveBeenCalledWith("[Lumino][W]", "hello", 42);
    });
});

// この一群は LuminoC/src/LuminoAPI.cpp の luminoLogToStdout が出力する書式に
// 依存している。C 側の書式を変更するとここが落ちる。
describe("Logger.writeNativeLine (ネイティブ出力の取り込み)", () => {
    it("プレフィックスのレベル文字に応じた console メソッドへ振り分ける", () => {
        Logger.writeNativeLine("[Lumino][W] Foo.cpp:12 something odd");
        expect(spies.warn).toHaveBeenCalledTimes(1);
        expect(spies.info).not.toHaveBeenCalled();
    });

    it("整形済みの行はプレフィックスを付け直さずそのまま出力する", () => {
        const line = "[Lumino][E] Foo.cpp:12 boom";
        Logger.writeNativeLine(line);
        expect(spies.error).toHaveBeenCalledWith(line);
    });

    it("プレフィックスを持たない stdout 行は Info として扱う", () => {
        Logger.writeNativeLine("Lumino HelloTest: 42");
        expect(spies.info).toHaveBeenCalledWith("[Lumino][I]", "Lumino HelloTest: 42");
    });

    it("レベルフィルタはネイティブ行にも効く", () => {
        Logger.setLevel(LogLevel.ERROR);
        Logger.writeNativeLine("[Lumino][I] Foo.cpp:12 info");
        expect(callCounts()).toEqual({ debug: 0, info: 0, warn: 0, error: 0 });
    });
});

describe("Logger.setSink", () => {
    it("sink 登録中は console へ出力せず sink へ渡す", () => {
        const received: Array<[LogLevel, string]> = [];
        Logger.setSink((level, message) => { received.push([level, message]); });

        Logger.warning("hello", 42);
        Logger.writeNativeLine("[Lumino][E] Foo.cpp:12 boom");

        expect(callCounts()).toEqual({ debug: 0, info: 0, warn: 0, error: 0 });
        expect(received).toEqual([
            [LogLevel.WARNING, "[Lumino][W] hello 42"],
            [LogLevel.ERROR, "[Lumino][E] Foo.cpp:12 boom"],
        ]);
    });

    it("undefined を渡すと console 出力へ戻る", () => {
        Logger.setSink(() => {});
        Logger.setSink(undefined);
        Logger.info("back to console");
        expect(spies.info).toHaveBeenCalledTimes(1);
    });
});
