/////////////////////////////////////////////////////////////////////
// FileSearch.cpp - File Searcher                                  //
// ver 1.2                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////

#include "FileSearch.h"

// ----------< sets patterns, directory, log and searching file types >------------
FileSearch::FileSearch(const patternsType &patterns, const std::string &directory, const std::vector<std::string> fileTypes, bool subDir)
	: pPatterns_(patterns.size() > 0 ? &patterns : &wildPattern), pDirectory_(&directory), fileTypes_(fileTypes), subDir_(subDir)
{
	Log::Debug::Title("Entering FileSearch class");
	Log::Demo::Title("Entering FileSearch class");

	if (fileTypes_.size() == 0) {
		fileTypes_.push_back("*.h");
		fileTypes_.push_back("*.cpp");
	}
}

// ------------< search files by calling DirExplorer-Naive pacage >-------------
bool FileSearch::searchFiles() {
	const filesType *filesTemp = nullptr;
	FileSystem::DirExplorerN dirExplorerN(*pDirectory_);
	for (auto fileType : fileTypes_) {
		dirExplorerN.addPattern(fileType);
	}
	dirExplorerN.recurse(subDir_);
	Log::Demo::title("Matching file types in provided folder.");
	Log::Debug::title("Following files are found by matching file types");

	dirExplorerN.search();
	filesTemp = &dirExplorerN.filesToProcess();
	if (filesTemp == nullptr || filesTemp->size() == 0) {
		throw std::exception("No file matches Pattern(s).");
	}
	filterFiles(filesTemp);
	if (filesTemp->size() == 0) {
		throw std::exception("No file matches Pattern(s).");
	}
	if (outputFiles_.size() == 0) {
		throw std::exception("No file matches Regular Expression(s).");
	}

	Log::Debug::writeLine("End of FileSearch!");
	return true;
}

// ---------< private - applying regular expresion on files found by DirExplorerN >----------
void FileSearch::filterFiles(const filesType *filesTemp) {
	Log::Demo::title("Matching regular expressions.");
	Log::Debug::title("Matching regular expressions.");
	Log::Debug::writeLine("Regular expressions: ");
	for (auto patt : *pPatterns_) {
		Log::Debug::writeLine(" " + patt);
	}
	if (filesTemp == nullptr) {
		return;
	}
	for (auto file : *filesTemp) {
		std::string purFileName = FileSystem::Path::getName(file);
		for (size_t patIndex = 0; patIndex < pPatterns_->size(); patIndex++) {
			if (std::regex_match(purFileName, std::regex((*pPatterns_)[patIndex]))) {
				file = FileSystem::Path::getFullFileSpec(file);
				outputFiles_.push_back(file);
				Log::Debug::writeLine("File: " + file);
				Log::Debug::writeLine(" matches pattern: " + (*pPatterns_)[patIndex]);
				break;
			}
		}
	}
}

// ----------< return found files >------------
const FileSearch::filesType FileSearch::getOutputFiles() const {
	return outputFiles_;
}


#ifdef TEST_FILESEARCH

int main(int argc, char* argv[])
{
	try {
		Log::startLoggers();

		Log::Debug::Title("Demonstrate FileSearch, " + FileSearch::version());
		
		std::string message;
		message += "\n Command line: [Directory Path] [Regualr Expression]*\n";
		message += "   [Directory Path] can be absolute path or relative path.\n";
		message += "   You can input as many [Regualr Expression] as you want.\n";
		message += "   If you input multiple [Regular Expression], seperate them with spaces.\n";
		Log::Debug::write(message);

		FileSearch::folderType directory;
		directory = argv[1];

		FileSearch::patternsType patterns;
		for (int i = 2; i < argc; i++) {
			patterns.push_back(argv[i]);
		}
		FileSearch fs(patterns, directory);
		fs.searchFiles();

		Log::stopLoggers();
	}
	catch (int ex) {
		Log::Debug::writeLine("std::exception No." + std::to_string(ex) + " caught\n\n");
	}
	catch (...) {
		Log::Debug::writeLine("unknown exception");
	}
	getchar();
	return 0;
}

#endif