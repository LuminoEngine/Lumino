
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
using namespace ln;

class UISandboxApp : public Application
{
public:

private:
};

int UISandboxMain()
{
	UISandboxApp app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
	return 0;
}




