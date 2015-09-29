#==============================================================================
# 音声ファイルをループ再生します。
#==============================================================================
require 'Lumino'
include Lumino

# (初期設定) Midi の再生に DirectMusic を使用する
Config.set_direct_music_initialize_mode(DirectMusicMode::NORMAL)

# 音声機能を初期化する
Application.initialize_audio

# 音声ファイルから Sound オブジェクトを作る
sound = Sound.new("Media/ln21.wav")
sound.volume = 1.0				# 音量
sound.pitch = 1.0				# ピッチ
sound.is_loop_enabled = true	# ループON

# 再生
sound.play

# Enter 待ち
gets

# 停止
sound.stop

# ライブラリの終了処理
Application.terminate

