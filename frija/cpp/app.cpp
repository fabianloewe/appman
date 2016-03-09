include "../headers/app.hpp"
#include "../headers/app_guard.hpp"

namespace frija
{

Application::Application(Flags flags) :
	app_flags(flags)
{
	ApplicationGuardian::set(this);
}

auto Application::getFlags() const -> unsigned int
{
	return app_flags;
}

}