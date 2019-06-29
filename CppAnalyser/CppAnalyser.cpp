/////////////////////////////////////////////////////////////////////////////////
// CppAnalyser.cpp - Analys C++ Codes with Provided Analysers                  //
// ver 1.1                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////

#include "CppAnalyser.h"

namespace CodeAnalysis {
	// ---------------< initializeall the supported analizers >----------------
	CppAnalyser::CppAnalyser() : pConfigureCodeAnalysers(new ConfigureCodeAnalysers)
	{
	}

	// ---------------< take care of all the analysers it owns >-------------------
	CppAnalyser::~CppAnalyser() {
		delete pConfigureCodeAnalysers;
	}

	// --------------< provide analysers that will be used for analysis >--------------
	CppAnalyser& CppAnalyser::addAnalyser(const std::string& analyser) {
		pConfigureCodeAnalysers->addAnalyser(analyser);
		analysers_ = pConfigureCodeAnalysers->getAnalysers();
		return *this;
	}

	// -------------< analyse the file >---------------
	contentType CppAnalyser::analyse(const std::string& file, const std::string& directory) {
		Log::Demo::writeLine();
		Log::Demo::writeLine("Analysing: " + file);
		resetForFile(file);
		contentType fileContent = generateFileContent(file);
		for (auto analyserClass : analysers_) {
			// modify fileContent in order (analyser wwith lower priority analyse first)
			analyserClass->set(file, directory, root_);
			analyserClass->analyse();
		}
		for (auto analyserClass : analysers_) {
			analyserClass->injectCode(fileContent, additionalFiles_);
		}
		return fileContent;
	}

	// -------< return the additonal files needed to be analyse in the future >----------
	const std::vector<std::string>& CppAnalyser::getAdditionalFiles() {
		return additionalFiles_;
	}

	// -----------< private - create fileContent vector >-----------
	contentType CppAnalyser::generateFileContent(const std::string& file) {
		contentType fileContent;
		std::ifstream in(FileSystem::Path::getFullFileSpec(file));
		std::string line;
		while (std::getline(in, line)) {
			fileContent.push_back(line + "\n");
		}
		return fileContent;
	}

	// ----------< private - reset class for a new file >-----------
	void CppAnalyser::resetForFile(const std::string& file) {
		additionalFiles_.clear();
		initializeParserHelper(file);
	}

	// ------------< private - parser the file >---------------
	void CppAnalyser::initializeParserHelper(const std::string& file) {
		parserHelper_.initializeParser(file);
		root_ = parserHelper_.getGlobalScope();
	}
}

#ifdef TEST_CPPANALYSER
# include "../Logger/Logger.h"

int main()
{
	try {
		using namespace CodeAnalysis;
		Log::startLoggers("debug");

		std::string file1 = FileSystem::Path::getFullFileSpec("CppAnalyser.h");
		std::string directory1 = FileSystem::Path::getPath(file1);

		CppAnalyser ca;
		ca.addAnalyser("specialCharacter").addAnalyser("comment").addAnalyser("dependency").addAnalyser("scope");
		CodeAnalysis::contentType fileContent = ca.analyse(file1, directory1);

		for (auto line : fileContent) {
			Log::Debug::writeLine(line);
		}

		Log::Debug::Title("End of CodeAnalysers Demo!");
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
