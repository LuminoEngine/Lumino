import type { Handle } from "./types";
import { Result } from "./types";
import { API, Runtime } from "./Runtime";
import { Logger } from "./Logger";

/** C 側のハンドルを保持するすべての Lumino API オブジェクトの基底クラス。 */
export class LuminoObject {
    protected _handle: Handle = 0;
    protected _autoRelease = false;

    /** 内部の C-API ハンドル。 */
    get handle(): Handle {
        return this._handle;
    }

    /** C 側のオブジェクトを解放する (このインスタンスが所有している場合のみ)。 */
    dispose(): void {
        if (this._autoRelease && this._handle !== 0) {
            const rc = (API.LNObject_Release as (h: number) => number)(this._handle);
            if (rc !== Result.OK) {
                Logger.warning(`LNObject_Release(${this._handle}) returned ${rc}`);
            }
            this._handle = 0;
        }
    }

    /** @internal 構築後にハンドルを設定する。 */
    _setHandle(handle: Handle, autoRelease: boolean): void {
        this._handle = handle;
        this._autoRelease = autoRelease;
    }
}
