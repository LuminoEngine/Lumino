
#pragma once

namespace ln {
namespace detail {

class ScriptingManager
    : public RefObject
{
public:
    struct Settings
    {
    };

    ScriptingManager();
    virtual ~ScriptingManager();
    void init(const Settings& settings);
    void dispose();


private:
};

} // namespace detail
} // namespace ln
