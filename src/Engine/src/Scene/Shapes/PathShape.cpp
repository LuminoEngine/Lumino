
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Scene/Shapes/PathShape.hpp>
#include "../../Rendering/RenderFeature/NanoVGHelper.hpp"

namespace ln {
    
namespace detail {

struct PathShape_NVGcontext : public GLNVGcontextBase {
    PathPlacementSpace m_space = PathPlacementSpace::XY;
    Ref<MeshPrimitive> m_mesh;
    Vertex* m_vertices;
    uint16_t m_indexOffset;
    uint16_t* m_indexBuffer;

    static NVGcontext* nvgCreate(int flags, PathShape_NVGcontext** glCtx);
    static void nvgDelete(NVGcontext* ctx)
    {
        nvgDeleteInternal(ctx);
    }

    static int glnvg__renderCreate(void* uptr)
    {
        PathShape_NVGcontext* gl = (PathShape_NVGcontext*)uptr;

        gl->fragSize = sizeof(GLNVGfragUniforms);
        return 1;
    }

    static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
    {
        return INT_MAX;
    }

    static int glnvg__renderDeleteTexture(void* uptr, int image)
    {
        // 削除はここでする必要はない
        return 1;	// 成功扱い
    }

    static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
    {
        // テキスト描画機能を使うときに必要になる
        LN_UNREACHABLE();	// unused
        return 0;
    }

    static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
    {
        // テキスト描画機能を使うときに必要になる
        LN_UNREACHABLE();	// unused
        return 0;
    }

    static void glnvg__renderFlush(void* uptr)
    {
        PathShape_NVGcontext* gl = (PathShape_NVGcontext*)uptr;

        if (LN_REQUIRE(!gl->m_mesh)) return;    // 2回禁止


        if (gl->ncalls > 0) {
            // インデックス数を見積もる
            int indexCount = 0;
            for (int i = 0; i < gl->ncalls; i++) {
                GLNVGcall* call = &gl->calls[i];
                GLNVGpath* paths = &gl->paths[call->pathOffset];
                const int npaths = call->pathCount;

                if (call->type == GLNVG_FILL) {
                    LN_NOTIMPLEMENTED();
                }
                else if (call->type == GLNVG_CONVEXFILL) {
                    for (int i = 0; i < npaths; i++) {
                        const GLNVGpath* path = &paths[i];
                        indexCount += (path->fillCount - 2) * 3;    // TriangleFan -> TriangleList
                    }
                }
                else if (call->type == GLNVG_STROKE) {
                    for (int i = 0; i < npaths; i++) {
                        const GLNVGpath* path = &paths[i];
                        indexCount += path->strokeCount;            // TriangleStrip
                    }
                }
                else if (call->type == GLNVG_TRIANGLES) {
                    LN_NOTIMPLEMENTED();
                }

            }


            gl->m_mesh = makeObject<MeshPrimitive>(gl->nverts, indexCount);
            gl->m_vertices = static_cast<Vertex*>(gl->m_mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));

            if (gl->m_space == PathPlacementSpace::XY) {
                for (int i = 0; i < gl->nverts; i++) {
                    const NVGvertex* v = &gl->verts[i];
                    gl->m_vertices[i].set(Vector3(v->x, v->y, 0.0f), Vector3::UnitZ, Vector2(v->u, v->v), Color::White);
                }
            }
            else {
                for (int i = 0; i < gl->nverts; i++) {
                    const NVGvertex* v = &gl->verts[i];
                    gl->m_vertices[i].set(Vector3(v->x, 0.0f, v->y), Vector3::UnitZ, Vector2(v->u, v->v), Color::White);
                }
            }

            //memcpy(gl->m_mesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main), gl->verts, gl->nverts * sizeof(NVGvertex));
            gl->m_indexOffset = 0;
            gl->m_indexBuffer = static_cast<uint16_t*>(gl->m_mesh->acquireMappedIndexBuffer());

            for (int i = 0; i < gl->ncalls; i++) {
                GLNVGcall* call = &gl->calls[i];
                if (call->type == GLNVG_FILL)
                    glnvg__fill(gl, call);
                else if (call->type == GLNVG_CONVEXFILL)
                    glnvg__convexFill(gl, call);
                else if (call->type == GLNVG_STROKE)
                    glnvg__stroke(gl, call);
                else if (call->type == GLNVG_TRIANGLES)
                    glnvg__triangles(gl, call);
            }
        }


