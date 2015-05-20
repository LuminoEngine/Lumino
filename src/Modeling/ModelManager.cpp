
#include "../Internal.h"
#include "ModelManager.h"

namespace Lumino
{
namespace Modeling
{

//=============================================================================
// Exception
//=============================================================================

static const unsigned char toon01Data[] =
{
#include "Resource/toon01.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon01Data);

static const unsigned char toon02Data[] =
{
#include "Resource/toon02.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon02Data);

static const unsigned char toon03Data[] =
{
#include "Resource/toon03.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon03Data);

static const unsigned char toon04Data[] =
{
#include "Resource/toon04.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon04Data);

static const unsigned char toon05Data[] =
{
#include "Resource/toon05.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon05Data);

static const unsigned char toon06Data[] =
{
#include "Resource/toon06.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon06Data);

static const unsigned char toon07Data[] =
{
#include "Resource/toon07.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon07Data);

static const unsigned char toon08Data[] =
{
#include "Resource/toon08.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon08Data);

static const unsigned char toon09Data[] =
{
#include "Resource/toon09.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon09Data);

static const unsigned char toon10Data[] =
{
#include "Resource/toon10.png.h"
};
static const size_t codeLen = LN_ARRAY_SIZE_OF(toon10Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelManager::ModelManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ModelManager::~ModelManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelManager::Initialize(const ConfigData& configData)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ModelManager::Finalize()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Graphics::Texture* ModelManager::GetMMDDefaultToonTexture(int index)
{ 
	LN_ASSERT(0 <= index && index < 10);
	return m_mmdDefaultToonTexture[index];
}

} // namespace Modeling
} // namespace Lumino
