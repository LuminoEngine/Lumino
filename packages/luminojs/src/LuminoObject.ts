import type { Handle } from "./types";
import { Result } from "./types";
import { API, Runtime } from "./Runtime";

/** Base class for all Lumino API objects that hold a C-side handle. */
export class LuminoObject {
    protected _handle: Handle = 0;
    protected _autoRelease = false;

    /** The underlying C-API handle. */
    get handle(): Handle {
        return this._handle;
    }

    /** Release the C-side object (if this instance owns it). */
    dispose(): void {
        if (this._autoRelease && this._handle !== 0) {
            const rc = (API.LNObject_Release as (h: number) => number)(this._handle);
            if (rc !== Result.OK) {
                console.warn(`LNObject_Release(${this._handle}) returned ${rc}`);
            }
            this._handle = 0;
        }
    }

    /** @internal Set the handle after construction. */
    _setHandle(handle: Handle, autoRelease: boolean): void {
        this._handle = handle;
        this._autoRelease = autoRelease;
    }
}
