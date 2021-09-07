
#include "Internal.hpp"
#include <LuminoCore/Serialization/Serialization.hpp>

namespace ln {

/*
	write 時の基本方針
	- 何か値を write しようとしたら、オブジェクト先頭など、何か「書き込み待ち」になっているものを書く

	文字列や配列扱いしたいオブジェクトの write 動作
	- まず普通の Object として writeValue		→ コンテナ扱い
	- serialize の中で setTag する


	read 時の基本方針
	- オブジェクト型ならまず setNextName()
	- read 使用としている型がプリミティブ型でなければ store->readContainer
	- 次に store->readValue すると、現在の Container と setNextName() した名前から値を取得する

*/



/*
	- in object, out object に関わらず、serialize() が呼ばれたとき、そのオブジェクト用の Node は作られている。
		- Write : Node の state は Ready になっている。まだ Object か Array かはわからない。→ 最初の serialize で nextName が呼ばれるかどうかで決まる。
		- Read  : Node の state は Object または Array になっている。

	- State
		- Value  : リーフノードを示す。state 変更に続いて、"1度だけ"、"プリミティブな Value" のみシリアライズできる。
		- Array  : makeArrayTag によって Node の状態を確定する。その後は任意の回数 serialize できるが、NVP を serialize することはできない。
		- Object : makeXXXXTag が呼ばれずに、子の serialize が行われた場合は Object となる。
		           NVP 以外の値を１度だけ serialize することができる。この場合、子は親 Node の一部として serialize される。(子の serialize 開始時は Ready (親Nodeのまま) となる。スマートポインタ・Variant用)

	- BaseClass
		- 考え方は NVP と同じ。NVP の name の部分が "_base_class" とかになった状態で別 Object としてシリアライズする。

	- makeStringTag
		- Write : 

	
	Load の基本方針
	- Save が遅延評価で Store に Node を書き込むのに対して、Load は子の serialize を呼ぶ前に、Store から ContainerNode を読み込む。
		- Save の serialize 開始時は Container は Open していない。
		- Load の serialize 開始時は Container は Open している。
		- これは、TypeInfo などのメタデータを Ref<> の serialize で読み取るために必須。
*/

/*
    Load は大方針として、makeXXX 時点で NodeType を確定して値の型を取得したり、
    Ref<Object> では "_type" : の値を読み取るためにコンテナを Open する必要がある。

    serialize 時点で対応コンテナを必ず Open ということにすると、
    Variant の serialize のために PrimitiveValue も Open 状態にできるようにする必要がある。
    ※ただ、この open 状態は m_store ではなく Node の状態として持っておいた方がいいだろう。
        ・・・と思ったけど、m_store 側も「現在操作中のノード（カレントノード）」を持った方が考えやすい。特にルートノードを コンテナ・値両方対応するとき。
    簡単に言うと、readValue で Node を push した後、子の serialze を呼び出す前に Node に現在の「型」を持たせてしまう。


    Load Store の使い方
    
    moveByName() or moveByIndex で「メンバ」に移動。current() が移動先の値を返すようになる。
    初期状態はルートノードが current() になっている状態。

    openContainer() で、Object または Array の中に入る。
    stack のトップに Object または Array の JsonElement が積まれる。（さっきまで currnet() だったもの）
    open 直後は current() は null を返す。moveXXXX で移動しなければならない。


    「serialize 時点で対応コンテナを必ず Open」はナシ。
    いくつかの makeTag で、Open したコンテナを一度 close して、makeTag の次の serialize でまた開けて値を読む、という手順が必要になる。
    この時点で本当に close していいかどうかも、WrapperObject をいくつもスタックしていると親まで辿るのが大変というか不自然。
    なので、save と同じく遅延で open する。（次の process で　Object として open、makeSmartPtrTag ならこの時点で Object として Open など）

*/

//==============================================================================
// Archive

const String Archive::ClassNameKey = _TT("lumino_class_name");
const String Archive::ClassVersionKey = _TT("lumino_class_version");
const String Archive::ClassBaseKey = _TT("lumino_base_class");

void Archive::makeArrayTag(int* outSize)
{
	//
	if (isSaving())
	{
		moveState(NodeHeadState::Array);
	}
	else if (isLoading())
	{
		moveState(NodeHeadState::Array);
		// ArrayContainer としてデシリアライズしている場合、この時点で size を返したいので、store を ArrayContainer まで移動して size を得る必要がある
		//preReadValue();
		tryOpenContainer();
		if (outSize) *outSize = m_store->getContainerElementCount();

		// makeArrayTag() を抜けた次の process は 0 インデックスを使う
		//m_store->moveToIndexedMember(0);
	}
}

void Archive::makeMapTag(int* outSize)
{
	if (isSaving())
	{
		moveState(NodeHeadState::Object);
	}
	else if (isLoading())
	{
		moveState(NodeHeadState::Object);
		tryOpenContainer();
		if (outSize) *outSize = m_store->getContainerElementCount();
	}
}

void Archive::makeVariantTag(ArchiveNodeType* type)
{

    if (isSaving()) {
        if (*type == ArchiveNodeType::Object || *type == ArchiveNodeType::Array) {
            moveState(NodeHeadState::WrapperObject);
        }
        else {
            moveState(NodeHeadState::PrimitiveValue);
        }
    }
    else if (isLoading()) {

        //m_store->closeContainer();
        //m_nodeInfoStack.back().containerOpend = false;

        *type = m_store->getReadingValueType();

        //*outIsNull = m_store->getOpendContainerType() == ArchiveContainerType::Null;
        //moveState(NodeHeadState::WrapperObject);
        moveState(NodeHeadState::WrapperObject);

        // makeVariantTag の次は何らかの値の process をする。
        // いまのところその process 
        //moveState(NodeHeadState::Ready);

        //*type = m_store->getReadingValueType();
    }

    //m_store->getContainerType()

}

void Archive::makeTypeInfo(String* value)
{
    if (isSaving()) {
        // この時点では Ref<> の serialize、つまり WrapperObject の serialize 中。
        // ここではまだ m_store に書き出すことはできない (コンテナの書き出しがまだ) ので、メタデータを Node に覚えておく。
        m_nodeInfoStack.back().typeInfo = *value;
    }
    else {
        //moveState(NodeHeadState::Object);
        if (LN_REQUIRE(m_nodeInfoStack.back().headState == NodeHeadState::WrapperObject)) return;	// 事前に makeSmartPtrTag 必須
        preReadValue();
        *value = readTypeInfo();
        //process(NameValuePair<String>(u"_type", value));
    }

    //moveState(NodeHeadState::Object);	// TypeInfo を読みたいなら Object confirmed.



    //if (isSaving()) {
    //}
    //else {
    //	preReadValue();
    //	*value = m_nodeInfoStack.top().typeInfo;
    //}
}

const String& Archive::readTypeInfo()
{
    m_store->moveToNamedMember(_TT("_type"));
    ln::String type;
    if (m_store->readValue(&type)) {
        m_nodeInfoStack.back().typeInfo = type;
    }
    return m_nodeInfoStack.back().typeInfo;
}


void Archive::popNodeWrite()
{
    // 空の serialize を呼び出した場合、state は変わっていない。
    // 空の Object として扱いたいので、ここで Object 状態にしておく。
    if (m_nodeInfoStack.back().headState == NodeHeadState::Ready) {
        moveState(NodeHeadState::Object);
    }

    if (m_nodeInfoStack.size() >= 2 && m_nodeInfoStack[m_nodeInfoStack.size() - 2].headState == NodeHeadState::WrapperObject) {
        // Ref<T> の、T の部分の write が終わった

        // makeSmartPtrTag() だけ呼んで、その後何も書かれなかったときはここに入る
        if (!m_nodeInfoStack.back().containerOpend) {
            m_store->writeObject();
            m_nodeInfoStack.back().containerOpend = true;
            writeClassVersion();
            writeTypeInfo();
        }

    }
    else {
        // serialize が空実装ではないが、makeArrayTag など Tag 設定だけして子値の process を行わなかった場合はコンテナ開始タグが書き込まれていないため、ここで書き込む。
        if (m_nodeInfoStack.back().headState == NodeHeadState::Object ||
            m_nodeInfoStack.back().headState == NodeHeadState::Array) {
            if (!m_nodeInfoStack.back().containerOpend) {
                preWriteValue();
            }
        }


    }

    //bool taggedValueObject = (m_nodeInfoStack.top().headState == NodeHeadState::PrimitiveValue);
    bool containerOpend = m_nodeInfoStack.back().containerOpend;
    NodeHeadState nodeType = m_nodeInfoStack.back().headState;

    m_nodeInfoStack.pop_back();

    // Close containers
    if (containerOpend)
    {
        if (nodeType == NodeHeadState::Object || nodeType == NodeHeadState::WrapperObject) {
            m_store->writeObjectEnd();
        }
        else if (nodeType == NodeHeadState::Array) {
            m_store->writeArrayEnd();
        }
        else {
            LN_UNREACHABLE();
        }
    }

    if (!m_nodeInfoStack.empty()) {
        m_nodeInfoStack.back().nextBaseCall = false;
    }
}


//==============================================================================
// JsonTextOutputArchive

JsonTextOutputArchive::JsonTextOutputArchive()
    : m_localDoc()
    , m_localStore(&m_localDoc)
    , m_processing(false)
{
    setup(&m_localStore, ArchiveMode::Save);
}

JsonTextOutputArchive::~JsonTextOutputArchive()
{
}

String JsonTextOutputArchive::toString(JsonFormatting formatting)
{
    return m_localDoc.toString(formatting);
}

//==============================================================================
// JsonTextInputArchive

JsonTextInputArchive::JsonTextInputArchive(const String& jsonText)
    : m_localDoc()
    , m_localStore(&m_localDoc)
    , m_processing(false)
{
    m_localDoc.parse(jsonText);
    setup(&m_localStore, ArchiveMode::Load);
}

JsonTextInputArchive::~JsonTextInputArchive()
{
}

} // namespace ln
