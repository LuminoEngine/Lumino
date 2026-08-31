#pragma once

#include <gtest/gtest.h>

/**
 * コンパイル済みシェーダの読み込みテストで使う .lcsh。
 *
 * ビルド時に luminosc が生成するものを直接指す。テスト用にコピーを置いておくと、
 * シェーダバイナリのフォーマットが変わったときに古いまま取り残される。
 */
#define LN_TEST_UNLIT_LCSH LN_REPO_ROOT_DIR "/packages/LuminoCore/shaders/Unlit.lcsh"
