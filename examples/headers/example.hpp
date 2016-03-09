#ifndef __FRIJA_EXAMPLE__
#define __FRIJA_EXAMPLE__

#include "../../frija/headers/app.hpp"

namespace frija
{

class ExampleClass : public Application
{
public:
	ExampleClass();

	void init();
	auto main() -> int;
	auto loop(int last_result) -> int;
	void finish(int reason);
};

extern ExampleClass eg_class;

}

#endif // !__FRIJA_EXAMPLE__

