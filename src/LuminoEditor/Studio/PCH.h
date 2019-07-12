#pragma once

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>

template<class T>
using Ref = ln::Ref<T>;


#include <QtGui>
#include <QtWidgets>

inline QString LnToQt(const ln::String& value) { return QString((QChar*)value.c_str()); }
inline QString LnToQt(const ln::Path& value) { return QString((QChar*)value.str().c_str()); }
inline ln::String QtToLn(const QString& value) { return ln::String((ln::Char*)value.constData()); }