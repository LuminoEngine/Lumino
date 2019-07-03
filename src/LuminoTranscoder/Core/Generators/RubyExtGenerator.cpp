
#include "RubyExtGenerator.hpp"

//==============================================================================
// RubyExtGenerator

void RubyExtGenerator::generate()
{
	OutputBuffer allFlatCApiDecls;

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

		for (auto& method : classSymbol->methods()) {
			allFlatCApiDecls.AppendLine(u"extern \"C\" " + FlatCCommon::makeFuncHeader(config(), method) + u";");
		}
	}


	// Ruby クラス定義 (rb_define_XXXX 呼び出し)
	OutputBuffer typeVALUEDecls;
	OutputBuffer moduleInitializer;
	{
		moduleInitializer.AppendLine(u"extern \"C\" void Init_{0}()", config()->moduleName);
		moduleInitializer.AppendLine(u"{");
		moduleInitializer.IncreaseIndent();

		// Root module (decl)
		typeVALUEDecls.AppendLine(u"VALUE g_rootModule;");

		// Root module (define)
		moduleInitializer.AppendLine(u"g_rootModule = rb_define_module(\"{0}\");", config()->moduleName);
		moduleInitializer.NewLine();

		// Enums
		moduleInitializer.AppendLine(makeEnumTypeRegisterCode());
		moduleInitializer.NewLine();

		
		for (auto& classSymbol : db()->classes()) {
			auto classInfoVar = makeRubyClassInfoVariableName(classSymbol);

			// class (decl)
			typeVALUEDecls.AppendLine(u"VALUE {0};", classInfoVar);

			// define class
			moduleInitializer.AppendLine(u"{0} = rb_define_class_under(g_rootModule, \"{1}\", rb_cObject);", classInfoVar, classSymbol->shortName());

			// define methods
			for (auto& overload : classSymbol->overloads()) {
				moduleInitializer.AppendLine(
					u"rb_define_singleton_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);",
					classInfoVar, makeRubyMethodName(overload->representative()), makeWrapFuncName(overload->representative()));


				//string def;
				//if (baseMethod.IsRefObjectConstructor)
				//	def = string.Format(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({ 1 }), -1); ", typeValName, funcName);
				//else if (baseMethod.IsInstanceMethod)
				//	def = string.Format(@"rb_define_method({0}, ""{1}"", LN_TO_RUBY_FUNC({ 2 }), -1); ", typeValName, rubyMethodName, funcName);
				//else
				//	def = string.Format(@"rb_define_singleton_method({0}, ""{1}"", LN_TO_RUBY_FUNC({ 2 }), -1); ", typeValName, rubyMethodName, funcName);
				//output.AppendLine(def);
			}

			moduleInitializer.NewLine();
		}

		moduleInitializer.DecreaseIndent();
		moduleInitializer.AppendLine(u"}");
	}

	// save
	{
		ln::String fileName = ln::String::format("{0}.RubyExt.generated.cpp", config()->moduleName);

		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("RubyExt.cpp.template")))
			.replace("%%Common%%", ln::FileSystem::readAllText(config()->flatCCommonHeader))
			.replace("%%FlatCDecls%%", allFlatCApiDecls.toString())
			.replace("%%EnumTypeVALUEDecls%%", makeEnumTypeVALUEVariableDecls())
			.replace("%%TypeVALUEDecls%%", typeVALUEDecls.toString())
			.replace("%%Code%%", code.toString())
			.replace("%%ModuleInitializer%%", moduleInitializer.toString());

		ln::FileSystem::writeAllText(makeOutputFilePath(u"Ruby", fileName), src);
	}
}

