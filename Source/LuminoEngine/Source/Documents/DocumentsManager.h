
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

	static DocumentsManager* GetInstance(DocumentsManager* priority = nullptr);

public:
	DocumentsManager();
	virtual ~DocumentsManager();
	void initialize(const ConfigData& configData);
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	EncodingConverter* GetTCharToUTF32Converter() { return &m_TCharToUTF32Converter; }	// TODO: fontmanager ‚ÌŽg‚¦‚Î‚¢‚¢‚©‚à
	EncodingConverter* GetUTF32ToTCharConverter() { return &m_UTF32ToTCharConverter; }

private:
	GraphicsManager*		m_graphicsManager;
	EncodingConverter		m_TCharToUTF32Converter;
	EncodingConverter		m_UTF32ToTCharConverter;
};

} // namespace detail
LN_NAMESPACE_END
