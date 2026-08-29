/**
 * Sprite2D サンプル
 *
 * LNCamera_SetOrthographic2D (TS: Camera.setOrthographic2D) による
 * 左上原点、Y軸下向きの 2D カメラと、その下での Renderer.drawSprite の動作確認サンプル。
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
    Matrix4x4,
} from "luminojs";

const WINDOW_W = 1280;
const WINDOW_H = 720;

async function main() {
    const canvas = document.getElementById("my_canvas") as HTMLCanvasElement;
    canvas.width = WINDOW_W;
    canvas.height = WINDOW_H;

    // WASM モジュールをロードし、Lumino インスタンスを初期化する (WebGPU デバイスが作成される)
    await Runtime.initialize({
        wasmPath: new URL("../../../luminojs/lib/LuminoC.wasm", import.meta.url).href,
    });

    // canvas から GraphicsContext を作成する
    const context = await GraphicsContext.createFromCanvas("#my_canvas");

    // テクスチャ (任意の RGBA 画像。上下非対称の絵柄だと反転に気づきやすい)
    const texture = await Texture.loadFromURL(
        new URL("../../public/picture1.png", import.meta.url).href);

    // マテリアル - 既定の CullMode::Back のまま。2D カメラ経路では調整不要。
    const material = Material.createUnlit();
    material.setMainTexture(texture);

    // 2D カメラ: 左上原点、Y軸下向き。
    // (0, 0) -> 画面左上、(WINDOW_W, WINDOW_H) -> 画面右下。
    const camera = Camera.create();
    camera.setOrthographic2D(WINDOW_W, WINDOW_H, -1000.0, 1000.0);

    const SPR = 64.0;
    const MARGIN = 8.0;
    // 位置はスクリーン空間 (ピクセル) でのスプライトの中心。
    // 半サイズ分ずらすことで、角のスプライトが画面の角にぴったり接する。
    const half = SPR * 0.5;

    let frame = 0;

    function render() {
        const t = frame * 0.02;

        const frameInfo = context.beginFrame();

        if (!frameInfo) {

            // デバイスロスト復旧待ち。このフレームはスキップする

            requestAnimationFrame(render);

            return;

        }

        const { renderer } = frameInfo;
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

        // 左上 (赤系)
        renderer.drawSprite(
            material, 0,
            Matrix4x4.makeTranslation(MARGIN + half, MARGIN + half, 0.0),
            0.0, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 0.4, 0.4, 1.0);

        // 右上 (緑系)
        renderer.drawSprite(
            material, 0,
            Matrix4x4.makeTranslation(WINDOW_W - MARGIN - half, MARGIN + half, 0.0),
            0.0, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            0.4, 1.0, 0.4, 1.0);

        // 左下 (青系)
        renderer.drawSprite(
            material, 0,
            Matrix4x4.makeTranslation(MARGIN + half, WINDOW_H - MARGIN - half, 0.0),
            0.0, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            0.4, 0.4, 1.0, 1.0);

        // 右下 (黄系)
        renderer.drawSprite(
            material, 0,
            Matrix4x4.makeTranslation(WINDOW_W - MARGIN - half, WINDOW_H - MARGIN - half, 0.0),
            0.0, 0.0,
            SPR, SPR,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 0.4, 1.0);

        // 中央の回転スプライト - 向きとワインディングを目視で確認する。
        // world = T(center) * Rz(t): pivot 中心まわりに回転して中央へ配置。
        const centerXf = Matrix4x4.makeTranslation(WINDOW_W * 0.5, WINDOW_H * 0.5, 0.0)
            .multiply(Matrix4x4.makeRotationZ(t));
        renderer.drawSprite(
            material, 0,
            centerXf,
            0.0, 0.0,
            SPR * 2.0, SPR * 2.0,
            0.5, 0.5,
            0.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0);

        renderer.endRenderPass();
        context.endFrame();

        frame++;
        requestAnimationFrame(render);
    }
    requestAnimationFrame(render);
}

main();
