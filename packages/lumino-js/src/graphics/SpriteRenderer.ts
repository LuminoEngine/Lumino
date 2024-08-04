import { GraphicsCommandList } from "../GraphicsCommandList";
import { API, Runtime } from "../Runtime";
import { Material } from "./Material";
import { Matrix } from "../math/Matrix";
import { IColor, IPoint, IRect, ISize } from "../math/Types";

/** 3D 空間での基準方向を表す値 */
export enum SpriteBaseDirection {
    XPlus = 0,   /**< X+ 方向 (右向き) */
    YPlus = 1,   /**< Y+ 方向 (上向き) */
    ZPlus = 2,   /**< Z+ 方向 (奥向き) */
    XMinus = 3,  /**< X- 方向 (左向き) */
    YMinus = 4,  /**< Y- 方向 (下向き) */
    ZMinus = 5,  /**< Z- 方向 (手前向き) */
    Basic2D = 6, /**< Z+ 方向、左上原点 */
};

/** ビルボードの計算方法 */
export enum BillboardType {
    None = 0,          /**< ビルボードの計算を行わない */
    ToCameraPoint = 1, /**< カメラ (ビュー行列) に対して正面を向く */
    ToScreen = 2,      /**< スクリーン (ビュー平面) に対して正面を向く */
    RotY = 3,          /**< Y 軸回転のみ行う */
};

export class SpriteRenderer {
    private static _instance: SpriteRenderer;
    private _handle: number;

    /**
     * Gets the singleton instance of the SpriteRenderer.
     */
    public static get(): SpriteRenderer {
        if (!SpriteRenderer._instance) {
            SpriteRenderer._instance = new SpriteRenderer();
        }
        return SpriteRenderer._instance;
    }

    /**
     * Creates a new SpriteRenderer.
     */
    private constructor() {
        this._handle = Runtime.safeCallWithReturnHandle((r) => API.LNSpriteRenderer_Get(r));
    }

    /**
     * Begins a batch.
     */
    public beginBatch(graphicsCommandList: GraphicsCommandList, material: Material, transform: Matrix): void {
        Runtime.safeCall(() => API.LNSpriteRenderer_BeginBatch(
            this._handle,
            graphicsCommandList.handle,
            material.handle,
            Runtime.setMatrix(transform.m)));
    }

    /**
     * Ends a batch.
     */
    public endBatch(): void {
        Runtime.safeCall(() => API.LNSpriteRenderer_EndBatch(this._handle));
    }

    /**
     * Draws a sprite.
     */
    public drawSprite(
        localTransform: Matrix | null | undefined,
        size: ISize,
        anchorRatio: IPoint,
        uvRect: IRect,
        color: IColor,
        baseDirection: SpriteBaseDirection,
        billboardType: BillboardType): void {
        Runtime.safeCall(() => API.LNSpriteRenderer_DrawSprite(
            this._handle,
            localTransform ? Runtime.setMatrix(localTransform.m) : 0,
            size.width, size.height,
            anchorRatio.x, anchorRatio.y,
            uvRect.x, uvRect.y, uvRect.width, uvRect.height,
            color.r, color.g, color.b, color.a,
            baseDirection, billboardType));
    }
}
