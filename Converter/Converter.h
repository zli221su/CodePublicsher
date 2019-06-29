#pragma once
/////////////////////////////////////////////////////////////////////
// Converter.h - Codes Converter                                   //
// ver 3.3                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * Converter provides a class, of the same name, that convert these
 * files to HTML files and store them in a default output folder.
 * All analysis will be done by calling other classes. Only
 * dummy text adding and file creating are done inside the class.
 *
 * Required Files:
 * ---------------
 * Display.h, Display.cpp                        // Main function
 * FileSystem.h, FileSystem.cpp                  // Directory and Path classes
 * Logger.h, Logger.cpp                          // Logging function
 * CodeAnalysers.h, CodeAnalysers.cpp            // Analysis functions
 * StringUtilities.h, StringUtilities.cpp        // Only for Main Function Testing
 *
 * Maintenance History:
 * --------------------
 * ver 3.3 : 25 Apr 2019
 * - Converted depencies are supported to be returned
 *
 * --------------------
 * ver 3.2 : 9 Apr 2019
 * - Exceptions are thrown when outputPath not valid or analyser cannot be added.
 *
 * --------------------
 * ver 3.1 : 22 Mar 2019
 * - provid BFS through dependency tree to convert all valid files
 *
 * --------------------
 * ver 3.0 : 8 Mar 2019
 * - moved all analysis functions out, now using "CppAnalyser" for analysis.
 * - only the file creating and dumy text adding functions are kept.
 *
 * --------------------
 * ver 2.0 : 27 Feb 2019
 * - add dependency analysis
 * - add scope analysis
 * - now using Logger to log to console
 * - add and delete some special characters witch stored in charMap
 *
 * --------------------
 * ver 1.0 : 5 Feb 2019
 * - first release
 *
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <regex>
#include <unordered_set>
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../CppAnalyser/CppAnalyser.h"

namespace CodeConvert {
	class Converter
	{
	public:
		using filesType = std::vector<std::string>;
		using contentType = std::vector<std::string>;

		static std::string version() { return "ver 3.3"; }
		Converter(const filesType& inputFiles, const std::string &outputPath, std::string inputDirectory);
		Converter& addAnalyser(std::string analyser);
		void doConversion();
		const filesType getOutputFiles() const;
		const filesType getRootOutputFiles() const;

	private:
		std::string inputDirectory_;
		filesType inputFiles_; // files already matches regex waiting for being converted
		filesType outputFiles_; // files waiting for being returning to outside
		filesType rootOutputFiles_;
		std::ofstream out_;
		std::string outputPath_; // the output folder
		CodeAnalysis::CppAnalyser ca_;

		bool inputIsValid(const std::string& inFile);
		void addLeadingCodeHTML(const std::string purFileName);
		void addPreTag();
		void addFileContents(const std::vector<std::string> &fileContent);
		void addClosingTags();
	};
}