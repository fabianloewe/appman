#include "../headers/app.hpp"
#include "../headers/app_guard.hpp"
#include "../headers/app_streams.hpp"

#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace hyronx::kommando;

namespace hyronx
{
namespace appman
{

ApplicationTaskScheduler ApplicationStream::as_sched;
ApplicationStream cout(std::cout.rdbuf());

Application::~Application()
{
    freeCache();
}

auto Application::getName() const noexcept -> const std::string &
{
    return app_name;
}

auto Application::getAuthor() const noexcept -> const std::string &
{
    return app_author;
}

auto Application::getPath() const noexcept -> const std::string &
{
    return app_path;
}

auto Application::getMinorVersion() const noexcept -> unsigned int
{
    return std::get<1>(app_version);
}

auto Application::getMajorVersion() const noexcept -> unsigned int
{
    return std::get<0>(app_version);
}

auto Application::getPatchLevel() const noexcept -> unsigned int
{
    return std::get<2>(app_version);
}

auto Application::getVersion() const noexcept -> const std::tuple<unsigned int, unsigned int, unsigned int> &
{
    return app_version;
}

auto Application::getFlags() const noexcept -> unsigned int
{
    return app_flags;
}

void Application::setName(std::string name) noexcept
{
    app_name = std::move(name);
}

void Application::setAuthor(std::string author_name) noexcept
{
    app_author =  std::move(author_name);
}

void Application::setMinorVersion(unsigned int minor_ver) noexcept
{
    std::get<1>(app_version) = minor_ver;
}

void Application::setMajorVersion(unsigned int major_ver) noexcept
{
    std::get<0>(app_version) = major_ver;
}

void Application::setPatchLevel(unsigned int patch_lvl) noexcept
{
    std::get<2>(app_version) = patch_lvl;
}

void Application::setVersion(unsigned int major_ver, unsigned int minor_ver, unsigned int patch_lvl) noexcept
{
    /* Creates a version in style x.y.z
    with x being major_ver
    with y being minor_ver
    with z being path_lvl */
    std::get<0>(app_version) = major_ver;
    std::get<1>(app_version) = minor_ver;
    std::get<2>(app_version) = patch_lvl;
}

void Application::setVersion(VersionNumber version) noexcept
{
    app_version = std::move(version);
}

void Application::setFlags(unsigned int flags)
{
    app_flags = flags;
}

auto Application::getArgumentManager() const noexcept -> const hyronx::kommando::ArgumentManager &
{
    return app_args;
}

auto Application::getUnexpectedException() -> std::exception_ptr
{
    std::exception_ptr except_ptr;
    try
    {
        throw;
    }
    catch(const std::exception &e)
    {
        except_ptr = std::current_exception();
    }
    return except_ptr;
}

void Application::handleUnexpectedException(const ExceptionHandler &except_handler)
{
    try
    {
        throw;
    }
    catch(const std::exception &e)
    {
        except_handler(e);
    }
}

void Application::addArgument(std::string s_arg, std::string l_arg, std::string cmd, std::string desc, std::string ex, ArgumentFlags flags, std::function<void(std::string)> func) noexcept
{
    app_args.add(s_arg, l_arg, cmd, desc, ex, flags, func);
}

Application::Application(unsigned int flags) :
    Application("", flags)
{
}

Application::Application(std::string app_name, unsigned int flags) :
    app_name(std::move(app_name)),
    app_flags(flags)
{
    app_args.getArgument("exit").func = [this](std::string string)
    {
        try
        {
            int exit_code = 0;
		    if(!string.empty())
			    exit_code = std::stoi(string);

		    exit(exit_code);
        }
        catch (const std::exception &e)
        {
            cout << "You must provide a number.\n";
        }
    };
    
    cout.setApplication(this);
    
    loadUserConfig();
    
    if(flags & APP_REDIRECT_STDOUT)
    {
        cout.registerMainThread(std::this_thread::get_id());
        cout.connectTo(std::cout);
    }
        
    if(flags & APP_SERVICE)
    {
        app_args.add("",
            "--install-service",
            "install-service",
            "Installs this application as a service",
            "example",
            ArgumentFlags(8),
            &Application::installService,
            this);

        app_args.add("",
            "--uninstall-service",
            "uninstall-service",
            "Uninstalls this service",
            "",
            8,
            &Application::uninstallService,
            this);

        app_args.add("",
            "--start-service",
            "start-service",
            "Starts this service",
            "",
            8,
            &Application::startService,
            this);

        app_args.add("",
            "--stop-service",
            "stop-service",
            "Stops this service",
            "",
            8,
            &Application::stopService,
            this);
    }
}

void Application::exit(int reason)
{
    details::ApplicationGuardian::finish(reason);
}

}
}
