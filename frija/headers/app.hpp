#ifndef __APPLICATION__
#define __APPLICATION__

//#ifndef FRIJA_APPLICATION
//#error Please define FRIJA_APPLICATION with your frija::Application derived class!
//#endif // !FRIJA_APPLICATION

#include <iostream>

#include <tyr/tyr>

namespace CPM_FRIJA_NS
{

class Application : public tyr::ArgumentParser
{
public:
	enum CloseReason
	{
		FINISHED,
		UNEXPECTED_EXCEPTION
	};

	enum Flags
	{
		USE_MAIN		= 0x1,
		USE_LOOP		= 0x2,
		USE_ARG_EXEC	= 0x4,
	};

public:
	Application(const Application &other) = delete;
	Application(Application &&other) = delete;
	virtual ~Application() = default;

	auto getFlags() const -> unsigned int;

	virtual void init()
	{
		std::cout << "init() of Application called...\n";
	}

	virtual auto main() -> int
	{
		std::cout << "main() of Application called...\n";
		return 0;
	}

	virtual auto loop(int last_result) -> int
	{
		std::cout << "loop() of Application called...\n";
		return 0;
	}

	virtual void finish(int reason)
	{
		std::cout << "finish() of Application called...\n";
	}

	auto operator =(const Application &other) -> Application & = delete;
	auto operator =(Application &&other) -> Application & = delete;

protected:
	Application(Flags flags = USE_MAIN);

private:
	unsigned int app_flags;
};

}

#endif // !__APPLICATION__
