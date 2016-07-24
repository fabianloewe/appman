#ifndef __APPLICATION_CONFIGURATION__
#define __APPLICATION_CONFIGURATION__

#include <string>
#include <fstream>

#include "../../json/src/json.hpp"

namespace hyronx
{
namespace appman
{

class ApplicationConfig
{
public:
    ApplicationConfig() = default;
    ApplicationConfig(const std::string &path, std::ios::openmode mode = std::ios::in | std::ios::out);
    virtual ~ApplicationConfig();

    void open(const std::string &path, std::ios::openmode mode = std::ios::in | std::ios::out);

    auto isValid() const noexcept -> bool;

    auto get() -> nlohmann::json &;
    auto get() const -> const nlohmann::json &;

    void update();

    void close();

private:
    std::fstream ac_file;
    nlohmann::json ac_config;
};

}
}

#endif // __APPLICATION_CONFIGURATION__
