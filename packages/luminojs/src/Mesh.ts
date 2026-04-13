import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Material } from "./Material";
import { API, Runtime } from "./Runtime";
import { type Vertex, type SubMesh, SIZEOF_VERTEX, SIZEOF_SUBMESH } from "./types";

export class Mesh extends LuminoObject {
    /**
     * Create a mesh from vertex, index, and sub-mesh data.
     *
     * @param ctx       GraphicsContext.
     * @param vertices  Array of vertex data.
     * @param indices   Index buffer (uint32).
     * @param submeshes Array of sub-mesh descriptors.
     */
    static create(
        ctx: GraphicsContext,
        vertices: Vertex[],
        indices: Uint32Array,
        submeshes: SubMesh[],
    ): Mesh {
        const m = Runtime.module;

        const vertBytes = vertices.length * SIZEOF_VERTEX;
        const idxBytes = indices.byteLength;
        const subBytes = submeshes.length * SIZEOF_SUBMESH;

        const vertPtr = m._malloc(vertBytes);
        const idxPtr = m._malloc(idxBytes);
        const subPtr = m._malloc(subBytes);

        try {
            // Serialize vertices (16 floats each = 64 bytes)
            const vView = new DataView(m.HEAPU8.buffer, vertPtr, vertBytes);
            for (let i = 0; i < vertices.length; i++) {
                const v = vertices[i];
                const base = i * SIZEOF_VERTEX;
                vView.setFloat32(base + 0,  v.position[0], true);
                vView.setFloat32(base + 4,  v.position[1], true);
                vView.setFloat32(base + 8,  v.position[2], true);
                vView.setFloat32(base + 12, v.normal[0], true);
                vView.setFloat32(base + 16, v.normal[1], true);
                vView.setFloat32(base + 20, v.normal[2], true);
                vView.setFloat32(base + 24, v.uv[0], true);
                vView.setFloat32(base + 28, v.uv[1], true);
                vView.setFloat32(base + 32, v.color[0], true);
                vView.setFloat32(base + 36, v.color[1], true);
                vView.setFloat32(base + 40, v.color[2], true);
                vView.setFloat32(base + 44, v.color[3], true);
                vView.setFloat32(base + 48, v.tangent[0], true);
                vView.setFloat32(base + 52, v.tangent[1], true);
                vView.setFloat32(base + 56, v.tangent[2], true);
                vView.setFloat32(base + 60, v.tangent[3], true);
            }

            // Copy indices
            m.HEAPU8.set(new Uint8Array(indices.buffer, indices.byteOffset, idxBytes), idxPtr);

            // Serialize submeshes (3 uint32 each = 12 bytes)
            const sView = new DataView(m.HEAPU8.buffer, subPtr, subBytes);
            for (let i = 0; i < submeshes.length; i++) {
                const s = submeshes[i];
                const base = i * SIZEOF_SUBMESH;
                sView.setUint32(base + 0, s.indexOffset, true);
                sView.setUint32(base + 4, s.indexCount, true);
                sView.setUint32(base + 8, s.materialIndex, true);
            }

            const handle = Runtime.safeCallWithReturnHandle((out) =>
                (API.LNMesh_Create as (
                    ctx: number,
                    verts: number, vertCount: number,
                    idx: number, idxCount: number,
                    subs: number, subCount: number,
                    out: number,
                ) => number)(
                    ctx.handle,
                    vertPtr, vertices.length,
                    idxPtr, indices.length,
                    subPtr, submeshes.length,
                    out,
                ));

            const mesh = new Mesh();
            mesh._setHandle(handle, true);
            return mesh;
        } finally {
            m._free(subPtr);
            m._free(idxPtr);
            m._free(vertPtr);
        }
    }

    /** Assign a material to the given slot index. */
    setMaterial(materialIndex: number, material: Material): void {
        Runtime.safeCall(() =>
            (API.LNMesh_SetMaterial as (
                mesh: number, idx: number, mat: number,
            ) => number)(this._handle, materialIndex, material.handle));
    }
}
