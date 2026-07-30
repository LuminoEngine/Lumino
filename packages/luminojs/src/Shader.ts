import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";

/**
 * コンパイル済みシェーダ (.lcsh) から構築した GPU シェーダモジュールと
 * パイプラインレイアウトを保持する共有オブジェクト。
 *
 * `Material.createFromShader` で作った Material はこれらを共有するため、
 * 同一シェーダの Material を何個作っても GPU リソースは増えません。
 * 1 フレーム内で異なるパラメータを使いたい場合 (Material のパラメータは
 * フレーム単位で後勝ちになるため、描画箇所ごとに Material が必要) や、
 * テクスチャごとに Material を分けたい場合は、Shader を 1 つ作って
 * そこから Material を量産してください。
 *
 * ```ts
 * const shader = Shader.createFromCompiledShader(bloomShaderData);
 * const passA = Material.createFromShader(shader);
 * const passB = Material.createFromShader(shader);  // モジュールは共有される
 * ```
 *
 * 共有できているかは `GraphicsContext.getProfiler().shaderPassCount` で計測できます
 * (Material を増やしてもこの値が増えなければ共有されています)。
 *
 * GPU 側の生成は最初に使う Material の描画時まで遅延されます。バイナリは JS 側に
 * 保持しているため、Residency 管理による解放やデバイスロストの後も自動で再生成されます。
 */
export class Shader extends LuminoObject implements ResidentResource {
    private _data: Uint8Array;
    private _lastUsedFrame = 0;

    get lastUsedFrame(): number { return this._lastUsedFrame; }

    private constructor(data: Uint8Array) {
        super();
        this._data = data;
    }

    /**
     * コンパイル済みシェーダバイナリ (.lcsh) からシェーダを作成します。GPU 側生成は遅延。
     * @param data コンパイル済みシェーダのバイナリデータ
     */
    static createFromCompiledShader(data: Uint8Array): Shader {
        return new Shader(data);
    }

    /**
     * @internal Material.ensure から呼び出されます。
     * 初回使用時に C++ Shader を生成します。
     */
    ensure(ctx: GraphicsContext): void {
        if (this._handle === 0) {
            const m = Runtime.module;
            const ptr = m._malloc(this._data.byteLength);
            try {
                m.HEAPU8.set(this._data, ptr);
                const handle = Runtime.safeCallWithReturnHandle((out) =>
                    (API.LNShader_CreateFromCompiledShader as (
                        ctx: number, data: number, size: number, out: number,
                    ) => number)(ctx.handle, ptr, this._data.byteLength, out));
                this._setHandle(handle, true);
            } finally {
                m._free(ptr);
            }
        }

        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /**
     * @internal GPU シェーダを解放します。次の ensure() で再生成されます。
     *
     * この Shader から作られた Material は ShaderPass を参照し続けるため、
     * 解放後も描画は継続できます。Material 側も同じ閾値で evict されるため、
     * 再生成後は再び同じモジュールを共有します。
     */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
    }

    override dispose(): void {
        this.evict();
    }
}
