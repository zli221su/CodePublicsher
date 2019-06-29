#pragma once
/////////////////////////////////////////////////////////////////
// ScopeTable.h - Table Storing Scope informations             //
// ver 1.0                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019        //
/////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * ScopeTable provides a container for scope information for a file.
 * It support addScopeTag, clear, show and getTable methods.
 *
 * Required Files:
 * ---------------
 * Logger.h, Logger.cpp              // Log
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 8 Feb 2019
 * - first release
 *
*/

# include <unordered_map>
# include "../Logger/Logger.h"

namespace CodeAnalysis {
	class ScopeTable
	{
	public:
		using scopeTagType = std::pair<std::string, std::string>;

		void addScopeTag(int lineNum, scopeTagType& scopeDiv);
		const std::unordered_map<int, std::vector<scopeTagType>>& getTable();
		void show();
		void clear();
	private:
		std::unordered_map<int, std::vector<scopeTagType>> scopeTable_;
	};
}