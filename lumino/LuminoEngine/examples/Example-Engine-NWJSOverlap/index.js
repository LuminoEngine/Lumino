
// try {
//     nw.Window.get().showDevTools();
// }
// catch (e) {

//     alert(e);
// }
// //
// alert("a");

let loaded = false;
let inited = false
exampleApp.initRuntime(
    "dist/Example_Engine_NWJSOverlap.wasm",
    () => {
        console.log("initRuntime onLoaded", exampleApp.native);
        exampleApp.native.myFunction(); // test
        loaded = true;
    },
    () => {
        console.log("initRuntime onError");
    });



class App extends PIXI.Application {

    contextHandle = 0;

    render() {
        if (loaded && !inited) {
            console.log("exampleApp.Module", exampleApp.Module);
            console.log("initLumino", this.renderer.gl);
            this.contextHandle = exampleApp.Module.GL.registerContext(this.renderer.gl, {
                majorVersion: "2",
                minorVersion: "0",
                enableExtensionsByDefault: true
            });
            exampleApp.Module.GL.makeContextCurrent(this.contextHandle);

            exampleApp.native.initLumino();
            exampleApp.native.initApp();
            inited = true;
        }

        super.render();

        if (inited) {
            const gl = this.renderer.gl;
            console.log("VERTEX_ARRAY_BINDING", gl.getParameter(gl.VERTEX_ARRAY_BINDING));
            console.log("ARRAY_BUFFER_BINDING", gl.getParameter(gl.ARRAY_BUFFER_BINDING));
            console.log("ELEMENT_ARRAY_BUFFER_BINDING", gl.getParameter(gl.ELEMENT_ARRAY_BUFFER_BINDING));
            console.log("FRAMEBUFFER_BINDING", gl.getParameter(gl.FRAMEBUFFER_BINDING));
            

            exampleApp.Module.GL.makeContextCurrent(this.contextHandle);
            exampleApp.native.renderApp(this.view.width, this.view.height);
            console.log("render.");
        
            this.renderer.texture.reset();
            this.renderer.geometry.reset();
            this.renderer.state.reset();
            this.renderer.shader.reset();
            this.renderer.framebuffer.reset();
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
    const texture = PIXI.Texture.from('assets/icon256.png');

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
