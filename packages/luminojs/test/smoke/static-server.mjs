// スモークテスト用の最小静的ファイルサーバ。
//
// WASM を検証するには luminojs を「ブラウザが実際にロードする経路」で読み込む必要がある。
// そのため lib/ (luminojs.mjs / LuminoC.mjs / LuminoC.wasm) を HTTP で配信する。
// 特に .wasm は `application/wasm` を返さないと WebAssembly.instantiateStreaming が
// 失敗するため、MIME タイプを明示する。
//
// また WebGPU (navigator.gpu) は secure context でのみ露出するため、127.0.0.1 上の
// http:// で配信する (localhost はブラウザが potentially trustworthy とみなす)。

import http from "node:http";
import { readFile } from "node:fs/promises";
import path from "node:path";

const MIME = {
    ".html": "text/html; charset=utf-8",
    ".mjs": "text/javascript; charset=utf-8",
    ".js": "text/javascript; charset=utf-8",
    ".json": "application/json; charset=utf-8",
    ".map": "application/json; charset=utf-8",
    ".wasm": "application/wasm",
    ".css": "text/css; charset=utf-8",
};

// ルートアクセス時に返す最小のセキュアな HTML ページ。
// このページを起点に動的 import で luminojs をロードする。
const INDEX_HTML =
    "<!doctype html><html><head><meta charset=\"utf-8\">" +
    "<title>luminojs smoke</title></head><body>luminojs smoke test harness</body></html>";

/**
 * 指定ディレクトリを配信する HTTP サーバを起動する。
 * @param {string} rootDir 配信するルートディレクトリ (luminojs パッケージルート)
 * @returns {Promise<{ server: import("node:http").Server, baseURL: string }>}
 */
export async function startStaticServer(rootDir) {
    const root = path.resolve(rootDir);

    const server = http.createServer(async (req, res) => {
        try {
            const urlPath = decodeURIComponent(new URL(req.url, "http://127.0.0.1").pathname);
            res.setHeader("Access-Control-Allow-Origin", "*");

            if (urlPath === "/" || urlPath === "/index.html") {
                res.statusCode = 200;
                res.setHeader("Content-Type", MIME[".html"]);
                res.end(INDEX_HTML);
                return;
            }

            // ブラウザが自動要求する favicon は中身なしで応答し、ログの 404 を防ぐ。
            if (urlPath === "/favicon.ico") {
                res.statusCode = 204;
                res.end();
                return;
            }

            // ディレクトリトラバーサル防止: 正規化後に root 配下であることを確認する。
            const filePath = path.normalize(path.join(root, urlPath));
            if (filePath !== root && !filePath.startsWith(root + path.sep)) {
                res.statusCode = 403;
                res.end("forbidden");
                return;
            }

            const body = await readFile(filePath);
            const ext = path.extname(filePath).toLowerCase();
            res.statusCode = 200;
            res.setHeader("Content-Type", MIME[ext] ?? "application/octet-stream");
            res.end(body);
        } catch {
            res.statusCode = 404;
            res.end("not found");
        }
    });

    await new Promise((resolve) => server.listen(0, "127.0.0.1", resolve));
    const { port } = /** @type {import("node:net").AddressInfo} */ (server.address());
    return { server, baseURL: `http://127.0.0.1:${port}` };
}
