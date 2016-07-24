#ifndef __APPLICATION_GUARDIAN__
#define __APPLICATION_GUARDIAN__

#include "app.hpp"

namespace hyronx
{
namespace appman
{
namespace details
{

class ApplicationGuardian
{
public:
    ApplicationGuardian(Application *app_ptr);

    auto operator()(int argc, char *argv[]) -> int;
    auto work(const unsigned int &flags) -> int;

    static void finish(int reason);
    static void terminate();

private:
    static Application *app_ptr;
    int app_argc;
    char **app_argv;
};

}
}
}

#endif //!__APPLICATION_GUARDIAN__
