#==============================================================================
# ゲーム用の音声再生機能のサンプルです。
#==============================================================================
require 'Lumino'
include Lumino

# (初期設定) Midi の再生に DirectMusic を使用する
Config.set_direct_music_initialize_mode(DirectMusicMode::NORMAL)

# 音声機能を初期化する
Application.initialize_audio

# BGM を再生する
GameAudio.play_bgm("Media/ln20.wav");

gets	# Enter 待ち

# BGS を再生する
GameAudio.play_bgs(Media/water01.wav);

gets	# Enter 待ち

# ME を再生する
GameAudio.play_me(Media/me.wav);

gets	# Enter 待ち

# SE を再生する
GameAudio.play_se(Media/se.wav);

# BGM を停止する (5秒かけてフェードアウト)
GameAudio.stop_bgm("Media/ln20.wav", 5.0);

# BGS を停止する
GameAudio.stop_bgs(Media/water01.wav);

gets	# Enter 待ち

# ライブラリの終了処理
Application.terminate

