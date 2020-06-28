
#pragma once

namespace ln {
class Assets;

/** アセットが保存されている場所へのアクセス優先度 */
enum class AssetStorageAccessPriority
{
	/** ディレクトリ優先。Assets フォルダの外側のローカルファイルへのアクセスを許可する */
	DirectoryFirst,

	/** アーカイブファイル優先 */
	ArchiveFirst,

	/** アーカイブファイルのみ */
	ArchiveOnly,
};

} // namespace ln
