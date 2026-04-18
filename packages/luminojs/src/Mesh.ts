import { LuminoObject } from "./LuminoObject";
import type { GraphicsContext } from "./GraphicsContext";
import type { Material } from "./Material";
import type { ResidentResource } from "./ResidencyManager";
import { API, Runtime } from "./Runtime";
import { type Vertex, type SubMesh, SIZEOF_VERTEX, SIZEOF_SUBMESH } from "./types";

export class Mesh extends LuminoObject implements ResidentResource {
    private _vertices: Vertex[] = [];
    private _indices: Uint32Array = new Uint32Array(0);
    private _submeshes: SubMesh[] = [];
    private _materials: (Material | null)[] = [];
    private _dirty = false;
    private _materialsDirty = false;
    private _lastUsedFrame = 0;
    private _isResidencyTarget = false;

    get lastUsedFrame(): number { return this._lastUsedFrame; }

    /**
     * Define a mesh from vertex, index, and sub-mesh data.
     * GPU バッファは最初の描画時に遅延生成される。
     */
    static create(
        vertices: Vertex[],
        indices: Uint32Array,
        submeshes: SubMesh[],
    ): Mesh {
        const mesh = new Mesh();
        mesh._vertices = vertices;
        mesh._indices = indices;
        mesh._submeshes = submeshes;
        mesh._materials = new Array(submeshes.length).fill(null);
        mesh._dirty = true;
        mesh._isResidencyTarget = true;
        return mesh;
    }

    /** Assign a material to the given slot index. (JS 側シャドウ、ensure 時に C 側へ反映) */
    setMaterial(materialIndex: number, material: Material): void {
        while (this._materials.length <= materialIndex) this._materials.push(null);
        this._materials[materialIndex] = material;
        this._materialsDirty = true;
    }

    /**
     * @internal Called by Renderer at draw time. Uploads vertex/index/submesh
     * buffers if not yet resident, then ensures dependent Materials and binds them.
     */
    ensure(ctx: GraphicsContext): void {
        if (!this._isResidencyTarget) return;

        if (this._handle === 0 || this._dirty) {
            if (this._handle !== 0) {
                (API.LNObject_Release as (h: number) => number)(this._handle);
                this._handle = 0;
            }
            this._createGpuMesh(ctx);
            this._dirty = false;
            // After (re)creation, all material bindings must be re-applied.
            this._materialsDirty = true;
        }

        if (this._materialsDirty) {
            for (let i = 0; i < this._materials.length; i++) {
                const mat = this._materials[i];
                if (!mat) continue;
                mat.ensure(ctx);
                if (mat.handle === 0) continue;
                Runtime.safeCall(() =>
                    (API.LNMesh_SetMaterial as (
                        mesh: number, idx: number, mat: number,
                    ) => number)(this._handle, i, mat.handle));
            }
            this._materialsDirty = false;
        }

        this._lastUsedFrame = ctx.currentFrame;
        ctx.residencyManager.register(this);
    }

    /** @internal Release the GPU mesh while keeping source data. */
    evict(): void {
        if (this._handle === 0) return;
        (API.LNObject_Release as (h: number) => number)(this._handle);
        this._handle = 0;
        // Next ensure() must re-bind materials on the new mesh.
        this._materialsDirty = true;
    }

    override dispose(): void {
        if (this._isResidencyTarget) {
            this.evict();
            this._vertices = [];
            this._indices = new Uint32Array(0);
            this._submeshes = [];
            this._materials = [];
            this._isResidencyTarget = false;
        } else {
            super.dispose();
        }
    }

    //--------------------------------------------------------------------------

    private _createGpuMesh(ctx: GraphicsContext): void {
        const m = Runtime.module;
        const vertices = this._vertices;
        const indices = this._indices;
        const submeshes = this._submeshes;

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

            this._setHandle(handle, true);
        } finally {
            m._free(subPtr);
            m._free(idxPtr);
            m._free(vertPtr);
        }
    }
}
