import { LuminoObject } from "./LuminoObject";
import { API, Runtime } from "./Runtime";

export class GraphicsViewPoint extends LuminoObject {
    
    public constructor() {
        super();
        API.LNGraphicsViewPoint_Create(Runtime.returnPointerView.byteOffset);
        this._setHandle(Runtime.returnPointerView[0], true);
    }

    public setupPerspective2D(width: number, height: number): void {
        API.LNGraphicsViewPoint_SetupPerspective2D(this.handle, 0, 0, 0, width, height, -500, 500);
    }
}

