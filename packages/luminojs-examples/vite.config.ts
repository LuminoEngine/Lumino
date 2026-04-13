import { defineConfig } from "vite";
import { resolve } from "path";
import { readdirSync, statSync } from "fs";

// Collect all sample entry points: src/*/index.html
function getSampleInputs(): Record<string, string> {
  const srcDir = resolve(__dirname, "src");
  const inputs: Record<string, string> = {};
  for (const name of readdirSync(srcDir)) {
    const dir = resolve(srcDir, name);
    if (statSync(dir).isDirectory()) {
      const html = resolve(dir, "index.html");
      inputs[name] = html;
    }
  }
  return inputs;
}

export default defineConfig({
  build: {
    target: "esnext",
    rollupOptions: {
      input: {
        main: resolve(__dirname, "index.html"),
        ...getSampleInputs(),
      },
    },
  },
  resolve: {
    alias: {
      luminojs: resolve(__dirname, "../luminojs/lib/luminojs.mjs"),
    },
  },
  server: {
    fs: {
      // Allow serving files from both this directory and the luminojs lib/
      allow: [resolve(__dirname), resolve(__dirname, "../luminojs")],
    },
  },
});
