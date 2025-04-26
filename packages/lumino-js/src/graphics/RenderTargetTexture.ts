import { GraphicsContext } from "./GraphicsContext";
import { LuminoObject } from "../LuminoObject";

export class RenderTexture extends LuminoObject {
    private _owner: GraphicsContext;
    public constructor(graphicsContext: GraphicsContext) {
        super();
        this._owner = graphicsContext;
    }
}
