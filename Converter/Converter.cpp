/////////////////////////////////////////////////////////////////////
// Converter.cpp - Codes Converter                                 //
// ver 3.3                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////

#include "Converter.h"

namespace CodeConvert {
	// ----------< Constructer sets inputFiles, outputPath and inputDirectory >--------------
	Converter::Converter(const filesType& inputFiles, const std::string &outputPath, std::string inputDirectory)
		:inputFiles_(inputFiles), outputPath_(outputPath), inputDirectory_(FileSystem::Path::getFullFileSpec(inputDirectory)), ca_()
	{
		Log::Debug::title("Entering Converter class");
		Log::Demo::Title("Analysis and Conversions");
		Log::Demo::title("Entering Converter class");
		ca_.addAnalyser("specialCharacter");
	}

	// --------------< add analyser to allow customize file analysing >----------------
	Converter& Converter::addAnalyser(std::string analyser) {
		ca_.addAnalyser(analyser);
		return *this;
	}

	// ------< convert files to HTML format with dependency links and scope analysis >------
	void Converter::doConversion() {
		outputFiles_.clear();
		rootOutputFiles_.clear();
		Log::Debug::Title("Converting files");
		std::string outFile;
		Log::Demo::writeLine("Converted HTML files are output to following location: " + outputPath_);
		Log::Demo::Title("Analysing and Converting! (Please wait.)");
		Log::Debug::writeLine();
		std::unordered_set<std::string> convertedFiles;
		std::unordered_set<std::string> additionalFiles(inputFiles_.begin(), inputFiles_.end());
		std::unordered_set<std::string> newFiles;
		int dependencyLevel = 0;
		while (!additionalFiles.empty()) {
			if (dependencyLevel > 0) {
				Log::Demo::writeLine();
				Log::Demo::title("Converting the files in dependency level: " + std::to_string(dependencyLevel));
			}
			dependencyLevel++;
			for (std::string convertingFile : additionalFiles) {
				std::string curInFullFileName = FileSystem::Path::getFullFileSpec(convertingFile);
				if (!convertedFiles.insert(curInFullFileName).second) { continue; }
				std::string curInPureFileName = FileSystem::Path::getName(curInFullFileName);
				if (!inputIsValid(curInFullFileName)) {	continue;}
				outFile = outputPath_ + curInPureFileName + ".html";
				out_.open(outFile, std::ofstream::out);
				if (!out_.is_open() || !out_.good()) {
					Log::Demo::write("Error -- unable to open output file: \"" + outFile + "\" for writing.");
					Log::Debug::write("Error -- unable to open output file: \"" + outFile + "\" for writing.");
					continue;
				}
				Log::Debug::writeLine("output: " + outFile);
				contentType fileContent = ca_.analyse(curInFullFileName, inputDirectory_);
				newFiles.insert(ca_.getAdditionalFiles().begin(), ca_.getAdditionalFiles().end());
				addLeadingCodeHTML(curInPureFileName);
				addPreTag();
				addFileContents(fileContent);
				addClosingTags();
				out_.close();
				if (dependencyLevel == 1) {
					// This part is correct! Checked on Apr 05 2019.
					rootOutputFiles_.push_back(outFile);
				}
				outFile = FileSystem::Path::getName(outFile);
				outputFiles_.push_back(outFile);
			}
			additionalFiles = std::move(newFiles);
		}
	}

	// ----------< return converted files. >----------
	const Converter::filesType Converter::getOutputFiles() const
	{
		return outputFiles_;
	}

	// ----------< return root converted files (without dependencies). >----------
	const Converter::filesType Converter::getRootOutputFiles() const {
		return rootOutputFiles_;
	}

