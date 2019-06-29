/////////////////////////////////////////////////////////////////////
// Properties.cpp - provides getter and setter methods             //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_PROPERTIES

#include "Properties.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <string>

using namespace Utilities;

class Widget
{
public:
  Widget(const std::string& msg = "widget here") : msg_(msg) {}
  void say() {
    std::cout << "\n  " << msg_;
  }
private:
  std::string msg_;
};

class X
{
public:
  Property<int> intProp;
  Property<double> dblProp;
  Property<Widget> wgtProp;
};

int main()
{
  Title("Demonstrating Properties");
  
  X x;
  Widget widget;
  x.intProp(3);
  x.dblProp(3.14159);
  x.wgtProp(widget);

  std::cout << "\n  x.intProp = " << x.intProp();
  std::cout << "\n  x.dblProp = " << x.dblProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  x.wgtProp().say();
  putline();
  
  title("after modifying X's properties:");
  x.intProp(-2);
  x.dblProp(0.33333);
  Widget newWgt("newWgt here");
  x.wgtProp(newWgt);
  std::cout << "\n  x.intProp = " << x.intProp();
  std::cout << "\n  x.dblProp = " << x.dblProp();
  std::cout << "\n  x.wgtProp().say() emits: ";
  x.wgtProp().say();

  Property<std::string> strProp;
  strProp("a string");
  std::cout << "\n  value of strProp(\"a string\") is: " << strProp();
  strProp.ref() += " with some more text";
  std::cout << "\n  after appending to strProp.ref(), strProp() yields: " << strProp();
  std::cout << "\n\n";
}
#endif
