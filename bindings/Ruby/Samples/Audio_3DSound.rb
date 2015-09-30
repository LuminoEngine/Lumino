#==============================================================================
# 3D サウンドのサンプルです。
#==============================================================================
require './Lumino'
include Lumino

# 音声機能を初期化する
Application.initialize_audio

# 音声ファイルから Sound オブジェクトを作る
sound = Sound.new("Media/water02.wav")
sound.is_3d_enabled = true                      # 3D 音声として再生する
sound.emitter_position = Vector3.new(12.0, 0, 0)# 発声位置
sound.emitter_max_distance = 10.0               # この距離まで聞こえる
sound.is_loop_enabled = true                    # ループON

# リスナーの位置・向き・速度を指定する
SoundListener.position = Vector3.new(5, 0, 0)
SoundListener.direction = Vector3.new(0, 0, 1)
SoundListener.velocity = Vector3.new(2, 0, 0)

# 再生
sound.play

# Enter 待ち
gets

# 停止
sound.stop

# ライブラリの終了処理
Application.terminate

