#include "Common.h"

int main()
{
	RefPtr<Application> app(Application::Create());

	do
	{

	} while (app->UpdateFrame());

	return 0;
}
