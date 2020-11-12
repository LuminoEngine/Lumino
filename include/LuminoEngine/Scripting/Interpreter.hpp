#pragma once
#include "../Base/Delegate.hpp"
#include "../Base/Serializer.hpp"


namespace ln {

	
/**
 * (実験的な機能) 
 */
LN_CLASS()
class InterpreterCommand
	: public Object
{
public:
	String m_code;
	List<String> m_params;

	void serialize(Serializer2& ar) override
	{
		ar & makeNVP(u"code", m_code);
		ar & makeNVP(u"params", m_params);
	}

	/** コマンドの実行コードを取得します。 */
	LN_METHOD()
	const String& code() const { return m_code; }

	/** コマンドのパラメータ数を取得します。 */
	LN_METHOD()
	int paramsCount() const { return m_params.size(); }

	/** コマンドのパラメータを取得します。 */
	LN_METHOD()
	const String& param(int index) const { return m_params[index]; }

};

/**
 * (実験的な機能) 
 */
LN_CLASS()
class InterpreterCommandList
	: public Object
{
public:
	List<Ref<InterpreterCommand>> commands;

	void addCommand(const String& code, const List<String>& params)
	{
		auto cmd = makeObject<InterpreterCommand>();
		cmd->m_code = code;
		cmd->m_params = params;
		commands.add(cmd);
	}

	void serialize(Serializer2& ar) override
	{
		ar & makeNVP(u"commands", commands);
	}

	/** コマンドを追加します。 */
	LN_METHOD()
	void addCommand(const String& code);

	/** コマンドと 1 つのパラメータを追加します。 */
	LN_METHOD()
	void addCommand1(const String& code, const String& param0);

	/** コマンドと 2 つのパラメータを追加します。 */
	LN_METHOD()
	void addCommand2(const String& code, const String& param0, const String& param1);

	/** コマンドと 3 つのパラメータを追加します。 */
	LN_METHOD()
	void addCommand3(const String& code, const String& param0, const String& param1, const String& param2);

	/** コマンドと 4 つのパラメータを追加します。 */
	LN_METHOD()
	void addCommand4(const String& code, const String& param0, const String& param1, const String& param2, const String& param3);

LN_CONSTRUCT_ACCESS:
	InterpreterCommandList();

    /** InterpreterCommandList を作成します。 */
    LN_METHOD()
	bool init();
};

/**
 * InterpreterCommandDelegate
 */
LN_DELEGATE()
using InterpreterCommandDelegate = Delegate<bool(InterpreterCommand* cmd)>;

/**
 * (実験的な機能) コルーチンベースのスクリプトインタプリタ。
 *
 * ツクールのと同じ仕組み。ただスクリプト実行するなら Lua とか使ってもよいのだが、これの重要な要件は「実行状態を保存できるようにすること」
 */
LN_CLASS()
class Interpreter
	: public Object
{
public:

	/** コマンドリストをクリアします。実行中のコマンドリストは強制的に破棄されます。 */
	LN_METHOD()
	void clear();

	/** コマンドリストを設定し、実行を開始します。*/
	LN_METHOD()
	void run(InterpreterCommandList* commandList);

	/** コマンドリストの実行中であるかを確認します。 */
	LN_METHOD(Property)
	bool isRunning() const;

	/* コマンドリストの実行状態を更新します。 */
	LN_METHOD()
	void update();

	/* コマンドリストの実行を終了します。 */
	LN_METHOD()
	void terminate();

	/* コマンドの実行内容を登録します。 */
	LN_METHOD()
	void registerCommandHandler(const StringRef& name, Ref<InterpreterCommandDelegate> handler);

	/** setWaitMode */
	LN_METHOD(Property)
	void setWaitMode(const String& mode) { m_waitMode = mode; }

	/** waitMode */
	LN_METHOD(Property)
	const String& waitMode() const { return m_waitMode; }

	/** setWaitCount */
	LN_METHOD(Property)
	void setWaitCount(int count) { m_waitCount = count; }
	
	/** waitCount */
	LN_METHOD(Property)
	int waitCount() const { return m_waitCount; }

LN_CONSTRUCT_ACCESS:
	Interpreter();
	virtual ~Interpreter() = default;
	
    /** Interpreter を作成します。 */
    LN_METHOD()
	bool init();

protected:

	/**  */
	InterpreterCommand* getCurrentCommand() const;

	/** waitMode に基づいて、実行停止中かを確認する (Wait 中なら true を返すこと) */
	LN_METHOD()
	virtual bool onUpdateWait();


private:

	bool updateChild();
	bool updateWait();
	bool checkFreeze();

	/** コマンドを1つ実行する */
	bool executeCommand();

	/** コマンドを1つ実行する */
	virtual bool onExecuteCommand(InterpreterCommand* cmd);

	List<Ref<InterpreterCommand>> m_commandList;
	int m_index;
	String m_waitMode;
	int m_waitCount;
	std::unordered_map<String, Ref<InterpreterCommandDelegate>> m_commandDelegateMap;
};


//struct TkMVSoundData
//{
//	String		name;
//	int				pitch;
//	int				volume;
//
//	void serialize(tr::Archive& ar, int version)
//	{
//		ar & LN_NVP(name);
//		ar & LN_NVP(pitch);
//		ar & LN_NVP(volume);
//	}
//};
//
//struct TkMVMapData
//{
//	bool			autoplayBgm;
//	bool			autoplayBgs;
//	TkMVSoundData	bgm;
//	TkMVSoundData	bgs;
//	String		displayName;
//
//	void serialize(tr::Archive& ar, int version)
//	{
//		ar & LN_NVP(autoplayBgm);
//		ar & LN_NVP(autoplayBgs);
//		ar & LN_NVP(bgm);
//		ar & LN_NVP(bgs);
//		ar & LN_NVP(displayName);
//	}
//};
//

} // namespace ln
