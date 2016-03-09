#include "../headers/app.hpp"

namespace CPM_FRIJA_NS
{

Application::Application(Flags flags) :
	app_flags(flags)
{
}

auto Application::getFlags() const -> unsigned int
{
	return app_flags;
}

}