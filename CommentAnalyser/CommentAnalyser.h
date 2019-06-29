#pragma once
/////////////////////////////////////////////////////////////////////
// CommentAnalyser.h - Analyse and Inject Codes to File            //
// ver 1.1                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * CommentAnalyser inheritate the base class "IcodeAnalyser". This class
 * can analyse commnets in c++ and inject HTML tags in to codes.
 *
 * Required Files:
 * ---------------
 * ICodeAnalyser.h              // Base Class
 * CodeAnalysisUtilities.h      // Inserting HTML tags
 * Logger.h, Logger.cpp         // Log
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

namespace CodeAnalysis {
	class CommentAnalyser :
		public ICodeAnalyser
	{
	public:
		CommentAnalyser();
		virtual ~CommentAnalyser();

		void set(const std::string& file, const std::string& directory, ASTNode* root);
		void analyse();
		void injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles);
		int getPriority();

	private:
		int priority_;
		bool multiLineCommentFlag_ = false;

		void wrapComments(std::string& textLine);
	};
}