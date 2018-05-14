// Copyright (c) 2018 lriki. Distributed under the MIT license.
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
    int getInt();

    /**
     * 指定した最大値より小さい整数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい整数値
     */
    int getInt(int maxValue);

    /**
     * 指定した範囲内の整数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい整数値
     */
    int getIntRange(int minValue, int maxValue);

    /**
     * 指定した振れ幅の整数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい整数値
     */
    int getIntWidth(int median, int width);

    /**
     * 実数ランダム値を返します。
     * @return  0.0 以上 1.0 未満の実数値
     */
    float floatValue();

    /**
     * 指定した最大値より小さい実数ランダム値を返します。(0 <= 値 < maxValue)
     * @param[in]   maxValue    : 上限値
     * @return      0 以上で maxValue より小さい実数値
     */
    float floatValue(float maxValue);

    /**
     * 指定した範囲内の実数ランダム値を返します。(minValue <= 値 < maxValue)
     * @param[in]   minValue    : 下限値
     * @param[in]   maxValue    : 上限値
     * @return      minValue 以上で maxValue より小さい実数値
     */
    float getFloatRange(float minValue, float maxValue);

    /**
     * 指定した振れ幅の実数ランダム値を返します。(median-width <= 値 < median+width)
     * @param[in]   median      : 中央値
     * @param[in]   width       : ±幅
     * @return      median-width 以上で median+width より小さい実数値
     */
    float getFloatWidth(float median, float width);

private:
    unsigned int x, y, z, w;
};

} // namespace ln
