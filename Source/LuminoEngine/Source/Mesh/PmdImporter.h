
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class BinaryReader;
class Texture2D;
namespace detail { class ModelManager; }

class PmdImporter
{
public:
	PmdImporter();
	virtual ~PmdImporter();
};

LN_NAMESPACE_END

