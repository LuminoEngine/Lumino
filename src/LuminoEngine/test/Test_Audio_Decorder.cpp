#include "Common.hpp"
#include "../src/Audio/Decoder/WaveAudioDecoder.hpp"
#include "../src/Audio/AudioManager.hpp"

//==============================================================================
class Test_Audio_Decorder : public LuminoSceneTest {};

static void saveSamples(const String& filePath, const std::vector<float>& samples)
{
	StreamWriter sw(filePath);
	for (auto& v : samples) {
		sw.writeLine(v);
	}
}

TEST_F(Test_Audio_Decorder, WaveDecoder)
{
	// 最終判定用のデータではなくて、baseSamples がなんとなく sin になってるかを判断するための基準
    const float sinTable[] = {
        0,
        0.098392,
        0.199408,
        0.293222,
        0.385174,
        0.468795,
        0.547166,
        0.614917,
        0.674947,
        0.722648,
        0.760460,
        0.785150,
        0.798456,
        0.798425,
        0.786645,
        0.761742,
        0.725608,
        0.677419,
        0.619129,
        0.550707,
        0.474013,
        0.389538,
        0.299203,
        0.204047,
        0.105625,
        0.005799,
        -0.094485,
        -0.192907,
        -0.288644,
        -0.379528,
        -0.464766,
        -0.542405,
        -0.611835,
        -0.671346,
        -0.720603,
        -0.758324,
        -0.784326,
        -0.797815,
        -0.799005,
        -0.787408,
        -0.763695,
        -0.727775,
        -0.680563,
        -0.622669,
        -0.554888,
        -0.478530,
        -0.394543,
        -0.304483,
        -0.209479,
        -0.111423,
    };

    auto diag = makeObject<DiagnosticsManager>();
    auto decorder = makeRef<detail::WaveDecoder>();
    auto stream = FileStream::create(LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_2ch.wav"));
    decorder->init(stream, diag);

    auto count = decorder->audioDataInfo().totalFrameCount();
    std::vector<float> baseSamples(count);
    for (int i = 0; i < count; i++)
    {
        float samples[2];
        ASSERT_EQ(1, decorder->read(i, samples, 1));

        baseSamples[i] = samples[0];
    }
	//saveSamples(u"tmp1.txt", baseSamples);

    // 何となく先頭 50 サンプルくらいがちゃんと sin となっていることを確認
    for (int i = 0; i < LN_ARRAY_SIZE_OF(sinTable); i++) {
        ASSERT_EQ(true, sinTable[i] - 0.1 < baseSamples[i] && baseSamples[i] < sinTable[i] + 0.1);
    }

	struct TP {
		int index;
		float value;
	};
	struct {
		const String path;
		TP tp[5];
	} files[] = {
		// 各ファイルに対して、サンプルするするポイント(5点) と、その地点の値を定義する
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_1ch.wav"), { { 0, 0.0 }, { 13, 0.8 }, { 25, 0.0 }, { 38, -0.8 }, { 50, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_2ch.wav"), { { 0, 0.0 }, { 13, 0.8 }, { 25, 0.0 }, { 38, -0.8 }, { 50, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_1ch.wav"), { { 0, 0.0 }, { 18, 0.8 }, { 36, 0.0 }, { 55, -0.8 }, { 73, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_2ch.wav"), { { 0, 0.0 }, { 18, 0.8 }, { 36, 0.0 }, { 55, -0.8 }, { 73, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_1ch.wav"), { { 0, 0.0 }, { 25, 0.8 }, { 50, 0.0 }, { 75, -0.8 }, { 100, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_2ch.wav"), { { 0, 0.0 }, { 25, 0.8 }, { 50, 0.0 }, { 75, -0.8 }, { 100, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_1ch.wav"), { { 0, 0.0 }, { 27, 0.8 }, { 55, 0.0 }, { 82, -0.8 }, { 109, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_2ch.wav"), { { 0, 0.0 }, { 27, 0.8 }, { 55, 0.0 }, { 82, -0.8 }, { 109, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_1ch.wav"), { { 0, 0.0 }, { 50, 0.8 }, { 100, 0.0 }, { 150, -0.8 }, { 200, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_2ch.wav"), { { 0, 0.0 }, { 50, 0.8 }, { 100, 0.0 }, { 150, -0.8 }, { 200, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_1ch.wav"), { { 0, 0.0 }, { 55, 0.8 }, { 109, 0.0 }, { 165, -0.8 }, { 218, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_2ch.wav"), { { 0, 0.0 }, { 55, 0.8 }, { 109, 0.0 }, { 165, -0.8 }, { 218, 0.0 } } },
    };

    const float thr = 0.2;
    for (int iFile = 0; iFile < LN_ARRAY_SIZE_OF(files); iFile++) {
        auto decorder2 = makeRef<detail::WaveDecoder>();
        ASSERT_EQ(true, decorder2->init(FileStream::create(files[iFile].path), diag));
        auto count = decorder2->audioDataInfo().totalFrameCount();
        std::vector<float> allSamples[6];
        for (int i = 0; i < count; i++) {
            float samples[6];
            ASSERT_EQ(1, decorder2->read(i, samples, 1));
            for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
                allSamples[iCh].push_back(samples[iCh]);
            }
        }
		////saveSamples(u"tmp2.txt", allSamples[0]);


		for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
			for (int iTp = 0; iTp < 5; iTp++) {
				float a = files[iFile].tp[iTp].value;
				float b = allSamples[iCh][files[iFile].tp[iTp].index];
				float diff = abs(a - b);
				ASSERT_EQ(true, diff < thr);
			}
		}


  //      for (int i = 0; i < count; i++) {
  //          for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
		//		float a = baseSamples[i];
		//		float b = allSamples[iCh][i];
		//		float diff = abs(a - b);
  //              ASSERT_EQ(true, baseSamples[i] - thr < allSamples[iCh][i] && allSamples[iCh][i] < baseSamples[i] + thr);
  //          }
  //      }
    }


}


TEST_F(Test_Audio_Decorder, OggDecoder)
{
	struct TP {
		int index;
		float value;
	};
	struct {
		const String path;
		TP tp[5];
	} files[] = {
		// 各ファイルに対して、サンプルするするポイント(5点) と、その地点の値を定義する
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_1ch.ogg"), { { 0, 0.0 }, { 13, 0.8 }, { 25, 0.0 }, { 38, -0.8 }, { 50, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_2ch.ogg"), { { 0, 0.0 }, { 13, 0.8 }, { 25, 0.0 }, { 38, -0.8 }, { 50, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_1ch.ogg"), { { 0, 0.0 }, { 18, 0.8 }, { 36, 0.0 }, { 55, -0.8 }, { 73, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_2ch.ogg"), { { 0, 0.0 }, { 18, 0.8 }, { 36, 0.0 }, { 55, -0.8 }, { 73, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_1ch.ogg"), { { 0, 0.0 }, { 25, 0.8 }, { 50, 0.0 }, { 75, -0.8 }, { 100, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_2ch.ogg"), { { 0, 0.0 }, { 25, 0.8 }, { 50, 0.0 }, { 75, -0.8 }, { 100, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_1ch.ogg"), { { 0, 0.0 }, { 27, 0.8 }, { 55, 0.0 }, { 82, -0.8 }, { 109, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_2ch.ogg"), { { 0, 0.0 }, { 27, 0.8 }, { 55, 0.0 }, { 82, -0.8 }, { 109, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_1ch.ogg"), { { 0, 0.0 }, { 50, 0.8 }, { 100, 0.0 }, { 150, -0.8 }, { 200, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_2ch.ogg"), { { 0, 0.0 }, { 50, 0.8 }, { 100, 0.0 }, { 150, -0.8 }, { 200, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_1ch.ogg"), { { 0, 0.0 }, { 55, 0.8 }, { 109, 0.0 }, { 165, -0.8 }, { 218, 0.0 } } },
		{ LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_2ch.ogg"), { { 0, 0.0 }, { 55, 0.8 }, { 109, 0.0 }, { 165, -0.8 }, { 218, 0.0 } } },
	};

	const float thr = 0.2;
	for (int iFile = 0; iFile < LN_ARRAY_SIZE_OF(files); iFile++) {
		auto decorder2 = detail::EngineDomain::audioManager()->createAudioDecoder(files[iFile].path);
		auto count = 500;	// 全部読むとものすごく時間がかかるので、テストで必要な分だけにする
		std::vector<float> allSamples[6];
		for (int i = 0; i < count; i++) {
			float samples[6];
			ASSERT_EQ(1, decorder2->read(i, samples, 1));
			for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
				allSamples[iCh].push_back(samples[iCh]);
			}
		}

		for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
			for (int iTp = 0; iTp < 5; iTp++) {
				float a = files[iFile].tp[iTp].value;
				float b = allSamples[iCh][files[iFile].tp[iTp].index];
				float diff = abs(a - b);
				ASSERT_EQ(true, diff < thr);
			}
		}

		detail::EngineDomain::audioManager()->releaseAudioDecoder(decorder2);
	}
}