ln::String RubyExtGenerator::makeSnakeStyleName(const ln::String& name) const
{
	ln::String output = "";

	// 後ろから
	bool isLastLow = islower(name[name.length() - 1]);
	int upperCount = 0;
	int lastIndex = name.length();
	int i;
	for (i = name.length() - 2; i >= 0; i--)
	{
		// 小文字の連続を探している状態
		if (isLastLow)
		{
			if (isupper(name[i]) || isdigit(name[i]))
			{
				if (lastIndex - i > 0)
				{
					if (output.length() != 0) output = u"_" + output;
					output = ln::String(name.substr(i, lastIndex - i)).toLower() + output;	// TODO: StringRef に toLower 実装
				}
				lastIndex = i;
				isLastLow = false;
			}
			else
			{
				// 小文字が続いている
			}
		}
		// 大文字の連続を探している状態
		else
		{
			upperCount++;
			if (islower(name[i]))
			{
				upperCount = 0;
				if (upperCount == 1)
					isLastLow = true;
				else
				{
					if (lastIndex - i - 1 > 0)
					{
						if (output.length() != 0) output = "_" + output;
						output = ln::String(name.substr(i + 1, lastIndex - i - 1)).toLower() + output;
					}
					lastIndex = i + 1;
					isLastLow = true;
				}
			}
			else
			{
				// 大文字が続いている
			}
		}
	}

	if (lastIndex != 0)
	{
		if (lastIndex - i > 0)
		{
			if (output.length() != 0) output = "_" + output;
			output = ln::String(name.substr(0, lastIndex)).toLower() + output;
		}
	}

	return output;
}

ln::String RubyExtGenerator::makeUpperSnakeName(const ln::String& name) const
{
	// まず大文字境界で分割
	ln::List<ln::String> tokens;
	for (int i = 0; i < name.length(); i++) {
		if (isupper(name[i])) {
			tokens.add(u"");
		}
		tokens.back() += name[i];
	}
	// RGBA8 -> [A,G,B,A8]


	ln::String output = tokens[0];
	for (int i = 1; i < tokens.size(); i++) {
		auto last = tokens[i - 1][tokens[i - 1].length() - 1];
		if (isupper(last) || isdigit(last)) {
		}
		else {
			output += u"_";
		}
		output += tokens[i];
	}

	return output;
}

ln::String RubyExtGenerator::makeRubyMethodName(MethodSymbol* method) const
{
	return makeSnakeStyleName(method->shortName());
#if 0
	ln::String name;
	if (method.OwnerProperty != null) {
		name = GetSnakeStyleName(method.OwnerProperty.Name);
	}
	else {
		name = GetSnakeStyleName(method.Name);
	}

	// is プロパティの場合は is_ を取り除く
	if (name.IndexOf("is_") == 0)
	{
		if (!char.IsNumber(name, 3))    // 変換した結果数値が識別子の先頭にならないこと
			name = name.Substring(3)/* + "?"*/;
		//else
		//    name += "?";                // ? はつけてあげる
		if (!method.IsSetterProperty)
		{
			name += "?";
		}
		// ? はつけてあげる
	}

	// まずはスネークスタイルに変換
	//var name = GetSnakeStyleName(method.Name);

	//// 先頭が is の場合は 末尾 ? に変換
	//if (name.IndexOf("is_") == 0)
	//{
	//    if (char.IsNumber(name, 3))    // 変換した結果数値が識別子の先頭にならないこと
	//        name += "?";                // ? はつけてあげる
	//    else
	//        name = name.Substring(3) + "?";
	//}

	// プロパティの場合は = 等に変更
	if (method.PropertyNameType != PropertyNameType.NotProperty)
	{
		/*if (method.PropertyNameType == PropertyNameType.Get)
			name = name.Substring(4);   // 先頭の get_ を取り除く
		else*/ if (method.PropertyNameType == PropertyNameType.Set)
	name += /*name.Substring(4) + */"=";   // 先頭の set_ を取り除き、後ろに =
	}
#endif
}

