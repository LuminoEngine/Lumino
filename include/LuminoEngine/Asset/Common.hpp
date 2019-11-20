
#pragma once

namespace ln {

/** アセットが保存されている場所へのアクセス優先度 */
enum class AssetStorageAccessPriority
{
    /** 開発用。Assets フォルダの外側のローカルファイルへのアクセスを許可する */
    AllowLocalDirectory,

	/** ディレクトリ優先 */
	DirectoryFirst,

	/** アーカイブファイル優先 */
	ArchiveFirst,

	/** アーカイブファイルのみ */
	ArchiveOnly,
};

} // namespace ln
