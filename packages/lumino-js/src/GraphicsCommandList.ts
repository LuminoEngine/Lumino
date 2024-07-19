import { GraphicsContext } from "./GraphicsContext";
import { GraphicsViewPoint } from "./GraphicsViewPoint";
import { LuminoObject } from "./LuminoObject";
import { RenderPass } from "./RenderPass";
import { API, Runtime } from "./Runtime";

export class GraphicsCommandList extends LuminoObject {
    private _owner: GraphicsContext;

    /**
     * Creates a new GraphicsCommandList.
     */
    public constructor(owner: GraphicsContext) {
        super();
        this._owner = owner;
        API.LNGraphicsCommandList_Create(owner.handle, Runtime.returnPointerView.byteOffset);
        this._setHandle(Runtime.returnPointerView[0], true);
    }

    public reset(viewPoint: GraphicsViewPoint): void {
        API.LNGraphicsCommandList_Reset(this.handle, viewPoint.handle);
    }

    public beginRenderPass(): RenderPass {

        const s = API.LNRenderPassDescriptor_Get();
        API.LNRenderPassDescriptor_SetRenderTarget(
            s, 0, this._owner.currentColorBuffer.handle,
            1, 0, 0, 1,
            1
        );
        API.LNRenderPassDescriptor_SetDepthBuffer(
            s, this._owner.currentDepthBuffer.handle,
            1, 0, 1, 1
        );
        
        API.LNGraphicsCommandList_BeginRenderPass(this.handle, s, Runtime.returnPointerView.byteOffset);

        const renderPass = new RenderPass(this);
        renderPass._setHandle(Runtime.returnPointerView[0], false);
        return renderPass;
    }
}

