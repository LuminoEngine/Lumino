import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Texture } from "./Texture";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { BlendMode, BuiltinShader, CullMode, TextureAddressMode, TextureFilterMode } from "./types";

type MaterialSource =
    | { kind: "unlit" }
    | { kind: "compiled"; data: Uint8Array };

/** サンプラー設定 (テクスチャのフィルタリングとアドレッシング)。 */
interface SamplerState {
    filter: TextureFilterMode;
    address: TextureAddressMode;
}

export class Material extends LuminoObject implements ResidentResource {
    private _source: MaterialSource;
    private _mainTexture: Texture | undefined;
    private _namedTextures: Map<string, Texture> = new Map();
    private _color: [number, number, number, number] | null = null;
    private _float4s: Map<string, [number, number, number, number]> = new Map();
    private _samplerState: SamplerState | null = null;
    private _namedSamplerStates: Map<string, SamplerState> = new Map();
    private _cullMode: CullMode | null = null;
    private _blendMode: BlendMode | null = null;
    private _depthTestEnabled: boolean | null = null;
    private _depthWriteEnabled: boolean | null = null;
    private _dirty = false;
    private _paramsDirty = false;
    private _lastUsedFrame = 0;

    get lastUsedFrame(): number { return this._lastUsedFrame; }

    constructor(source: MaterialSource) {
        super();
        this._source = source;
    }

    /** 組み込みの Unlit シェーダーを用いてマテリアルを作成します。(テクスチャ * カラー、ライティングなし)
     * GPU 側リソースは最初の描画時に遅延生成されます。 */
    static createUnlit(): Material {
        const mat = new Material({ kind: "unlit" });
        mat._dirty = true;
        return mat;
    }

    /**
     * コンパイル済みシェーダバイナリ (.lcsh) からマテリアルを作成します。GPU 側生成は遅延。
     * @param data コンパイル済みシェーダのバイナリデータ
     */
    static createFromCompiledShader(data: Uint8Array): Material {
        const mat = new Material({ kind: "compiled", data });
        mat._dirty = true;
        return mat;
    }

    /**
     * メイン (ディフューズ) テクスチャを設定します。(シャドウ状態のみ保持し、ensure 時に反映)
     *
     * undefined が指定された場合、組み込みの小さな白テクスチャが描画に使用されます。
     */
    setMainTexture(texture: Texture | undefined): void {
        this._mainTexture = texture;
        this._paramsDirty = true;
    }

    /**
     * ベースカラー (RGBA, 0.0 - 1.0) を設定します。
     * @param r 赤成分 (0.0 - 1.0)
     * @param g 緑成分 (0.0 - 1.0)
     * @param b 青成分 (0.0 - 1.0)
     * @param a アルファ成分 (0.0 - 1.0)
     */
    setColor(r: number, g: number, b: number, a: number): void {
        this._color = [r, g, b, a];
        this._paramsDirty = true;
    }

    /** 名前付き float4 シェーダパラメータを設定します。 */
    setFloat4(name: string, values: [number, number, number, number]): void {
        this._float4s.set(name, [values[0], values[1], values[2], values[3]]);
        this._paramsDirty = true;
    }

    /**
     * 名前付きシェーダ uniform にテクスチャをバインドします。
     *
     * ある名前に対して一度もバインドされなかった場合(あるいは下層の C++ Material に
     * その名前で `null` が渡された場合)、未バインドのリソースをサンプリングしないよう、
     * 組み込みの 1x1 白テクスチャがスロットに割り当てられます。
     * @param name    シェーダ内のテクスチャバインディング名 (例: `"u_sceneColor"`)
     * @param texture バインドするテクスチャ
     */
    setNamedTexture(name: string, texture: Texture): void {
        this._namedTextures.set(name, texture);
        this._paramsDirty = true;
    }

    /**
     * このマテリアルが参照する全テクスチャのサンプリング方法を設定します。
     *
     * 既定は `TextureFilterMode.Linear` + `TextureAddressMode.ClampToEdge` です。
     * ドット絵を拡大表示する場合は `Nearest` を、タイリングする模様やノイズ
     * テクスチャには `Repeat` を指定します。
     *
     * 個別のテクスチャだけ変えたい場合は `setNamedSamplerState` で上書きしてください。
     * @param filter  拡大/縮小フィルタ
     * @param address UV が 0.0 - 1.0 の範囲外に出たときの回り込み方法
     */
    setSamplerState(filter: TextureFilterMode, address: TextureAddressMode): void {
        this._samplerState = { filter, address };
        this._paramsDirty = true;
    }

    /**
     * 名前付きテクスチャ 1 スロットのサンプリング方法を設定します。
     * `setSamplerState` によるマテリアル単位の設定を上書きします。
     *
     * `name` にはシェーダ内の `Texture2D` 変数名 (例: `"u_sceneColor"`) を指定します。
     * ペアになる `SamplerState` 変数名 (例: `"u_sceneColorSampler"`) ではありません。
     * @param name    シェーダ内のテクスチャバインディング名 (例: `"u_sceneColor"`)
     * @param filter  拡大/縮小フィルタ
     * @param address UV が 0.0 - 1.0 の範囲外に出たときの回り込み方法
     */
    setNamedSamplerState(name: string, filter: TextureFilterMode, address: TextureAddressMode): void {
        this._namedSamplerStates.set(name, { filter, address });
        this._paramsDirty = true;
    }

    /**
     * フェースカリングモードを設定します。
     * @param mode カリングモード
     */
    setCullMode(mode: CullMode): void {
        this._cullMode = mode;
        this._paramsDirty = true;
    }

