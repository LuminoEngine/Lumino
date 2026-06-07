import {
    Result,
    GraphicsBackend,
    type Handle,
    type RuntimeOptions,
    SIZEOF_INSTANCE_INIT_SETTINGS,
} from "./types";

//------------------------------------------------------------------------------
// Emscripten Module type (subset we use)
//------------------------------------------------------------------------------

interface EmscriptenModule {
    cwrap(ident: string, returnType: string | null, argTypes: string[], opts?: { async?: boolean }): (...args: unknown[]) => unknown;
    _malloc(size: number): number;
    _free(ptr: number): void;
    UTF8ToString(ptr: number): string;
    HEAPU8: Uint8Array;
    HEAPU32: Uint32Array;
    HEAPF32: Float32Array;
}

type ModuleFactory = (opts?: Record<string, unknown>) => Promise<EmscriptenModule>;

//------------------------------------------------------------------------------
// Internal cwrap bindings
//------------------------------------------------------------------------------

/** @internal Holds cwrap'ed C function references. */
export const API: Record<string, (...args: never[]) => unknown> = {};

//------------------------------------------------------------------------------
// Runtime
//------------------------------------------------------------------------------

export class Runtime {
    /** The raw Emscripten module. Accessible for advanced usage. */
    static module: EmscriptenModule;

    // Pre-allocated 4-byte slot for single out-parameter reads.
    private static _returnPtr = 0;
    private static _returnView: Uint32Array | undefined;
    private static _heapBuffer: ArrayBufferLike | undefined;

    /** Has `initialize` been called? */
    static get initialized(): boolean {
        return !!this.module;
    }

    /**
     * このバイナリがビルドされた日時文字列を返す。
     * ランタイム初期化不要。正しい WASM が読み込まれているかの確認用。
     */
    static getBuildTimestamp(): string {
        const ptr = API.LNBuildInfo_GetBuildTimestamp() as number;
        return this.module.UTF8ToString(ptr);
    }

    /**
     * Load the Emscripten WASM module and bind all C-API symbols.
     */
    static async initialize(options?: RuntimeOptions): Promise<void> {
        if (this.initialized) return;

        // Dynamic import – the file sits in lib/ alongside the built TS output.
        const { default: LuminoC } = await import("./LuminoC.mjs") as { default: ModuleFactory };

        const moduleOpts: Record<string, unknown> = {};
        if (options?.wasmPath) {
            const wasmPath = options.wasmPath;
            moduleOpts["locateFile"] = (path: string) =>
                path.endsWith(".wasm") ? wasmPath : path;
        }

        // Setup print functions.
        let printFunc: ((text: string) => void) | undefined = options?.print;
        let printErrFunc: ((text: string) => void) | undefined = options?.printErr;
        if (!printFunc)  {
            printFunc = (text: string) => console.log("[stdout]", text);
        }
        if (!printErrFunc)  {
            printErrFunc = (text: string) => console.warn("[stderr]", text);
        }
        moduleOpts["print"]    = printFunc;
        moduleOpts["printErr"] = printErrFunc;

        const preferredBackend: GraphicsBackend = GraphicsBackend.WebGPU;

        this.module = await LuminoC(moduleOpts);

        // Pre-allocate a 4-byte slot for out-parameter reads.
        this._returnPtr = this.module._malloc(4);

        // Bind all C-API functions via cwrap.
        this._bindAPI();

        console.log("[Lumino] Build:", Runtime.getBuildTimestamp());

        // Initialize Lumino graphics instance.
        const m = this.module;
        const ptr = m._malloc(SIZEOF_INSTANCE_INIT_SETTINGS);
        const view = new DataView(m.HEAPU8.buffer, ptr, SIZEOF_INSTANCE_INIT_SETTINGS);
        view.setUint32(0, preferredBackend, true);
        view.setUint32(4, options?.enableValidation ? 1 : 0, true);
        
        try {
            await this.safeCallAsync(() =>
                (API.LNInstance_Initialize as (p: number) => number | Promise<number>)(ptr));
        } finally {
            if (ptr) m._free(ptr);
        }
    }

