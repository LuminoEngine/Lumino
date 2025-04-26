import { GraphicsContext } from "./GraphicsContext";
import { LuminoObject } from "../LuminoObject";
import { API, Runtime } from "../Runtime";

export class Texture2D extends LuminoObject {
    public constructor(buffer: ArrayBuffer) {
        super();

        const ptr = Runtime.module._malloc(buffer.byteLength) as number;
        Runtime.module.HEAPU8.set(new Uint8Array(buffer), ptr);

        //const u81 = new Uint8Array(buffer, 0, buffer.byteLength);


        console.log("--- s", typeof ptr);
        const handle = Runtime.safeCallWithReturnHandle((r) => API.LNTexture2D_CreateFromImageFileData(ptr, buffer.byteLength, r));
        ;
        Runtime.module._free(ptr);
        console.log("--- e");
        this._setHandle(handle, true);
    }
}


