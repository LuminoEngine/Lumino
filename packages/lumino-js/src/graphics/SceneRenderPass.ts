import { Material } from "./Material";
import { GraphicsCommandList } from "./GraphicsCommandList";
import { LuminoObject } from "../LuminoObject";
import { IColor } from "../math/Color";
import { Matrix } from "../math/Matrix";
import { IRect } from "../math/Rect";
import { ISize } from "../math/Types";
import { IVector2 } from "../math/Vector2";
import { API, Handle, Pointer, Runtime } from "../Runtime";
import { GraphicsContext } from "./GraphicsContext";

export class SceneRenderPass extends LuminoObject {
    private _owner: GraphicsContext;
    
    public constructor(owner: GraphicsContext) {
        super();
        this._owner = owner;
    }

    public drawSprite(params: LNDrawSpriteParams): void {
        const ptr = LNDrawSpriteParamsBuffer.apply(params);
        API.LNSceneRenderPass_DrawSprite(this.handle, ptr);
    }

}

export enum LNSpriteBaseDirection {
    LN_SPRITE_BASE_DIRECTION_XPLUS = 0,
    LN_SPRITE_BASE_DIRECTION_YPLUS = 1,
    LN_SPRITE_BASE_DIRECTION_ZPLUS = 2,
    LN_SPRITE_BASE_DIRECTION_XMINUS = 3,
    LN_SPRITE_BASE_DIRECTION_YMINUS = 4,
    LN_SPRITE_BASE_DIRECTION_ZMINUS = 5,
    LN_SPRITE_BASE_DIRECTION_BASIC2D = 6,
}

export enum LNBillboardType {
    LN_BILLBOARD_TYPE_NONE = 0,
    LN_BILLBOARD_TYPE_TO_CAMERA_POINT = 1,
    LN_BILLBOARD_TYPE_TO_SCREEN = 2,
    LN_BILLBOARD_TYPE_ROT_Y = 3,
};

export interface LNDrawSpriteParams {
    material: Material;
    transform: Matrix | undefined;
    size: ISize;
    anchorRatio: IVector2;
    uvRect: IRect;
    color: IColor;
    baseDirection: LNSpriteBaseDirection;
    billboardType: LNBillboardType;
}

class LNDrawSpriteParamsBuffer {
    private static SIZE = 124;
    private static _cachedInstance: LNDrawSpriteParamsBuffer;

    public static apply(params: LNDrawSpriteParams): Pointer {
        if (!this._cachedInstance) {
            this._cachedInstance = new LNDrawSpriteParamsBuffer();
        }
        this._cachedInstance.set(params);
        return this._cachedInstance.heapPtr;
    }

    public readonly heapPtr: number;
    private _heap8: Uint8Array;
    private _view: DataView;

    constructor() {
        this._heap8 = Runtime.module.HEAPU8 as Uint8Array;
        this.heapPtr = Runtime.module._malloc(LNDrawSpriteParamsBuffer.SIZE);
        this._view =  new DataView(this._heap8.buffer, this.heapPtr, LNDrawSpriteParamsBuffer.SIZE);
    }

    public set(params: LNDrawSpriteParams): void {
        if (this._heap8 !== Runtime.module.HEAPU8) {
            this._heap8 = Runtime.module.HEAPU8 as Uint8Array;
            //this.heapPtr = Runtime.module._malloc(124);
            this._view = new DataView(this._heap8.buffer, this.heapPtr, LNDrawSpriteParamsBuffer.SIZE);
        }

        const transform = params.transform ?? Matrix.Identity;

        let offset = 0;
        this._view.setUint32(offset, params.material.handle, true);
        offset += 4;
        const floatArray = transform.m;
        for (let i = 0; i < floatArray.length; i++) {
            this._view.setFloat32(offset, floatArray[i], true);
            offset += 4;
        }

        this._view.setFloat32(offset, params.size.width, true);
        offset += 4;
        this._view.setFloat32(offset, params.size.height, true);
        offset += 4;

        this._view.setFloat32(offset, params.anchorRatio.x, true);
        offset += 4;
        this._view.setFloat32(offset, params.anchorRatio.y, true);
        offset += 4;

        this._view.setFloat32(offset, params.uvRect.x, true);
        offset += 4;
        this._view.setFloat32(offset, params.uvRect.y, true);
        offset += 4;
        this._view.setFloat32(offset, params.uvRect.width, true);
        offset += 4;
        this._view.setFloat32(offset, params.uvRect.height, true);
        offset += 4;

        this._view.setFloat32(offset, params.color.r, true);
        offset += 4;
        this._view.setFloat32(offset, params.color.g, true);
        offset += 4;
        this._view.setFloat32(offset, params.color.b, true);
        offset += 4;
        this._view.setFloat32(offset, params.color.a, true);
        offset += 4;

        this._view.setInt32(offset, params.baseDirection, true);
        offset += 4;
        this._view.setInt32(offset, params.billboardType, true);
        offset += 4;
    }
}
