#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
#==============================================================================
# ゲーム用の音声再生機能のサンプルです。
#==============================================================================
require './Lumino'
include Lumino

# 音声機能を初期化する
Engine.initialize_audio

p "Enter を押すと BGM を再生します..."
gets	# Enter 待ち

# BGM を再生する
GameAudio.play_bgm("Media/ln23.ogg");

p "Enter を押すと BGS を再生します..."
gets	# Enter 待ち

# BGS を再生する
GameAudio.play_bgs("Media/water02.wav", 0.5);

p "Enter を押すと ME を再生します..."
gets	# Enter 待ち

# ME を再生する
GameAudio.play_me("Media/lnme_victory1.ogg");

p "Enter を押すと SE を再生します..."
gets	# Enter 待ち

# SE を再生する
GameAudio.play_se("Media/ln_cursor_1.wav");

p "Enter を押すと BGM と BGS を停止します..."
gets	# Enter 待ち

# BGM を停止する (5秒かけてフェードアウト)
GameAudio.stop_bgm(5.0);

# BGS を停止する
GameAudio.stop_bgs;

p "Enter を押すと終了します..."
gets	# Enter 待ち

# ライブラリの終了処理
Engine.terminate

