import { defineConfig } from "vite";
import { resolve } from "path";
import { readdirSync, statSync, readFileSync, existsSync } from "fs";

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

// Plugin that serves raw TypeScript source files without transformation,
// so the example viewer can display the original source (with comments, etc.).
function rawSourcePlugin() {
  const srcDir = resolve(__dirname, "src");
  return {
    name: "raw-source",
    configureServer(server: any) {
      server.middlewares.use((req: any, res: any, next: any) => {
        // Match requests like /src/<id>/main.ts?raw-source (query param distinguishes
        // source-display fetches from actual module script imports)
        const m = req.url?.match(/^\/src\/([^/]+)\/main\.ts\?.*raw-source/);
        if (!m) return next();
        const filePath = resolve(srcDir, m[1], "main.ts");
        if (!existsSync(filePath)) return next();
        const content = readFileSync(filePath, "utf-8");
        res.setHeader("Content-Type", "text/plain; charset=utf-8");
        res.end(content);
      });
    },
  };
}

export default defineConfig({
  plugins: [rawSourcePlugin()],
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
