#pragma once

namespace ln {
class Texture2D;
namespace detail {

class AssetManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	AssetManager();
	virtual ~AssetManager();
	void initialize(const Settings& settings);
	void dispose();

	bool existsFile(const StringRef& filePath) const;
	Ref<Texture2D> loadTexture(const StringRef& filePath);

    Ref<Stream> openFileStream(const StringRef& filePath);

private:
};

} // namespace detail
} // namespace ln

