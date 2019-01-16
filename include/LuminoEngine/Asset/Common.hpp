
#pragma once

namespace ln {

/** アセットが保存されている場所へのアクセス優先度 */
enum class AssetStorageAccessPriority
{
	/** ディレクトリ優先 */
	DirectoryFirst,

	/** アーカイブファイル優先 */
	ArchiveFirst,

	/** アーカイブファイルのみ */
	ArchiveOnly,
};

} // namespace ln
