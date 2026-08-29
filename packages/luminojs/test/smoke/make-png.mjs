// テスト用の小さな PNG を生成するための最小エンコーダ。
//
// 外部依存を増やさずに「既知のピクセル値を持つ PNG」を作り、デコード結果を
// バイト単位で検証できるようにする。フィルタなし (filter type 0) の RGBA8 画像のみ対応。

import zlib from "node:zlib";

/** PNG チャンクの CRC32 を計算する。 */
function crc32(buf) {
    let c = ~0;
    for (let i = 0; i < buf.length; i++) {
        c ^= buf[i];
        for (let k = 0; k < 8; k++) {
            c = (c >>> 1) ^ (0xedb88320 & -(c & 1));
        }
    }
    return (~c) >>> 0;
}

/** 長さ + タイプ + データ + CRC の PNG チャンクを組み立てる。 */
function chunk(type, data) {
    const typeBuf = Buffer.from(type, "latin1");
    const lenBuf = Buffer.alloc(4);
    lenBuf.writeUInt32BE(data.length, 0);
    const crcBuf = Buffer.alloc(4);
    crcBuf.writeUInt32BE(crc32(Buffer.concat([typeBuf, data])), 0);
    return Buffer.concat([lenBuf, typeBuf, data, crcBuf]);
}

/**
 * RGBA8 ピクセル列から PNG バイト列を生成する。
 * @param {number} width  画像の幅
 * @param {number} height 画像の高さ
 * @param {Buffer} rgba   長さ width*height*4 の RGBA8 ピクセル列 (行優先, 上から下)
 * @returns {Buffer} PNG バイト列
 */
export function makePng(width, height, rgba) {
    const signature = Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]);

    const ihdr = Buffer.alloc(13);
    ihdr.writeUInt32BE(width, 0);
    ihdr.writeUInt32BE(height, 4);
    ihdr[8] = 8; // ビット深度
    ihdr[9] = 6; // カラータイプ: RGBA
    ihdr[10] = 0; // 圧縮方式
    ihdr[11] = 0; // フィルタ方式
    ihdr[12] = 0; // インターレース方式

    // 各スキャンラインの先頭にフィルタバイト 0 を付与する。
    const stride = width * 4;
    const raw = Buffer.alloc((stride + 1) * height);
    for (let y = 0; y < height; y++) {
        raw[y * (stride + 1)] = 0;
        rgba.copy(raw, y * (stride + 1) + 1, y * stride, y * stride + stride);
    }
    const idat = zlib.deflateSync(raw);

    return Buffer.concat([
        signature,
        chunk("IHDR", ihdr),
        chunk("IDAT", idat),
        chunk("IEND", Buffer.alloc(0)),
    ]);
}
