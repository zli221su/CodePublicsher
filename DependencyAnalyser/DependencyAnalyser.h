#pragma once
/////////////////////////////////////////////////////////////////////////
// DependencyAnalyser.h - Analyse and Inject Dependency Links to File  //
// ver 1.1                                                             //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                //
/////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * DependencyAnalyser inheritate the base class "IcodeAnalyser". This class
 * can analyse dependencies in c++ files and inject HTML button tags to file.
 *
 * Required Files:
 * ---------------
 * ICodeAnalyser.h                            // Base Class
 * CodeAnalysisUtilities.h                    // Inserting HTML tags
 * Logger.h, Logger.cpp                       // Log
 * FileSystem.h, FileSystem.cpp               // Path Operations
 * ConfigureParser.h, ConfigureParser.cpp     // Toker and ASTNode Operations
 * ParserHelper.h, ParserHelper.cpp           // Only for Testing Main Function
 *
 * Maintenance History:
 * --------------------
 * ver 1.1 : 22 Mar 2019
 * - add addtionalFiles input in order to support BFS in dependency tree
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
# include "../Parser/ConfigureParser.h"

namespace CodeAnalysis {
	class DependencyAnalyser :
		public ICodeAnalyser
	{
	public:
		DependencyAnalyser();
		~DependencyAnalyser();

		void set(const std::string& file, const std::string& directory, ASTNode* root);
		void analyse();
		void injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles);
		int getPriority();
	private:
		int priority_;
		std::string file_;
		std::string fileLocation_;
		std::string directory_;
		ASTNode* root_;

		std::vector<std::string> dependencies_;

		void extractDependency(const std::string line);
		bool addDependency(std::string dependency);
		std::string standardize(std::string path);
	};
}
