/////////////////////////////////////////////////////////////////////
// CodePublisher.h - Codes Convert and Return Results              //
// ver 2.0                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////

#include "CodePublisher.h"

// --------------< Constructor >----------------
namespace CodePublish {
	std::vector<std::string> processMsg(Msg receive) {
		CodePublisher cp;
		return cp.procMsg(receive);
	}

	// --------------< comstructor initial output path >------------------
	CodePublisher::CodePublisher()
	{
		Log::Demo::title("Rules needs to be followed: ");
		Log::Demo::subTitle("Patterns and Regexes have to be separated by one space (If you provided multiple patterns or regexes). (Attetion!)");
		Log::Demo::writeLine();

		initializeMsgMap();
	}

	// -------------< default destructor >---------------
	CodePublisher::~CodePublisher()
	{
		Log::Demo::writeLine("CodePublisher instance destroyed");
	}

	// -----------< get and proceed message >-----------
	CodePublisher::filesType CodePublisher::procMsg(Msg received) {
		resetVariables();
		parseMsg(received);
		if (received.command() == "search") {
			return searchFiles();
		}
		else if (received.command() == "publish") {
			searchFiles();
			return convert(filesFound_);
		}
		filesType emptyResponse;
		return emptyResponse;
	}

	// -------------< private - initialize message map >-------------
	void CodePublisher::initializeMsgMap() {
		msgMap_["Analyser"] = msgEnum::Analyser;
		msgMap_["Directory"] = msgEnum::Directory;
		msgMap_["OutputPath"] = msgEnum::OutputPath;
		msgMap_["Pattern"] = msgEnum::Pattern;
		msgMap_["Regex"] = msgEnum::Regex;
		msgMap_["SubDir"] = msgEnum::SubDir;
	}

	// -----------< private - Reset the variables to empty >----------
	void CodePublisher::resetVariables() {
		analysers_.clear();
		directory_.clear();
		//outputFiles_.clear();
		patterns_.clear();
		regexes_.clear();
		subDir_ = true;
		filesFound_.clear();
	}

	// ---------------< private - extract infor mation from messages >----------------
	void CodePublisher::parseMsg(Msg& msg) {
		Log::Demo::writeLine();
		Log::Demo::title("Message after parsing:");
		auto msgs = msg.attributes();
		for (auto itMsg = msgs.begin(); itMsg != msgs.end(); itMsg++) {
			for (auto itMap = msgMap_.begin(); itMap != msgMap_.end(); itMap++) {
				if (itMsg->first.find(itMap->first) != std::string::npos) {
					// action found
					switch (itMap->second) {
					case Analyser:
						addAnalyser(itMsg->second);
						break;
					case Directory:
						setDirectory(itMsg->second);
						break;
					case OutputPath:
						setOutputPath(itMsg->second);
						break;
					case Pattern:
						addPattern(itMsg->second);
						break;
					case Regex:
						addRegex(itMsg->second);
						break;
					case SubDir:
						setSubDir(itMsg->second);
						break;
					default: Log::Debug::warning("Unexpected Error! Check parseMsg() function in CodePublisher!");
					}
					break;
				}
			}
		}
	}

	// -------------< set parameters to CodePublisher >--------------

	void CodePublisher::addAnalyser(std::string analyser) {
		analysers_.push_back(analyser);
		Log::Demo::writeLine("Analyser: " + analyser);
	}

	void CodePublisher::setDirectory(std::string directory) {
		directory_ = FileSystem::Path::getFullFileSpec(directory);
		Log::Demo::writeLine("Directory: " + directory_);
	}

	void CodePublisher::setOutputPath(std::string outputPath) {
		outputPath_ = FileSystem::Path::getFullFileSpec(outputPath) + "/";
		Log::Demo::writeLine("Output Path: " + outputPath_);
	}

	void CodePublisher::addPattern(std::string pattern) {
		patterns_.push_back(pattern);
		Log::Demo::writeLine("Pattern: " + pattern);
	}

	void CodePublisher::addRegex(std::string regex) {
		regexes_.push_back(regex);
		Log::Demo::writeLine("Regex: " + regex);
	}

	void CodePublisher::setSubDir(std::string subDir) {
		subDir_ = !(subDir == "false");
		Log::Demo::writeLine("SubDir: " + std::to_string(subDir_));
	}

