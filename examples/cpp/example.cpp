#include "../headers/example.hpp"

#include <iostream>

using namespace std;
using namespace frija;

ExampleClass::ExampleClass() : 
	Application()
{
	cout << "constructor of ExampleClass called...\n";
}

void ExampleClass::init()
{
	std::cout << "init() of ExampleClass called...\n";

	add("-e", "--example", "example", "Gives out an example string", "", tyr::ArgumentFlags(), [&](std::string s)
	{
		std::cout << "Just an example... You entered " << (s.empty() ? "nothing" : s) << std::endl;
	});
}

auto ExampleClass::main() -> int
{
	std::cout << "main() of ExampleClass called...\n";
	return 0;
}

auto ExampleClass::loop(int last_result) -> int
{
	std::cout << "loop() with " << last_result << " of ExampleClass called...\n";

	if(last_result == 3)
		return 0;

	return (++last_result);
}

void ExampleClass::finish(int reason)
{
	std::cout << "finish() of ExampleClass with reason " << reason << " called...\n";
	cin.get();
}