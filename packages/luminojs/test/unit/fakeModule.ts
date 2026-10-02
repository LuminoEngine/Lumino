// 単体テスト用のフェイク WASM モジュール。Runtime.module を差し替えて使う。
// _malloc は前から順に切り出すだけで、_free は何もしない。

const HEAP_SIZE = 4096;

export interface FakeModule {
    buffer: ArrayBuffer;
    nextPtr: number;
    _malloc(size: number): number;
    _free(ptr: number): void;
    HEAPU8: Uint8Array;
    HEAPU32: Uint32Array;
    HEAPF32: Float32Array;
    UTF8ToString(ptr: number): string;
    cwrap(): never;
}

export function createFakeModule(): FakeModule {
    const buffer = new ArrayBuffer(HEAP_SIZE);
    return {
        buffer,
        nextPtr: 16, // 先頭は Runtime._returnPtr (0) 用に空けておく
        _malloc(size: number): number {
            const p = this.nextPtr;
            this.nextPtr += (size + 7) & ~7;
            return p;
        },
        _free(): void {},
        HEAPU8: new Uint8Array(buffer),
        HEAPU32: new Uint32Array(buffer),
        HEAPF32: new Float32Array(buffer),
        UTF8ToString(): string { return ""; },
        cwrap(): never { throw new Error("not supported in fake module"); },
    };
}
