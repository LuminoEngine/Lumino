
// @ts-ignore
import LuminoFFIModule from "../dist/LuminoFFI.js";
console.log("aaa", LuminoFFIModule());

export class Runtime {
    public static initialize(): Promise<void> {
        return LuminoFFIModule().then((module: any) => {
            console.log("lloaded", module);
        });
    }
}

