#ifndef DEBUG_HPP_INCLUDED
#define DEBUG_HPP_INCLUDED

#include <iostream>

#ifdef __REPORT__
  // class to capture the caller and print it.
  class ClassNameDecorator
  {
    public:
      ClassNameDecorator()
        : className_()
      {}

      ClassNameDecorator& close(std::string Caller, std::string File, int Line)
      {
        std::cout
          << "Reporter: ApplicationConfig::close() is being called by "
          << Caller << "() in " << File << ":" << Line << std::endl;
        return *this;
      }
      int operator()(int one, int two)
      {
        return className_.close(one,two);
      }
    private:
      ApplicationConfig className_;
  };


// remove the symbol for the function, then define a new version that instead
// creates a stack temporary instance of ClassNameDecorator.
// FunctionName is then replaced with a version that takes the caller information
// and uses Method Chaining to allow operator() to be invoked with the original
// parameters.
#  undef ApplicationConfig
#  define ApplicationConfig ClassNameDecorator
#  undef close
#  define close close(__FUNCTION__,__FILE__,__LINE__)
#endif




#endif // DEBUG_HPP_INCLUDED