	// -----------< search files >--------------
	CodePublisher::filesType CodePublisher::searchFiles() {
		filesType filesFound;
		try {
			Log::Demo::writeLine("\n Searching files.");
			Log::Demo::writeLine("regexes_");
			Log::Demo::write(regexes_);
			Log::Demo::writeLine("directory_");
			Log::Demo::writeLine(directory_);
			Log::Demo::writeLine("patterns_");
			Log::Demo::write(patterns_);
			Log::Demo::writeLine("subDir_");
			Log::Demo::writeLine(std::to_string(subDir_));
			FileSearch fs(regexes_, directory_, patterns_, subDir_);
			if (!fs.searchFiles()) {
				filesFound.clear();
				filesFound.push_back("fileNotFound");
			}
			else {
				filesFound = fs.getOutputFiles();
				filesFound_ = filesFound;
				filesFound.insert(filesFound.begin(), "filesFound");
			}
		}
		catch (std::exception& ex) {
			Log::Demo::warning("Execption caught: " + std::string(ex.what()));
			filesFound.clear();
			filesFound.push_back("StatusBar\n" + std::string(ex.what()));
		}
		catch (...) {
			Log::Demo::warning("Unknow Exception Caught!");
			filesFound.clear();
			filesFound.push_back("StatusBar\nUnknow Exception caught in CodePublisher, please check if your inputs are valid!");
		}
		return filesFound;
	}

	// --------------< convert files >----------------
	CodePublisher::filesType CodePublisher::convert(CodePublisher::filesType filesFound) {
		filesType filesConverted;
		try {
			Log::Demo::writeLine("\n Sending files for conversion.");
			CodeConvert::Converter cvt(filesFound, outputPath_, directory_);
			for (std::string analyser : analysers_) {
				cvt.addAnalyser(analyser);
			}
			cvt.doConversion();
			filesConverted = cvt.getOutputFiles();
			filesConverted.insert(filesConverted.begin(), "filesConverted");
			filesConverted.push_back("filesFound");
			for (auto rootFile : cvt.getRootOutputFiles()) {
				filesConverted.push_back(rootFile);
			}
		}
		catch (std::exception& ex) {
			Log::Demo::warning("Execption caught: " + std::string(ex.what()));
			filesConverted.clear();
			filesConverted.push_back("StatusBar\n" + std::string(ex.what()));
		}
		catch (...) {
			Log::Demo::warning("Unknow Exception Caught!");
			filesConverted.clear();
			filesConverted.push_back("StatusBar\nUnknow Exception caught in CodePublisher, please check if your inputs are valid!");
		}
		return filesConverted;
	}
}


#ifdef TEST_CODEPUBLISHER

//namespace CodePublish {
//	std::vector<std::string> cmdToVector(int argc, char* argv[]) {
//		std::vector<std::string> result;
//		for (int i = 0; i < argc; i++) {
//			result.push_back(argv[i]);
//		}
//		return result;
//	}
//	std::string cmdToString(int argc, char* argv[], std::string separator = "\n") {
//		std::string result;
//		for (int i = 0; i < argc; i++) {
//			result += argv[i] + separator;
//		}
//		return result;
//	}
//}

int main(int argc, char* argv[])
{
	try {
		using namespace CodePublish;

		Log::startLoggers("demo");
		Log::Demo::Title("Demo Mode");

		Log::Demo::Title("Demonstrate Project 4 from CodePublisher, " + CodePublisher::version());

		Msg msg;
		msg.attribute("Analyser1", "comment");
		msg.attribute("Analyser2", "scope");
		msg.attribute("Directory", "..\\..\\CodePublisher");
		msg.attribute("OutputPath", "../convertedPages");
		msg.attribute("Pattern", "*.h");
		msg.attribute("Regex", "(C)(.*)");
		msg.attribute("SubDir", "true");

		msg.command("search");
		std::vector<std::string> foundFiles = processMsg(msg);
		for (auto s : foundFiles) {
			Log::Demo::writeLine(s);
		}

		msg.command("convert");
		std::vector<std::string> convertedFiles = processMsg(msg);
		for (auto s : convertedFiles) {
			Log::Demo::writeLine(s);
		}

		Log::Demo::write("\n Showing Result! \n");

		Log::Debug::Title("End of debuging!");
		getchar();
	}
	catch (std::exception& ex) {
		Log::Debug::Title("std::exception caught\n\n" + std::string(ex.what()));
	}
	catch (...) {
		std::cout << "unknown exception";
	}
	getchar();
	return 0;
}

#endif

