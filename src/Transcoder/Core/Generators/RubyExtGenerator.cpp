
#include "RubyExtGenerator.hpp"

//==============================================================================
// RubyGeneratorBase

ln::String RubyGeneratorBase::makeSnakeStyleName(const ln::String& name) const {
    ln::String output = U"";

    // 後ろから
    bool isLastLow = islower(name[name.length() - 1]);
    int upperCount = 0;
    int lastIndex = name.length();
    int i;
    for (i = name.length() - 2; i >= 0; i--) {
        // 小文字の連続を探している状態
        if (isLastLow) {
            if (isupper(name[i]) || isdigit(name[i])) {
                if (lastIndex - i > 0) {
                    if (output.length() != 0) output = U"_" + output;
                    output = ln::String(name.substr(i, lastIndex - i)).toLower() + output; // TODO: StringRef に toLower 実装
                }
                lastIndex = i;
                isLastLow = false;
            }
            else {
                // 小文字が続いている
            }
        }
        // 大文字の連続を探している状態
        else {
            upperCount++;
            if (islower(name[i])) {
                upperCount = 0;
                if (upperCount == 1)
                    isLastLow = true;
                else {
                    if (lastIndex - i - 1 > 0) {
                        if (output.length() != 0) output = U"_" + output;
                        output = ln::String(name.substr(i + 1, lastIndex - i - 1)).toLower() + output;
                    }
                    lastIndex = i + 1;
                    isLastLow = true;
                }
            }
            else {
                // 大文字が続いている
            }
        }
    }

    if (lastIndex != 0) {
        if (lastIndex - i > 0) {
            if (output.length() != 0) output = U"_" + output;
            output = ln::String(name.substr(0, lastIndex)).toLower() + output;
        }
    }

    return output;
}

ln::String RubyGeneratorBase::makeRubyMethodName(MethodSymbol* method) const {
    ln::String name;

    if (method->isConstructor()) {
        name = U"initialize";
    }
    else if (method->ownerProperty()) {
        auto prop = method->ownerProperty();
        name = makeSnakeStyleName(prop->shortName());

        // bool 型の getter である場合 ? を付ける。先頭が is であれば外す
        if (method->isPropertyGetter() && method->returnType().type == PredefinedTypes::boolType) {
            if (name.indexOf(U"is_") == 0) {
                if (!isdigit(name[3])) { // 変換した結果数値が識別子の先頭にならないこと
                    name = name.substr(3);
                }
            }
            name += U"?";
        }
        // setter の場合は = 代入形式にする
        else if (method->isPropertySetter()) {
            name += U"=";
        }
    }
    else {
        name = makeSnakeStyleName(method->shortName());

        // Property ではなくても、元々の名前が is で始まり bool を返すものは is を外して ? を付ける
        if (name.indexOf(U"is_") == 0 && method->returnType().type == PredefinedTypes::boolType) {
            name = ln::String(name.substr(3)) + U"?";
        }
    }

    return name;
}

//==============================================================================
// RubyExtGenerator

