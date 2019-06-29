#pragma once
/////////////////////////////////////////////////////////////////////////////
// DirExplorerN.h - Naive directory explorer                               //
// ver 1.0                                                                 //
// Origin: Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
// Origin: Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * DirExplorerN provides a class, of the same name, that executes a
 * depth first search of a directory tree rooted at a specified path.
 * Each time it enters a directory, it invokes its member function
 * doDir, and for every file in that directory, it invokes doFile.
 *
 * We call this a "Naive Directory Explorer" because, while it carries
 * out its assigned responsibilities well, there is no way to change
 * what its doDir and doFile functions do, without changing the class
 * itself.  It would be much better to provide a mechanism to allow
 * a using application to supply this processing without changing
 * the Directory Navigator. 
 *
 * Other projects in this solution do just that, in different ways.  

 * - DirExplorer-Naive:
 *     Implements basic processing well, but applications have to
 *     change its code to affect how files and directories are
 *     handled.
 * - DirExplorer-Template:
 *     Applications provide template class parameters to define file
 *      and directory processing.
 * - DirExplorer-Inheritance:
 *     Applications subclass DirExplorerI, overriding virtual functions
 *     doFile, doDir, and doQuit to define how files and directories
 *     are handled, and whether processing should terminate before
 *     visiting all directories.
 * - DirExplorer-Events:
 *     Applications use Event Interfaces, published by DirExplorerE,
 *     by subscribing event-handler functions, and control terminate
 *      processing using a plug-in interface.
 * - DirExplorer-Provider:
 *     Applications implement a file system provider that implements
 *     a Provider interface published by DirExplorerP.
 *     
 * We'll be using this solution to illustrate techniques for building
 * flexible software.
 *
 * Required Files:
 * ---------------
 * DirExplorerN.h, DirExplorerN.cpp
 * FileSystem.h, FileSystem.cpp      // Directory and Path classes
 * StringUtilities.h                 // Title function
 * CodeUtilities.h                   // ProcessCmdLine class
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 5 Feb 2019
 * - first release
 *
*/
#include <vector>
#include <iostream>
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"


namespace FileSystem
{
  class DirExplorerN
  {
  public:
    using patterns = std::vector<std::string>;

    static std::string version() { return "ver 1.0"; }

    DirExplorerN(const std::string& path);

    // set options for analysis

    void addPattern(const std::string& patt);
    void recurse(bool doRecurse = true);

    // conduct depth first search

	void search();
	void find(const std::string& path);

    // define what happens when a file or dir is encountered

    void doFile(const std::string& filepath, const std::string& filename);
    void doDir(const std::string& dirname);

    // extract traversal statistics 

    size_t fileCount();
    size_t dirCount();
    void showStats();
	const std::vector<std::string>& filesToProcess() const;

  private:
	std::vector<std::string> files_;
    std::string path_;
    patterns patterns_;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
  };

  //----< construct DirExplorerN instance with default pattern >-----

  inline DirExplorerN::DirExplorerN(const std::string& path) : path_(path)
  {
	patterns_.push_back("*.*");
  }
  //----< add specified patterns for selecting file names >----------

  inline void DirExplorerN::addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  //----< set option to recusively walk dir tree >-------------------

  inline void DirExplorerN::recurse(bool doRecurse)
  {
    recurse_ = doRecurse;
  }
  //----< start Depth First Search at path held in path_ >-----------

  inline void DirExplorerN::search()
  {
    return find(path_);
  }
  //----< search for directories and their files >-------------------
  /*
    Recursively finds all the dirs and files on the specified path,
    executing doDir when entering a directory and doFile when finding a file
  */
  inline void DirExplorerN::find(const std::string& path)
  {
    // show current directory

    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    doDir(fpath);

    for (auto patt : patterns_)
    {
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      for (auto f : files)
      {
        doFile(fpath, f);  // show and add each file in current directory
      }
    }

    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;
      if (recurse_)
      {
        find(dpath);   // recurse into subdirectories
      }
      else
      {
        doDir(dpath);  // show subdirectories
      }
    }
	return;
  }
  //----< an application changes to enable specific file ops >-------

  inline void DirExplorerN::doFile(const std::string& filepath, const std::string& filename)
  {
    ++fileCount_;
	Log::Debug::writeLine("--   " + filepath + "\\" + filename);
	files_.push_back(filepath + "\\" +filename); //fileToProcess
  }
  //----< an application changes to enable specific dir ops >--------

  inline void DirExplorerN::doDir(const std::string& dirname)
  {
    ++dirCount_;
	Log::Debug::writeLine(" ++ " + dirname);
  }
  //----< return number of files processed >-------------------------

  inline size_t DirExplorerN::fileCount()
  {
    return fileCount_;
  }
  //----< return number of directories processed >-------------------

  inline size_t DirExplorerN::dirCount()
  {
    return dirCount_;
  }
  //----< show final counts for files and dirs >---------------------

  inline void DirExplorerN::showStats()
  {
    Log::Debug::write("\n\n  processed " + std::to_string(fileCount_) + " files in " + std::to_string(dirCount_) + " directories");
  }

  //---------------------< return found files >----------------------

  inline const std::vector<std::string>& DirExplorerN::filesToProcess() const {
	  return files_;
  }
}

