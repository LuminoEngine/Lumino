import { defineConfig } from "vite";
import { resolve } from "path";
import { readdirSync, statSync, readFileSync, existsSync } from "fs";

// すべてのサンプルのエントリポイント (src/*/index.html) を収集する
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

// TypeScript ソースファイルを変換せずそのまま配信するプラグイン。
// サンプルビューアが元のソース (コメントなどを含む) を表示できるようにする。
function rawSourcePlugin() {
  const srcDir = resolve(__dirname, "src");
  return {
    name: "raw-source",
    configureServer(server: any) {
      server.middlewares.use((req: any, res: any, next: any) => {
        // /src/<id>/main.ts?raw-source 形式のリクエストに一致させる (クエリパラメータで
        // ソース表示用の fetch とモジュールスクリプトとしての import を区別する)
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
      // このディレクトリと luminojs の lib/ の両方からファイルを配信できるようにする
      allow: [resolve(__dirname), resolve(__dirname, "../luminojs")],
    },
  },
});
