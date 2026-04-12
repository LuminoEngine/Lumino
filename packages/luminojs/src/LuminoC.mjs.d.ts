/** Type declaration for the Emscripten-generated LuminoC WASM module. */
declare function LuminoCFactory(opts?: Record<string, unknown>): Promise<LuminoCModule>;

interface LuminoCModule {
    cwrap(
        ident: string,
        returnType: string | null,
        argTypes: string[],
        opts?: { async?: boolean },
    ): (...args: unknown[]) => unknown;
    _malloc(size: number): number;
    _free(ptr: number): void;
    HEAPU8: Uint8Array;
    HEAPU32: Uint32Array;
    HEAPF32: Float32Array;
}

export default LuminoCFactory;
