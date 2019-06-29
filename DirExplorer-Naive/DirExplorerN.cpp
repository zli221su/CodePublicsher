/////////////////////////////////////////////////////////////////////
// DirExplorerN.cpp - Naive directory explorer                     //
// ver 1.0                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_DIREXPLORERN  // only compile the following when defined

#include "DirExplorerN.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"

using namespace Utilities;
using namespace FileSystem;

ProcessCmdLine::Usage customUsage()
{
  std::string usage;
  usage += "\n  Command Line: path [/option]* [/pattern]*";
  usage += "\n    path is relative or absolute path where processing begins";
  usage += "\n    [/option]* are one or more options of the form:";
  usage += "\n      /s - walk directory recursively";
  //usage += "\n      /h - hide empty directories";
  //usage += "\n      /a - on stopping, show all files in current directory";
  usage += "\n    [pattern]* are one or more pattern strings of the form:";
  usage += "\n      *.h *.cpp *.cs *.txt or *.*";
  usage += "\n";
  return usage;
}

int main(int argc, char *argv[])
{
	Log::startLoggers("debug");

  Title("Demonstrate DirExplorer-Naive, " + DirExplorerN::version());

  ProcessCmdLine pcl(argc, argv);
  pcl.usage(customUsage());

  preface("Command Line: ");
  pcl.showCmdLine();
  putline();

  if (pcl.parseError())
  {
    pcl.usage();
    std::cout << "\n\n";
    return 1;
  }

  DirExplorerN de(pcl.path());
  for (auto patt : pcl.patterns())
  {
    de.addPattern(patt);
  }

  if (pcl.hasOption('s'))
  {
    de.recurse();
  }

  de.search();
  de.showStats();

  

  Log::stopLoggers();

  return 0;
}

#endif