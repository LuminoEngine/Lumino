// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include "Common.hpp"

namespace ln {

/**
 * 疑似乱数を生成するのクラスです。
 * 
 * アルゴリズムは Xorshift 法です。
 */
class LN_API Random
{
public:
    /**
     * 現在の時間に応じて決定されるシード値を使用してインスタンスを初期化します。
     */
    Random();

    /**
     * 指定したシード値を使用してインスタンスを初期化します。
     * @param[in]   seed    : シード値
     */
    Random(int seed);

public:
    /**
     * シード値を変更します。疑似乱数生成器が初期状態にリセットされます。
     * @param[in]   seed    : シード値
     */
    void setSeed(int seed);

    /**
     * 整数ランダム値を返します。
     * @return  0 以上の整数値
     */
    int nextInt();

    /**
     * 指定した最大値より小さい整数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい整数値
     */
    int nextInt(int maxValue);

    /**
     * 指定した範囲内の整数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい整数値
     */
    int nextInt(int minValue, int maxValue);

    /**
     * 指定した振れ幅の整数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい整数値
     */
    int nextIntWidth(int median, int width);

    /**
     * 実数ランダム値を返します。
     * @return  0.0 以上 1.0 未満の実数値
     */
    float nextFloat();

    /**
     * 指定した最大値より小さい実数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい実数値
     */
    float nextFloat(float maxValue);

    /**
     * 指定した範囲内の実数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい実数値
     */
    float nextFloat(float minValue, float maxValue);

    /**
     * 指定した振れ幅の実数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい実数値
     */
    float nextFloatWidth(float median, float width);

public:
    /**
     * シード値を変更します。疑似乱数生成器が初期状態にリセットされます。
     * @param[in]   seed    : シード値
     */
    static void randSeed(int seed);

    /**
     * 整数ランダム値を返します。
     * @return  0 以上の整数値
     */
    static int randInt();

    /**
     * 指定した最大値より小さい整数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい整数値
     */
    static int randInt(int maxValue);

    /**
     * 指定した範囲内の整数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい整数値
     */
    static int randInt(int minValue, int maxValue);

    /**
     * 指定した振れ幅の整数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい整数値
     */
    static int randIntWidth(int median, int width);

    /**
     * 実数ランダム値を返します。
     * @return  0.0 以上 1.0 未満の実数値
     */
    static float randFloat();

    /**
     * 指定した最大値より小さい実数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい実数値
     */
    static float randFloat(float maxValue);

    /**
     * 指定した範囲内の実数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい実数値
     */
    static float randFloat(float minValue, float maxValue);

    /**
     * 指定した振れ幅の実数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい実数値
     */
    static float randFloatWidth(float median, float width);

private:
    unsigned int x, y, z, w;
    static Random s_generator;
};

inline void Random::randSeed(int seed) { s_generator.setSeed(seed); }
inline int Random::randInt() { return s_generator.nextInt(); }
inline int Random::randInt(int maxValue) { return s_generator.nextInt(maxValue); }
inline int Random::randInt(int minValue, int maxValue) { return s_generator.nextInt(minValue, maxValue); }
inline int Random::randIntWidth(int median, int width) { return s_generator.nextIntWidth(median, width); }
inline float Random::randFloat() { return s_generator.nextFloat(); }
inline float Random::randFloat(float maxValue) { return s_generator.nextFloat(maxValue); }
inline float Random::randFloat(float minValue, float maxValue) { return s_generator.nextFloat(minValue, maxValue); }
inline float Random::randFloatWidth(float median, float width) { return s_generator.nextFloatWidth(median, width); }

} // namespace ln
