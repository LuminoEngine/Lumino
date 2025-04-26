
// @ts-ignore
//import LuminoFFIModule from "../dist/LuminoFFI.js";
import { DepthBuffer } from "./graphics/DepthBuffer";
import { GraphicsContext } from "./graphics/GraphicsContext";
export * from "./math/Types";
export * from "./math/Matrix";
export * from "./graphics/GraphicsContext";
export * from "./graphics/GraphicsCommandList";
export * from "./graphics/SceneRenderPass";
export * from "./graphics/GraphicsViewPoint";
export * from "./graphics/Material";
export * from "./graphics/Texture2D";
import { RenderTexture } from "./graphics/RenderTargetTexture";

import { Runtime, API } from "./Runtime";
//console.log("aaa", LuminoFFIModule());

export * from "./Runtime";
export * from "./graphics/WebGPUGraphicsContext";
export * from "./graphics/WebGLGraphicsContext";

// const canvas = document.getElementById("maincanvas") as HTMLCanvasElement;
// var gl = canvas.getContext("webgl");

