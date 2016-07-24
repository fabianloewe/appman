#include "../headers/app_guard.hpp"

#include <exception>
#include <functional>
#include <future>

using hyronx::appman::details::ApplicationGuardian;

hyronx::appman::Application *ApplicationGuardian::app_ptr;

ApplicationGuardian::ApplicationGuardian(hyronx::appman::Application *app)
{
    app_ptr = app;
}

auto ApplicationGuardian::operator ()(int argc, char *argv[]) -> int
{
    app_argc = argc;
    app_argv = argv;

    std::set_terminate(&ApplicationGuardian::terminate);

    app_ptr->init();

    unsigned int flags = app_ptr->getFlags();
    int result = 0;
    do
    {
        result = work(flags);
    }
    while(flags != app_ptr->getFlags());

    finish(0);
    return result;
}

auto ApplicationGuardian::work(const unsigned int &flags) -> int
{
    auto parser = app_ptr->getArgumentManager();
    if(flags & Application::APP_SERVICE)
    {
        parser.parse(app_argc, app_argv);
    }

    int result = 0;
    if(flags & Application::APP_MAIN)
    {
        result = app_ptr->main();
    }

    if(flags & Application::APP_ARG_EXEC)
    {
        if(flags & Application::APP_ARG_EXEC_LOOP)
            parser.loop(app_argc, app_argv, true);
        else
            parser.parse(app_argc, app_argv);
    }

    if(flags & Application::APP_LOOP)
    {
        do
        {
            result = app_ptr->loop(result);
        }
        while(result > 0);
    }

    return result;
}

void ApplicationGuardian::terminate()
{
    finish(Application::UNEXPECTED_EXCEPTION);
}

void ApplicationGuardian::finish(int reason)
{
    app_ptr->finish(reason);
    app_ptr->~Application();
    exit(reason);
}
