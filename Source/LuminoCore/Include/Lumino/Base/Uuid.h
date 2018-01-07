#pragma once
#include "String.h"

namespace ln {

/**
	@brief	UUID (Universally Unique Identifier) のクラスです。
*/
class Uuid
{
public:
	
	/** 新しい UUID を生成します。*/
	static Uuid generate();

	/** 空の UUID を生成します。 */
	Uuid();

	/** 
		UUID 文字列を指定してインスタンスを生成します。

		形式は次のいずれかです。
		- 32 桁 : "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
		- ハイフンで区切られた 32 桁 :  "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
		- 中かっこで囲まれ、ハイフンで区切られた 32 桁 :  "{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}"
		- 丸かっこで囲まれ、ハイフンで区切られた 32 桁 :  "(xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)"
	*/
	Uuid(const StringRef& uuidText);
	
	/** 16Byte の配列を指定してインスタンスを生成します。*/
	Uuid(const uint8_t* bytes);

	/** 文字列に変換します。*/
	String toString() const;

	// copy
	Uuid(const Uuid &other);
	Uuid& operator=(const Uuid& other);

	// compare
	bool operator==(const Uuid& other) const;
	bool operator!=(const Uuid& other) const;

private:
	std::vector<byte_t> m_data;
};

} // namespace ln

