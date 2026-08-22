#pragma once

/**
 * @file VulkanLoader.hpp
 * Vulkan ローダーの動的ロード。
 *
 * Lumino は vulkan-1.lib / libvulkan.so をリンクせず、volk を通じて実行時に
 * ローダーを dlopen します。これには次の利点があります。
 *
 * - Vulkan が使えない環境でもプロセスが起動できる。インポートライブラリを
 *   直接リンクすると、ローダー DLL が無い環境ではプログラムの起動そのものが
 *   失敗し、「Vulkan 非対応」を通知することすらできません。
 * - ビルドに Vulkan SDK が不要になり、vulkan-headers だけで完結します。
 * - デバイスレベルの関数を vkGetDeviceProcAddr で直接取得できるため、
 *   ローダーのディスパッチを 1 段省けます。
 *
 * 注意: volk が宣言する関数ポインタの集合は VK_USE_PLATFORM_* の定義有無で
 * 変化します。Lumino ではサーフェス生成を GLFW に任せておりこれらを定義しない
 * ため、volk.c も同じ条件でビルドする必要があります
 * (CMakeLists.txt の volk::volk_headers を参照)。
 */

#include <volk.h>

namespace ln::rhi::vulkan {

/**
 * Vulkan ローダー (Windows: vulkan-1.dll, Linux: libvulkan.so.1) をロードし、
 * インスタンス生成前に必要なグローバル関数を取得する。
 *
 * 何度呼び出しても安全で、2 回目以降は最初の結果をそのまま返す。
 * ローダーが見つからない環境では false を返すだけでプロセスは継続できる。
 */
bool loadVulkanLoader();

} // namespace ln::rhi::vulkan
