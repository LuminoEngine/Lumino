
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief	パーリンノイズ(Perlin noise) を生成するためのクラスです。
*/
class PerlinNoise
{
public:

	/**
		@brief		現在の時間に応じて決定されるシード値を使用してインスタンスを初期化します。
	*/
	PerlinNoise();

	/**
		@brief		指定したシード値を使用してインスタンスを初期化します。
		@param[in]	seed		: シード値
	*/
	PerlinNoise(int seed);
	
	/**
		@brief		シード値を変更します。
		@param[in]	seed		: シード値
	*/
	void SetSeed(int seed);
	
	/**
		@brief		タイル状にノイズを並べて表示するための繰り返し間隔を設定します。
		@param[in]	repeat		: サンプリングの周波数
	*/
	void SetTiledRepeatFrequency(float repeat);

	/**
		@brief		1D ノイズ値を生成します。
		@param[in]	x			: サンプリング位置の X 座標
		@param[in]	octaves		: オクターブの合成レベル
		@param[in]	minValue	: 下限値
		@param[in]	maxValue	: 上限値
		@return		minValue 以上 maxValue 以下の値
	*/
	float Noise1D(float x, int octaves = 1, float minValue = 0.0f, float maxValue = 1.0f) const;

	/**
		@brief		2D ノイズ値を生成します。
		@param[in]	x			: サンプリング位置の X 座標
		@param[in]	y			: サンプリング位置の Y 座標
		@param[in]	octaves		: オクターブの合成レベル
		@param[in]	minValue	: 下限値
		@param[in]	maxValue	: 上限値
		@return		minValue 以上 maxValue 以下の値
	*/
	float Noise2D(float x, float y, int octaves = 1, float minValue = 0.0f, float maxValue = 1.0f) const;

	/**
		@brief		3D ノイズ値を生成します。
		@param[in]	x			: サンプリング位置の X 座標
		@param[in]	y			: サンプリング位置の Y 座標
		@param[in]	z			: サンプリング位置の Z 座標
		@param[in]	octaves		: オクターブの合成レベル
		@param[in]	minValue	: 下限値
		@param[in]	maxValue	: 上限値
		@return		minValue 以上 maxValue 以下の値
	*/
	float Noise3D(float x, float y, float z, int octaves = 1, float minValue = 0.0f, float maxValue = 1.0f) const;

private:
	float NoiseNormal(float x, float y, float z) const;
	float OctaveNoiseNormal(float x, float y, float z, int octaves) const;
	float Fade(float t) const;
	float lerp(float a, float b, float t) const;
	float Grad(int hash, float x, float y, float z) const;
	int Increment(int num) const;

	int p[512];
	int m_repeat;
};

LN_NAMESPACE_END