    /**
     * ブレンドモードを設定します。
     * @param mode 合成方法
     */
    setBlendMode(mode: BlendMode): void {
        this._blendMode = mode;
        this._paramsDirty = true;
    }

    /**
     * デプステストの有効/無効を設定します。
     * @param enabled true で有効
     */
    setDepthTestEnabled(enabled: boolean): void {
        this._depthTestEnabled = enabled;
        this._paramsDirty = true;
    }

    /**
     * デプス書き込みの有効/無効を設定します。
     * @param enabled true で有効
     */
    setDepthWriteEnabled(enabled: boolean): void {
        this._depthWriteEnabled = enabled;
        this._paramsDirty = true;
    }

    /**
     * @internal 描画時に Renderer (または Mesh.ensure) から呼び出されます。
     * 初回使用時に C++ Material を生成し、シャドウされたパラメータ変更を適用します。
     */
    ensure(ctx: GraphicsContext): void {
        if (this._handle === 0) {
            this._createGpuMaterial(ctx);
            // 生成直後は _paramsDirty の以前の状態にかかわらず、
            // すべてのパラメータを適用する必要がある。
            this._paramsDirty = true;
        }

        if (this._paramsDirty) {
            this._applyParams(ctx);
            this._paramsDirty = false;
        }

        this._dirty = false;
        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /** @internal GPU マテリアルを解放します。次の ensure() で再生成され、パラメータも再適用されます。 */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
        this._paramsDirty = true;
    }

    override dispose(): void {
        this.evict();
        this._mainTexture = undefined;
        this._namedTextures.clear();
        this._namedSamplerStates.clear();
        this._float4s.clear();
    }

    //--------------------------------------------------------------------------

    private _createGpuMaterial(ctx: GraphicsContext): void {
        if (this._source.kind === "unlit") {
            const handle = Runtime.safeCallWithReturnHandle((out) =>
                (API.LNMaterial_CreateFromBuiltinShader as (
                    ctx: number, shader: number, out: number,
                ) => number)(ctx.handle, BuiltinShader.Unlit, out));
            this._setHandle(handle, true);
        } else {
            const data = this._source.data;
            const m = Runtime.module;
            const ptr = m._malloc(data.byteLength);
            try {
                m.HEAPU8.set(data, ptr);
                const handle = Runtime.safeCallWithReturnHandle((out) =>
                    (API.LNMaterial_CreateFromCompiledShader as (
                        ctx: number, data: number, size: number, out: number,
                    ) => number)(ctx.handle, ptr, data.byteLength, out));
                this._setHandle(handle, true);
            } finally {
                m._free(ptr);
            }
        }
    }

    private _applyParams(ctx: GraphicsContext): void {
        if (this._mainTexture) {
            this._mainTexture.ensure(ctx);
            if (this._mainTexture.handle !== 0) {
                Runtime.safeCall(() =>
                    (API.LNMaterial_SetMainTexture as (
                        mat: number, tex: number,
                    ) => number)(this._handle, this._mainTexture!.handle));
            }
        }

        for (const [name, tex] of this._namedTextures) {
            tex.ensure(ctx);
            if (tex.handle === 0) continue;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetNamedTexture as (
                    mat: number, name: string, tex: number,
                ) => number)(this._handle, name, tex.handle));
        }

        if (this._color) {
            const c = this._color;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetColor as (
                    mat: number, r: number, g: number, b: number, a: number,
                ) => number)(this._handle, c[0], c[1], c[2], c[3]));
        }

        if (this._float4s.size > 0) {
            const m = Runtime.module;
            const ptr = m._malloc(16);
            try {
                const floatOffset = ptr >> 2;
                for (const [name, v] of this._float4s) {
                    m.HEAPF32[floatOffset]     = v[0];
                    m.HEAPF32[floatOffset + 1] = v[1];
                    m.HEAPF32[floatOffset + 2] = v[2];
                    m.HEAPF32[floatOffset + 3] = v[3];
                    Runtime.safeCall(() =>
                        (API.LNMaterial_SetFloat4 as (
                            mat: number, name: string, values: number,
                        ) => number)(this._handle, name, ptr));
                }
            } finally {
                m._free(ptr);
            }
        }

        if (this._samplerState !== null) {
            const s = this._samplerState;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetSamplerState as (
                    mat: number, filter: number, address: number,
                ) => number)(this._handle, s.filter, s.address));
        }

        for (const [name, s] of this._namedSamplerStates) {
            Runtime.safeCall(() =>
                (API.LNMaterial_SetNamedSamplerState as (
                    mat: number, name: string, filter: number, address: number,
                ) => number)(this._handle, name, s.filter, s.address));
        }

        if (this._cullMode !== null) {
            const mode = this._cullMode;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetCullMode as (
                    mat: number, mode: number,
                ) => number)(this._handle, mode));
        }

        if (this._blendMode !== null) {
            const mode = this._blendMode;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetBlendMode as (
                    mat: number, mode: number,
                ) => number)(this._handle, mode));
        }

        if (this._depthTestEnabled !== null) {
            const enabled = this._depthTestEnabled ? 1 : 0;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetDepthTestEnabled as (
                    mat: number, enabled: number,
                ) => number)(this._handle, enabled));
        }

        if (this._depthWriteEnabled !== null) {
            const enabled = this._depthWriteEnabled ? 1 : 0;
            Runtime.safeCall(() =>
                (API.LNMaterial_SetDepthWriteEnabled as (
                    mat: number, enabled: number,
                ) => number)(this._handle, enabled));
        }
    }
}
