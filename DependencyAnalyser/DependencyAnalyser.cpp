///////////////////////////////////////////////////////////////////////////
// DependencyAnalyser.cpp - Analyse and Inject Dependency Links to File  //
// ver 1.1                                                               //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                  //
///////////////////////////////////////////////////////////////////////////

#include "DependencyAnalyser.h"

namespace CodeAnalysis {
	// ---------------< set analysis priority >-----------------
	DependencyAnalyser::DependencyAnalyser() : priority_(3)
	{
	}

	// ----------< empty destructor >----------
	DependencyAnalyser::~DependencyAnalyser()
	{
	}

	// ----------------------< pass variables to class >--------------------------
	void DependencyAnalyser::set(const std::string& file, const std::string& directory, ASTNode* root) {
		file_ = FileSystem::Path::getFullFileSpec(file);
		fileLocation_ = FileSystem::Path::getPath(file);
		directory_ = FileSystem::Path::getFullFileSpec(directory);
		dependencies_.clear();
		root_ = root;
	}

	// --------------------< analyse dependencies and store them as private >-----------------------
	void DependencyAnalyser::analyse() {
		std::vector<Lexer::ITokenCollection*>  semis = root_->statements_;
		for (auto semi : semis) {
			std::string line = semi->ToString();
			extractDependency(line);
		}
	}

	// ------------< private - try to get dependency information for current line >-----------
	void DependencyAnalyser::extractDependency(const std::string line) {
		std::string dependencyTemp;
		std::string preString = "# include \"";
		std::size_t preMatch = line.find(preString);
		if (preMatch != std::string::npos) {
			std::size_t postMatch = line.find('\"', preMatch + preString.length());
			if (postMatch != std::string::npos) {
				std::string dependency = line.substr(preMatch + preString.length(), postMatch - (preMatch + preString.length()));
				addDependency(dependency);
			}
		}
	}

	// --------------< private - pushback dependency to private container >---------------
	bool DependencyAnalyser::addDependency(std::string dependency) {
		if (dependency != FileSystem::Path::getFullFileSpec(dependency)) {
			//if this is a relative path, get the full path
			dependency = standardize(FileSystem::Path::getFullFileSpec(fileLocation_ + "\\" + dependency));
		}
		// Check if file dependency is in directory.
		if (dependency.compare(0, directory_.length(), standardize(directory_), 0, directory_.length()) == 0) {
			Log::Debug::writeLine("dependency: \"" + dependency + "\" is in directory!");
			dependencies_.push_back(dependency);
			return true;
		}
		Log::Demo::writeLine("dependency: \"" + dependency + "\" is not in directory tree!");
		// else ignore the file
		return false;
	}

	// ---------------< switch all '/' in the path to '\' >---------------
	std::string DependencyAnalyser::standardize(std::string path) {
		std::string newPath = path;
		for (size_t i = 0; i < newPath.length(); i++) {
			if (newPath[i] == '/') {
				newPath.replace(i, 1, "\\");
			}
		}
		return newPath;
	}

	// -------------< add depedency links markup code >----------------
	void DependencyAnalyser::injectCode(std::vector<std::string>& fileContent, std::vector<std::string>& addtionalFiles) {
		int i = 1;
		if (dependencies_.size() == 0) {
			Log::Debug::writeLine(" No dependencies found for [" + FileSystem::Path::getName(file_) + "]. Skipping dependency links..");
			return;
		}
		std::string links;
		links += "    <h3>Dependencies: \n";
		for (auto dependency : dependencies_) {
			addtionalFiles.push_back(FileSystem::Path::getFullFileSpec(dependency));
			links += "      <a href=\"" + FileSystem::Path::getName(dependency) + ".html\">" + FileSystem::Path::getName(dependency) + "</a>\n";
		}
		links += "    </h3>\n";
		fileContent.insert(fileContent.begin(), links);
	}

	// ------------< lower number means hight priority >--------------
	int DependencyAnalyser::getPriority() {
		return priority_;
	}
}

#ifdef TEST_DEPENDENCYANALYSER
#include "../ParserHelper/ParserHelper.h"

int main() {
	try {
		Log::startLoggers("debug");

		using namespace CodeAnalysis;
		ICodeAnalyser* ca(new DependencyAnalyser);

		std::string file = "DependencyAnalyser.cpp";
		std::string directory = "../DependencyAnalyser";

		ParserHelper ph;
		ph.initializeParser(file);
		ASTNode* root = ph.getGlobalScope();

		std::vector<std::string> fileContent;
		std::ifstream in(file);
		std::string line;

		Log::Debug::writeLine();
		Log::Debug::Title("Codes before Dependency Analysis");
		Log::Debug::writeLine();
		while (std::getline(in, line)) {
			fileContent.push_back(line);
			Log::Debug::writeLine(line);
		}

		ca->set(file, directory, root);
		ca->analyse();
		ca->injectCode(fileContent);

		Log::Debug::writeLine();
		Log::Debug::Title("Codes after Dependency Analysis");
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

