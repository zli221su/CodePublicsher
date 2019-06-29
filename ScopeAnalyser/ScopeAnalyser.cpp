/////////////////////////////////////////////////////////////////////////
// ScopeAnalyser.cpp - Analyse and Inject Scope informations to File   //
// ver 1.1                                                             //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                //
/////////////////////////////////////////////////////////////////////////

#include "ScopeAnalyser.h"

namespace CodeAnalysis {
	// -------------------< set analysis priority >--------------------
	ScopeAnalyser::ScopeAnalyser() : priority_(2)
	{
	}

	// -------------------< empty destructor >--------------------
	ScopeAnalyser::~ScopeAnalyser()
	{
	}

	// ----------< initializing variables >-------------
	void ScopeAnalyser::set(const std::string& file, const std::string& directory, ASTNode* root) {
		file_ = file;
		root_ = root;
	}

	// -----------------< analyse scope information >-------------------
	void ScopeAnalyser::analyse() {
		Log::Debug::Title("Scope Analysis (with Parser)");
		tagId_ = 0;
		st_.clear();
		// Analyzing files
		std::string fileSpec;
		fileSpec = FileSystem::Path::getFullFileSpec(file_);
		scopeWalk(root_);
	}

	// ----------------< private - extract scope information from ASTTree >------------------
	void ScopeAnalyser::scopeWalk(ASTNode* root) {
		if (root->type_ == "class" || root->type_ == "struct" || root->type_ == "function") {
			ScopeTable::scopeTagType startLineDiv;
			startLineDiv.first = "start";
			startLineDiv.second = button(tagId_, " class=\"IndividualButton\"");
			startLineDiv.second += customTag(root->type_, " id=" + std::to_string(tagId_));
			st_.addScopeTag(root->startLineCount_, startLineDiv);

			ScopeTable::scopeTagType endLineDiv;
			endLineDiv.first = "end";
			endLineDiv.second = endTag();
			st_.addScopeTag(root->endLineCount_, endLineDiv);

			tagId_++;
		}
		std::vector<ASTNode*>::iterator iter = root->children_.begin();
		while (iter != root->children_.end()) {
			scopeWalk(*iter);
			++iter;
		}
	}

	// --------------< inject scope tags to file content >----------------
	void ScopeAnalyser::injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles) {
		int lineCount = 0;
		for (auto itLine = fileContent.begin(); itLine != fileContent.end(); itLine++) {
			wrapScopes(*itLine, ++lineCount);
		}
		std::string scopeButtons;
		scopeButtons += "    <button class=\"scopesButton\" onClick=\"showHideClass('Class')\" style=\"top: 20px;\">Show or Hide Class Contents</button>\n";
		scopeButtons += "    <button class=\"scopesButton\" onClick = \"showHideClass('Function')\" style=\"top: 50px;\">Show or Hide Function Contents</button>\n";
		fileContent.push_back(scopeButtons);
	}

	// -----< private - wrap scopes based on the line number and scope table >------
	void ScopeAnalyser::wrapScopes(std::string& line, int lineNumber) {
		// Call ScopeWrapper to insert contents
		auto it1 = st_.getTable().find(lineNumber);
		if (it1 != st_.getTable().end()) {
			for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
				if (it2->first == "start") {
					auto position = line.find('{');
					line.replace(position == std::string::npos ? line.length() : position, 0, it2->second);
				}
				else if (it2->first == "end") {
					auto position = line.find('}');
					line.replace(position == std::string::npos ? 0 : (position + 1), 0, it2->second);
				}
				else {
					Log::Debug::Title("Warning!!! ScopeMap format not supported!");
				}
			}
		}
	}

	// ------------< lower number means hight priority >--------------
	int ScopeAnalyser::getPriority() {
		return priority_;
	}
}

#ifdef TEST_SCOPEANALYSER
#include "../ParserHelper/ParserHelper.h"

int main() {
	try {
		Log::startLoggers("debug");

		using namespace CodeAnalysis;
		ICodeAnalyser* ca(new ScopeAnalyser);

		std::string file = "ScopeAnalyser.cpp";
		std::string directory = "../ScopeAnalyser";

		ParserHelper ph;
		ph.initializeParser(file);
		ASTNode* root = ph.getGlobalScope();

		std::vector<std::string> fileContent;
		std::ifstream in(file);
		std::string line;

		Log::Debug::writeLine();
		Log::Debug::Title("Codes before Scope Analysis");
		Log::Debug::writeLine();
		while (std::getline(in, line)) {
			fileContent.push_back(line);
			Log::Debug::writeLine(line);
		}

		ca->set(file, directory, root);
		ca->analyse();
		ca->injectCode(fileContent);

		Log::Debug::writeLine();
		Log::Debug::Title("Codes after Scope Analysis");
		Log::Debug::writeLine();
		for (auto line : fileContent) {
			Log::Debug::writeLine(line);
		}
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