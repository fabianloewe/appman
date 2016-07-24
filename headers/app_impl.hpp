#ifndef __APPLICATION_RUN__
#define __APPLICATION_RUN__

#include <memory>
#include <mutex>

namespace hyronx
{

namespace appman
{

class Application;

template<typename T>
static auto run(int argc, char *argv[], T &derived_class) -> int;

template<typename T, typename ...Args>
static auto run(int argc, char *argv[], Args ...args) -> int;


}
}

#endif // !__APPLICATION_RUN__

