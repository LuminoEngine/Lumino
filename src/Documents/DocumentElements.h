
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {
class DocumentsManager;

/**
	@brief
*/
class Document
	: public Object
{
public:
	Document();
	virtual ~Document();
	void Initialize(DocumentsManager* manager);

private:
	DocumentsManager*	m_manager;
};

/**
	@brief
*/
class TextElement
	: public Object
{
public:
	TextElement();
	virtual ~TextElement();
	void Initialize(DocumentsManager* manager);

private:
	DocumentsManager*	m_manager;
};

/**
	@brief	コンテンツをグループ化して段落にするために使用される。
*/
class Paragraph
	: public TextElement
{
public:
	Paragraph();
	virtual ~Paragraph();
	void Initialize(DocumentsManager* manager);

private:
	// Inline List
};

/**
	@brief
*/
class Inline
	: public TextElement
{
public:
	Inline();
	virtual ~Inline();
	void Initialize(DocumentsManager* manager);

private:
};

/**
	@brief
*/
class Run
	: public Inline
{
public:
	Run();
	virtual ~Run();
	void Initialize(DocumentsManager* manager);

	//virtual SizeF Measure();
	//virtual void Render(DocumentsRenderer* renderer);

private:
	StringBuilder	m_text;
};

/**
	@brief
*/
class Span
	: public Inline
{
public:
	Span();
	virtual ~Span();
	void Initialize(DocumentsManager* manager);

private:
	// Inline List
};

} // namespace detail
LN_NAMESPACE_END
