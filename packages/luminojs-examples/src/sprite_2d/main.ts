/**
 * Sprite2D example
 *
 * LNCamera_SetOrthographic2D (TS: Camera.setOrthographic2D) による
 * 左上原点・Y軸下向きの 2D カメラと、その下での Renderer.drawSprite の動作確認サンプル。
 *
 * 確認項目:
 *   - 画面座標 (0, 0) が左上、(WINDOW_W, WINDOW_H) が右下に対応すること
 *   - スプライトのテクスチャが上下逆さにならず正立すること
 *   - マテリアルの既定 CullMode (Back) のままで描画されること
 *   - 回転スプライトが「スクリーン上で時計回り」に回ること
 *
 * 期待される画面レイアウト:
 *   ┌──────────────────────────────────────┐
 *   │ [TL]                            [TR] │  ← 左上 / 右上の角スプライト
 *   │                                      │
 *   │              [CENTER+ROT]            │  ← 中央で回転するスプライト
 *   │                                      │
 *   │ [BL]                            [BR] │  ← 左下 / 右下の角スプライト
 *   └──────────────────────────────────────┘
 */
import {
    Runtime,
    GraphicsContext,
    LoadOp,
    Texture,
    Material,
    Camera,
} from "luminojs";

const WINDOW_W = 1280;
const WINDOW_H = 720;

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = WINDOW_W;
    canvas.height = WINDOW_H;

    // Load WASM module and initialize Lumino instance (creates WebGPU device)
    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });

    // Create GraphicsContext from canvas
    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // Texture (any RGBA image; up/down asymmetric content makes flip easier to spot)
    const texture = await Texture.loadFromURL(
        new URL("../../public/picture1.png", import.meta.url).href);

    // Material - default CullMode::Back. No tweaks needed for 2D camera path.
    const material = Material.createUnlit();
    material.setMainTexture(texture);

    // 2D camera: top-left origin, Y+ down.
    // (0, 0) -> screen top-left, (WINDOW_W, WINDOW_H) -> screen bottom-right.
    const camera = Camera.create();
    camera.setOrthographic2D(WINDOW_W, WINDOW_H, -1000.0, 1000.0);

    const SPR = 64.0;
    const MARGIN = 8.0;
    // Position is the sprite CENTER in screen-space pixels.
    // Half-size offsets place corner sprites flush against the corners.
    const half = SPR * 0.5;

    let frame = 0;

    function render() {
        const t = frame * 0.02;

        const { renderer } = context.beginFrame();
        renderer.beginRenderPass(
            context,
            {
                colorAttachments: [
                    {
                        clearColor: [0.10, 0.10, 0.15, 1.0],
                        loadOp: LoadOp.Clear,
                    },
                ],
            },
            camera,
        );

        // Top-Left (red tint)
        renderer.drawSprite(
            material, 0,
            MARGIN + half, MARGIN + half, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 0.4, 0.4, 1.0,
            0.0);

        // Top-Right (green tint)
        renderer.drawSprite(
            material, 0,
            WINDOW_W - MARGIN - half, MARGIN + half, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            0.4, 1.0, 0.4, 1.0,
            0.0);

        // Bottom-Left (blue tint)
        renderer.drawSprite(
            material, 0,
            MARGIN + half, WINDOW_H - MARGIN - half, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            0.4, 0.4, 1.0, 1.0,
            0.0);

        // Bottom-Right (yellow tint)
        renderer.drawSprite(
            material, 0,
            WINDOW_W - MARGIN - half, WINDOW_H - MARGIN - half, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 0.4, 1.0,
            0.0);

        // Center sprite, rotating - visually verifies orientation & winding.
        renderer.drawSprite(
            material, 0,
            WINDOW_W * 0.5, WINDOW_H * 0.5, 0.0,
            SPR * 2.0, SPR * 2.0,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0,
            t);

        renderer.endRenderPass();
        context.endFrame();

        frame++;
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

main();