        // Reset calls
        gl->nverts = 0;
        gl->npaths = 0;
        gl->ncalls = 0;
        gl->nuniforms = 0;
    }

    static void glnvg__renderDelete(void* uptr)
    {
        PathShape_NVGcontext* gl = (PathShape_NVGcontext*)uptr;
        gl->dispose();
        delete gl;
    }

    static void glnvg__fill(PathShape_NVGcontext* gl, GLNVGcall* call)
    {
        LN_NOTIMPLEMENTED();
    }
    static void glnvg__convexFill(PathShape_NVGcontext* gl, GLNVGcall* call)
    {
        GLNVGpath* paths = &gl->paths[call->pathOffset];
        const int npaths = call->pathCount;
        const GLNVGfragUniforms* uniform = reinterpret_cast<const GLNVGfragUniforms*>(gl->uniforms + call->uniformOffset);
        

        for (int i = 0; i < npaths; i++) {
            const GLNVGpath* path = &paths[i];
            // TriangleFan を TriangleList に展開する
            //   DX12 では Fan は使えない。NanoVG は TriangleStrip や TriangleFan に最適化した頂点バッファを出してくれるので
            //   インデックスバッファの多くが無駄になってしまうが、Fan は対応できないのでやむなし。

            int startIndex = gl->m_indexOffset;
            uint16_t base = path->fillOffset;
            uint16_t prev = base + 1;
            for (int iVertex = 2; iVertex < path->fillCount; iVertex++) {
                gl->m_indexBuffer[gl->m_indexOffset] = base; gl->m_indexOffset++;
                gl->m_indexBuffer[gl->m_indexOffset] = prev; gl->m_indexOffset++;
                gl->m_indexBuffer[gl->m_indexOffset] = path->fillOffset + iVertex; gl->m_indexOffset++;
                prev = path->fillOffset + iVertex;
            }
            gl->m_mesh->addSection(startIndex, path->fillCount - 2, 0, PrimitiveTopology::TriangleList);
            gl->applyVertexColors(path->fillOffset, path->fillCount, uniform);

            // Draw fringes
            if (paths[i].strokeCount > 0) {
                LN_NOTIMPLEMENTED();
            }
        }
    }
    static void glnvg__stroke(PathShape_NVGcontext* gl, GLNVGcall* call)
    {
        const GLNVGpath* paths = &gl->paths[call->pathOffset];
        const int npaths = call->pathCount;
        const GLNVGfragUniforms* uniform = reinterpret_cast<const GLNVGfragUniforms*>(gl->uniforms + call->uniformOffset);

        if (gl->flags & NVG_STENCIL_STROKES) {
            LN_NOTIMPLEMENTED();
        }
        else {
            int startIndex = gl->m_indexOffset;
            for (int i = 0; i < npaths; i++) {
                const GLNVGpath* path = &paths[i];
                for (int iVertex = 0; iVertex < path->strokeCount; iVertex++) {
                    gl->m_indexBuffer[gl->m_indexOffset] = path->strokeOffset + iVertex; gl->m_indexOffset++;
                }
                gl->m_mesh->addSection(startIndex, path->strokeCount - 2, 0, PrimitiveTopology::TriangleStrip);
                gl->applyVertexColors(path->strokeOffset, path->strokeCount, uniform);
            }
        }
    }
    static void glnvg__triangles(PathShape_NVGcontext* gl, GLNVGcall* call)
    {
        LN_NOTIMPLEMENTED();
    }


