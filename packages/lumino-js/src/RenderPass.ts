import { GraphicsCommandList } from "./GraphicsCommandList";
import { LuminoObject } from "./LuminoObject";
import { API } from "./Runtime";

export class RenderPass extends LuminoObject {
    private _owner: GraphicsCommandList;
    
    public constructor(commandList: GraphicsCommandList) {
        super();
        this._owner = commandList;
    }

    public end(): void {
        API.LNRenderPass_End(this.handle);
    }
}
