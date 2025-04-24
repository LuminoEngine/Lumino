#include <vector>
#include <string>
#include <format>
#include <lumino.h>

struct Sprite {
    float x;
    float y;
    float vx;
    float vy;
    float angle;
    int iconIndex;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_COUNT = 1000;
const float ICON_SIZE = 24;
const int ICON_MAX_INDEX = 2;

void updateSprite(Sprite* s);

int main() {
    LNConfig_SetGraphicsBackend(LN_GRAPHICS_BACKEND_VULKAN);
    //LNConfig_SetGraphicsBackend(LN_GRAPHICS_BACKEND_WEBGPU);

    LNResult result = LNInstance_Initialize();
    if (result != LN_OK) {
        return 1;
    }

    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;
    LNHandle viewPoint = LN_NULL_HANDLE;
    LNWindow_Create(640, 480, "Example", &window);
    LNWindow_GetGraphicsContext(window, &graphicsContext);
    LNViewPoint_Create(&viewPoint);

    LNHandle texture1 = LN_NULL_HANDLE;
    if (LNTexture2D_CreateFromImageFile(ASSETS_DIR "/IconSet001.png", &texture1) != LN_OK) {
        return 1;
    }
    int textureWidth = 384;
    int textureHeight = 384;

    LNHandle material1 = LN_NULL_HANDLE;
    LNMaterial_Create(&material1);
    LNMaterial_SetMainTexture(material1, texture1);

    LNHandle spriteRenderer = LN_NULL_HANDLE;
    LNBatchRenderer_Get(&spriteRenderer);

    std::vector<Sprite> sprites;
    for (int i = 0; i < SPRITE_COUNT; ++i) {
        Sprite sprite;
        sprite.x = rand() % 640;
        sprite.y = rand() % 480;
        sprite.vx = (rand() % 100 - 50) / 100.0f;
        sprite.vy = (rand() % 100 - 50) / 100.0f;
        sprite.angle = (rand() % 360) * (3.14159f / 180.0f);
        sprite.iconIndex = rand() % (ICON_MAX_INDEX + 1);
        sprites.push_back(sprite);
    }

    LNGraphicsProfilerng profilerng = {};
    int frameCount = 0;
    while (true) {
        LNBool quit = LN_FALSE;
        LNInstance_ProcessEvents();
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }

        int width = 0;
        int height = 0;
        LNHandle colorBuffer = LN_NULL_HANDLE;
        LNHandle depthBuffer = LN_NULL_HANDLE;
        LNHandle commandList = LN_NULL_HANDLE;
        LNWindow_GetFramebufferSize(window, &width, &height);
        LNGraphicsContext_BeginFrame(graphicsContext, width, height, &colorBuffer, &depthBuffer, &commandList);

        LNViewPoint_SetupPerspective2DLH(viewPoint, 0, 0, 0, width, height, -500, 500);
        //LNViewPoint_SetupPerspectiveOrthoLH(viewPoint, 0, 0, 100, 0, 0, 0, width, height, -1000, 1000);

        LNHandle renderingPass = LN_NULL_HANDLE;
        LNRenderPassDescriptor descriptor;
        descriptor.renderTargets[0].renderTarget = colorBuffer;
        descriptor.renderTargets[0].clearColor[0] = 0.60f;
        descriptor.renderTargets[0].clearColor[1] = 0.85f;
        descriptor.renderTargets[0].clearColor[2] = 0.60f;
        descriptor.renderTargets[0].clearColor[3] = 1.0f;
        descriptor.renderTargets[0].clearEnable = LN_TRUE;
        descriptor.depthBuffer.depthBuffer = depthBuffer;
        descriptor.depthBuffer.clearDepth = 1.0f;
        descriptor.depthBuffer.clearStencil = 0;
        descriptor.depthBuffer.clearDepthEnable = LN_TRUE;
        descriptor.depthBuffer.clearStencilEnable = LN_TRUE;
        LNCommandList_BeginRenderPass(commandList, descriptor, viewPoint, &renderingPass);


        LNBatchRenderer_BeginBatch(spriteRenderer, commandList, material1, nullptr);
        for (int i = 0; i < SPRITE_COUNT; ++i) {
            Sprite& sprite = sprites[i];
            updateSprite(&sprite);
            LNMatrix transform;
            LNMatrix_SetIdentity(&transform);
            transform.m41 = sprite.x;
            transform.m42 = sprite.y;
            transform.m43 = 0.0f;
            transform.m11 = cosf(sprite.angle);
            transform.m12 = -sinf(sprite.angle);
            transform.m21 = sinf(sprite.angle);
            transform.m22 = cosf(sprite.angle);

            float frameX = static_cast<float>(sprite.iconIndex % 16) * ICON_SIZE / textureWidth;
            float frameY = static_cast<float>(sprite.iconIndex / 16) * ICON_SIZE / textureHeight;
            float frameW = ICON_SIZE / textureWidth;
            float frameH = ICON_SIZE / textureHeight;

            LNBatchRenderer_DrawSprite_deprecated(
                spriteRenderer,
                &transform,
                ICON_SIZE,
                ICON_SIZE,
                0.5f,
                0.5f,
                frameX,
                frameY,
                frameW,
                frameH,
                1,
                1,
                1,
                1,
                LN_SPRITE_BASE_DIRECTION_BASIC2D,
                LN_BILLBOARD_TYPE_NONE);
        }
        LNBatchRenderer_EndBatch(spriteRenderer);

        
        //LNDebug_Print(graphicsContext, (std::string("Frame: ") + std::to_string(frameCount)).c_str());
        LNDebug_Print(
            graphicsContext,
            (std::string("FPS: ") + std::to_string(profilerng.actualFPS)).c_str());
        LNDebug_Print(
            graphicsContext,
            (std::string("FrameTime: ") + std::to_string(profilerng.lastFrameTime)).c_str());
        LNDebug_Print(
            graphicsContext,
            (std::string("DrawCall: ") + std::to_string(profilerng.drawCallCount)).c_str());


        LNCommandList_EndRenderPass(commandList, renderingPass);

        

        LNGraphicsContext_EndFrame(graphicsContext);
        LNDebug_GetGraphicsProfilerng(commandList, &profilerng);
        LNWindow_Present(window);



        frameCount++;
    };

    LNObject_Release(material1);
    LNObject_Release(texture1);
    LNObject_Release(viewPoint);
    LNObject_Release(window);
    LNInstance_Terminate();
    return 0;
}

void updateSprite(Sprite* s) {
    s->x += s->vx;
    s->y += s->vy;

    if (s->x < 0) {
        s->x = -s->x;
        s->vx = -s->vx;
    }
    else if (s->x > SCREEN_WIDTH) {
        s->x = 2 * SCREEN_WIDTH - s->x;
        s->vx = -s->vx;
    }
    if (s->y < 0) {
        s->y = -s->y;
        s->vy = -s->vy;
    }
    else if (s->y > SCREEN_HEIGHT) {
        s->y = 2 * SCREEN_HEIGHT - s->y;
        s->vy = -s->vy;
    }
}
