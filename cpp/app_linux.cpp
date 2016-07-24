#include "../headers/app.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <fstream>
#include <cstring>

static std::string home_path;

using hyronx::appman::Application;

auto Application::getConfigDirectory() const noexcept -> std::string
{
    return app_config.get()["config_dir"];
}

auto Application::getDataDirectory() const noexcept -> std::string
{
    return app_config.get()["data_dir"];
}

auto Application::getCacheDirectory() const noexcept -> std::string
{
    return app_config.get()["tmp_data_dir"];
}

auto Application::getPluginDirectory() const noexcept -> std::string
{
    return app_config.get()["plugin_dir"];
}

void Application::installService(std::string)
{
//    SC_HANDLE manager = OpenSCManager(NULL, NULL, GENERIC_WRITE);

}

void Application::uninstallService(std::string)
{
    std::cout << "uninstalling..." << std::endl;
}

void Application::startService(std::string)
{
    std::cout << "starting..." << std::endl;
}

void Application::stopService(std::string)
{
    std::cout << "stopping..." << std::endl;
}

void Application::loadUserConfig()
{
    const char *home = std::getenv("HOME");
    home_path = (!home) ? "" : home;

    std::string config_dir = findConfigDirectory();
    app_config.open(config_dir + "/config.json");
    if(!app_config.isValid())
    {
        app_config.open(config_dir + "/config.json", std::ios::out);
        
        auto &config = app_config.get();
        config["config_dir"] = std::move(config_dir);
        config["data_dir"] = findDataDirectory();
        config["tmp_data_dir"] = findCacheDirectory();
        config["plugin_dir"] = findPluginDirectory();

        app_config.update();
    }
}

auto Application::findConfigDirectory() const noexcept -> std::string
{
    struct stat sb;
    char *env = std::getenv("XDG_CONFIG_HOME");
    std::string config_path = (!env) ? "" : env;
    if(!config_path.empty())
    {
        config_path += "/" + app_name;
        mkdir(config_path.c_str(), S_IRWXU | S_IRWXG);
        return config_path;
    }
    else
    {
        config_path = home_path + "/.config";
        stat(config_path.c_str(), &sb);
        if(S_ISDIR(sb.st_mode))
        {
            config_path += "/" + app_name;
            mkdir(config_path.c_str(), S_IRWXU | S_IRWXG);
            return config_path;
        }
    }

    config_path = home_path + "/." + app_name + "/config";
    mkdir(config_path.c_str(), S_IRWXU | S_IRWXG);
    return config_path;
}

auto Application::findDataDirectory() const noexcept -> std::string
{
    struct stat sb;
    char *env = std::getenv("XDG_DATA_HOME");
    std::string data_path = (!env) ? "" : env;
    if(!data_path.empty())
    {
        data_path += "/" + app_name;
        mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
        return data_path;
    }
    else
    {
        data_path = home_path + "/.local/share";
        stat(data_path.c_str(), &sb);
        if(S_ISDIR(sb.st_mode))
        {
            data_path += "/" + app_name;
            mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
            return data_path;
        }
    }

    data_path = home_path + "/." + app_name + "/data";
    mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
    return data_path;
}

auto Application::findCacheDirectory() const noexcept -> std::string
{
    struct stat sb;
    char *env = std::getenv("XDG_CACHE_HOME");
    std::string tmp_data_path = (!env) ? "" : env;
    if(!tmp_data_path.empty())
    {
        tmp_data_path += "/" + app_name;
        mkdir(tmp_data_path.c_str(), S_IRWXU | S_IRWXG);
        return tmp_data_path;
    }
    else
    {
        tmp_data_path = home_path + "/.cache";
        stat(tmp_data_path.c_str(), &sb);
        if(S_ISDIR(sb.st_mode))
        {
            tmp_data_path += "/" + app_name;
            mkdir(tmp_data_path.c_str(), S_IRWXU | S_IRWXG);
            return tmp_data_path;
        }
    }

    tmp_data_path = home_path + "/." + app_name + "/cache";
    mkdir(tmp_data_path.c_str(), S_IRWXU | S_IRWXG);
    return tmp_data_path;
}

auto Application::findPluginDirectory() const noexcept -> std::string
{
    struct stat sb;
    char *env = std::getenv("XDG_DATA_HOME");
    std::string data_path = (!env) ? "" : env;
    if(!data_path.empty())
    {
        data_path += "/" + app_name;
        mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
        return data_path;
    }
    else
    {
        data_path = home_path + "/.local/share";
        stat(data_path.c_str(), &sb);
        if(S_ISDIR(sb.st_mode))
        {
            data_path += "/" + app_name + "-plugins";
            mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
            return data_path;
        }
    }

    data_path = home_path + "/." + app_name + "/plugins";
    mkdir(data_path.c_str(), S_IRWXU | S_IRWXG);
    return data_path;
}

void Application::freeCache()
{
    std::function<void(std::string)> cleaner = [&cleaner](std::string dir_name)
    {
        dir_name += '/';
        auto pos = dir_name.find_last_of('/') + 1;
        
        auto cache_dir = opendir(dir_name.c_str());
        if(cache_dir == nullptr)
            return;
        
        struct dirent *dir_info = nullptr;
        while((dir_info = readdir(cache_dir)) != nullptr)
        {
            std::string name = dir_info->d_name;
            if(dir_info->d_type == DT_REG)
            {
                if(dir_name.length() == pos)
                    dir_name += std::move(name);
                else
                    dir_name.replace(pos, dir_name.length(), std::move(name));
                    
                if(unlink(dir_name.c_str()) == -1)
                    std::cout << "Cannot unlink " << dir_name << "! Error: " << std::strerror(errno) << std::endl;
            }
            else if(dir_info->d_type == DT_DIR)
            {
                if(name != ".." && name != ".")
                    cleaner(std::move(dir_name) + std::move(name));
            }
        }
        
        closedir(cache_dir);
    };  
    
    cleaner(getCacheDirectory());    
}

void Application::redirectStdOut()
{
    
}