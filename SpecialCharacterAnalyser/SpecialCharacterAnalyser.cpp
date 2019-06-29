///////////////////////////////////////////////////////////////////////////////////
// SpecialCharacterAnalyser.cpp - Replace Special Characters in File             //
// ver 1.1                                                                       //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                          //
///////////////////////////////////////////////////////////////////////////////////

#include "SpecialCharacterAnalyser.h"

namespace CodeAnalysis {
	// ---------------< set priority in code analysers >---------------
	SpecialCharacterAnalyser::SpecialCharacterAnalyser() : priority_(0) {
		initializeCharMap();
	}

	// ---------------< empty destructor >---------------
	SpecialCharacterAnalyser::~SpecialCharacterAnalyser()
	{
	}

	// ---------------< empty function >----------------
	void SpecialCharacterAnalyser::set(const std::string& file, const std::string& directory, ASTNode* root)
	{
	}

	// ---------------< empty function >----------------
	void SpecialCharacterAnalyser::analyse()
	{
	}

	// ---------------< replacing special characters >---------------
	void SpecialCharacterAnalyser::injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles) {
		Log::Debug::title("Following special characters are substituted with HTML codes: ");
		Log::Debug::writeLine();
		for (auto it = charMap_.begin(); it != charMap_.end(); it++) {
			Log::Debug::write(" " + std::string(1, it->first) + "  ");
		}
		for (auto itLine = fileContent.begin(); itLine != fileContent.end(); itLine++) {
			replaceSpecialCharacters(*itLine);
		}
	}

	// -----< private - Initialize special characters that needed to be take care when converting >-----
	void SpecialCharacterAnalyser::initializeCharMap() {
		charMap_['"'] = "&quot;";
		charMap_['\''] = "&apos;";
		charMap_['&'] = "&amp;";
		charMap_['<'] = "&lt;";
		charMap_['>'] = "&gt;";
	}

	// ----------< private - convert special characters for a single line >----------
	void SpecialCharacterAnalyser::replaceSpecialCharacters(std::string& textLine) {
		for (size_t position = 0; position < textLine.size(); position++) {
			auto it = charMap_.find(textLine[position]);
			if (it != charMap_.end()) {
				auto newString = &(it->second);
				textLine.replace(position, 1, *newString);
				position += newString->size() - 1;
				continue;
			}
		}
	}

	// ------------< lower number means hight priority >--------------
	int SpecialCharacterAnalyser::getPriority() {
		return priority_;
	}
}

#ifdef TEST_SPECIALCHARACTERANALYSER
#include <fstream>

int main() {
	try {
		Log::startLoggers("debug");

		using namespace CodeAnalysis;
		ICodeAnalyser* ca(new SpecialCharacterAnalyser);

		std::string file = "SpecialCharacterAnalyser.cpp";
		std::string directory = "../SpecialCharacterAnalyser";

		std::vector<std::string> fileContent;
		std::ifstream in(file);
		std::string line;

		Log::Debug::writeLine();
		Log::Debug::Title("Codes before SpecialCharacter Analysis");
		Log::Debug::writeLine();
		while (std::getline(in, line)) {
			fileContent.push_back(line);
			Log::Debug::writeLine(line);
		}

		ca->set(file, directory, nullptr);
		ca->analyse();
		ca->injectCode(fileContent);

		Log::Debug::writeLine();
		Log::Debug::Title("Codes after SpecialCharacter Analysis");
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