    /** Shut down the Lumino runtime. */
    static async terminate(): Promise<void> {
        await (API.LNInstance_Terminate as () => Promise<void>)();
    }

    //--------------------------------------------------------------------------
    // Return-pointer helpers (handles HEAP buffer grow)
    //--------------------------------------------------------------------------

    /**
     * Get the pre-allocated return pointer and a Uint32Array view into it.
     * Automatically recreates the view if the backing ArrayBuffer has changed
     * due to WASM memory growth.
     */
    static getReturnPointerInfo(): [ptr: number, view: Uint32Array] {
        const buf = this.module.HEAPU8.buffer;
        if (!this._returnView || this._heapBuffer !== buf) {
            this._heapBuffer = buf;
            this._returnView = new Uint32Array(buf, this._returnPtr, 1);
        }
        return [this._returnPtr, this._returnView];
    }

    //--------------------------------------------------------------------------
    // Safe-call wrappers (sync)
    //--------------------------------------------------------------------------

    /** Call a C function and throw on non-OK result. */
    static safeCall(fn: () => number): void {
        const rc = fn();
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
    }

    /** Call a C function that returns a handle via the pre-allocated out-pointer. */
    static safeCallWithReturnHandle(fn: (ptr: number) => number): Handle {
        const [ptr, view] = this.getReturnPointerInfo();
        const rc = fn(ptr);
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
        return view[0];
    }

    //--------------------------------------------------------------------------
    // Safe-call wrappers (async — ASYNCIFY)
    //--------------------------------------------------------------------------

    /** Async variant of `safeCall`. */
    static async safeCallAsync(fn: () => number | Promise<number>): Promise<void> {
        const rc = await fn();
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
    }

    /** Async variant of `safeCallWithReturnHandle`. */
    static async safeCallWithReturnHandleAsync(fn: (ptr: number) => number | Promise<number>): Promise<Handle> {
        const [ptr, view] = this.getReturnPointerInfo();
        const rc = await fn(ptr);
        if (rc !== Result.OK) {
            throw new Error(`Lumino C-API error: ${rc}`);
        }
        // Re-read view after await (HEAP may have grown).
        const [, freshView] = this.getReturnPointerInfo();
        return freshView[0];
    }

    //--------------------------------------------------------------------------
    // Private: cwrap bindings
    //--------------------------------------------------------------------------

