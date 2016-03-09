#ifndef __APPLICATION_GUARDIAN__
#define __APPLICATION_GUARDIAN__

#include <memory>
#include <mutex>

namespace CPM_FRIJA_NS
{

class Application;

template<typename T>
static auto run(int argc, char *argv[], T &derived_class) -> int;

template<typename T, typename ...Args>
static auto run(int argc, char *argv[], Args ...args) -> int;


}

#endif // !__APPLICATION_GUARDIAN__

