#pragma once
#include "String.hpp"

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

    bool isEmpty() const;

    const std::vector<byte_t>& data() const { return m_data; }

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

// for unordered_map key
namespace std {
template<>
struct hash<ln::Uuid>
{
    std::size_t operator()(const ln::Uuid& key) const;
};
} // namespace std
