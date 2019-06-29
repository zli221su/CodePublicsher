/////////////////////////////////////////////////////////////////////
// CommentAnalyser.cpp - Analyse and Inject Codes to File          //
// ver 1.1                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////

#include "CommentAnalyser.h"

namespace CodeAnalysis {
	// ----------------< set priorities >-----------------
	CommentAnalyser::CommentAnalyser() : priority_(1) {
	}

	// ----------------< empty destructor >-----------------
	CommentAnalyser::~CommentAnalyser()
	{
	}

	// ---------------< empty function >----------------
	void CommentAnalyser::set(const std::string& file, const std::string& directory, ASTNode* root)
	{
	}

	// ---------------< empty function >----------------
	void CommentAnalyser::analyse()
	{
	}

	// --------------------< inject HTML tags to file >------------------------
	void CommentAnalyser::injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles) {
		multiLineCommentFlag_ = false;
		for (auto itLine = fileContent.begin(); itLine != fileContent.end(); itLine++) {
			wrapComments(*itLine);
		}
		if (multiLineCommentFlag_ == true) {
			(fileContent.end() - 1)->append(endTag());
			multiLineCommentFlag_ = false;
		}
		std::string commentButton;
		commentButton += "    <button class=\"scopesButton\" onClick = \"showHideClass('SingleLineComment'); showHideClass('MultiLineComment');\" style=\"top: 80px;\">Show or Hide Comment Contents</button>\n";
		fileContent.push_back(commentButton);
	}

	// ------------< private - detect comments and then add tags before and after it >-----------
	void CommentAnalyser::wrapComments(std::string& textLine) {
		for (size_t position = 0; position < textLine.size(); position++) {
			// if single-line comment detected
			if (multiLineCommentFlag_ == false && (position + 1) < textLine.size() && textLine[position] == '/' && textLine[position + 1] == '/') {
				textLine.replace(position, 0, singleLineCommentTag());
				textLine.replace(textLine.size(), 0, endTag());
				break;
			}
			// if multi-line comment start detected
			if (multiLineCommentFlag_ == false && (position + 1) < textLine.size() && textLine[position] == '/' && textLine[position + 1] == '*') {
				textLine.replace(position, 0, multiLineCommentTag());
				multiLineCommentFlag_ = true;
				position += multiLineCommentTag().size() + 1;
				continue;
			}
			// if multi-line comment end detected
			if (multiLineCommentFlag_ == true && (position + 1) < textLine.size() && textLine[position] == '*' && textLine[position + 1] == '/') {
				textLine.replace(position + 2, 0, endTag());
				multiLineCommentFlag_ = false;
				position += endTag().size() + 1;
				continue;
			}
		}
	}

	// ------------< lower number means hight priority >--------------
	int CommentAnalyser::getPriority() {
		return priority_;
	}
}

#ifdef TEST_COMMENTANALYSER
#include <fstream>

int main() {
	try {
		Log::startLoggers("debug");

		using namespace CodeAnalysis;
		ICodeAnalyser* ca(new CommentAnalyser);
		
		std::string file = "CommentAnalyser.cpp";
		std::string directory = "../CommentAnalyser";
		std::vector<std::string> fileContent;
		std::ifstream in(file);
		std::string line;

		Log::Debug::writeLine();
		Log::Debug::Title("Codes before comment Analysis");
		Log::Debug::writeLine();
		while (std::getline(in, line)) {
			fileContent.push_back(line);
			Log::Debug::writeLine(line);
		}
		
		ca->set(file, directory, nullptr);
		ca->analyse();
		ca->injectCode(fileContent);

		Log::Debug::writeLine();
		Log::Debug::Title("Codes after comment Analysis");
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