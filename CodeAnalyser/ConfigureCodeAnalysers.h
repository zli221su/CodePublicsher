#pragma once
/////////////////////////////////////////////////////////////////////////////////
// ConfigureCodeAnalyser.h - Build Analysers and Return                        //
// ver 1.2                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * ConfigureCodeAnalyser create different analysers and return the ICodeAnalyser
 * Interface Pointer back.
 *
 * Required Files:
 * ---------------
 * Logger.h, Logger.cpp                          // Log
 * ICodeAnalyser.h                               // Base Class for Analysers
 *
 * For .cpp File Only:
 * SpecialCharacterAnalyser.h, SpecialCharacterAnalyser.cpp   // Replace Special Characters
 * CommentAnalyser.h, CommentAnalyser.cpp        // Analyse Comments
 * DependencyAnalyser.h, DependencyAnalyser.cpp  // Analyse Dependencies
 * ScopeAnalyser.h, ScopeAnalyser.cpp            // Analyse Scopes
 *
 * Maintenance History:
 * --------------------
 * ver 1.2 : 9 Apr 2019
 * - Exception thrown when analyser not existed while using addAnalyser(string);
 *
 * --------------------
 * ver 1.1 : 22 Mar 2019
 * - comments and variable names refined
 *
 * --------------------
 * ver 1.0 : 8 Mar 2019
 * - first release
 *
*/


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../Logger/Logger.h"
#include "../CodeAnalyser/ICodeAnalyser.h"


namespace CodeAnalysis{
	class ConfigureCodeAnalysers
	{
	public:
		enum analysisName { specialCharacter, comment, dependency, scope };

		ConfigureCodeAnalysers();
		~ConfigureCodeAnalysers();

		ConfigureCodeAnalysers& addAnalyser(const std::string& analyser);
		std::vector<ICodeAnalyser*> getAnalysers();

	private:
		std::vector<ICodeAnalyser*> analysers_;
		std::map<std::string, analysisName> analysisNameTable_;

		void initializeAnalysisNameTable();
		analysisName getEnum(const std::string& s);
		void sortAnalysers();
	};
	bool analyserComparator(ICodeAnalyser* analyser1, ICodeAnalyser* analyser2);
}