#include "ScopeTable.h"
/////////////////////////////////////////////////////////////////
// ScopeTable.cpp - Table Storing Scope informations           //
// ver 1.0                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019        //
/////////////////////////////////////////////////////////////////

namespace CodeAnalysis {
	// -------------< add method >---------------
	void ScopeTable::addScopeTag(int lineNum, scopeTagType& scopeTag) {
		scopeTable_[lineNum].push_back(scopeTag);
	}

	// ----------------< get method >-------------------
	const std::unordered_map<int, std::vector<ScopeTable::scopeTagType>>& ScopeTable::getTable() {
		return scopeTable_;
	}

	// ---------------< show table in console >----------------
	void ScopeTable::show() {
		for (auto it1 = scopeTable_.begin(); it1 != scopeTable_.end(); it1++) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
				Log::Debug::writeLine("-- Line " + std::to_string(it1->first) + "   ");
				Log::Debug::write(it2->first + ", " + it2->second);
			}
		}
	}

	// ---------------< reset the table to an empty one >------------------
	void ScopeTable::clear() {
		scopeTable_.clear();
	}
}

#ifdef TEST_SCOPETABLE

int main() {
	try {
		Log::startLoggers("debug");

		using namespace CodeAnalysis;

		ScopeTable st;
		ScopeTable::scopeTagType tag1;
		tag1.first = "start";
		tag1.second = "<test-div>";
		ScopeTable::scopeTagType tag2;
		tag2.first = "end";
		tag2.second = "</test-div>";

		st.addScopeTag(10, tag1);
		st.addScopeTag(23, tag2);

		Log::Debug::title("After adding two tags to table:");
		st.show();

		st.clear();
		Log::Debug::title("After clearing the table:");
		st.show();
	}
	catch (std::exception& ex) {
		Log::Debug::Title("std::exception caught\n\n" + std::string(ex.what()));
	}
	catch (...) {
		std::cout << "unknown exception";
	}
	return 0;
}

#endif