#pragma once
/////////////////////////////////////////////////////////////////////////////////
// ParserHelper.h - Parse  by using Parser                                     //
// ver 1.0                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * ParserHelper can parser a file, then return the parsing information back.
 *
 * Required Files:
 * ---------------
 * Logger.h, Logger.cpp                          // Log
 * FileSystem.h, FileSystem.cpp                  // Path Operations
 * ConfigureParser.h, ConfigureParser.cpp        // Configure Parser
 * StringUtilities.h, StringUtilities.cpp        // Console Output
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/

# include <iostream>
# include <string>
# include "../Logger/Logger.h"
# include "../FileSystem/FileSystem.h"
# include "../Parser/ConfigureParser.h"
# include "../Utilities/StringUtilities/StringUtilities.h"

namespace CodeAnalysis {
	// This calss create reuseable parser for CodeAnalysers
	class ParserHelper
	{
	public:
		~ParserHelper();

		void initializeParser(const std::string& file);
		Parser* getParser();
		ASTNode* getGlobalScope();

	private:
		ConfigParseForCodeAnal* pConfigure_;
		Parser* pParser_;
		ASTNode* pGlobalScope_;
	};
}

