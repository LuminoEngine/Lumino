import type { Handle } from "./types";

/**
 * Residency 管理の対象となるリソース。
 * Texture / Mesh / Material が実装する。
 */
export interface ResidentResource {
    readonly handle: Handle;
    readonly lastUsedFrame: number;
    evict(): void;
}

/**
 * GPU にロード済みのリソース (Texture / Mesh / Material) を管理し、
 * `threshold` フレーム以上使用されなかったリソースを GPU から解放する。
 * ソースデータは JS ヒープ上にリソースオブジェクトが保持しているため、
 * 解放後も次の `ensure()` 呼び出し時に再アップロードできる。
 */
export class ResidencyManager {
    private _tracked: Set<ResidentResource> = new Set();
    private _threshold = 300;

    register(r: ResidentResource): void {
        this._tracked.add(r);
    }

    unregister(r: ResidentResource): void {
        this._tracked.delete(r);
    }

    /** GraphicsContext.beginFrame() から毎フレーム 1 回呼び出す。 */
    gc(currentFrame: number): void {
        for (const r of this._tracked) {
            if (r.handle !== 0 && currentFrame - r.lastUsedFrame > this._threshold) {
                r.evict();
            }
        }
    }

    /** GraphicsContext.dispose() から呼び出す。全リソースを GPU から解放する。 */
    disposeAll(): void {
        for (const r of this._tracked) {
            if (r.handle !== 0) r.evict();
        }
        this._tracked.clear();
    }

    /**
     * デバイスロスト復旧後に GraphicsContext から呼び出す。
     * 全リソースの stale な GPU ハンドルを解放する (disposeAll と異なり追跡は維持)。
     * ソースデータは JS 側に保持されているため、次の描画時の ensure() で
     * 自動的に再アップロードされる。
     */
    invalidateAll(): void {
        for (const r of this._tracked) {
            if (r.handle !== 0) r.evict();
        }
    }

    /** @internal デバッグ・テスト用 */
    get threshold(): number { return this._threshold; }
    set threshold(v: number) { this._threshold = v; }
}
