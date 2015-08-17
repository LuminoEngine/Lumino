/**
	@file	ASyncIOObject.h
*/
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/IO/Common.h>

namespace Lumino
{
class Exception;
class FileManager;

/**
	@brief	
*/
class ASyncIOObject
	: public RefObject
{
public:
	ASyncIOObject();
	virtual ~ASyncIOObject();

public:

	/**
		@brief		非同期処理の状態を取得します。
	*/
	ASyncIOState GetASyncIOState() const;
	
	/**
		@brief		処理終了後、例外が発生した場合はその例外を返します。
		@return		例外が発生していなければ NULL を返します。
	*/
	Exception* GetASyncIOException() const;

protected:

	/**
		@brief		処理内容を実装します。
		@details	この関数は直接呼び出さないでください。
					RequestASyncIOProc() を呼び出すことで、必要なタイミングで実行されます。
	*/
	virtual void OnASyncIOProc() = 0;

	/**
		@brief		処理を実行します。
	*/
	void InvokeIOProc(bool isASync, FileManager* fileManager = NULL);

private:
	friend class FileManager;
	ASyncIOState	m_ayncIOState;
	Exception*		m_ayncIOException;
};

} // namespace Lumino
