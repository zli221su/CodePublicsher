#pragma once
/////////////////////////////////////////////////////////////////////////////////
// IcodeAnalyser.h - BaseClass for different Code Analysers                    //
// ver 1.1                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * IcodeAnalyser provides an abstract (Interface-Like) base class. It support three major 
 * methods to analyse codes: set, analyse and injectCode. The analysisPriority_
 * variable is used for setting analysis priorities since some analysis require
 * relative sequence. The lower the analysisPriority_ is, the higher priority it
 * will be.
 *
 * Required Files:
 * ---------------
 * AbstrStynTree.h                       // Support Input Types
 *
 * Maintenance History:
 * --------------------
 * ver 1.1 : 22 Mar 2019
 * - add additionalFiles as input in injectCode() in order to support BFS in dependency tree
 *
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/

# include <iostream>
# include <string>
# include <vector>
# include "../AbstractSyntaxTree/AbstrSynTree.h"

namespace CodeAnalysis {
	struct ICodeAnalyser {
		virtual ~ICodeAnalyser() = 0 {};
		virtual void set(const std::string& file, const std::string& directory, ASTNode* root) = 0;  // insert file to be analysed
		virtual void analyse() = 0;                                                   // analyse file
		virtual void injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles) = 0;          // modify fileContent based on the analysis
		virtual int getPriority() = 0;
	};
}