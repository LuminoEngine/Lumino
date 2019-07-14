#include "Common.hpp"
#include "../src/Audio/AudioDecoder.hpp"

//==============================================================================
//# AudioDecorder のテスト
class Test_Audio_Decorder : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## 
TEST_F(Test_Audio_Decorder, WaveDecoder)
{
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
        ASSERT_EQ(1, decorder->read2(samples, 1));

        baseSamples[i] = samples[0];
    }

    // 何となく先頭 50 サンプルくらいがちゃんと sin となっていることを確認
    for (int i = 0; i < LN_ARRAY_SIZE_OF(sinTable); i++) {
        ASSERT_EQ(true, sinTable[i] - 0.1 < baseSamples[i] && baseSamples[i] < sinTable[i] + 0.1);
    }

    const String files[] = {
        LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_22050_2ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_32000_2ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_44100_2ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_48000_2ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_88200_2ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_1ch.wav"),
        LN_ASSETFILE("Audio/sin_440_3s_S16L_96000_2ch.wav"),
    };

    const float thr = 0.2;
    for (int iFile = 0; iFile < LN_ARRAY_SIZE_OF(files); iFile++) {
        auto stream2 = FileStream::create(files[iFile]);
        auto decorder2 = makeRef<detail::WaveDecoder>();
        ASSERT_EQ(true, decorder2->init(stream2, diag));
        auto count = decorder2->audioDataInfo().totalFrameCount();
        std::vector<float> allSamples[6];
        for (int i = 0; i < count; i++) {
            float samples[6];
            ASSERT_EQ(1, decorder2->read2(samples, 1));
            for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
                allSamples[iCh].push_back(samples[iCh]);
            }
        }

        for (int i = 0; i < count; i++) {
            for (int iCh = 0; iCh < decorder2->audioDataInfo().channelCount; iCh++) {
                ASSERT_EQ(true, baseSamples[i] - thr < allSamples[iCh][i] && allSamples[iCh][i] < baseSamples[i] + thr);
            }
        }
    }
}