    void applyVertexColors(int offset, int count, const GLNVGfragUniforms* uniform)
    {
        for (int i = 0; i < count; i++) {
            m_vertices[offset + i].color.set(uniform->innerCol.r, uniform->innerCol.g, uniform->innerCol.b, uniform->innerCol.a);
        }
    }
};

NVGcontext* PathShape_NVGcontext::nvgCreate(int flags, PathShape_NVGcontext** glCtx)
{
    NVGparams params;
    NVGcontext* ctx = NULL;
    PathShape_NVGcontext* gl = new PathShape_NVGcontext();//(PathShape_NVGcontext*)malloc(sizeof(PathShape_NVGcontext));
    if (gl == NULL) goto error;
    gl->clear();

    gl->makeNVGparams(&params);
    params.renderCreate = glnvg__renderCreate;
    params.renderCreateTexture = glnvg__renderCreateTexture;
    params.renderDeleteTexture = glnvg__renderDeleteTexture;
    params.renderUpdateTexture = glnvg__renderUpdateTexture;
    params.renderGetTextureSize = glnvg__renderGetTextureSize;
    params.renderFlush = glnvg__renderFlush;
    params.renderDelete = glnvg__renderDelete;
    params.userPtr = gl;
    params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

    gl->flags = flags;

    ctx = nvgCreateInternal(&params);
    if (ctx == NULL) goto error;

    *glCtx = gl;

    return ctx;

error:
    // 'gl' is freed by nvgDeleteInternal.
    if (ctx != NULL) nvgDeleteInternal(ctx);
    return NULL;
}

} // namespace detail





//=============================================================================
// PathShapeComponent

LN_OBJECT_IMPLEMENT(PathShapeComponent, VisualComponent) {}

PathShapeComponent::PathShapeComponent()
{
}

bool PathShapeComponent::init()
{
    if (!VisualComponent::init()) return false;

    m_material = makeObject<Material>();

    //m_mesh = makeObject<MeshPrimitive>(4, 6);
    //m_mesh->setVertex(0, Vertex(Vector3(-1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(1, Vertex(Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(2, Vertex(Vector3(1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(3, Vertex(Vector3(1, -1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //    
    //m_mesh->setIndex(0, 0);
    //m_mesh->setIndex(1, 1);
    //m_mesh->setIndex(2, 2);
    //m_mesh->setIndex(3, 2);
    //m_mesh->setIndex(4, 1);
    //m_mesh->setIndex(5, 3);

    //m_mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);



    detail::PathShape_NVGcontext* m_glnvgContext;
    NVGcontext* m_nvgContext = detail::PathShape_NVGcontext::nvgCreate(0/*NVG_ANTIALIAS*/, &m_glnvgContext);

    nvgBeginPath(m_nvgContext);
    //nvgRect(m_nvgContext, 0, 0, 20, 10);
    nvgMoveTo(m_nvgContext, 1, 2);
    nvgLineTo(m_nvgContext, 1, 5);
    nvgLineTo(m_nvgContext, 3, 3);
    nvgFillColor(m_nvgContext, nvgRGBA(255, 192, 0, 255));
    nvgFill(m_nvgContext);

    //nvgStrokeColor(m_nvgContext, nvgRGBA(255, 255, 255, 255));
    //nvgStrokeWidth(m_nvgContext, 1);
    //nvgStroke(m_nvgContext);

    nvgEndFrame(m_nvgContext);
    

    m_mesh = m_glnvgContext->m_mesh;
    detail::PathShape_NVGcontext::nvgDelete(m_nvgContext);



    return true;
}

void PathShapeComponent::onRender(RenderingContext* context)
{
    context->setMaterial(m_material);

    const int sectionCount = m_mesh->sections().size();
    for (int i = 0; i < sectionCount; i++) {
        context->drawMesh(m_mesh, i);
    }
}

//=============================================================================
// PathShape

LN_OBJECT_IMPLEMENT(PathShape, VisualObject) {}

PathShape::PathShape()
{
}

bool PathShape::init()
{
    if (!VisualObject::init()) return false;
    m_component = makeObject<PathShapeComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
    return true;
}

} // namespace ln

