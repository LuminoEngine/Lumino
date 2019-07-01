
#include "RubyExtGenerator.hpp"

//==============================================================================
// RubyExtGenerator

void RubyExtGenerator::generate()
{

	// classes
	OutputBuffer code;

	for (auto& classSymbol : db()->classes()) {

		code.AppendLine("//==============================================================================");
		code.AppendLine("// {0}", classSymbol->fullName());
		code.NewLine();

		{
			OutputBuffer wrapStruct;
			// クラスをラップする構造体
			/* 例:
			struct Wrap_MyClass
				: public Wrap_BaseClass
			{
				VALUE Prop1;		// 特に get を公開する場合、Ruby オブジェクトが GC されないように保持しておく必要がある。
									// また、別途 mark コールバックにて rb_gc_mark() する必要がある。
				Wrap_MyClass()
					: Prop1(Qnil), Layers(Qnil)
				{}
			};
			*/
			wrapStruct.AppendLine(u"struct {0}", makeWrapStructName(classSymbol));
			if (classSymbol->baseClass()) {  // 継承
				wrapStruct.AppendLine("    : public " + makeWrapStructName(classSymbol));
			}
			wrapStruct.AppendLine("{");
			// プロパティフィールド
			//wrapStruct.AppendLine(_currentClassInfo.AdditionalWrapStructMember.ToString());
			// コンストラクタと初期化子
			wrapStruct.IncreaseIndent();
			wrapStruct.AppendLine("{0}()", makeWrapStructName(classSymbol));
			//if (!_currentClassInfo.AdditionalWrapStructMemberInit.IsEmpty)
			//{
			//	wrapStruct.Append("    : ");
			//	wrapStruct.AppendLine(_currentClassInfo.AdditionalWrapStructMemberInit.ToString());
			//}
			wrapStruct.AppendLine("{}");
			wrapStruct.DecreaseIndent();
			wrapStruct.AppendLine("};").NewLine();
			//wrapStruct.AppendLine(_currentClassInfo.AdditionalClassStaticVariables.ToString());


			code.append(wrapStruct);
		}


		for (auto& overload : classSymbol->overloads()) {
			code.append(makeWrapFuncImplement(overload));
		}
	}


	// save
	{
		ln::String fileName = ln::String::format("{0}.RubyExt.generated.cpp", config()->moduleName);

		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("RubyExt.cpp.template")))
			.replace("%%Code%%", code.toString());

		ln::FileSystem::writeAllText(makeOutputFilePath(u"Ruby", fileName), src);
	}
}

ln::String RubyExtGenerator::makeWrapFuncImplement(MethodOverloadInfo* overloadInfo) const
{
	OutputBuffer code;

	// Func header
	code.AppendLine(u"static VALUE {0}(int argc, VALUE *argv, VALUE self)", makeWrapFuncName(overloadInfo->representative()));

	// begin body
	code.AppendLine("{");
	code.IncreaseIndent();

	// Body 作成。if () { ～ } までのオーバーロード呼び出し1つ分
	for (auto& method : overloadInfo->methods()) {
	    makeWrapFuncCallBlock(method);
	}

	// 関数終端まで到達してしまったら例外
	code.AppendLine(u"rb_raise(rb_eArgError, \"{0} - wrong argument type.\");", overloadInfo->representative()->fullName());
	code.AppendLine("return Qnil;");

	// end body
	code.DecreaseIndent();
	code.AppendLine("}").NewLine();

	return code.toString();

            //CLMethod baseMethod = overloadMethods[0];

            //// メソッドを持つクラスを表現するグローバル変数
            //string typeValName = RubyCommon.GetModuleVariableName(baseMethod.OwnerClass);

            //// 関数名
            //string funcName = "lnrb" + baseMethod.FuncDecl.OriginalFullName;
            //if (baseMethod.IsStatic) funcName = "static_" + funcName;    // static メソッドの場合は先頭に static_ を付ける
            //
            //// 関数ヘッダ
            //_allFuncDefines.AppendLine("static VALUE {0}(int argc, VALUE *argv, VALUE self)", funcName);
            //_allFuncDefines.AppendLine("{");
            //_allFuncDefines.IncreaseIndent();

            //// インスタンスメソッドの場合は this を表すオブジェクトを Data_Get_Struct で取りだす
            //var funcBody = new OutputBuffer();
            //if (baseMethod.IsInstanceMethod)
            //{
            //    funcBody.AppendLine("{0}* selfObj;", GetWrapStructName(baseMethod.OwnerClass));
            //    funcBody.AppendLine("Data_Get_Struct(self, {0}, selfObj);", GetWrapStructName(baseMethod.OwnerClass));
            //}

            //// Body 作成
            //foreach (var method in overloadMethods)
            //{
            //    // if () { ～ } までのオーバーロード呼び出し1つ分
            //    RubyCommon.MakeOverloadedMethod(method, funcBody);
            //}

            //// 関数終端まで到達してしまったら例外
            //string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);
            //funcBody.AppendLine(@"rb_raise(rb_eArgError, ""Lumino::{0}.{1} - wrong argument type."");", baseMethod.OwnerClass.Name, rubyMethodName);
            //funcBody.AppendLine("return Qnil;");

            //// funcBody テキストを _allFuncDefines へ結合
            //_allFuncDefines.AppendWithIndent(funcBody.ToString());

            //// 関数終端の }
            //_allFuncDefines.DecreaseIndent();
            //_allFuncDefines.AppendLine("}").NewLine();

            //// rb_define_method 等のメソッド登録を作成する
            //RubyCommon.MakeRubyMethodRegister(_allModuleDefines, baseMethod, typeValName, rubyMethodName, funcName);
        
}

ln::String RubyExtGenerator::makeWrapFuncCallBlock(MethodSymbol* method) const
{
	return u"";
}
