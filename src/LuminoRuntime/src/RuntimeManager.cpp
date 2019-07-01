#include "RuntimeManager.hpp"

namespace ln {
namespace detail {

static RuntimeManager* s_instance = nullptr;

RuntimeManager* RuntimeManager::instance()
{
	assert(s_instance);
	return s_instance;
}

RuntimeManager::RuntimeManager()
{
}

RuntimeManager::~RuntimeManager()
{
}

void RuntimeManager::init(const Settings& settings)
{
	assert(!s_instance);
	s_instance = this;
}

void RuntimeManager::dispose()
{
	s_instance = nullptr;
}

LnResult RuntimeManager::procException(Exception* e)
{
	return LN_ERROR_UNKNOWN;
}

} // namespace detail
} // namespace ln