void RubyExtGenerator::generate() {
    OutputBuffer allFlatCApiDecls;

    OutputBuffer code;

    m_RubyRequiredStructMethodsTemplate = ln::FileSystem::readAllText(makeTemplateFilePath(U"RubyRequiredStructMethods.template.cpp"));
    m_RubyRequiredClassMethodsTemplate = ln::FileSystem::readAllText(makeTemplateFilePath(U"RubyRequiredClassMethods.template.cpp"));

    // structs
    for (auto& structSymbol : db()->structs()) {

        code.AppendLine(U"//==============================================================================");
        code.AppendLine(U"// {0}", structSymbol->fullName());
        code.NewLine();

        code.AppendLine(U"VALUE {0};", makeRubyClassInfoVariableName(structSymbol));
        code.NewLine();

        code.AppendLine(m_RubyRequiredStructMethodsTemplate
                            .replace(U"%%FlatStructName%%", makeFlatClassName(structSymbol)));

        for (auto& overload : structSymbol->overloads()) {
            code.append(makeWrapFuncImplement(structSymbol, overload));
        }
    }

    // classes
    for (auto& classSymbol : db()->classes()) {
        code.AppendLines(makeClassRequiredImplementation(classSymbol));
        code.AppendLines(makeClassImplementation(classSymbol));
    }

    // Ruby クラス定義 (rb_define_XXXX 呼び出し)
    OutputBuffer typeVALUEDecls;
    OutputBuffer moduleInitializer;
    {
        moduleInitializer.AppendLine(U"extern \"C\" void Init_{0}_RubyExt()", config()->moduleName);
        moduleInitializer.AppendLine(U"{");
        moduleInitializer.IncreaseIndent();
        moduleInitializer.AppendLine(U"InitLuminoRubyRuntimeManager();");

        // Root module (decl)
        typeVALUEDecls.AppendLine(U"VALUE g_rootModule;");

        // Root module (define)
        moduleInitializer.AppendLine(U"g_rootModule = rb_define_module(\"{0}\");", config()->moduleName);
        moduleInitializer.NewLine();

        // Enums
        moduleInitializer.AppendLine(makeEnumTypeRegisterCode());
        moduleInitializer.NewLine();

        // structs
        for (auto& structSymbol : db()->structs()) {
            auto classInfoVar = makeRubyClassInfoVariableName(structSymbol);

            moduleInitializer.AppendLine(U"{0} = rb_define_class_under(g_rootModule, \"{1}\", rb_cObject);", classInfoVar, structSymbol->shortName());
            moduleInitializer.AppendLine(U"rb_define_alloc_func({0}, {1}_allocate);", classInfoVar, makeFlatClassName(structSymbol));

            for (auto& overload : structSymbol->overloads()) {
                moduleInitializer.AppendLine(U"rb_define_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(overload->representative()), makeWrapFuncName(overload->representative()));
            }

            moduleInitializer.NewLine();
        }

        // classes
        for (auto& classSymbol : db()->classes()) {

            auto classInfoVar = makeRubyClassInfoVariableName(classSymbol);

            // class (decl)
            typeVALUEDecls.AppendLine(U"VALUE {0};", classInfoVar);

            // define class
            auto baseClass = (!classSymbol->baseClass() || classSymbol->isRootObjectClass()) ? ln::String(U"rb_cObject") : makeRubyClassInfoVariableName(classSymbol->baseClass());
            moduleInitializer.AppendLine(U"{0} = rb_define_class_under(g_rootModule, \"{1}\", {2});", classInfoVar, classSymbol->shortName(), baseClass);
            if (!classSymbol->isStatic()) {
                moduleInitializer.AppendLine(U"rb_define_alloc_func({0}, {1}_allocate);", classInfoVar, makeFlatClassName(classSymbol));
            }

            // define methods
            for (auto& overload : classSymbol->overloads()) {
                if (overload->representative()->isConstructor()) {
                    moduleInitializer.AppendLine(U"rb_define_private_method({0}, \"initialize\", LN_TO_RUBY_FUNC({1}), -1);", classInfoVar, makeWrapFuncName(overload->representative()));
                }
                else if (overload->representative()->isStatic()) {
                    moduleInitializer.AppendLine(U"rb_define_singleton_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(overload->representative()), makeWrapFuncName(overload->representative()));
                }
                else if (overload->representative()->isVirtual()) {
                    moduleInitializer.AppendLine(U"rb_define_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(overload->representative()), makeWrapFuncName(overload->representative()));
                }
                else {
                    auto propertySetter = overload->methods().findIf([](auto& x) { return x->isPropertySetter(); });
                    auto normalSetter = overload->methods().findIf([](auto& x) { return !x->isPropertySetter(); });
                    if (propertySetter && normalSetter) {
                        // 次の 2 つのようなオーバーロードを想定する。
                        // - void setPosition(const Vector3& pos);
                        // - void setPosition(float x, float y, float z);
                        moduleInitializer.AppendLine(U"rb_define_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(*propertySetter), makeWrapFuncName(overload->representative()));
                        moduleInitializer.AppendLine(U"rb_define_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(*normalSetter), makeWrapFuncName(overload->representative()));
                    }
                    else {
                        moduleInitializer.AppendLine(U"rb_define_method({0}, \"{1}\", LN_TO_RUBY_FUNC({2}), -1);", classInfoVar, makeRubyMethodName(overload->representative()), makeWrapFuncName(overload->representative()));
                    }
                }
            }

            // register typeinfo
            if (!classSymbol->isStatic()) {
                moduleInitializer.AppendLine(U"{0}(LuminoRubyRuntimeManager::instance->registerTypeInfo({1}, {2}_allocateForGetObject));", makeFlatAPIName_SetManagedTypeInfoId(classSymbol), classInfoVar, makeFlatClassName(classSymbol));
            }

            // override callbacks
            for (auto& method : classSymbol->leafVirtualMethods()) {
                moduleInitializer.AppendLine(U"{0}({1});", makeFlatAPIName_SetOverrideCallback(classSymbol, method, FlatCharset::Unicode), makeWrapFuncName_OverrideCallback(classSymbol, method));
            }

            moduleInitializer.NewLine();
        }

        // EventSignal
        //moduleInitializer.AppendLine(makeEventSignalDefinition());

        moduleInitializer.DecreaseIndent();
        moduleInitializer.AppendLine(U"}");
    }

    // save
    {
        auto outputDir = ln::Path(makeOutputFilePath(U"Ruby", U"GemProject/ext"));
        ln::FileSystem::createDirectory(outputDir);

        ln::String fileName = ln::format(U"{0}.RubyExt.generated.cpp", config()->moduleName);

        ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(U"RubyExt.template.cpp"))
                             .replace(U"%%Common%%", U"")
                             .replace(U"%%FlatCDecls%%", allFlatCApiDecls.toString())
                             .replace(U"%%EnumTypeVALUEDecls%%", makeEnumTypeVALUEVariableDecls())
                             .replace(U"%%TypeVALUEDecls%%", typeVALUEDecls.toString())
                             .replace(U"%%Code%%", code.toString())
                             .replace(U"%%ModuleInitializer%%", moduleInitializer.toString());

        ln::FileSystem::writeAllText(ln::Path(outputDir, fileName), src);
    }
}

ln::String RubyExtGenerator::makeWrapStructName(const TypeSymbol* type) const {
    if (type->isStruct())
        return makeFlatClassName(type);
    else
        return U"Wrap_" + type->shortName();
}

//ln::String RubyExtGenerator::makeEventSignalClassName(const TypeSymbol* delegateType) const
//{
//	for (auto& param : delegateType->delegateDeclaration()->parameters()) {
//
//	}
//}

// 普通の class や delegate など共通で定義する必要がある、Ruby Ext 必須定義 (Wrap struct, allocate, delete, mark)
ln::String RubyExtGenerator::makeClassRequiredImplementation(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    OutputBuffer markExprs;

    code.AppendLine(U"//==============================================================================");
    code.AppendLine(U"// {0}", classSymbol->fullName());
    code.NewLine();

    // クラスをラップする構造体
    {
        OutputBuffer wrapStruct;
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
        wrapStruct.AppendLine(U"struct {0}", makeWrapStructName(classSymbol));
        if (classSymbol->baseClass()) { // 継承
            wrapStruct.AppendLine(U"    : public " + makeWrapStructName(classSymbol->baseClass()));
        }
        else if (
            classSymbol->isRootObjectClass() ||
            classSymbol->isDelegateObject()) {
            wrapStruct.AppendLine(U"    : public Wrap_RubyObject");
        }
        wrapStruct.AppendLine(U"{");
        wrapStruct.IncreaseIndent();

        // Accessor Cache
        wrapStruct.AppendLine(makeAccessorCacheFieldDecls(classSymbol));
        markExprs.AppendLine(makeAccessorCacheFieldMarks(classSymbol));

        if (classSymbol->isDelegateObject()) {
            wrapStruct.AppendLine(U"VALUE m_proc = Qnil;");
            markExprs.AppendLine(U"rb_gc_mark(obj->m_proc);");
        }

        // Signal and Connection
        //for (auto& eventConnectionMethod : classSymbol->eventMethods()) {
        //	wrapStruct.AppendLine(U"VALUE {0};", makeSignalValueName(eventConnectionMethod));
        //	wrapStruct.AppendLine(U"bool {0} = false;", makeEventConnectValueName(eventConnectionMethod));
        //	markExprs.AppendLine(U"rb_gc_mark(obj->{0});", makeSignalValueName(eventConnectionMethod));
        //}

        // Constructor
        wrapStruct.AppendLine(U"{0}()", makeWrapStructName(classSymbol));
        //if (!_currentClassInfo.AdditionalWrapStructMemberInit.IsEmpty)
        //{
        //	wrapStruct.Append("    : ");
        //	wrapStruct.AppendLine(_currentClassInfo.AdditionalWrapStructMemberInit.ToString());
        //}
        wrapStruct.AppendLine(U"{}");
        wrapStruct.DecreaseIndent();
        wrapStruct.AppendLine(U"};").NewLine();
        //wrapStruct.AppendLine(_currentClassInfo.AdditionalClassStaticVariables.ToString());

        code.append(wrapStruct);
    }

    // Ruby requierd methods (allocate, free, mark)
    if (!classSymbol->isStatic()) {
        code.AppendLine(m_RubyRequiredClassMethodsTemplate
                            .replace(U"%%FlatClassName%%", makeFlatClassName(classSymbol))
                            .replace(U"%%WrapStructName%%", makeWrapStructName(classSymbol))
                            .replace(U"%%MarkExprs%%", markExprs.toString()));
    }

    return code.toString();
}

ln::String RubyExtGenerator::makeClassImplementation(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    for (auto& overload : classSymbol->overloads()) {
        /*if (overload->representative()->isEventConnector()) {
			code.append(makeWrapFuncImplement_SignalCaller(overload->representative()));
			code.append(makeWrapFuncImplement_EventConnector(overload->representative()));
		}
		else*/
        if (classSymbol->isDelegateObject()) {
            code.append(makeWrapFuncImplement_ProcCaller(classSymbol->delegateProtoType()));
            //code.append(makeWrapFuncImplement_DelegateObjectConstructor(classSymbol));
            code.append(makeWrapFuncImplement(classSymbol, overload));
        }
        else {
            code.append(makeWrapFuncImplement(classSymbol, overload));
        }
    }

    code.append(makeWrapFuncImplement_SetOverrideCallback(classSymbol));

    return code.toString();
}

ln::String RubyExtGenerator::makeAccessorCacheFieldDecls(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    for (auto& prop : classSymbol->properties()) {
        if (prop->type()->isClass()) {
            if (prop->getter()) {
                code.AppendLine(U"VALUE {0} = Qnil;", makeAccessorCacheName(prop->getter()));
            }
        }
    }

    if (classSymbol->isCollection() && classSymbol->collectionItemType()->isClass()) {
        code.AppendLine(U"std::vector<VALUE> Items_AccessorCache;");
    }

    return code.toString();
}

ln::String RubyExtGenerator::makeAccessorCacheFieldMarks(const TypeSymbol* classSymbol) const {
    OutputBuffer code;

    for (auto& prop : classSymbol->properties()) {
        if (prop->type()->isClass()) {
            if (prop->getter()) {
                code.AppendLine(U"rb_gc_mark(obj->{0});", makeAccessorCacheName(prop->getter()));
            }
        }
    }

    if (classSymbol->isCollection() && classSymbol->collectionItemType()->isClass()) {
        code.AppendLine(U"for(VALUE& v : obj->Items_AccessorCache) rb_gc_mark(v);");
    }

    return code.toString();
}

ln::String RubyExtGenerator::makeWrapFuncImplement(const TypeSymbol* classSymbol, const MethodOverloadInfo* overloadInfo) const {
    OutputBuffer code;

    // Func header
    code.AppendLine(U"static VALUE {0}(int argc, VALUE* argv, VALUE self)", makeWrapFuncName(overloadInfo->representative()));

    // begin body
    code.AppendLine(U"{");
    code.IncreaseIndent();

    // インスタンスメソッドの場合は、関数先頭で this を表すオブジェクトを Data_Get_Struct で取り出しておく
    if (overloadInfo->representative()->isInstance()) {
        code.AppendLine(U"{0}* selfObj;", makeWrapStructName(overloadInfo->representative()->ownerType()));
        code.AppendLine(U"Data_Get_Struct(self, {0}, selfObj);", makeWrapStructName(overloadInfo->representative()->ownerType()));
    }

    // Body 作成。if () { ～ } までのオーバーロード呼び出し1つ分
    for (auto& method : overloadInfo->methods()) {
        if (method->isFieldAccessor()) {
            code.AppendLine(makeWrapFuncCallBlock_FieldAccessor(classSymbol, method));
        }
        else if (classSymbol->isDelegateObject() && method->isConstructor()) {
            code.AppendLine(makeWrapFuncCallBlock_DelegateObjectConstructor(classSymbol, method));
        }
        else {
            code.AppendLine(makeWrapFuncCallBlock(classSymbol, method));
        }

        if (method->parameters().size() == 1 && method->parameters().front()->type()->isDelegateObject()) {
            code.AppendLine(makeWrapFuncCallBlock_DelegateObjectSetter(classSymbol, method));
        }
    }

    // 関数終端まで到達してしまったら例外
    code.AppendLine(U"rb_raise(rb_eArgError, \"{0} - wrong argument type.\");", overloadInfo->representative()->fullName());
    code.AppendLine(U"return Qnil;");

    // end body
    code.DecreaseIndent();
    code.AppendLine(U"}").NewLine();

    return code.toString();
}

ln::String RubyExtGenerator::makeWrapFuncCallBlock(const TypeSymbol* classSymbol, const MethodSymbol* method) const {
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
    int optionalArgsCount = 0;       // デフォルト引数の数
    OutputBuffer rb_scan_args_Decls; // rb_scan_args により引数情報を格納する VALUE 型ローカル変数宣言
    OutputBuffer rb_scan_args_Args;  // rb_scan_args の実引数リスト
    OutputBuffer rb_scan_args_TypeChecks;
    rb_scan_args_TypeChecks.setCommandSeparator(U" && ");

    // API 1つの呼び出しの各引数について、宣言、実引数渡し、後処理、return文 を作っていく
    OutputBuffer callerArgDecls;
    OutputBuffer callerArgList;
    OutputBuffer callerPostStmt;
    OutputBuffer callerReturnStmt;

    {
        for (auto& param : method->flatParameters()) {
            if (param->isThis()) {
                if (classSymbol->isStruct())
                    callerArgList.AppendCommad(U"selfObj");
                else
                    callerArgList.AppendCommad(U"selfObj->handle");
            }
            else if (param->isReturn()) {
                // return として選択されている out 属性の引数である場合

                // コンストラクタの最後の引数 (LNTexture_Create(x, y, &tex) の tex) は、WrapStruct::Handle へ格納する
                if (method->isConstructor()) {
                    callerArgList.AppendCommad(U"&selfObj->handle");

                    // コンストラクトされた Object を register
                    callerPostStmt.AppendLine(U"LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);");
                    callerReturnStmt.AppendLine(U"return Qnil;");
                }
                else {
                    auto localVarName = U"_" + param->name();
                    // decl
                    callerArgDecls.AppendLine(U"{0} {1};", makeFlatTypeName2(param->type()), localVarName);
                    // arg
                    callerArgList.AppendCommad(U"&" + localVarName);
                    // return
                    callerReturnStmt.AppendLine(makeVALUEReturnExpr(param->type(), method, localVarName));
                }
            }
            else {
                if (param->defaultValue())
                    optionalArgsCount++;
                else
                    requiredArgsCount++;

                // rb_scan_args の格納先 VALUE 宣言
                rb_scan_args_Decls.AppendLine(U"VALUE {0};", param->name());
                // rb_scan_args の引数
                rb_scan_args_Args.AppendCommad(U"&{0}", param->name());
                // type check expr
                rb_scan_args_TypeChecks.AppendCommad(makeTypeCheckExpr(param->type(), param->name()));

                // C変数宣言 & 初期化代入
                callerArgDecls.AppendLine(makeVALUEToNativeCastDecl(param));
                // API実引数
                if (param->type()->isStruct())
                    callerArgList.AppendCommad(U"&_" + param->name()); // struct はポインタ渡し
                else if (param->isOut())
                    callerArgList.AppendCommad(U"&_" + param->name()); // Primitive 型の出力引数はポインタ渡し
                else
                    callerArgList.AppendCommad(U"_" + param->name());

                // FlatC call args
            }
        }
    }

    OutputBuffer code;
    code.append(U"if ({0} <= argc && argc <= {1}) {{", requiredArgsCount, requiredArgsCount + optionalArgsCount).NewLine();
    code.IncreaseIndent();

    // call rb_scan_args and verify
    {
        code.AppendLine(rb_scan_args_Decls.toString().trim());

        if (!rb_scan_args_Args.isEmpty()) {
            code.AppendLine(U"rb_scan_args(argc, argv, \"{0}{1}\", {2});", requiredArgsCount, (optionalArgsCount > 0) ? ln::String::fromNumber(optionalArgsCount).c_str() : U"", rb_scan_args_Args.toString());
        }

        if (!rb_scan_args_TypeChecks.isEmpty()) {
            code.AppendLine(U"if ({0})", rb_scan_args_TypeChecks.toString());
        }
    }

    ln::String funcName = (method->isVirtual()) ? makeFlatAPIName_CallOverrideBase(classSymbol, method, FlatCharset::Ascii) : makeFlatFullFuncName(method, FlatCharset::Ascii);

    code.AppendLine(U"{");
    code.IncreaseIndent();

    code.AppendLine(callerArgDecls.toString().trim());
    code.AppendLine(U"LNResult errorCode = {0}({1});", funcName, callerArgList.toString());
    code.AppendLine(U"if (errorCode < 0) rb_raise(rb_eRuntimeError, \"Lumino runtime error. (%d)\\n%s\", errorCode, LNRuntime_GetLastErrorMessage());");
    if (!callerPostStmt.isEmpty()) {
        code.AppendLine(callerPostStmt.toString());
    }
    if (method->isConstructor()) {
        // call given block for cascade initializer.
        code.AppendLine(U"if (rb_block_given_p()) rb_yield(self);");
    }
    code.AppendLine((callerReturnStmt.isEmpty()) ? U"return Qnil;" : callerReturnStmt.toString().trim());
    code.DecreaseIndent();
    code.AppendLine(U"}");

    code.DecreaseIndent();
    code.append(U"}");

    return code.toString();
}

ln::String RubyExtGenerator::makeWrapFuncCallBlock_FieldAccessor(const TypeSymbol* classSymbol, const MethodSymbol* method) const {
    if (method->parameters().size() == 1) {
        // setter
        MethodParameterSymbol* param = method->parameters()[0];

        OutputBuffer code;
        code.AppendLine(U"if (argc == 1) {");
        code.IncreaseIndent();
        {
            code.AppendLine(U"VALUE value;");
            code.AppendLine(U"rb_scan_args(argc, argv, \"1\", &value);");
            code.AppendLine(U"if ({0}) {{", makeTypeCheckExpr(param->type(), U"value"));
            code.IncreaseIndent();
            {
                code.AppendLine(makeVALUEToNativeCastDecl(param));

                code.AppendLine(U"selfObj->{0} = _value;", method->linkedField()->name());

                code.AppendLine(U"return Qnil;");
            }
            code.DecreaseIndent();
            code.AppendLine(U"}");
        }
        code.DecreaseIndent();
        code.AppendLine(U"}");
        return code.toString().trim();
    }
    else {
        // getter
        QualType returnType = method->returnType();

        OutputBuffer code;
        code.AppendLine(U"if (argc == 0) {");
        code.IncreaseIndent();
        {
            code.AppendLine(makeVALUEReturnExpr(returnType.type, method, U"selfObj->" + method->linkedField()->name()));
        }
        code.DecreaseIndent();
        code.AppendLine(U"}");
        return code.toString().trim();
    }
}

ln::String RubyExtGenerator::makeWrapFuncCallBlock_DelegateObjectConstructor(const TypeSymbol* classSymbol, const MethodSymbol* method) const {
    OutputBuffer code;
    code.AppendLine(U"if (0 <= argc && argc <= 1) {");
    code.IncreaseIndent();
    {
        code.AppendLine(U"VALUE proc, block;");
        code.AppendLine(U"rb_scan_args(argc, argv, \"01&\", &proc, &block); // (handler=nil, &block)");
        code.AppendLine(U"if (proc != Qnil) selfObj->m_proc = proc;");
        code.AppendLine(U"if (block != Qnil) selfObj->m_proc = block;");
        code.AppendLine(U"LNResult result = {0}({1}, &selfObj->handle);", makeFlatFullFuncName(method, FlatCharset::Ascii), makeWrapFuncName_ProcCaller(classSymbol, classSymbol->delegateProtoType()));
        code.AppendLine(U"if (result < 0) rb_raise(rb_eRuntimeError, \"Lumino runtime error. (%d)\\n%s\", result, LNRuntime_GetLastErrorMessage());");
        code.AppendLine(U"LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);");
        code.AppendLine(U"return Qnil;");
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    return code.toString().trim();
}

ln::String RubyExtGenerator::makeWrapFuncCallBlock_DelegateObjectSetter(const TypeSymbol* classSymbol, const MethodSymbol* method) const {
    auto& param = method->parameters().front();

    OutputBuffer code;
    code.AppendLine(U"if (argc == 0) {");
    code.IncreaseIndent();
    {
        code.AppendLine(U"VALUE block;");
        code.AppendLine(U"rb_scan_args(argc, argv, \"0&\", &block);");
        code.AppendLine(U"if (block != Qnil) {");
        code.IncreaseIndent();
        {
            if (method->hasReturnType()) {
                // とりいそぎ、戻り値 EventConnection 固定
                code.AppendLine(U"VALUE value = rb_funcall({0}, rb_intern(\"new\"), 1, block);", makeRubyClassInfoVariableName(param->type()));
                code.AppendLine(U"LNHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);");
                code.AppendLine(U"LNHandle _outReturn;");
                code.AppendLine(U"LNResult result = {0}(selfObj->handle, _value, &_outReturn);", makeFlatFullFuncName(method, FlatCharset::Ascii));
                code.AppendLine(U"if (result < 0) rb_raise(rb_eRuntimeError, \"Lumino runtime error. (%d)\\n%s\", result, LNRuntime_GetLastErrorMessage());");
                code.AppendLine(U"return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);");
            }
            else {
                code.AppendLine(U"VALUE value = rb_funcall({0}, rb_intern(\"new\"), 1, block);", makeRubyClassInfoVariableName(param->type()));
                code.AppendLine(U"LNHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);");
                code.AppendLine(U"LNResult result = {0}(selfObj->handle, _value);", makeFlatFullFuncName(method, FlatCharset::Ascii));
                code.AppendLine(U"if (result < 0) rb_raise(rb_eRuntimeError, \"Lumino runtime error. (%d)\\n%s\", result, LNRuntime_GetLastErrorMessage());");
                code.AppendLine(U"return Qnil;");
            }
        }
        code.DecreaseIndent();
        code.AppendLine(U"}");
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    return code.toString().trim();
}

// C言語変数 → VALUE 変換式の作成 (return 用)
ln::String RubyExtGenerator::makeVALUEReturnExpr(const TypeSymbol* type, const MethodSymbol* method, const ln::String& varName) const {
    // primitive type
    if (type->isPrimitive()) {
        return ln::format(U"return LNI_TO_RUBY_VALUE({0});", varName);
    }

    // enum type
    if (type->isEnum()) {
        return ln::format(U"return INT2FIX({0});", varName);
    }

    // struct type
    if (type->isStruct()) {
        // 新しいインスタンスを作って返す
        OutputBuffer output;
        output.AppendLine(U"VALUE retObj = {0}_allocate({1});", makeFlatClassName(type), makeRubyClassInfoVariableName(type));
        output.AppendLine(U"*(({0}*)DATA_PTR(retObj)) = {1};", makeFlatClassName(type), varName);
        output.AppendLine(U"return retObj;");
        return output.toString();
    }
    // class type
    else if (type->isClass()) {
        if (method->returnType().strongReference) {
            return ln::format(U"return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN({0});", varName);
        }
        else if (method->isPropertyGetter() && !method->isStatic()) {
            return ln::format(U"return LNRB_HANDLE_WRAP_TO_VALUE({0}, selfObj->{1});", varName, makeAccessorCacheName(method));
        }
        else if (method->isCollectionGetItem()) {
            return ln::format(U"return LNRB_HANDLE_WRAP_TO_VALUE({0}, selfObj->Items_AccessorCache, _index);", varName);
        }
        else {
            return ln::format(U"return LNRB_HANDLE_WRAP_TO_VALUE({0});", varName);
        }
    }

    LN_UNREACHABLE();
    return ln::String::Empty;
}

ln::String RubyExtGenerator::makeTypeCheckExpr(const TypeSymbol* type, const ln::String& varName) const {
    if (type == PredefinedTypes::boolType) {
        return ln::format(U"LNRB_VALUE_IS_BOOL({0})", varName);
    }
    else if (
        type == PredefinedTypes::intType ||
        type == PredefinedTypes::int16Type ||
        type == PredefinedTypes::uint32Type ||
        type == PredefinedTypes::intptrType) {
        return ln::format(U"LNRB_VALUE_IS_NUMBER({0})", varName);
    }
    else if (
        type == PredefinedTypes::floatType ||
        type == PredefinedTypes::doubleType) {
        return ln::format(U"LNRB_VALUE_IS_FLOAT({0})", varName);
    }
    else if (type->isString()) {
        return ln::format(U"LNRB_VALUE_IS_STRING({0})", varName);
    }
    else if (type->isEnum()) {
        return ln::format(U"LNRB_VALUE_IS_NUMBER({0})", varName);
    }
    else if (type->isClass() || type->isStruct() || type->isDelegateObject()) {
        return ln::format(U"LNRB_VALUE_IS_OBJECT({0})", varName);
    }
    else {
        LN_NOTIMPLEMENTED();
        return ln::String::Empty;
    }
}

ln::String RubyExtGenerator::makeVALUEToNativeCastExpr(const MethodParameterSymbol* param, const ln::String& varName) const {
    auto type = param->type();
    //auto varName = U"_" + param->name();
    ln::String castExpr;

    if (type->isStruct()) {
        LN_NOTIMPLEMENTED();
    }
    else if (type->isClass() || type->isDelegateObject()) {
        castExpr = ln::format(U"LuminoRubyRuntimeManager::instance->getHandle({0})", varName);
    }
    else if (type->isEnum()) {
        castExpr = ln::format(U"({0})FIX2INT({1})", makeFlatClassName(type), varName);
    }
    else {
        if (type == PredefinedTypes::boolType) {
            castExpr = ln::format(U"LNRB_VALUE_TO_BOOL({0})", varName);
        }
        else if (
            type == PredefinedTypes::intType ||
            type == PredefinedTypes::int16Type ||
            type == PredefinedTypes::uint32Type ||
            type == PredefinedTypes::intptrType) {
            castExpr = ln::format(U"LNRB_VALUE_TO_NUMBER({0})", varName);
        }
        else if (
            type == PredefinedTypes::floatType ||
            type == PredefinedTypes::doubleType) {
            castExpr = ln::format(U"LNRB_VALUE_TO_FLOAT({0})", varName);
        }
        else if (type->isString()) {
            castExpr = ln::format(U"LNRB_VALUE_TO_STRING({0})", varName);
        }
        else if (type->isEnum()) {
            castExpr = ln::format(U"LNRB_VALUE_TO_NUMBER({0})", varName);
        }
        else {
            LN_NOTIMPLEMENTED();
            return ln::String::Empty;
        }
    }

    return castExpr;
}

// Flat API コールに渡す引数用変数の宣言文を作成する。(VALUE を C の型に変換する)
ln::String RubyExtGenerator::makeVALUEToNativeCastDecl(const MethodParameterSymbol* param) const {
    auto type = param->type();
    auto varName = U"_" + param->name();

    ln::String declExpr;

    if (type->isStruct()) {
        auto t = ln::format(U"{0}* tmp_{1}; Data_Get_Struct({2}, {0}, tmp_{1});", makeFlatClassName(type), varName, param->name());
        t = t + ln::format(U"{0}& {1} = *tmp_{1};", makeFlatClassName(type), varName);
        return t;
    }

    ln::String castExpr = makeVALUEToNativeCastExpr(param, param->name());

    if (type->isClass() || type->isDelegateObject()) {
        declExpr = ln::format(U"LNHandle {0}", varName);
    }
    else if (type->isEnum()) {
        declExpr = ln::format(U"{0} {1}", makeFlatClassName(type), varName);
    }
    else {
        if (type == PredefinedTypes::boolType) {
            declExpr = U"LNBool " + varName;
        }
        else if (
            type == PredefinedTypes::intType ||
            type == PredefinedTypes::int16Type ||
            type == PredefinedTypes::uint32Type ||
            type == PredefinedTypes::intptrType) {
            declExpr = ln::format(U"{0} {1}", type->shortName(), varName);
        }
        else if (
            type == PredefinedTypes::floatType ||
            type == PredefinedTypes::doubleType) {
            declExpr = ln::format(U"{0} {1}", type->shortName(), varName);
        }
        else if (type->isString()) {
            declExpr = U"const char* " + varName;
        }
        else if (type->isEnum()) {
            declExpr = ln::format(U"{0} {1}", type->shortName(), varName);
        }
        else {
            LN_NOTIMPLEMENTED();
            return ln::String::Empty;
        }
    }

    if (param->defaultValue()) {
        return ln::format(U"{0} = ({1} != Qnil) ? {2} : {3};", declExpr, param->name(), castExpr, makeFlatConstantValue(param->defaultValue()));
    }
    else {
        return ln::format(U"{0} = {1};", declExpr, castExpr);
    }
}

// ruby API コールに渡す引数用変数の宣言文を作成する。(C の型 を VALUE に変換する)
ln::String RubyExtGenerator::makeNativeToVALUECastDecl(const MethodParameterSymbol* param) const {
    if (param->type()->isClass()) {
        return ln::format(U"LNRB_HANDLE_WRAP_TO_VALUE({0})", param->name());
    }
    else {
        return ln::format(U"LNI_TO_RUBY_VALUE({0})", param->name());
    }
}

//
ln::String RubyExtGenerator::makeWrapFuncImplement_ProcCaller(const MethodSymbol* delegateProtoType) const {
    auto wrapStructName = makeWrapStructName(delegateProtoType->ownerType());
    auto procCallerName = makeWrapFuncName_ProcCaller(delegateProtoType->ownerType(), delegateProtoType);
    auto procValueName = U"m_proc";

    OutputBuffer argList;
    for (auto& param : delegateProtoType->parameters()) {
        argList.AppendCommad(makeNativeToVALUECastDecl(param));
    }

    OutputBuffer code;
    code.AppendLine(U"static LNResult {0}({1})", procCallerName, makeFlatParamList(delegateProtoType, FlatCharset::Ascii));
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        code.AppendLine(U"{0}* selfObj;", wrapStructName);
        code.AppendLine(U"Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE({0}), {1}, selfObj);", delegateProtoType->flatThisParam()->name(), wrapStructName);
        code.AppendLine(U"VALUE retval = rb_funcall(selfObj->{0}, rb_intern(\"call\"), {1}, {2});", procValueName, delegateProtoType->parameters().size(), argList.toString());

        if (delegateProtoType->hasReturnType()) {
            code.AppendLine(U"*outReturn = {0};", makeVALUEToNativeCastExpr(delegateProtoType->flatReturnParam(), U"retval"));
        }

        code.AppendLine(U"return LN_OK;	// TODO: error handling.", wrapStructName);
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    code.NewLine();
    return code.toString();
}

// LNWorldObject_OnUpdate_SetOverrideCallback() などに登録するコールバック関数の生成
ln::String RubyExtGenerator::makeWrapFuncImplement_SetOverrideCallback(const TypeSymbol* classSymbol) const {
    /*
	出力例：
	```
	LNResult Wrap_LNInterpreter_OnSerialize_OverrideCallback(LNHandle object, LNHandle ar)
	{
		VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
		VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
		return LN_OK;
	}
	```

	出力例：
	```
	LNResult Wrap_LNInterpreter_OnUpdateWait_OverrideCallback(LNHandle interpreter, LNBool* outReturn)
	{
		VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(interpreter);
		VALUE retval = rb_funcall(obj, rb_intern("on_update_wait"), 1);
		*outReturn = ;
		return LN_OK;
	}
	```
	*/
    OutputBuffer code;

    for (auto& method : classSymbol->leafVirtualMethods()) {
        if (method->isVirtual()) {
            // Func header
            {
                // make params
                OutputBuffer params;
                for (auto& param : method->flatParameters()) {
                    params.AppendCommad(U"{0} {1}", makeFlatCParamQualTypeName(method, param, FlatCharset::Unicode), param->name());
                }
                code.AppendLine(U"LNResult {0}({1})", makeWrapFuncName_OverrideCallback(classSymbol, method), params.toString());
            }

            // begin body
            code.AppendLine(U"{");
            code.IncreaseIndent();

            // make args
            OutputBuffer args;
            int argsCount = 0;
            for (int i = 1; i < method->flatParameters().size(); i++) {
                const auto& param = method->flatParameters()[i];
                if (!param->isReturn()) {
                    args.AppendCommad(makeNativeToVALUECastDecl(param));
                    argsCount++;
                }
            }
            if (argsCount == 0) {
                args.AppendCommad(U"0"); // 引数0個の時のダミー
            }

            code.AppendLine(U"VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE({0});", method->flatParameters().front()->name());
            code.AppendLine(U"VALUE retval = rb_funcall(obj, rb_intern(\"{0}\"), {1}, {2});", makeRubyMethodName(method), argsCount, args.toString());

            if (method->returnType().type != PredefinedTypes::voidType) {
                code.AppendLine(U"*outReturn = {0};", makeVALUEToNativeCastExpr(method->flatReturnParam(), U"retval"));
            }

            code.AppendLine(U"return LN_OK;");

            // end body
            code.DecreaseIndent();
            code.AppendLine(U"}");
        }
    }

    return code.toString();
}

// TODO: makeWrapFuncImplement_ProcCaller を使ってもいいかも
ln::String RubyExtGenerator::makeWrapFuncImplement_SignalCaller(const MethodSymbol* method) const {
    auto wrapStructName = makeWrapStructName(method->ownerType());
    auto signalCallerName = makeWrapFuncName_SignalCaller(method->ownerType(), method);
    auto signalValueName = makeSignalValueName(method);

    OutputBuffer code;
    code.AppendLine(U"static void {0}(LNHandle self, LNHandle e)", signalCallerName);
    code.AppendLine(U"{");
    code.IncreaseIndent();
    {
        code.AppendLine(U"{0}* selfObj;", wrapStructName);
        code.AppendLine(U"Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(self), {0}, selfObj);", wrapStructName);
        code.AppendLine(U"rb_funcall(selfObj->{0}, rb_intern(\"raise\"), 1, LNRB_HANDLE_WRAP_TO_VALUE(e));", signalValueName);
    }
    code.DecreaseIndent();
    code.AppendLine(U"}");
    code.NewLine();
    return code.toString();
}

//ln::String RubyExtGenerator::makeWrapFuncImplement_EventConnector(const MethodSymbol* method) const
//{
//	auto wrapStructName = makeWrapStructName(method->ownerType());
//	auto signalCallerName = makeWrapFuncName_SignalCaller(method->ownerType(), method);
//	auto connectorName = makeFlatShortFuncName(method, FlatCharset::Ascii);	// e.g) ConnectOnClicked
//	auto signalValueName = makeSignalValueName(method);
//	auto eventConnectValueName = makeEventConnectValueName(method);
//
//	OutputBuffer code;
//	code.AppendLine(U"static VALUE Wrap_LNUIButton_{0}(int argc, VALUE* argv, VALUE self)", connectorName);
//	code.AppendLine(U"{");
//	code.IncreaseIndent();
//	{
//		code.AppendLine(U"{0}* selfObj;", wrapStructName);
//		code.AppendLine(U"Data_Get_Struct(self, {0}, selfObj);", wrapStructName);
//
//		code.AppendLine(U"if (!selfObj->{0}) {{  // differed initialization.", eventConnectValueName);
//		code.IncreaseIndent();
//		{
//			code.AppendLine(U"selfObj->{0} = rb_funcall(LuminoRubyRuntimeManager::instance->eventSignalClass(), rb_intern(\"new\"), 0);", signalValueName);
//			code.AppendLine(U"{0}(selfObj->handle, {1});", makeFlatFullFuncName(method, FlatCharset::Ascii), signalCallerName);
//			code.AppendLine(U"selfObj->{0} = true;", eventConnectValueName);
//		}
//		code.DecreaseIndent();
//		code.AppendLine(U"}");
//		code.NewLine();
//
//		code.AppendLine(U"VALUE handler, block;");
//		code.AppendLine(U"rb_scan_args(argc, argv, \"01&\", &handler, &block);	// (handler=nil, &block)");
//		code.AppendLine(U"if (handler != Qnil) rb_funcall(selfObj->{0}, rb_intern(\"add\"), 1, handler);", signalValueName);
//		code.AppendLine(U"if (block != Qnil) rb_funcall(selfObj->{0}, rb_intern(\"add\"), 1, block);", signalValueName);
//		code.AppendLine(U"return Qnil;");
//	}
//	code.DecreaseIndent();
//	code.AppendLine(U"}");
//	code.NewLine();
//	return code.toString();
//}

ln::String RubyExtGenerator::makeEnumTypeVALUEVariableDecls() const {
    OutputBuffer code;

    // VALUE global variables
    for (auto& symbol : db()->enums()) {
        code.AppendLine(U"VALUE {0};", makeEnumTypeVALUEName(symbol));
    }

    return code.toString().trim();
}

ln::String RubyExtGenerator::makeEnumTypeRegisterCode() const {
    OutputBuffer code;

    for (auto& symbol : db()->enums()) {
        auto varName = makeEnumTypeVALUEName(symbol);

        // enum module
        code.AppendLine(U"{0} = rb_define_module_under(g_rootModule, \"{1}\");", varName, symbol->shortName());

        // enum member
        for (auto& member : symbol->constants()) {
            code.AppendLine(U"rb_define_const({0}, \"{1}\", INT2FIX({2})); ", varName, makeUpperSnakeName(member->name()), member->value()->get<int>());
        }
        code.NewLine();
    }

    return code.toString().trim();
}

//ln::String RubyExtGenerator::makeEventSignalDefinition() const
//{
//	OutputBuffer code;
//	code.AppendLine(U"int state = 0;");
//	code.AppendLine(U"rb_eval_string_protect(");
//	code.IncreaseIndent();
//
//	ln::StreamReader r(makeTemplateFilePath(U"RubyEventSignalClass.template.rb"));
//	ln::String line;
//	while (r.readLine(&line)) {
//		code.AppendLine(U"\"{0}\\n\"", line);
//	}
//	code.AppendLine(U",");
//	code.AppendLine(U"&state);");
//
//	code.DecreaseIndent();
//	return code.toString();
//}

//==============================================================================
// RubyYARDOCSourceGenerator

void RubyYARDOCSourceGenerator::generate() {
    OutputBuffer code;

    for (auto& enumSymbol : db()->enums()) {
        code.AppendLine(U"# " + enumSymbol->shortName());
        code.AppendLine(U"module " + makeRubyTypeFullName(enumSymbol));
        code.IncreaseIndent();
        for (auto& member : enumSymbol->constants()) {
            code.AppendLine(U"# " + member->document()->summary());
            code.AppendLine(U"{0} = {1}", makeUpperSnakeName(member->name()), member->value()->get<int>());
        }
        code.DecreaseIndent();
        code.AppendLine(U"end");
        code.NewLine();
    }

    for (auto& structSymbol : db()->structs()) {
        code.AppendLine(U"# " + structSymbol->document()->summary());
        code.AppendLine(U"# ");
        code.append(U"class " + makeRubyTypeFullName(structSymbol));
        code.NewLine();
        code.IncreaseIndent();

        for (auto& overload : structSymbol->overloads()) {
            code.AppendLines(makeMethodDoc(overload));
            code.NewLine();
        }

        code.DecreaseIndent();
        code.AppendLine(U"end");
        code.NewLine();
    }

    for (auto& classSymbol : db()->classes()) {
        code.AppendLine(U"# " + classSymbol->document()->summary());
        code.AppendLine(U"# ");
        code.append(U"class " + makeRubyTypeFullName(classSymbol));
        if (classSymbol->baseClass() && classSymbol->isRootObjectClass()) {
            code.append(U" < " + classSymbol->baseClass()->shortName());
        }
        code.NewLine();
        code.IncreaseIndent();

        for (auto& overload : classSymbol->overloads()) {
            code.AppendLines(makeMethodDoc(overload));
            code.NewLine();
        }

        code.DecreaseIndent();
        code.AppendLine(U"end");
        code.NewLine();
    }

    ln::String fileName = ln::format(U"APIReference/{0}.RubyYARDOCSource.generated.rb", config()->moduleName);
    ln::String src = code.toString();
    ln::FileSystem::writeAllText(makeOutputFilePath(U"Ruby", fileName), src);
}

ln::String RubyYARDOCSourceGenerator::makeMethodDoc(const MethodOverloadInfo* overloadInfo) const {
    OutputBuffer code;

    auto representative = overloadInfo->representative();
    auto rubyMethodName = makeRubyMethodName(representative);

    // Overall overview
    code.AppendLine(U"# " + translateText(representative->document()->summary()));
    if (!representative->document()->details().isEmpty()) {
        code.AppendLinesHeaderd(translateText(representative->document()->details()), U"#   ");
    }

    for (auto& method : overloadInfo->methods()) {
        auto paramNames = OutputBuffer();
        auto params = OutputBuffer();
        for (auto& param : method->parameters()) {
            paramNames.AppendCommad(param->name());

            params.AppendLine(U"@param [{0}] {1} {2}", makeRubyTypeFullName(param->type()), param->name(), translateText(param->document()->summary()));
        }
        auto returns = ln::String::Empty;
        if (method->returnType().type != PredefinedTypes::voidType) {
            returns = ln::format(U"@return [{0}] {1}", makeRubyTypeFullName(method->returnType().type), method->document()->returns());
        }

        if (overloadInfo->methods().size() == 1) {
            // dno't have override
            if (!params.isEmpty()) code.AppendLinesHeaderd(params.toString(), U"# ");
            if (!returns.isEmpty()) code.AppendLine(U"# " + returns);
        }
        else {
            // dno't have override
            code.AppendLine(U"# @overload {0}({1})", rubyMethodName, paramNames.toString());
            code.AppendLine(U"#   " + translateText(method->document()->summary()));
            if (!method->document()->details().isEmpty()) {
                code.AppendLinesHeaderd(translateText(method->document()->details()), U"#     ");
            }
            if (!params.isEmpty()) code.AppendLinesHeaderd(params.toString(), U"#   ");
            if (!returns.isEmpty()) code.AppendLine(U"#   " + returns);
        }
    }

    code.AppendLine(U"def {0}(*args)", rubyMethodName);
    code.AppendLine(U"end");
    code.NewLine();

    return code.toString();
}

ln::String RubyYARDOCSourceGenerator::makeRubyTypeFullName(const TypeSymbol* type) const {
    if (type->isPrimitive()) {
        std::unordered_map<const TypeSymbol*, ln::String> rubyTypeMap = {
            //{ PredefinedTypes::voidType, U"" },
            //{ PredefinedTypes::nullptrType, U"" },
            { PredefinedTypes::boolType, U"Boolean" },
            { PredefinedTypes::intType, U"Integer" },
            { PredefinedTypes::int16Type, U"Integer" },
            { PredefinedTypes::uint32Type, U"Integer" },
            { PredefinedTypes::intptrType, U"Integer" },
            { PredefinedTypes::floatType, U"Float" },
            { PredefinedTypes::stringType, U"String" },
            //{ PredefinedTypes::stringRefType, U"" },
            //{ PredefinedTypes::objectType, U"Object" },
            //{ PredefinedTypes::EventConnectionType, U"" },
        };

        return rubyTypeMap[type];
    }
    else {
        return ln::format(U"{0}::{1}", config()->moduleName, type->shortName());
    }
}

ln::String RubyYARDOCSourceGenerator::translateText(const ln::String& text) const {
    // TODO:
    return text;
}
