
#include "Internal.hpp"
#include <LuminoEngine/Base/Regex.hpp>

#define PCRE2_STATIC
#if LN_USTRING32
#define PCRE2_CODE_UNIT_WIDTH 32
#else
#define PCRE2_CODE_UNIT_WIDTH 16
#endif
#include <pcre2.h>

// https://www.pcre.org/current/doc/html/pcre2demo.html

namespace ln {

static bool matchInternal(pcre2_code* re, uint32_t options, const StringRef& input, void** outResult)
{
	if (LN_REQUIRE(re)) {
		return false;
	}
	if (input.isEmpty()) {
		return false;
	}

	PCRE2_UCHAR* subject = (PCRE2_UCHAR*)input.data();
	pcre2_match_data* matchData = pcre2_match_data_create_from_pattern(re, NULL);
	int rc = pcre2_match(
		re,
		subject,
		input.length(),
		0,
		options,
		matchData,
		NULL);
	if (rc == PCRE2_ERROR_NOMATCH) {
		pcre2_match_data_free(matchData);
		return false;
	}
	else if (rc < 0) {
		pcre2_match_data_free(matchData);
		LN_ENSURE(0, "Regex engine assertion. (%d)", rc);
		return false;
	}

	if (outResult)
	{
		(*outResult) = matchData;
	}
	else
	{
		pcre2_match_data_free(matchData);
	}

	return true;
}

//==============================================================================
// Regex

Regex::Regex(const StringRef& pattern)
	: m_pcre(nullptr)
{
	if (LN_REQUIRE(!pattern.isEmpty())) {
		return;
	}

	int errorcode = 0;	// pcre2_compile で、正常終了でも 100 が格納される。エラーを示す値は 101 以上で、PCRE2_ERROR_ が定義されている。
	PCRE2_SIZE errorffset = 0;
	pcre2_code* re = pcre2_compile((PCRE2_UCHAR*)pattern.data(), pattern.length(), PCRE2_EXTENDED | PCRE2_UTF, &errorcode, &errorffset, NULL);
	if (!re) {
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
		ln::String message = (Char*)buffer;
		LN_ENSURE(0, "PCRE2 compilation failed at offset %d: %s\n", (int)errorffset, message.toStdString().c_str());
		return;
	}
	m_pcre = re;
}

Regex::~Regex()
{
	if (m_pcre) {
		pcre2_code* re = reinterpret_cast<pcre2_code*>(m_pcre);
		pcre2_code_free(re);
		m_pcre = nullptr;
	}
}

bool Regex::match(const StringRef& input, MatchResult* outResult) const
{
	if (outResult) {
		outResult->m_subject = input.data();
	}
	return matchInternal(reinterpret_cast<pcre2_code*>(m_pcre), PCRE2_ANCHORED | PCRE2_ENDANCHORED, input, (outResult) ? &outResult->m_matchData : nullptr);
}

bool Regex::search(const StringRef& input, MatchResult* outResult) const
{
	if (outResult) {
		outResult->m_subject = input.data();
	}
	return matchInternal(reinterpret_cast<pcre2_code*>(m_pcre), 0, input, (outResult) ? &outResult->m_matchData : nullptr);
}

bool Regex::match(const StringRef& input, const StringRef& pattern, MatchResult* outResult)
{
	Regex re(pattern);
	return re.match(input, outResult);
}

bool Regex::search(const StringRef& input, const StringRef& pattern, MatchResult* outResult)
{
	Regex re(pattern);
	return re.search(input, outResult);
}


//==============================================================================
// MatchResult

MatchResult::MatchResult()
	: m_matchData(nullptr)
{
}

MatchResult::~MatchResult()
{
	if (m_matchData) {
		pcre2_match_data_free(reinterpret_cast<pcre2_match_data*>(m_matchData));
		m_matchData = nullptr;
	}
}

int MatchResult::position() const
{
	if (LN_REQUIRE(m_matchData)) return 0;
	if (LN_REQUIRE(groupCount() > 0)) return 0;

	pcre2_match_data* matchData = reinterpret_cast<pcre2_match_data*>(m_matchData);
	size_t* ovector = pcre2_get_ovector_pointer(matchData);
	return ovector[0];
}

int MatchResult::length() const
{
	if (LN_REQUIRE(m_matchData)) return 0;
	if (LN_REQUIRE(groupCount() > 0)) return 0;

	pcre2_match_data* matchData = reinterpret_cast<pcre2_match_data*>(m_matchData);
	size_t* ovector = pcre2_get_ovector_pointer(matchData);
	return ovector[1] - ovector[0];
}

StringRef MatchResult::value() const
{
	return groupValue(0);
}

int MatchResult::groupCount() const
{
	if (LN_REQUIRE(m_matchData)) return 0;
	pcre2_match_data* matchData = reinterpret_cast<pcre2_match_data*>(m_matchData);
	return pcre2_get_ovector_count(matchData);
}

StringRef MatchResult::groupValue(int index) const
{
	if (LN_REQUIRE(m_matchData)) return StringRef();
	if (LN_REQUIRE(index < groupCount())) return StringRef();

	pcre2_match_data* matchData = reinterpret_cast<pcre2_match_data*>(m_matchData);
	size_t* ovector = pcre2_get_ovector_pointer(matchData);

	PCRE2_UCHAR* subject = (PCRE2_UCHAR*)m_subject;

	PCRE2_SPTR start = subject + ovector[2 * index];
	size_t length = ovector[2 * index + 1] - ovector[2 * index];

	return StringRef((const Char*)start, length);
}

} // namespace ln