ln::String RubyExtGenerator::makeWrapFuncImplement(MethodOverloadInfo* overloadInfo) const
{
	OutputBuffer code;

	// Func header
	code.AppendLine(u"static VALUE {0}(int argc, VALUE* argv, VALUE self)", makeWrapFuncName(overloadInfo->representative()));

	// begin body
	code.AppendLine("{");
	code.IncreaseIndent();

	// Body 作成。if () { ～ } までのオーバーロード呼び出し1つ分
	for (auto& method : overloadInfo->methods()) {
		code.AppendLine(makeWrapFuncCallBlock(method));
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
	/* 出力例：
	if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbFloat(volume) && isRbFloat(pitch) && isRbFloat(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_PlayBGM(_filePath, _volume, _pitch, _fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
	*/

	// 集計
	int requiredArgsCount = 0;
	int optionalArgsCount = 0;			// デフォルト引数の数
	OutputBuffer rb_scan_args_Decls;	// rb_scan_args により引数情報を格納する VALUE 型ローカル変数宣言
	OutputBuffer rb_scan_args_Args;		// rb_scan_args の実引数リスト

	// API 1つの呼び出しの各引数について、宣言、実引数渡し、後処理、return文 を作っていく
	OutputBuffer callerArgDecls;
	OutputBuffer callerArgList;
	OutputBuffer callerReturnStmt;

	{
		for (auto& param : method->flatParameters())
		{
			// return として選択されている out 属性の引数である場合
			if (param->isReturn())
			{
				auto localVarName = u"_" + param->name();
				// decl
				callerArgDecls.AppendLine(u"{0} {1};", FlatCCommon::makeTypeName(config(), param->type()), localVarName);
				// arg
				callerArgList.AppendCommad(u"&" + localVarName);
				// return
				callerReturnStmt.AppendLine(makeVALUEReturnExpr(param->type(), localVarName));
			}
			else
			{

				if (param->defaultValue)
					optionalArgsCount++;
				else
					requiredArgsCount++;

				// rb_scan_args の格納先 VALUE 宣言
				rb_scan_args_Decls.AppendLine(u"VALUE {0};", param->name());
				// rb_scan_args の引数
				rb_scan_args_Args.AppendCommad(u"&{0}", param->name());

				// FlatC call args
			}
		}
	}




	OutputBuffer code;
	code.append(u"if ({0} <= argc && argc <= {1}) {{", requiredArgsCount, requiredArgsCount + optionalArgsCount).NewLine();
	code.IncreaseIndent();

	if (!rb_scan_args_Decls.isEmpty()) code.AppendLine(rb_scan_args_Decls);
	if (!rb_scan_args_Args.isEmpty()) code.AppendLine(rb_scan_args_Args);

	code.AppendLine(u"{");
	code.IncreaseIndent();
	code.AppendLine(callerArgDecls.toString().trim());
	code.AppendLine(u"LnResult errorCode = {0}({1});", FlatCCommon::makeFuncName(config(), method), callerArgList.toString());
	code.AppendLine(u"if (errorCode < 0) rb_raise(rb_eRuntimeError, \"Lumino runtime error. (%d)\\n%s\", errorCode, LnRuntime_GetLastErrorMessage());");
	code.AppendLine((callerReturnStmt.isEmpty()) ? u"return Qnil;" : callerReturnStmt.toString().trim());
	code.DecreaseIndent();
	code.AppendLine(u"}");


	code.DecreaseIndent();
	code.append("}");

	return code.toString();

#if 0

	var callBody = new OutputBuffer();

	int normalArgsCount = 0;
	int defaultArgsCount = 0;
	var scan_args_Inits = new OutputBuffer();
	var scan_args_Args = new OutputBuffer();
	string typeCheckExp = "";

	// API 1つの呼び出しの各引数について、宣言、実引数渡し、後処理、return文 を作っていく
	var initStmt = new OutputBuffer();
	var argsText = new OutputBuffer();
	var postStmt = new OutputBuffer();
	var returnStmt = new OutputBuffer();

	// オリジナルの全仮引数を見ていく
	foreach(var param in method.FuncDecl.Params)
	{
		// コンストラクタの最後の引数は、WrapStruct::Handle への格納になる
		if (method.IsRefObjectConstructor && param == method.FuncDecl.Params.Last())
		{
			argsText.AppendCommad("&selfObj->Handle");

			// Handle がアタッチされたので Register
			postStmt.AppendLine("Manager::RegisterWrapperObject(self);");
		}
		// 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
		else if (
			!method.IsRefObjectConstructor &&
			method.IsInstanceMethod &&
			param == method.FuncDecl.Params.First())
		{
			var classType = param.Type as CLClass;
			if (classType == null) throw new InvalidOperationException("インスタンスメソッドの第1引数が不正。");
			if (classType.IsStruct)
				argsText.AppendCommad("selfObj");
			else
				argsText.AppendCommad("selfObj->Handle");
		}
		// return として選択されている引数である場合
		else if (param == method.ReturnParam)
		{
			var varName = "_" + param.Name;
			// 宣言
			initStmt.AppendLine("{0} {1};", CppCommon.ConvertTypeToCName(param.Type), varName);
			// API実引数
			argsText.AppendCommad("&" + varName);

			// getプロパティの場合はメンバにキャッシュする
			if (method.IsGetterProperty && CLType.CheckRefObjectType(param.Type))
			{
				// variable に、キャッシュ先の変数名を作る
				var propName = method.OwnerProperty.Name;
				string variable;
				if (method.IsStatic)
					variable = string.Format("wrap{0}::{1}", method.OwnerClass.Name, propName);
				else
					variable = string.Format("selfObj->{0}", propName);

				// post
				postStmt.AppendLine("if (!checkEqualHandle({0}, {1})) {{", variable, varName);
				postStmt.AppendLine("    {0} = Manager::GetWrapperObjectFromHandle({1});", variable, varName);
				postStmt.AppendLine("}");
				// return
				returnStmt.AppendLine("return {0};", variable);
			}
			else
			{
				// return
				RubyCommon.MakeReturnCastExpCToVALUE(param.Type, varName, returnStmt);
			}
		}
		// 普通の in/out の引数はここに来る
		else
		{
			/*
				LNTexture2D_Create(w, h, format, mipmap) の例

				if (2 <= argc && argc <= 4) {

					// まず、実引数を取り出す必要がある
					VALUE width;
					VALUE height;
					VALUE format;
					VALUE mipmap;
					rb_scan_args(argc, argv, "22", &width, &height, &format, &mipmap);

					// 型チェック
					if (isRbNumber(width) && isRbNumber(height) && isRbNumber(format) && isRbBool(mipmap)) {

						// C言語の型へ変換する
						int _width = FIX2INT(width);
						int _height = FIX2INT(height);
						LNTextureFormat _format = (format != Qnil) ? (LNTextureFormat)FIX2INT(format) : LN_FMT_A8R8G8B8;
						LNBool _mipmap = (mipmap != Qnil) ? RbBooltoBool(mipmap) : LN_FALSE;

						// API 呼び出し
						LNResult errorCode = LNTexture2D_Create(_width, _height, _format, _mipmap, &selfObj->Handle);
						if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());

						return Qnil;
					}
			*/
			// 通常引数とデフォルト引数のカウント
			if (string.IsNullOrEmpty(param.OriginalDefaultValue))
				normalArgsCount++;
			else
				defaultArgsCount++;

			// out の場合は C++ 型で受け取るための変数を定義
			if (param.IOModifier == IOModifier.Out)
			{
				// ruby は複数 out を扱えないためここに来ることはないはず
				throw new InvalidOperationException();
			}
			// 入力引数 (in とinout)
			else
			{
				// rb_scan_args の格納先 VALUE 宣言
				scan_args_Inits.AppendLine("VALUE {0};", param.Name);
				// rb_scan_args の引数
				scan_args_Args.AppendCommad("&{0}", param.Name);
				// 型チェック条件式
				if (!string.IsNullOrEmpty(typeCheckExp))
					typeCheckExp += " && ";
				typeCheckExp += RubyCommon.GetTypeCheckExp(param.Type, param.Name);
				// C変数宣言 & 初期化代入
				initStmt.AppendLine(RubyCommon.GetDeclCastExpVALUEtoC(param.Type, "_" + param.Name, param.Name, param.OriginalDefaultValue));
				// API実引数
				if (param.Type is CLClass &&
					((CLClass)param.Type).IsStruct)
					argsText.AppendCommad("&_" + param.Name);   // struct は 参照渡し
				else
					argsText.AppendCommad("_" + param.Name);


				if (method.IsSetterProperty && CLType.CheckRefObjectType(param.Type))
				{
					// post
					var propName = method.OwnerProperty.Name;
					postStmt.AppendLine("selfObj->{0} = {1};", propName, param.Name);
				}
			}
		}
	}

	// rb_scan_args の呼び出し
	string rb_scan_args_Text = "";
	if (!scan_args_Args.IsEmpty)
		rb_scan_args_Text = string.Format(@"rb_scan_args(argc, argv, ""{0} {1}"", { 2 }); ", normalArgsCount, (defaultArgsCount > 0) ? defaultArgsCount.ToString() : "", scan_args_Args);

		// 型チェック式が空なら true にしておく
		if (string.IsNullOrEmpty(typeCheckExp))
			typeCheckExp = "true";

	// エラーコードと throw
	string preErrorStmt = "";
	string postErrorStmt = "";
	if (method.FuncDecl.ReturnType.IsResultCodeType)
	{
		preErrorStmt = "LNResult errorCode = ";
		postErrorStmt = @"if (errorCode != LN_OK) rb_raise(g_luminoError, ""Lumino error. (%d)\n%s"", errorCode, LNGetLastErrorMessage()); " + OutputBuffer.NewLineCode;
	}

	// API 呼び出し
	var apiCall = string.Format("{0}({1});", method.FuncDecl.OriginalFullName, argsText.ToString());

	// オーバーロードひとつ分の塊を作成する
	callBody.AppendWithIndent("if ({0} <= argc && argc <= {1}) {{", normalArgsCount.ToString(), (normalArgsCount + defaultArgsCount).ToString()).NewLine();
	callBody.IncreaseIndent();
	callBody.AppendWithIndent(scan_args_Inits.ToString());
	callBody.AppendWithIndent(rb_scan_args_Text).NewLine();
	callBody.AppendWithIndent("if ({0}) {{", typeCheckExp).NewLine();
	callBody.IncreaseIndent();
	callBody.AppendWithIndent(initStmt.ToString());
	callBody.AppendWithIndent(preErrorStmt + apiCall + OutputBuffer.NewLineCode);
	callBody.AppendWithIndent(postErrorStmt);
	callBody.AppendWithIndent(postStmt.ToString());
	callBody.AppendLine((returnStmt.IsEmpty) ? "return Qnil;" : returnStmt.ToString());
	callBody.DecreaseIndent();
	callBody.AppendWithIndent("}").NewLine();
	callBody.DecreaseIndent();
	callBody.AppendWithIndent("}").NewLine();

	funcBody.AppendWithIndent(callBody.ToString());
#endif
}

// C言語変数 → VALUE 変換式の作成 (return 用)
ln::String RubyExtGenerator::makeVALUEReturnExpr(TypeSymbol* type, const ln::String& varName) const
{
	// primitive type
	if (type->isPrimitive()) {
		return ln::String::format(u"return LNI_TO_RUBY_VALUE({0});", varName);
	}

	// enum type
	if (type->isEnum()) {
		return ln::String::format(u"return INT2FIX({0});", varName);
	}

	// struct type
	if (type->isStruct()) {
		// 新しいインスタンスを作って返す
		OutputBuffer output;
		//output.AppendLine("VALUE retObj = {0}_allocate({1});", classType.OriginalName, GetModuleVariableName(classType));
		//output.AppendLine("*(({0}*)DATA_PTR(retObj)) = {1};", classType.OriginalName, varName);
		//output.AppendLine("return retObj;");
		LN_NOTIMPLEMENTED();
		return output.toString();
	}
	// class type
	else if (type->isClass()) {
		return ln::String::format(u"return Manager::GetWrapperObjectFromHandle({0});", varName);
	}

	LN_UNREACHABLE();
	return ln::String::Empty;
}

ln::String RubyExtGenerator::makeEnumTypeVALUEVariableDecls() const
{
	OutputBuffer code;

	// VALUE global variables
	for (auto& symbol : db()->enums()) {
		code.AppendLine(u"VALUE {0};", makeEnumTypeVALUEName(symbol));
	}

	return code.toString().trim();
}

ln::String RubyExtGenerator::makeEnumTypeRegisterCode() const
{
	OutputBuffer code;

	for (auto& symbol : db()->enums()) {
		auto varName = makeEnumTypeVALUEName(symbol);

		// enum module
		code.AppendLine(u"{0} = rb_define_module_under(g_rootModule, \"{1}\");", varName, symbol->shortName());

		// enum member
		for (auto& member : symbol->constants()) {
			code.AppendLine(u"rb_define_const({0}, \"{1}\", INT2FIX({2})); ", varName, makeUpperSnakeName(member->name()), member->value()->get<int>());
		}
		code.NewLine();
	}

	return code.toString().trim();
}
