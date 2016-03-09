#include "../headers/app_guard.hpp"
#include "../headers/app.hpp"

#include <exception>
#include <functional>

namespace frija
{

namespace details
{

Application *app_ptr;

void callFinish()
{
	app_ptr->finish(frija::Application::UNEXPECTED_EXCEPTION);
}

}

template<typename T, typename ...Args>
auto run(int argc, char *argv[], Args ...args) -> int
{
	details::app_ptr = new T(args...);

	std::set_terminate(callFinish);

	details::app_ptr->init();

	int result = 0;
	if(details::app_ptr->getFlags() & Application::USE_LOOP)
	{
		do
		{
			result = details::app_ptr->loop(result);
		} while(result > 0);
	}
	else if(details::app_ptr->getFlags() & Application::USE_ARG_EXEC)
	{
		details::app_ptr->parse(argc, argv);
	}
	else
		result = details::app_ptr->main();

	details::app_ptr->finish(0);

	delete details::app_ptr;
	return result;
}

}