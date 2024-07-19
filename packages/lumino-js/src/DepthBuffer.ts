import { GraphicsContext } from "./GraphicsContext";
import { LuminoObject } from "./LuminoObject";

export class DepthBuffer extends LuminoObject {
    private _owner: GraphicsContext;
    
    public constructor(graphicsContext: GraphicsContext) {
        super();
        this._owner = graphicsContext;
    }
}
