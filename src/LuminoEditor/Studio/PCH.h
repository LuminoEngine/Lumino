#pragma once

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <QtGui>
#include <QtWidgets>

inline QString LnToQt(const ln::String& str) { return QString((QChar*)str.c_str()); }
inline ln::String QtToLn(const QString& str) { return ln::String((ln::Char*)str.constData()); }
