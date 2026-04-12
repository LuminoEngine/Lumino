import { LuminoObject } from "./LuminoObject";
import { GraphicsContext } from "./GraphicsContext";
import { API, Runtime } from "./Runtime";
import { LN_NULL_HANDLE, Result, type Handle } from "./types";

export class Window extends LuminoObject {
    private _graphicsContext: GraphicsContext | null = null;

    /**
     * Create a Window backed by an HTML `<canvas>` element.
     * Web-only — on desktop builds the underlying C function returns an error.
     */
    static async createFromCanvas(
        canvasSelector: string,
        width: number,
        height: number,
    ): Promise<Window> {
        const handle = await Runtime.safeCallWithReturnHandleAsync((ptr) =>
            (API.LNWindow_CreateFromCanvas as (s: string, w: number, h: number, p: number) => number | Promise<number>)(
                canvasSelector, width, height, ptr,
            ));
        const w = new Window();
        w._setHandle(handle, true);
        return w;
    }

    /** Get (or cache) the `GraphicsContext` associated with this window. */
    getGraphicsContext(): GraphicsContext {
        if (this._graphicsContext) return this._graphicsContext;
        const handle = Runtime.safeCallWithReturnHandle((ptr) =>
            (API.LNWindow_GetGraphicsContext as (h: number, p: number) => number)(
                this._handle, ptr));
        const ctx = new GraphicsContext();
        // The context is owned by the Window on the C side – no autoRelease.
        ctx._setHandle(handle, false);
        this._graphicsContext = ctx;
        return ctx;
    }

    /**
     * Pump the platform event queue.
     * @returns `true` if the application should quit.
     */
    processEvents(): boolean {
        const m = Runtime.module;
        const [ptr, view] = Runtime.getReturnPointerInfo();
        view[0] = 0;
        const rc = (API.LNWindow_ProcessEvents as (h: number, p: number) => number)(this._handle, ptr);
        if (rc !== Result.OK) throw new Error(`LNWindow_ProcessEvents error: ${rc}`);
        return view[0] !== 0;
    }

    override dispose(): void {
        this._graphicsContext = null;
        super.dispose();
    }
}
