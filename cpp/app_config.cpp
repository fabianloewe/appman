#include "../headers/app_config.hpp"

#include <iostream>

using hyronx::appman::ApplicationConfig;

ApplicationConfig::ApplicationConfig(const std::string &path, std::ios::openmode mode) :
    ac_file(path)
{
    if(!ac_file.is_open())
        ac_file.open(path, mode);
    else
        ac_file >> ac_config;
}

ApplicationConfig::~ApplicationConfig()
{
    close();
}

void ApplicationConfig::open(const std::string &path, std::ios::openmode mode)
{
    if(!ac_file.is_open())
        ac_file.open(path, mode);
    
    std::string buffer;    
    ac_file >> buffer;
    if(!buffer.empty())
        ac_config = nlohmann::json::parse(buffer);
}

auto ApplicationConfig::isValid() const noexcept -> bool
{
    return (ac_config.empty()) ? false : true;
}

auto ApplicationConfig::get() -> nlohmann::json &
{
    return ac_config;
}

auto ApplicationConfig::get() const -> const nlohmann::json &
{
    return ac_config;
}

void ApplicationConfig::update()
{
    ac_file << ac_config;
    ac_file.flush();
}

void ApplicationConfig::close()
{
    update();
    ac_file.close();
}
