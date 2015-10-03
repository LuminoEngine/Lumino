
#pragma once
#include "../../../src/ApplicationImpl.h"	// TODO
#include <Lumino/Application.h>
#include "../include/LNCommon.h"

/// Handle の管理リストの要素
struct ObjectEntry
{
	CoreObject*		Object;		///< オブジェクト本体
	void*			UserData;	///< バインダ側からセットするユーザーデータ。基本的に管理配列のインデックスとなる。
	int				Index;		///< このオブジェクトがある管理配列上のインデックス
	int				RefCount;	///< 外部に公開する参照カウント (この値を直接操作しないこと。必ず Manager 経由で操作する。CoreObject の参照カウントとは別管理である点に注意)

	ObjectEntry()
	{
		Object = NULL;
		UserData = NULL;
		Index = 0;
		RefCount = 0;
	}
};

/// CoreObject のユーザーデータとして登録する
struct ObjectRegisterData
{
	int				Index;		///< このオブジェクトがある管理配列上のインデックス
};
	
/// C_API 管理クラス
class LFManager
{
public:

	/// Application 初期化前処理
	static void PreInitialize();

	/// Application 初期化後処理
	static void PostInitialize();

	/// 終了処理
	static void Terminate();

	/// obj の Handle を返す。obj がこのクラスに登録済みかを確認し、登録されていなければ新しく登録する。
	static LNHandle CheckRegisterObject(CoreObject* obj);

	/// 例外発生時の処理 (Exeption 以外の例外は NULL を指定して呼び出すことで Unknown が返る)
	static LNResult ProcException(Exception* e);

	/// オブジェクトの参照カウントをデクリメント (CoreObject ではなく ObjectEntry のカウントを操作する)
	static void ReleaseObject(LNHandle handle);

	/// オブジェクトの参照カウントをインクリメント (CoreObject ではなく ObjectEntry のカウントを操作する)
	static void AddRefObject(LNHandle handle);

	/// handle に対する ObjectEntry を取得する
	static ObjectEntry* GetObjectEntry(LNHandle handle);

public:
	static Lumino::ApplicationSettings	ConfigData;
	static Lumino::ApplicationImpl*		Application;
	static bool							IsSystemInitialized;
	static Exception*					LastException;
	static LNResult						LastErrorCode;
	static EncodingConverter*			TCharToUTF8Converter;

private:
	typedef Array<ObjectEntry>	ObjectEntryList;
	static ObjectEntryList		m_objectEntryList;
	static Stack<int>			m_objectIndexStack;

};

