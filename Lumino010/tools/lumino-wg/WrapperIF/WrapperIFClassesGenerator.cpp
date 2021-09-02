
#include "FlatCCommon.hpp"
#include "WrapperIFClassesGenerator.hpp"

static const ln::String wrapperIFClassTemplate =
	"class LN%ClassName% : public %ClassName%\n"
	"{\n"
	"public:\n"
	"\n"
	"    void postInitialize()\n"
	"    {\n"
	"%PostInitializeExps%\n"
	"    }\n"
	"\n"
	"%EventWrappers%\n"
	"\n"
	"%OverrideFuncPtrs%\n"
	"\n"
	"%OverrideMethods%\n"
	"};\n"
	"%OverrideFuncPtrImpls%\n";

void WrapperIFClassesGenerator::generate()
{
	// save classes
	{
		ln::String src = ln::FileSystem::readAllText(makeTemplateFilePath(_T("WrapperIF.h")));
		src = src.replace("%OverrideCallerFuncPtrs%", GenerateOverrideCallerFuncPtrs());
		src = src.replace("%WrapperClasses%", GenerateWrapperIFClasses());

		ln::String fileName = ln::String::format("{0}.Wrapper.generated.h", moduleFullName());
		ln::FileSystem::writeAllText(makeOutputFilePath(fileName).c_str(), src);
	}
}

ln::String WrapperIFClassesGenerator::GenerateWrapperIFClasses()
{
	OutputBuffer classes;
	for (auto& classInfo : db()->classes)
	{
		m_eventWrappers.clear();
		m_eventWrappers.IncreaseIndent();
		m_eventWrapperConnectExps.clear();
		m_eventWrapperConnectExps.IncreaseIndent(2);

		if (!classInfo->isStatic())
		{
			OutputBuffer overrideCallersDecl(1);
			OutputBuffer overrideCallersImpl(0);
			OutputBuffer overrideMethods(1);
			for (auto& methodInfo : classInfo->declaredMethods)
			{
				if (methodInfo->isVirtual)
				{
					// member variable decl
					ln::String typeName = ln::String::format("{0}_{1}_OverrideCaller", classInfo->shortName(), methodInfo->name);
					ln::String varName = ln::String::format("m_{0}_OverrideCaller", methodInfo->name);
					overrideCallersDecl.append("static {0} {1};", typeName, varName).NewLine();

					// member variable impl
					overrideCallersImpl.append("{0} LN{1}::{2} = nullptr;", typeName, classInfo->shortName(), varName).NewLine();

					// call args
					OutputBuffer args;
					for (auto& paramInfo : methodInfo->capiParameters)
					{
						if (paramInfo->isThis)
							args.AppendCommad("LWIG_TO_HANDLE(this)");
						else
							LN_NOTIMPLEMENTED();
					}

					// member method
					overrideMethods.append("virtual {0} {1}() override", FlatCCommon::makeCppTypeName(methodInfo->returnType), methodInfo->name).NewLine();
					overrideMethods.append("{").NewLine();
					overrideMethods.IncreaseIndent();
					overrideMethods.append("{0}({1});", varName, args.toString()).NewLine();
					overrideMethods.DecreaseIndent();
					overrideMethods.append("}").NewLine();

					// member method
					overrideMethods.append("template<typename... TArgs>").NewLine();
					overrideMethods.append("{0} {1}_CallBase(TArgs... args)", FlatCCommon::makeCppTypeName(methodInfo->returnType), methodInfo->name).NewLine();
					overrideMethods.append("{").NewLine();
					overrideMethods.IncreaseIndent();
					overrideMethods.append("return {0}::{1}(args...);", methodInfo->owner->shortName(), methodInfo->name).NewLine();
					overrideMethods.DecreaseIndent();
					overrideMethods.append("}").NewLine();
				}
				else if (methodInfo->IsEventSetter())
				{
					// ※Event は引数1つが前提
					auto delegateClass = methodInfo->parameters[0]->type;
					auto invokeMethod = delegateClass->declaredMethods[0];

					// make params
					OutputBuffer params;
					OutputBuffer funcParams;
					OutputBuffer args;
					for (auto& paramInfo : invokeMethod->parameters)
					{
						params.AppendCommad("{0} {1}", FlatCCommon::makeFlatCParamTypeName(invokeMethod, paramInfo), paramInfo->name);
						funcParams.AppendCommad("{0} {1}", FlatCCommon::makeCppTypeName(paramInfo->type), paramInfo->name);

						if (paramInfo->type->IsClass())
							args.AppendCommad("LWIG_TO_HANDLE({0})", paramInfo->name);
						else
							args.AppendCommad("{0}", paramInfo->name);
					}
					m_eventWrappers.AppendLine("Event<void(LNHandle self, {0})> {1};", params.toString(), MakeEventWrapperMemberVariableName(methodInfo));

					// wrapper method
					ln::String eventCallbackName = ln::String::format("{0}_EventCallback", methodInfo->name);
					m_eventWrappers.AppendLines(
						"void {0}_EventCallback({1})\n"
						"{{\n"
						"    {2}.Raise(LWIG_TO_HANDLE(this), {3});\n"
						"}}\n",
						methodInfo->name,
						funcParams.toString(),
						MakeEventWrapperMemberVariableName(methodInfo),
						args.toString());

					// initialize
					m_eventWrapperConnectExps.AppendLine("{0}(CreateDelegate(this, &LN{1}::{2}));", methodInfo->name, classInfo->shortName(), eventCallbackName);
				}
			}

			classes.append(wrapperIFClassTemplate
				.replace("%ClassName%", classInfo->shortName())
				.replace("%OverrideFuncPtrs%", overrideCallersDecl.toString())
				.replace("%OverrideMethods%", overrideMethods.toString())
				.replace("%OverrideFuncPtrImpls%", overrideCallersImpl.toString())
				.replace("%PostInitializeExps%", m_eventWrapperConnectExps.toString())
				.replace("%EventWrappers%", m_eventWrappers.toString()));
		}
	}

	return classes.toString();
}

ln::String WrapperIFClassesGenerator::GenerateOverrideCallerFuncPtrs()
{
	OutputBuffer funcPtrDefs;
	for (auto& classInfo : db()->classes)
	{
		if (!classInfo->isStatic())
		{
			for (auto& methodInfo : classInfo->declaredMethods)
			{
				if (!methodInfo->isVirtual) continue;

				// make params
				OutputBuffer params;
				for (auto& paramInfo : methodInfo->capiParameters)
				{
					params.AppendCommad("{0} {1}", FlatCCommon::makeFlatCParamTypeName(methodInfo, paramInfo), paramInfo->name);
				}

				funcPtrDefs.append("typedef LNResultCode (*{0}_{1}_OverrideCaller)({2});", classInfo->shortName(), methodInfo->name, params.toString()).NewLine();
				//funcPtrDefs.Append("{0}_OverrideCaller	m_{0}_OverrideCaller;", methodInfo->name).NewLine();
			}
		}
	}

	return funcPtrDefs.toString();
}

ln::String WrapperIFClassesGenerator::MakeEventWrapperMemberVariableName(ln::Ref<MethodSymbol> connectMethod)
{
	return _T("m_") + connectMethod->name + _T("_EventWrapper");
}
