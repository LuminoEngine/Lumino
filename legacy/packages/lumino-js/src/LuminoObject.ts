import { API, Handle } from "./Runtime";

export class LuminoObject {
    private _handle: Handle;
    private _autoRelease: boolean;

    public get handle(): Handle {
        return this._handle;
    }

    public constructor() {
        this._handle = 0;
        this._autoRelease = false;
    }

    public dispose(): void {
        if (this._handle !== 0 && this._autoRelease) {
            API.LNObject_Release(this._handle);
            this._handle = 0;
            this._autoRelease = false;
        }
    }

    public _setHandle(handle: Handle, autoRelease: boolean): void {
        this.dispose();
        this._handle = handle;
        this._autoRelease = autoRelease;
    }
}
