#pragma once
/////////////////////////////////////////////////////////////////////////////////
// SpecialCharacterAnalyser.h - Replace Special Characters in File             //
// ver 1.1                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * SpecialCharacterAnalyser inheritate the base class "IcodeAnalyser". This class
 * can replace all HTML reserved characters with HTML codes.
 *
 * Required Files:
 * ---------------
 * ICodeAnalyser.h                            // Base Class
 * Logger.h, Logger.cpp                       // Log
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
# include <map>
# include "../CodeAnalyser/ICodeAnalyser.h"
# include "../Logger/Logger.h"

namespace CodeAnalysis {
	class SpecialCharacterAnalyser :
		public ICodeAnalyser
	{
	public:
		SpecialCharacterAnalyser();
		~SpecialCharacterAnalyser();

		void set(const std::string& file, const std::string& directory, ASTNode* root);
		void analyse();
		void injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles);
		int getPriority();
	private:
		int priority_;
		std::map<char, std::string> charMap_; // a map uesed for mapping special characters in the file

		void initializeCharMap();
		void replaceSpecialCharacters(std::string& textLine);
	};
}
