#pragma once
/////////////////////////////////////////////////////////////////////
// FileSearch.h - File Searcher                                    //
// ver 1.2                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * FileSearch provides a class, of the same name, that first search
 * all files with type.h and.cpp (by default). Second, filter those
 * files with Regex regular expressions.
 *
 * Required Files:
 * ---------------
 * DirExplorerN.h, DirExplorerN.cpp  // searchFiles function
 * FileSystem.h, FileSystem.cpp      // Directory and Path classes
 * StringUtilities.h                 // Title function
 *
 * Maintenance History:
 * --------------------
 * ver 1.2 : 8 Apr 2019
 * - Instead of logging warnings, now, exceptions are thrown if no file matches Patterns or Regexes.
 * - SubDir (recurse) option is added to constructor.
 *
 * --------------------
 * ver 1.1 : 8 Mar 2019
 * - inline functions move to .cpp and they are not inline now
 * - Demos modified
 *
 * --------------------
 * ver 1.0 : 5 Feb 2019
 * - first release
 *
*/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Logger/Logger.h"


class FileSearch
{
public:
	using patternsType = std::vector<std::string>;
	using filesType = std::vector<std::string>;
	using folderType = std::string;

	static std::string version() { return "ver 1.1"; }

	FileSearch(const patternsType &patterns, const std::string &directory, const std::vector<std::string> fileTypes = std::vector<std::string>(), bool subDir = true);

	bool searchFiles();
	const filesType getOutputFiles() const;

private:
	bool subDir_;
	const std::string* pDirectory_;
	const patternsType wildPattern = patternsType(1, ".*");
	const patternsType* pPatterns_; // regular expresions needed to be applied on outputFIles
	std::vector<std::string> fileTypes_; // if defaultFilesType is true, only return .h and .cpp files
	filesType outputFiles_; // files already matches regex waiting for being converted

	void filterFiles(const filesType *filesTemp);
};