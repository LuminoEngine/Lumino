#pragma once
#include "../Base/Delegate.hpp"
#include "../Base/Serializer.hpp"


namespace ln {


class InterpreterCommand : public Object
{
public:
	String code;
	List<String> params;

	void serialize(Serializer2& ar) override
	{
		ar & makeNVP(u"code", code);
		ar & makeNVP(u"params", params);
	}
};

class InterpreterCommandList : public Object
{
public:
	List<Ref<InterpreterCommand>> commands;

	void serialize(Serializer2& ar) override
	{
		ar & makeNVP(u"commands", commands);
	}
};

using InterpreterCommandHandler = Delegate<bool(InterpreterCommand* cmd)>;

/**
 * コルーチンベースのスクリプトインタプリタ。
 *
 * ツクールのと同じ仕組み。ただスクリプト実行するなら Lua とか使ってもよいのだが、これの重要な要件は「実行状態を保存できるようにすること」
 */
class Interpreter : public Object
{
public:
	Interpreter();
	virtual ~Interpreter() = default;

	/** コマンドリストを設定し、実行を開始する */
	void run(const Ref<InterpreterCommandList>& commandList);

	/** コマンドリストの実行中であるか */
	bool isRunning() const;

	void update();

	/* コマンドリストの実行を終了する */
	void terminate();

	void registerCommandHandler(const StringRef& name, Ref<InterpreterCommandHandler> handler);

protected:

	void setWaitMode(const String& mode) { m_waitMode = mode; }
	void setWaitCount(int count) { m_waitCount = count; }

	/**  */
	const Ref<InterpreterCommand>& getCurrentCommand() const;

	/** Wait 中なら true を返す */
	virtual bool onUpdateWait(const String& waitMode);


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
	std::unordered_map<String, Ref<InterpreterCommandHandler>> m_commandDelegateMap;
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
