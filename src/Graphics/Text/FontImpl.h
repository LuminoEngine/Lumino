
#pragma once

#include <Lumino/Imaging/Font.h>

namespace Lumino
{
namespace Imaging
{
class Bitmap;

/// ビットマップフォント
class BitmapFontImpl
	: public Font
{
public:
	BitmapFontImpl(Bitmap* bitmap);
	virtual ~BitmapFontImpl();

};

} // namespace Imaging
} // namespace Lumino
