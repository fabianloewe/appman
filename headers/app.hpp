#ifndef __APPLICATION__
#define __APPLICATION__

#include <iostream>
#include <tuple>
#include <functional>

#include "../../kommando/kommando.hpp"

#include "app_config.hpp"

namespace hyronx
{
namespace appman
{

class Application
{
public:
	enum CloseReason
	{
        FINISHED                = 0,
        UNEXPECTED_EXCEPTION    = 0xFFFFFFFF,
	};

    enum Flags : unsigned int
	{
        APP_MAIN		    = 0x1,
        APP_LOOP		    = 0x2,
        APP_ARG_EXEC	    = 0x4,
        APP_SERVICE         = 0x8,
        APP_REDIRECT_STDOUT = 0x10,

        APP_ARG_EXEC_ONCE   = APP_ARG_EXEC,
        APP_ARG_EXEC_LOOP   = APP_ARG_EXEC | 0x100,
        APP_ARG_EXEC_SERV   = APP_ARG_EXEC | 0x200
	};

	using ExceptionHandler = std::function<void(const std::exception &)>;
	using VersionNumber = std::tuple<unsigned int, unsigned int, unsigned int>;

public:
	virtual ~Application();

    auto getName() const noexcept -> const std::string &;
    auto getAuthor() const noexcept -> const std::string &;

    auto getPath() const noexcept -> const std::string &;

    auto getConfig() const noexcept -> ApplicationConfig &;

    auto getConfigDirectory() const noexcept -> std::string;
    auto getDataDirectory() const noexcept -> std::string;
    auto getCacheDirectory() const noexcept -> std::string;
    auto getPluginDirectory() const noexcept -> std::string;

    auto getMinorVersion() const noexcept -> unsigned int;
    auto getMajorVersion() const noexcept  -> unsigned int;
    auto getPatchLevel() const noexcept  -> unsigned int;
    auto getVersion() const noexcept -> const VersionNumber &;

    auto getFlags() const noexcept -> unsigned int;

    auto getArgumentManager() const noexcept -> const hyronx::kommando::ArgumentManager &;

    auto getUnexpectedException() -> std::exception_ptr;
    void handleUnexpectedException(const ExceptionHandler &except_handler);

    void addArgument(std::string s_arg, std::string l_arg, std::string cmd, std::string desc, std::string ex, hyronx::kommando::ArgumentFlags flags, std::function<void(std::string)> func) noexcept;
    void addArgument(hyronx::kommando::Argument &arg) noexcept;

    void setName(std::string name) noexcept;
    void setAuthor(std::string author_name) noexcept;

    auto setConfigDirectory() noexcept -> std::string;
    auto setDataDirectory() noexcept -> std::string;
    auto setCacheDirectory() noexcept -> std::string;
    auto setPluginDirectory() noexcept -> std::string;

    void setMinorVersion(unsigned int version) noexcept;
    void setMajorVersion(unsigned int version) noexcept;
    void setPatchLevel(unsigned int version) noexcept;
    void setVersion(unsigned int major_ver, unsigned int minor_ver, unsigned int patch_lvl) noexcept;
    void setVersion(VersionNumber version) noexcept;

    void setFlags(unsigned int flags);

    void exit(int reason);

	virtual void init()
	{
	}

	virtual auto main() -> int
	{
		return 0;
	}

	virtual auto loop(int last_result) -> int
	{
        return last_result;
	}

	virtual void finish(int reason)
	{
	}

//	auto operator =(const Application &other) -> Application & = delete;
//	auto operator =(Application &&other) -> Application & = delete;

protected:
    Application(unsigned int flags = APP_MAIN | APP_REDIRECT_STDOUT);
    Application(std::string app_name, unsigned int flags);

    virtual void installService(std::string);
    virtual void uninstallService(std::string);
    virtual void startService(std::string);
    virtual void stopService(std::string);
    
    void freeCache();

private:
    kommando::ArgumentManager app_args;
    ApplicationConfig app_config;
	unsigned int app_flags;
    std::string app_name;
    std::string app_path;
    std::string app_author;
    VersionNumber app_version;

private:
    void loadUserConfig();

    auto findConfigDirectory() const noexcept -> std::string;
    auto findDataDirectory() const noexcept -> std::string;
    auto findCacheDirectory() const noexcept -> std::string;
    auto findPluginDirectory() const noexcept -> std::string;
    
    void redirectStdOut();
};

}
}

#endif // !__APPLICATION__
