
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class ReadWriteMutexImpl; }

/**
	@brief	複数の読み取りロックと、1つの書き込みロックを行う Mutex
*/
class ReadWriteMutex
{
public:
	ReadWriteMutex();
	~ReadWriteMutex();

public:

	/**
		@brief	読み取りロック
	*/
	void lockRead();

	/**
		@brief	読み取りアンロック
	*/
	void unlockRead();

	/**
		@brief	書き込みロック
	*/
	void lockWrite();

	/**
		@brief	書き込みアンロック
	*/
	void unlockWrite();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ReadWriteMutex);
	detail::ReadWriteMutexImpl*	m_impl;
};

LN_NAMESPACE_END
