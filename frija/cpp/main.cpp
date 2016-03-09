#include "../headers/app_guard.hpp"

auto main(int argc, char *argv[]) -> int
{
	return frija::ApplicationGuardian::run(argc, argv);
}