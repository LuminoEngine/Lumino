﻿#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsManager;
class IGraphicsDeviceContext;
}

class LN_API GraphicsResource
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	GraphicsResource();
	virtual ~GraphicsResource();
	void initialize();

LN_INTERNAL_ACCESS:
	detail::GraphicsManager* manager() const { return m_manager; }
	detail::IGraphicsDeviceContext* deviceContext() const;

private:
	detail::GraphicsManager* m_manager;
};

} // namespace ln
