#pragma once
/////////////////////////////////////////////////////////////////////////////////
// CppAnalyser.h - Analys C++ Codes with Provided Analysers                    //
// ver 1.2                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * CppAnalyser mainly use other analysers through base class "IcodeAnalyser".
 * This class will do analyse operations base on the analysers recieved.
 *
 * Required Files:
 * ---------------
 * Logger.h, Logger.cpp                          // Log
 * FileSystem.h, FileSystem.cpp                  // Path Operations
 * ParserHelper.h, ParserHelper.cpp              // Parsing files
 * ICodeAnalyser.h                               // Base Class for Analysers
 * ConfigureCodeAnalysers.h                      // Build and use Analysers
 *
 * Maintenance History:
 * --------------------
 * ver 1.2 : 7 Apr 2019
 * - use blocking queue to send file name which is being converted
 *
 * --------------------
 * ver 1.1 : 22 Mar 2019
 * - support BFS through dependency tree
 *
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "../Logger/Logger.h"
# include "../FileSystem/FileSystem.h"
# include "../ParserHelper/ParserHelper.h"
# include "../CodeAnalyser/ConfigureCodeAnalysers.h"

namespace CodeAnalysis {
	using filesType = std::vector<std::string>;
	using contentType = std::vector<std::string>;
	class CppAnalyser
	{
	public:
		CppAnalyser();
		~CppAnalyser();

		CppAnalyser& addAnalyser(const std::string& analyser);
		contentType analyse(const std::string& file, const std::string& directory);
		const std::vector<std::string>& getAdditionalFiles();

	private:
		ConfigureCodeAnalysers* pConfigureCodeAnalysers;
		std::vector<ICodeAnalyser*> analysers_;
		ParserHelper parserHelper_;
		ASTNode* root_;
		std::vector<std::string> additionalFiles_;

		void resetForFile(const std::string& file);
		void initializeParserHelper(const std::string& file);
		contentType generateFileContent(const std::string& file);
	};
}

