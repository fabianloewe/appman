#include "../headers/app_impl.hpp"
#include "../headers/app_guard.hpp"

namespace hyronx
{
namespace appman
{

template<typename T, typename ...Args>
auto run(int argc, char *argv[], Args ...args) -> int
{
	Application *app_ptr = new T(args...);

	return details::ApplicationGuardian(app_ptr)(argc, argv);
}

}
}