	// -----< private - add HTML head tags for HTML pages >-----
	void Converter::addLeadingCodeHTML(const std::string purFileName) {
		out_ << "<DOCTYPE !HTML>" << std::endl;
		out_ << "<html>" << std::endl;
		out_ << "  <head>" << std::endl;
		out_ << "    <Title>" << purFileName << "</Title>" << std::endl;
		out_ << "    <style>" << std::endl;
		out_ << "      body {" << std::endl;
		out_ << "        padding:15px 40px;" << std::endl;
		out_ << "        font-family: Consolas;" << std::endl;
		out_ << "        font-size: 1.25em;" << std::endl;
		out_ << "        font-weight: normal;" << std::endl;
		out_ << "      }" << std::endl;
		out_ << "      .scopesButton {" << std::endl;
		out_ << "        position: fixed;" << std::endl;
		out_ << "        right: 50px;" << std::endl;
		out_ << "      }" << std::endl;
		out_ << "      .individualButton {" << std::endl;
		out_ << "        left: 10px;" << std::endl;
		out_ << "      }" << std::endl;
		out_ << "    </style>" << std::endl;
		out_ << "    <script src=\"script.js\"></script>" << std::endl;
		out_ << "    <script>" << std::endl;
		out_ << "      function showHideClass(className) {" << std::endl;
		out_ << "        let elems = document.getElementsByClassName(className);" << std::endl;
		out_ << "        if (elems === null) {" << std::endl;
		out_ << "          return;" << std::endl;
		out_ << "        }" << std::endl;
		out_ << "        let curState = elems[0].style.display;" << std::endl;
		out_ << "        for (let i = 0; i < elems.length; ++i) {" << std::endl;
		out_ << "          if (curState === 'none')" << std::endl;
		out_ << "            elems[i].style['display'] = \"inline\";" << std::endl;
		out_ << "          else" << std::endl;
		out_ << "            elems[i].style['display'] = \"none\";" << std::endl;
		out_ << "        }" << std::endl;
		out_ << "      }" << std::endl;
		out_ << "      function controlId(id) {" << std::endl;
		out_ << "        let elem = document.getElementById(id);" << std::endl;
		out_ << "        if (elem === null) {" << std::endl;
		out_ << "          return;" << std::endl;
		out_ << "        }" << std::endl;
		out_ << "        let displayProp = elem.style.display;" << std::endl;
		out_ << "        if (displayProp === 'none')" << std::endl;
		out_ << "          elem.style['display'] = \"inline\";" << std::endl;
		out_ << "        else" << std::endl;
		out_ << "          elem.style['display'] = \"none\";" << std::endl;
		out_ << "      }" << std::endl;
		out_ << "    </script>" << std::endl;
		out_ << "  </head>" << std::endl << std::endl;
		out_ << "  <body>" << std::endl;
	}

	// -------------< private - add buttons and pre tags >-------------
	void Converter::addPreTag()
	{
		out_ << "    <pre>" << std::endl;
	}

	// ----------< priavte - write converted content to out file >----------
	void Converter::addFileContents(const std::vector<std::string> &fileContent)
	{
		for (auto line : fileContent) {
			out_ << line;
		}
	}

	// ----------< private - add closing tags for HTML files >----------
	void Converter::addClosingTags()
	{
		out_ << "    </pre>" << std::endl;
		out_ << "  </body>" << std::endl;
		out_ << "</html>" << std::endl;
	}

	//---------< private - check if input file is valid >----------
	bool Converter::inputIsValid(const std::string& inFile) {
		std::ifstream in(inFile);
		if (!in.is_open() || !in.good()) {
			Log::Demo::subTitle("Error -- unable to read file: \"" + inFile + "\" path may be invalid.");
			Log::Debug::subTitle("Error -- unable to read file: \"" + inFile + "\" path may be invalid.");
			in.close();
			return false;
		}
		in.close();
		return true;
	}
}

#ifdef TEST_CONVERTER
#include "../Display/Display.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

int main(int argc, char* argv[])
{
	try {
		Log::startLoggers("debug");

		Log::Debug::Title("Demonstrate Converter, " + CodeConvert::Converter::version());

		std::string appPath = argv[1];
		std::string outputPath = argv[2];
		std::string directory = argv[3];

		CodeConvert::Converter::filesType inputFiles;
		for (int i = 4; i < argc; i++) {
			inputFiles.push_back(FileSystem::Path::getFullFileSpec(argv[i]));
		}

		CodeConvert::Converter cvt(inputFiles, outputPath, directory)
			.addAnalyser("comment")
			.addAnalyser("dependency")
			.addAnalyser("scope");
		cvt.doConversion();

		Display dp(cvt.getOutputFiles(),appPath);
		dp.showFiles();
	}
	catch (std::exception& ex) {
		Log::Debug::writeLine("std::exception caught\n\n");
	}
	catch (...) {
		Log::Debug::writeLine("unknown exception");
	}
	Log::Debug::Title("End of Debuging!");
	Log::Demo::Title("End of Demo!");
	getchar();
	return 0;
}

#endif
