//#pragma once
//
//namespace lna {
//
//// Toolset は監視対象の Asset や Editor の必要な機能をジャンルごとに決めるもの。
//// 例えば、アクションゲームであれば Motion を管理する必要があるが、RPG では不要、など。
//class Toolset
//	: public ln::Object
//{
//public:
//	virtual ln::String name() const = 0;
//
//	/**  */
//	virtual const ln::List<ln::String>& editorFeatures() const = 0;
//
//private:
//    LN_INTERNAL_NEW_OBJECT;
//	Toolset();
//    void init();
//};
//
//} // namespace lna
