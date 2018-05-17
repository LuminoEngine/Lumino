// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once

#include "Lumino/Base/Common.hpp"
#include "Lumino/Base/Assertion.hpp"
#include "Lumino/Base/Buffer.hpp"
#include "Lumino/Base/Common.hpp"
#include "Lumino/Base/CRCHash.hpp"
#include "Lumino/Base/EnumFlags.hpp"
#include "Lumino/Base/Environment.hpp"
#include "Lumino/Base/Event.hpp"
#include "Lumino/Base/List.hpp"
#include "Lumino/Base/Locale.hpp"
#include "Lumino/Base/Logger.hpp"
#include "Lumino/Base/Optional.hpp"
#include "Lumino/Base/Platform.hpp"
#include "Lumino/Base/RefObject.hpp"
#include "Lumino/Base/StlHelper.hpp"
#include "Lumino/Base/String.hpp"
#include "Lumino/Base/StringHelper.hpp"
#include "Lumino/Base/Uuid.hpp"
#include "Lumino/Base/Variant.hpp"

#include "Lumino/IO/Common.hpp"
#include "Lumino/IO/CommandLineParser.hpp"
#include "Lumino/IO/FileStream.hpp"
#include "Lumino/IO/FileSystem.hpp"
#include "Lumino/IO/Path.hpp"
#include "Lumino/IO/Process.hpp"
#include "Lumino/IO/Stream.hpp"
#include "Lumino/IO/StreamReader.hpp"
#include "Lumino/IO/StreamWriter.hpp"
#include "Lumino/IO/StringReader.hpp"
#include "Lumino/IO/StringWriter.hpp"
#include "Lumino/IO/TextReader.hpp"
#include "Lumino/IO/TextWriter.hpp"

#include "Lumino/Math/Common.hpp"
#include "Lumino/Math/AttitudeTransform.hpp"
#include "Lumino/Math/Geometries.hpp"
#include "Lumino/Math/Math.hpp"
#include "Lumino/Math/Matrix.hpp"
#include "Lumino/Math/Plane.hpp"
#include "Lumino/Math/Quaternion.hpp"
#include "Lumino/Math/Random.hpp"
#include "Lumino/Math/Vector2.hpp"
#include "Lumino/Math/Vector3.hpp"
#include "Lumino/Math/Vector4.hpp"
#include "Lumino/Math/ViewFrustum.hpp"

#include "Lumino/Text/Common.hpp"
#include "Lumino/Text/Encoding.hpp"
#include "Lumino/Text/EncodingConverter.hpp"

#include "Lumino/Json/JsonReader.hpp"
#include "Lumino/Json/JsonWriter.hpp"

#include "Lumino/Serialization/Common.hpp"
#include "Lumino/Serialization/ArchiveStore.hpp"
#include "Lumino/Serialization/Serialization.hpp"

#ifdef _MSC_VER

#ifdef _DEBUG
#define LN_MSVC_BUILD_CONFIG "Debug"
#else
#define LN_MSVC_BUILD_CONFIG "Release"
#endif

#ifdef _WIN64
#define LN_MSVC_BUILD_ARCH "x64"
#else
#define LN_MSVC_BUILD_ARCH "x86"
#endif

#ifdef _DLL
#define LN_MSVC_BUILD_RUNTIMELIBRARY "MD"
#else
#define LN_MSVC_BUILD_RUNTIMELIBRARY "MT"
#endif

#pragma comment(lib, "MSVC2017-" LN_MSVC_BUILD_ARCH "-" LN_MSVC_BUILD_RUNTIMELIBRARY "/" LN_MSVC_BUILD_CONFIG "/LuminoCore.lib")

#endif
