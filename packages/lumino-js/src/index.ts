
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";
console.log("aaa", LuminoFFIModule());

export class Runtime {
    public static module: any;

    public static initialize(): Promise<void> {
        return LuminoFFIModule().then((module: any) => {
            this.module = module;
            API.LNRuntime_Initialize = module.cwrap("LNRuntime_Initialize", "number", []),
            API.LNRuntime_Terminate = module.cwrap("LNRuntime_Terminate", "void", []),
            API.LNRuntime_Initialize();
        });
    }
}

export enum Result {
    LN_OK = 0,
    LN_ERROR_UNKNOWN = -1,
}

export class API {
    public static LNRuntime_Initialize: () => Result = null;
    public static LNRuntime_Terminate: () => void = null;
}
