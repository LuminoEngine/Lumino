
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

/**
	@brief
*/
class DocumentsManager
	: public RefObject
{
public:
	struct ConfigData
	{
		GraphicsManager*	graphicsManager = nullptr;
	};

	static DocumentsManager* getInstance(DocumentsManager* priority = nullptr);

public:
	DocumentsManager();
	virtual ~DocumentsManager();
	void initialize(const ConfigData& configData);
	GraphicsManager* getGraphicsManager() const { return m_graphicsManager; }
	EncodingConverter* getTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }	// TODO: fontmanager ‚ÌŽg‚¦‚Î‚¢‚¢‚©‚à
	EncodingConverter* getUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }

private:
	GraphicsManager*		m_graphicsManager;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;
};

} // namespace detail
LN_NAMESPACE_END