    private static _bindAPI(): void {

        // NOTE: ccall と cwrap の違い
        // ccall
        // const result = Module.ccall('Add', 'number', ['number', 'number'], [1, 2]);
        // cwrap
        // const add = Module.cwrap('Add', 'number', ['number', 'number']);
        // const result = add(4, 5);

        const m = this.module;
        const cw = m.cwrap.bind(m);

        // Phase 0
        API.LNHelloTest = cw("LNHelloTest", "number", ["number"]);
        API.LNBuildInfo_GetBuildTimestamp = cw("LNBuildInfo_GetBuildTimestamp", "number", []);

        // Instance
        API.LNInstance_Initialize = cw("LNInstance_Initialize", "number", ["number"], { async: true });
        API.LNInstance_Terminate  = cw("LNInstance_Terminate",  null,     [],         { async: true });

        // Object
        API.LNObject_Release = cw("LNObject_Release", "number", ["number"]);

        // Window
        API.LNWindow_CreateFromCanvas   = cw("LNWindow_CreateFromCanvas",   "number", ["string", "number", "number", "number"], { async: true });
        API.LNWindow_GetGraphicsContext = cw("LNWindow_GetGraphicsContext", "number", ["number", "number"]);

        // GraphicsContext
        API.LNGraphicsContext_BeginFrame = cw("LNGraphicsContext_BeginFrame", "number", ["number", "number", "number", "number", "number", "number"]);
        API.LNGraphicsContext_EndFrame   = cw("LNGraphicsContext_EndFrame",   "number", ["number"]);

        // RenderPassDesc
        API.LNRenderPassDesc_Init = cw("LNRenderPassDesc_Init", null, ["number"]);

        // Renderer
        API.LNRenderer_BeginRenderPass = cw("LNRenderer_BeginRenderPass", "number", ["number", "number", "number", "number"]);
        API.LNRenderer_EndRenderPass   = cw("LNRenderer_EndRenderPass",   "number", ["number"]);
        API.LNRenderer_DrawMesh        = cw("LNRenderer_DrawMesh",        "number", ["number", "number", "number", "number"]);
        API.LNRenderer_DrawScreenRect  = cw("LNRenderer_DrawScreenRect",  "number", ["number", "number"]);
        API.LNRenderer_DrawSprite      = cw("LNRenderer_DrawSprite",      "number", ["number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNRenderer_PushStencilMask = cw("LNRenderer_PushStencilMask", "number", ["number", "number", "number", "number"]);
        API.LNRenderer_PopStencilMask  = cw("LNRenderer_PopStencilMask",  "number", ["number"]);

        // Texture
        API.LNTexture2D_CreateFromPixels    = cw("LNTexture2D_CreateFromPixels",    "number", ["number", "number", "number", "number", "number", "number", "number"]);
        API.LNTexture2D_CreateRenderTargetEx = cw("LNTexture2D_CreateRenderTargetEx", "number", ["number", "number", "number", "number", "number"]);
        API.LNTexture2D_CreateDepthStencil   = cw("LNTexture2D_CreateDepthStencil",   "number", ["number", "number", "number", "number"]);

        // Image (GPU 非依存の画像デコード)
        API.LNImage_DecodeFromMemory = cw("LNImage_DecodeFromMemory", "number", ["number", "number", "number", "number", "number", "number"]);
        API.LNImage_FreePixels       = cw("LNImage_FreePixels",       "number", ["number"]);

        // Material
        API.LNMaterial_CreateUnlit              = cw("LNMaterial_CreateUnlit",              "number", ["number", "number"]);
        API.LNMaterial_CreateFromCompiledShader  = cw("LNMaterial_CreateFromCompiledShader",  "number", ["number", "number", "number", "number"]);
        API.LNMaterial_SetMainTexture            = cw("LNMaterial_SetMainTexture",            "number", ["number", "number"]);
        API.LNMaterial_SetColor                  = cw("LNMaterial_SetColor",                  "number", ["number", "number", "number", "number", "number"]);
        API.LNMaterial_SetFloat4                 = cw("LNMaterial_SetFloat4",                 "number", ["number", "string", "number"]);
        API.LNMaterial_SetNamedTexture           = cw("LNMaterial_SetNamedTexture",           "number", ["number", "string", "number"]);
        API.LNMaterial_SetCullMode               = cw("LNMaterial_SetCullMode",               "number", ["number", "number"]);
        API.LNMaterial_SetBlendMode              = cw("LNMaterial_SetBlendMode",              "number", ["number", "number"]);
        API.LNMaterial_SetDepthTestEnabled       = cw("LNMaterial_SetDepthTestEnabled",       "number", ["number", "number"]);
        API.LNMaterial_SetDepthWriteEnabled      = cw("LNMaterial_SetDepthWriteEnabled",      "number", ["number", "number"]);

        // Mesh
        API.LNMesh_Create      = cw("LNMesh_Create",      "number", ["number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNMesh_SetMaterial = cw("LNMesh_SetMaterial", "number", ["number", "number", "number"]);

        // Camera
        API.LNCamera_Create         = cw("LNCamera_Create",         "number", ["number"]);
        API.LNCamera_SetPerspective = cw("LNCamera_SetPerspective", "number", ["number", "number", "number", "number", "number"]);
        API.LNCamera_SetOrthographic2D = cw("LNCamera_SetOrthographic2D", "number", ["number", "number", "number", "number", "number"]);
        API.LNCamera_SetLookAt      = cw("LNCamera_SetLookAt",      "number", ["number", "number", "number", "number", "number", "number", "number", "number", "number", "number"]);
        API.LNCamera_SetMatrices    = cw("LNCamera_SetMatrices",    "number", ["number", "number", "number"]);
    }
}
