import {
    type InstanceInitializeSettings,
    GraphicsBackend,
    SIZEOF_INSTANCE_INIT_SETTINGS,
} from "./types";
import { API, Runtime } from "./Runtime";

export class Instance {
    /**
     * Initialize the Lumino runtime.
     * Must be called after `Runtime.initialize()`.
     */
    static async initialize(settings?: InstanceInitializeSettings): Promise<void> {
        const m = Runtime.module;
        let ptr = 0; // nullptr → defaults
        if (settings) {
            ptr = m._malloc(SIZEOF_INSTANCE_INIT_SETTINGS);
            const view = new DataView(m.HEAPU8.buffer, ptr, SIZEOF_INSTANCE_INIT_SETTINGS);
            view.setUint32(0, settings.preferredBackend ?? GraphicsBackend.Default, true);
            view.setUint32(4, settings.enableValidation ? 1 : 0, true);
        }
        try {
            await Runtime.safeCallAsync(() =>
                (API.LNInstance_Initialize as (p: number) => number | Promise<number>)(ptr));
        } finally {
            if (ptr) m._free(ptr);
        }
    }

    /** Shut down the Lumino runtime. */
    static async terminate(): Promise<void> {
        await (API.LNInstance_Terminate as () => Promise<void>)();
    }
}
