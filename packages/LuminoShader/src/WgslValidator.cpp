// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <LuminoShader/WgslValidator.hpp>

#ifdef LUMINO_SHADER_VALIDATE_WGSL
#include <cstring>
#include <webgpu/webgpu.h>
#endif

namespace ln {
namespace shader {

#ifdef LUMINO_SHADER_VALIDATE_WGSL

namespace {

// Dawn の非同期 API は WGPUCallbackMode_AllowSpontaneous であればネイティブ環境では
// 呼び出し中に同期的に発火する。ただし将来の実装変更に備えて明示的にポンプする。
static const int kMaxPumpCount = 1000;

std::string toStdString(WGPUStringView view) {
    if (!view.data) return std::string();
    size_t length = (view.length == WGPU_STRLEN) ? strlen(view.data) : view.length;
    return std::string(view.data, length);
}

} // anonymous namespace

//------------------------------------------------------------------------------
// WgslValidator::Impl

struct WgslValidator::Impl {
    WGPUInstance instance = nullptr;
    WGPUAdapter adapter = nullptr;
    WGPUDevice device = nullptr;

    ~Impl() {
        if (device) {
            wgpuDeviceDestroy(device);
            wgpuDeviceRelease(device);
        }
        if (adapter) wgpuAdapterRelease(adapter);
        if (instance) wgpuInstanceRelease(instance);
    }

    void pump() {
        if (instance) wgpuInstanceProcessEvents(instance);
    }
};

//------------------------------------------------------------------------------
// WgslValidator

bool WgslValidator::available() {
    return true;
}

Result<std::unique_ptr<WgslValidator>> WgslValidator::create() {
    std::unique_ptr<WgslValidator> ref(new WgslValidator());
    auto result = ref->init();
    if (!result) return tl::make_unexpected(result.error());
    return ref;
}

WgslValidator::WgslValidator()
    : m_impl(std::make_unique<Impl>()) {
}

WgslValidator::~WgslValidator() = default;

VoidResult WgslValidator::init() {
    // Instance
    {
        WGPUInstanceDescriptor instDesc = WGPU_INSTANCE_DESCRIPTOR_INIT;
        m_impl->instance = wgpuCreateInstance(&instDesc);
        if (!m_impl->instance) {
            return LNSHADER_MAKE_ERROR("WgslValidator: wgpuCreateInstance failed.");
        }
    }

    // Adapter (Null バックエンド)
    // GPU を必要としないため、CI やリモートデスクトップ環境でも動作する。
    {
        struct AdapterRequest {
            WGPURequestAdapterStatus status = WGPURequestAdapterStatus_Error;
            WGPUAdapter adapter = nullptr;
            bool done = false;
            std::string message;
        } request;

        WGPURequestAdapterOptions options = WGPU_REQUEST_ADAPTER_OPTIONS_INIT;
        options.backendType = WGPUBackendType_Null;

        WGPURequestAdapterCallbackInfo callbackInfo = WGPU_REQUEST_ADAPTER_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.userdata1 = &request;
        callbackInfo.callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter,
                                   WGPUStringView message, void* userdata1, void*) {
            auto* r = static_cast<AdapterRequest*>(userdata1);
            r->status = status;
            r->adapter = adapter;
            r->done = true;
            r->message = toStdString(message);
        };

        wgpuInstanceRequestAdapter(m_impl->instance, &options, callbackInfo);
        for (int i = 0; i < kMaxPumpCount && !request.done; i++) {
            m_impl->pump();
        }
        if (!request.adapter) {
            return LNSHADER_MAKE_ERROR(
                "WgslValidator: wgpuInstanceRequestAdapter (Null backend) failed. " + request.message);
        }
        m_impl->adapter = request.adapter;
    }

    // Device
    {
        struct DeviceRequest {
            WGPURequestDeviceStatus status = WGPURequestDeviceStatus_Error;
            WGPUDevice device = nullptr;
            bool done = false;
            std::string message;
        } request;

        WGPUDeviceDescriptor deviceDesc = WGPU_DEVICE_DESCRIPTOR_INIT;
        deviceDesc.label = { "LuminoWgslValidator", WGPU_STRLEN };
        // 検証結果はエラースコープで受け取るため、キャプチャされなかったエラーは無視する。
        deviceDesc.uncapturedErrorCallbackInfo.callback =
            [](WGPUDevice const*, WGPUErrorType, WGPUStringView, void*, void*) {};

        WGPURequestDeviceCallbackInfo callbackInfo = WGPU_REQUEST_DEVICE_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.userdata1 = &request;
        callbackInfo.callback = [](WGPURequestDeviceStatus status, WGPUDevice device,
                                   WGPUStringView message, void* userdata1, void*) {
            auto* r = static_cast<DeviceRequest*>(userdata1);
            r->status = status;
            r->device = device;
            r->done = true;
            r->message = toStdString(message);
        };

        wgpuAdapterRequestDevice(m_impl->adapter, &deviceDesc, callbackInfo);
        for (int i = 0; i < kMaxPumpCount && !request.done; i++) {
            m_impl->pump();
        }
        if (!request.device) {
            return LNSHADER_MAKE_ERROR(
                "WgslValidator: wgpuAdapterRequestDevice failed. " + request.message);
        }
        m_impl->device = request.device;
    }

