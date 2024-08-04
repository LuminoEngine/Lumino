import { Texture2D } from "../Texture2D";
import { LuminoObject } from "../LuminoObject";
import { API, Runtime } from "../Runtime";

/**
 * Represents a material.
 */
export class Material extends LuminoObject {

    /**
     * Creates a new material.
     */
    public constructor() {
        super();
        this._setHandle(Runtime.safeCallWithReturnHandle((r) => API.LNMaterial_Create(r)), true);
    }

    /**
     * Sets the main texture.
     */
    public setMainTexture(texture: Texture2D): void {
        Runtime.safeCall(() => API.LNMaterial_SetMainTexture(this.handle, texture.handle));
    }
}
