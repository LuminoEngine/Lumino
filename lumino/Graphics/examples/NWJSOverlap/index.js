

let loaded = false;
let inited = false
renderer_lib.initRuntime(
    "dist/NWJSOverlap_Native.wasm",
    () => {
        console.log("initRuntime onLoaded", renderer_lib.native);
        renderer_lib.native.myFunction();
        loaded = true;
    },
    () => {
        console.log("initRuntime onError");
    });



class App extends PIXI.Application {

    contextHandle = 0;

    render() {
        if (loaded && !inited) {
            console.log("renderer_lib.Module", renderer_lib.Module);
            console.log("initLumino", this.renderer.gl);
            this.contextHandle = renderer_lib.Module.GL.registerContext(this.renderer.gl, {
                majorVersion: "2",
                minorVersion: "0",
                enableExtensionsByDefault: true
            });
            renderer_lib.Module.GL.makeContextCurrent(this.contextHandle);

            renderer_lib.native.initLumino();
            renderer_lib.native.initApp();
            inited = true;
        }

        super.render();

        if (inited) {
            renderer_lib.Module.GL.makeContextCurrent(this.contextHandle);
            renderer_lib.native.renderApp();
            console.log("render.");
        }
    }
}


//const  PIXI = require('pixi.js');
// const app = new PIXI.Application();
// document.body.appendChild(app.view);

// console.log("aaaa");


window.onload = async () =>{


    const app = new App({
        width: 800, height: 600, backgroundColor: 0x1099bb, resolution: window.devicePixelRatio || 1,
    });
    document.body.appendChild(app.view);

    const container = new PIXI.Container();

    app.stage.addChild(container);

    // Create a new texture
    const texture = PIXI.Texture.from('assets/bunny.png');

    console.log("texture", texture);

    // Create a 5x5 grid of bunnies
    for (let i = 0; i < 25; i++) {
        const bunny = new PIXI.Sprite(texture);
        bunny.anchor.set(0.5);
        bunny.x = (i % 5) * 40;
        bunny.y = Math.floor(i / 5) * 40;
        container.addChild(bunny);
    }

    // Move container to the center
    container.x = app.screen.width / 2;
    container.y = app.screen.height / 2;

    // Center bunny sprite in local container coordinates
    container.pivot.x = container.width / 2;
    container.pivot.y = container.height / 2;

    // Listen for animate update
    app.ticker.add((delta) => {
        // rotate the container!
        // use delta to create frame-independent transform
        container.rotation -= 0.01 * delta;
    });

}
