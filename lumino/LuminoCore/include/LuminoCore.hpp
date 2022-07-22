// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once

#include "LuminoCore/Common.hpp"
#include "LuminoCore/Base/Common.hpp"
#include "LuminoCore/Base/Assertion.hpp"
#include "LuminoCore/Base/Array.hpp"
#include "LuminoCore/Base/Buffer.hpp"
#include "LuminoCore/Base/Common.hpp"
#include "LuminoCore/Base/CRCHash.hpp"
#include "LuminoCore/Base/EnumFlags.hpp"
#include "LuminoCore/Base/ElapsedTimer.hpp"
#include "LuminoCore/Base/Environment.hpp"
#include "LuminoCore/Base/List.hpp"
#include "LuminoCore/Base/Locale.hpp"
#include "LuminoCore/Base/Logger.hpp"
#include "LuminoCore/Base/Optional.hpp"
#include "LuminoCore/Base/Platform.hpp"
#include "LuminoCore/Base/RefObject.hpp"
#include "LuminoCore/Base/URefObject.hpp"
#include "LuminoCore/Base/StlHelper.hpp"
#include "LuminoCore/Base/String.hpp"
#include "LuminoCore/Base/StringHelper.hpp"
#include "LuminoCore/Base/Uuid.hpp"
#include "LuminoCore/Base/Result.hpp"

#include "LuminoCore/IO/Common.hpp"
#include "LuminoCore/IO/BinaryReader.hpp"
#include "LuminoCore/IO/BinaryWriter.hpp"
#include "LuminoCore/IO/CommandLineParser.hpp"
#include "LuminoCore/IO/FileStream.hpp"
#include "LuminoCore/IO/FileSystem.hpp"
#include "LuminoCore/IO/MemoryStream.hpp"
#include "LuminoCore/IO/Path.hpp"
#include "LuminoCore/IO/Process.hpp"
#include "LuminoCore/IO/Stream.hpp"
#include "LuminoCore/IO/StreamReader.hpp"
#include "LuminoCore/IO/StreamWriter.hpp"
#include "LuminoCore/IO/StringReader.hpp"
#include "LuminoCore/IO/StringWriter.hpp"
#include "LuminoCore/IO/TextReader.hpp"
#include "LuminoCore/IO/TextWriter.hpp"
#include "LuminoCore/IO/Console.hpp"

#include "LuminoCore/Math/Common.hpp"
#include "LuminoCore/Math/AttitudeTransform.hpp"
#include "LuminoCore/Math/Geometries.hpp"
#include "LuminoCore/Math/Math.hpp"
#include "LuminoCore/Math/Matrix.hpp"
#include "LuminoCore/Math/Plane.hpp"
#include "LuminoCore/Math/Quaternion.hpp"
#include "LuminoCore/Math/Random.hpp"
#include "LuminoCore/Math/Vector2.hpp"
#include "LuminoCore/Math/Vector3.hpp"
#include "LuminoCore/Math/Vector4.hpp"
#include "LuminoCore/Math/ViewFrustum.hpp"

#include "LuminoCore/Geometries/GeometryStructs.hpp"

#include "LuminoCore/Text/Common.hpp"
#include "LuminoCore/Text/Encoding.hpp"
#include "LuminoCore/Text/EncodingConverter.hpp"

#include "LuminoCore/Threading/Common.hpp"
#include "LuminoCore/Threading/Thread.hpp"
#include "LuminoCore/Threading/ConditionEvent.hpp"

#include "LuminoCore/Json/Common.hpp"
#include "LuminoCore/Json/JsonReader.hpp"
#include "LuminoCore/Json/JsonWriter.hpp"

#include "LuminoCore/Serialization/Common.hpp"
#include "LuminoCore/Serialization/ArchiveStore.hpp"

#include "LuminoCore/Runtime/Object.hpp"
#include "LuminoCore/Runtime/TypeInfo.hpp"
#include "LuminoCore/Runtime/Property.hpp"
#include "LuminoCore/Runtime/Archive.hpp"
#include "LuminoCore/Runtime/RuntimeContext.hpp"

#ifdef LN_MSVC_AUTO_LINK_LIBRARIES
#ifdef _MSC_VER

#ifdef _DEBUG
#define LN_MSVC_LIBRARY_POSTFIX "d"
#else
#define LN_MSVC_LIBRARY_POSTFIX
#endif

#define LN_MSVC_MAKE_LIB_PATH(name) name LN_MSVC_LIBRARY_POSTFIX ".lib"

#pragma comment(lib, LN_MSVC_MAKE_LIB_PATH("LuminoCore"))

#endif
#endif

#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Userenv.lib")