    return LNSHADER_OK();
}

Result<WgslValidationReport> WgslValidator::validate(
    const char* wgsl, size_t length, const std::string& label) {
    if (!m_impl->device) {
        return LNSHADER_MAKE_ERROR("WgslValidator: not initialized.");
    }

    WgslValidationReport report;

    wgpuDevicePushErrorScope(m_impl->device, WGPUErrorFilter_Validation);

    WGPUShaderSourceWGSL wgslSource = {};
    wgslSource.chain.next = nullptr;
    wgslSource.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgslSource.code.data = wgsl;
    wgslSource.code.length = length;

    WGPUShaderModuleDescriptor moduleDesc = {};
    moduleDesc.nextInChain = &wgslSource.chain;
    moduleDesc.label = { label.c_str(), label.size() };

    WGPUShaderModule module = wgpuDeviceCreateShaderModule(m_impl->device, &moduleDesc);

    // エラースコープを閉じて検証エラーを取り出す。
    {
        struct PopRequest {
            bool done = false;
            WGPUErrorType type = WGPUErrorType_NoError;
            std::string message;
        } request;

        WGPUPopErrorScopeCallbackInfo callbackInfo = WGPU_POP_ERROR_SCOPE_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.userdata1 = &request;
        callbackInfo.callback = [](WGPUPopErrorScopeStatus, WGPUErrorType type,
                                   WGPUStringView message, void* userdata1, void*) {
            auto* r = static_cast<PopRequest*>(userdata1);
            r->done = true;
            r->type = type;
            r->message = toStdString(message);
        };

        wgpuDevicePopErrorScope(m_impl->device, callbackInfo);
        for (int i = 0; i < kMaxPumpCount && !request.done; i++) {
            m_impl->pump();
        }

        if (request.type != WGPUErrorType_NoError) {
            report.failed = true;
            report.detail = request.message;
        }
    }

    // 行番号付きの診断を取り出す。エラーだけでなく警告も取得できる。
    if (module) {
        struct InfoRequest {
            bool done = false;
            std::vector<WgslDiagnostic> diagnostics;
        } request;

        WGPUCompilationInfoCallbackInfo callbackInfo = WGPU_COMPILATION_INFO_CALLBACK_INFO_INIT;
        callbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;
        callbackInfo.userdata1 = &request;
        callbackInfo.callback = [](WGPUCompilationInfoRequestStatus,
                                   WGPUCompilationInfo const* info, void* userdata1, void*) {
            auto* r = static_cast<InfoRequest*>(userdata1);
            r->done = true;
            if (!info) return;
            for (size_t i = 0; i < info->messageCount; i++) {
                const WGPUCompilationMessage& m = info->messages[i];
                WgslDiagnostic d;
                switch (m.type) {
                    case WGPUCompilationMessageType_Error:
                        d.severity = WgslDiagnostic::Severity::Error;
                        break;
                    case WGPUCompilationMessageType_Warning:
                        d.severity = WgslDiagnostic::Severity::Warning;
                        break;
                    default:
                        d.severity = WgslDiagnostic::Severity::Info;
                        break;
                }
                d.line = m.lineNum;
                d.column = m.linePos;
                d.message = toStdString(m.message);
                r->diagnostics.push_back(std::move(d));
            }
        };

        wgpuShaderModuleGetCompilationInfo(module, callbackInfo);
        for (int i = 0; i < kMaxPumpCount && !request.done; i++) {
            m_impl->pump();
        }
        report.diagnostics = std::move(request.diagnostics);

        wgpuShaderModuleRelease(module);
    }

    for (const auto& d : report.diagnostics) {
        if (d.severity == WgslDiagnostic::Severity::Error) {
            report.failed = true;
        }
    }

    return report;
}

#else // LUMINO_SHADER_VALIDATE_WGSL

struct WgslValidator::Impl {};

bool WgslValidator::available() {
    return false;
}

Result<std::unique_ptr<WgslValidator>> WgslValidator::create() {
    return LNSHADER_MAKE_ERROR(
        "WgslValidator is not available in this build (LUMINO_SHADER_VALIDATE_WGSL is off).");
}

WgslValidator::WgslValidator() = default;
WgslValidator::~WgslValidator() = default;

VoidResult WgslValidator::init() {
    return LNSHADER_OK();
}

Result<WgslValidationReport> WgslValidator::validate(const char*, size_t, const std::string&) {
    return LNSHADER_MAKE_ERROR("WgslValidator is not available in this build.");
}

#endif // LUMINO_SHADER_VALIDATE_WGSL

} // namespace shader
} // namespace ln
