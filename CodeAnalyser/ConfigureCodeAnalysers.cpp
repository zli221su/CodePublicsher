/////////////////////////////////////////////////////////////////////////////////
// ConfigureCodeAnalyser.h - Build Analysers and Return                        //
// ver 1.2                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////

#include "configureCodeAnalysers.h"
#include "../CommentAnalyser/CommentAnalyser.h"
#include "../DependencyAnalyser/DependencyAnalyser.h"
#include "../ScopeAnalyser/ScopeAnalyser.h"
#include "../SpecialCharacterAnalyser/SpecialCharacterAnalyser.h"

namespace CodeAnalysis {
	// ----------------< Constructor >-----------------
	ConfigureCodeAnalysers::ConfigureCodeAnalysers()
	{
		initializeAnalysisNameTable();
	}

	// ------------------< taking care of all created analysers >---------------------
	ConfigureCodeAnalysers::~ConfigureCodeAnalysers()
	{
		for (auto analyser : analysers_) {
			delete analyser;
		}
	}

	// --------------< return vector of analysers >-----------------
	std::vector<ICodeAnalyser*> ConfigureCodeAnalysers::getAnalysers() {
		return analysers_;
	}

	// --------------< provide analysers that will be used for analysis >--------------
	ConfigureCodeAnalysers& ConfigureCodeAnalysers::addAnalyser(const std::string& analyser) {
		ICodeAnalyser* pAnalyserClass;
		switch (getEnum(analyser)) {
		case specialCharacter: pAnalyserClass = new SpecialCharacterAnalyser;
			Log::Demo::writeLine("--< Special Character Analysis >-- (Always turned on by default.)(Translating HTML reserved characters.)\n");
			break;
		case comment: pAnalyserClass = new CommentAnalyser;
			Log::Demo::writeLine("--< Comment Analysis >-- (Comment blocks tags added.)\n");
			break;
		case dependency: pAnalyserClass = new DependencyAnalyser;
			Log::Demo::writeLine("--< Dependency Analysis >-- (Adding dependency links.)\n");
			break;
		case scope: pAnalyserClass = new ScopeAnalyser;
			Log::Demo::writeLine("--< Scope Analysis >-- (Function bodies tags and Class bodies tags added.)\n");
			break;
		default: Log::Debug::writeLine("Error: Analyser: " + analyser + "not exist!");
			throw std::exception("Analyser not exist! Please check log for detail (analyser name is printed in logger).");
		}
		Log::Debug::writeLine("pushing_back: " + analyser + " analyser");
		analysers_.push_back(pAnalyserClass);
		sortAnalysers();
		return *this;
	}

	// ----------< private - keep the analysers sorted in the container from higher priority to lower >----------
	void ConfigureCodeAnalysers::sortAnalysers() {
		std::sort(analysers_.begin(), analysers_.end(), analyserComparator);
	}

	// --------------< private - set up the analizers this class supported >-----------------
	void ConfigureCodeAnalysers::initializeAnalysisNameTable() {
		analysisNameTable_["specialCharacter"] = specialCharacter;
		analysisNameTable_["comment"] = comment;
		analysisNameTable_["dependency"] = dependency;
		analysisNameTable_["scope"] = scope;
	}

	// -------------< private - reflect a Enum type from a string type >------------
	ConfigureCodeAnalysers::analysisName ConfigureCodeAnalysers::getEnum(const std::string& s) {
		return analysisNameTable_[s];
	}
	
	// ----------< method to support sorting and priority queue for ICodeAnalyser Objects >------------
	bool analyserComparator(ICodeAnalyser* analyser1, ICodeAnalyser* analyser2)
	{
		return (analyser1->getPriority() < analyser2->getPriority());
	}
}

#ifdef TEST_CONFIGURECODEANALYSERS

int main() {
	try {
		using namespace CodeAnalysis;
		Log::startLoggers("debug");

		std::string file1 = FileSystem::Path::getFullFileSpec("CppAnalyser.h");
		std::string directory1 = FileSystem::Path::getPath(file1);

		ConfigureCodeAnalysers cca;
		cca.addAnalyser("specialCharacter").addAnalyser("comment").addAnalyser("dependency").addAnalyser("scope");
		auto analysers = cca.getAnalysers();

		for (auto analyser : analysers) {
			Log::Debug::writeLine("Analyser with a priority of " + std::to_string(analyser->getPriority()) + " is generated and ready to use.");
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

