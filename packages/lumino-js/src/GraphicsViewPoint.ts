import { LuminoObject } from "./LuminoObject";
import { API, Runtime } from "./Runtime";

export class GraphicsViewPoint extends LuminoObject {
    
    public constructor() {
        super();
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNCamera_Create(r));
        this._setHandle(handle, true);
    }

    public setupPerspective2D(width: number, height: number): void {
        API.LNCamera_SetupPerspective2D(this.handle, 0, 0, 0, width, height, -500, 500);
    }
}

