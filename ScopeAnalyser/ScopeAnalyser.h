#pragma once
/////////////////////////////////////////////////////////////////////////
// ScopeAnalyser.h - Analyse and Inject Scope informations to File     //
// ver 1.1                                                             //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                //
/////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * ScopeAnalyser inheritate the base class "IcodeAnalyser". This class
 * can analyse scopes (classes, struct, functions) in c++ files and
 * inject HTML button tags to file.
 *
 * Required Files:
 * ---------------
 * ICodeAnalyser.h                            // Base Class
 * CodeAnalysisUtilities.h                    // Inserting HTML tags
 * Logger.h, Logger.cpp                       // Log
 * FileSystem.h, FileSystem.cpp               // Path Operations
 * ScopeTable.h, ScopeTable.cpp               // Storing Scope Information
 * ParserHelper.h, ParserHelper.cpp           // Only for Testing Main Function
 *
 * Maintenance History:
 * --------------------
 * ver 1.1 : 22 Mar 2019
 * - add addtionalFiles input to interface
 *
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/

# include <iostream>
# include <string>
# include <vector>
# include "../CodeAnalyser/ICodeAnalyser.h"
# include "../CodeAnalyser/CodeAnalysisUtilities.h"
# include "../Logger/Logger.h"
# include "../FileSystem/FileSystem.h"
# include "ScopeTable.h"

namespace CodeAnalysis {
	class ScopeAnalyser :
		public ICodeAnalyser
	{
	public:
		ScopeAnalyser();
		~ScopeAnalyser();

		void set(const std::string& file, const std::string& directory, ASTNode* root);
		void analyse();
		void injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles);
		int getPriority();
	private:
		int priority_;
		std::string file_;
		ASTNode* root_;
		ScopeTable st_;
		int tagId_;

		void scopeWalk(ASTNode* root);
		void wrapScopes(std::string& line, int lineNumber);
	};
}
