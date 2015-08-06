
#pragma once

#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{

/// フォントの管理クラスの実装
///		実装クラスを分けているのは、FreeType のヘッダ類を公開しないようにするため。
class FontManagerImpl
	: public FontManager
{
public:
	FontManagerImpl(FileManager* fileManager);
	~FontManagerImpl();

private:
	RefPtr<FileManager>	m_fileManager;
};

} // namespace Imaging
} // namespace Lumino
