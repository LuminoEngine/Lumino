import type { RenderPassDesc, Transform } from "./types";
import {
    LN_NULL_HANDLE,
    LN_MAX_COLOR_ATTACHMENTS,
    LoadOp,
    SIZEOF_RENDER_PASS_DESC,
    SIZEOF_COLOR_ATTACHMENT_DESC,
} from "./types";

//------------------------------------------------------------------------------
// WASM 非依存の純シリアライズ関数
//------------------------------------------------------------------------------
// Renderer.ts の _serializeDesc / _serializeTransform から、DataView への書き込み
// ロジックだけを切り出したもの。WASM ヒープへのポインタ確保 (malloc) や
// shaderPassName 文字列の書き込みは Renderer.ts 側の責務のまま残し、ここでは
// 「構造体レイアウト通りに DataView へ値を書く」ことだけを行う。
// これにより単体テストではプレーンな ArrayBuffer を渡すだけで検証できる
// (WASM モジュールのロードや Runtime のモックが不要)。
//------------------------------------------------------------------------------

/**
 * `RenderPassDesc` を DataView に書き込む。
 *
 * C レイアウト (wasm32, 4 バイトアライン、合計 224 バイト):
 * ```
 * offset 0:   uint32_t colorAttachmentCount
 * offset 4:   LNColorAttachmentDesc colorAttachments[8]  (各 24 バイト)
 *   アタッチメントごと:
 *     +0  uint32_t renderTarget
 *     +4  float    clearColor[4]
 *     +20 uint32_t loadOp
 * offset 196: LNDepthStencilAttachmentDesc depthStencil  (20 バイト)
 *     +0  uint32_t depthBuffer
 *     +4  float    clearDepth
 *     +8  uint32_t clearStencil
 *     +12 uint32_t depthLoadOp
 *     +16 uint32_t stencilLoadOp
 * offset 216: const char* shaderPassName  (ポインタ, 4 バイト)
 * offset 220: LNSortMode sortMode         (uint32_t, 4 バイト)
 * ```
 *
 * @param view              書き込み先 (少なくとも `SIZEOF_RENDER_PASS_DESC` バイト)。
 * @param desc              書き込む内容。
 * @param shaderPassNamePtr `desc.shaderPassName` を WASM ヒープへ書き込んだ後のポインタ
 *                          (未指定時は 0)。文字列自体の確保は呼び出し側の責務。
 */
export function writeRenderPassDesc(view: DataView, desc: RenderPassDesc, shaderPassNamePtr: number): void {
    // 構造体全体をゼロクリアする (安全なデフォルト値: 全ゼロ = CLEAR、
    // renderTarget=NULL_HANDLE、clearDepth は後で設定する)。
    const bytes = new Uint8Array(view.buffer, view.byteOffset, SIZEOF_RENDER_PASS_DESC);
    bytes.fill(0);

    // --- カラーアタッチメント ---
    const attachments = desc.colorAttachments ?? [];
    const count = Math.min(attachments.length, LN_MAX_COLOR_ATTACHMENTS);
    view.setUint32(0, count, true);

    for (let i = 0; i < count; i++) {
        const a = attachments[i];
        const base = 4 + i * SIZEOF_COLOR_ATTACHMENT_DESC;
        view.setUint32(base + 0, a.renderTarget ?? LN_NULL_HANDLE, true);
        const c = a.clearColor ?? [0, 0, 0, 1];
        view.setFloat32(base + 4,  c[0], true);
        view.setFloat32(base + 8,  c[1], true);
        view.setFloat32(base + 12, c[2], true);
        view.setFloat32(base + 16, c[3], true);
        view.setUint32(base + 20, a.loadOp ?? LoadOp.Clear, true);
    }

    // --- デプス/ステンシル ---
    const dsBase = 4 + LN_MAX_COLOR_ATTACHMENTS * SIZEOF_COLOR_ATTACHMENT_DESC; // 196
    const ds = desc.depthStencil;
    view.setUint32(dsBase + 0,  ds?.depthBuffer   ?? LN_NULL_HANDLE, true);
    view.setFloat32(dsBase + 4, ds?.clearDepth     ?? 1.0, true);
    view.setUint32(dsBase + 8,  ds?.clearStencil   ?? 0, true);
    view.setUint32(dsBase + 12, ds?.depthLoadOp    ?? LoadOp.Clear, true);
    view.setUint32(dsBase + 16, ds?.stencilLoadOp  ?? LoadOp.Clear, true);

    // --- shaderPassName (offset 216 の const char*) ---
    view.setUint32(216, shaderPassNamePtr, true);

    // --- sortMode (offset 220) ---
    view.setUint32(220, desc.sortMode ?? 0, true);
}

/**
 * `Transform` を DataView に書き込む。
 *
 * C レイアウト (wasm32, 40 バイト):
 * ```
 * offset 0:  float posX, posY, posZ        (12 バイト)
 * offset 12: float rotX, rotY, rotZ, rotW  (16 バイト)
 * offset 28: float scaleX, scaleY, scaleZ  (12 バイト)
 * ```
 */
export function writeTransform(view: DataView, t: Transform): void {
    view.setFloat32(0,  t.position[0], true);
    view.setFloat32(4,  t.position[1], true);
    view.setFloat32(8,  t.position[2], true);
    view.setFloat32(12, t.rotation[0], true);
    view.setFloat32(16, t.rotation[1], true);
    view.setFloat32(20, t.rotation[2], true);
    view.setFloat32(24, t.rotation[3], true);
    view.setFloat32(28, t.scale[0], true);
    view.setFloat32(32, t.scale[1], true);
    view.setFloat32(36, t.scale[2], true);
}
