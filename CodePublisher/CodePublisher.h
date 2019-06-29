#pragma once
/////////////////////////////////////////////////////////////////////
// CodePublisher.h - Codes Convert and Return Results              //
// ver 2.0                                                         //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019            //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * CodePublisher provides a class, of the same name, accept arguments
 * from comandline. It convert paticular file types (.h and .cpp by default) files who
 * match the regular expressions provided to HTML files. After
 * conversion, it return the converted files list (vector) back.
 *
 * Required Files:
 * ---------------
 * ICodePublisher.h                  // Implements ICodePublisher
 * Converter.h, Converter.cpp        // Convert files to HTML
 * FileSearch.h, FileSearch.cpp      // FileSearch class
 * FileSystem.h, FileSystem.cpp      // Path functions needed
 * StringUtilities.h                 // Title function
 * Logger.h, Logger.cpp              // Used for logging
 * Message.h, Message.cpp            // Supporting input message type
 *
 * Maintenance History:
 * --------------------
 * ver 2.0 : 25 Apr 2019
 * - remove threads and blocking queues
 *
 * --------------------
 * ver 1.1 : 9 Apr 2019
 * - exceptions in file search and convertion are handled
 *
 * --------------------
 * ver 1.0 : 3 Apr 2019
 * - first release
 *
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include "../Converter/Converter.h"
#include "../FileSearch/FileSearch.h"
#include "../FileSystem/FileSystem.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Logger/Logger.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../ICodePublisher/ICodePublisher.h"

namespace CodePublish {
	using Msg = MsgPassingCommunication::Message;

	std::vector<std::string> processMsg(Msg receive);

	class CodePublisher : public ICodePublisher
	{
	public:
		using patternsType = std::vector<std::string>;
		using filesType = std::vector<std::string>;

		static std::string version() { return "ver 2.0"; }

		CodePublisher();
		~CodePublisher() override;

		filesType procMsg(Msg recieved) override; // Send converted files to outside

	private:
		bool subDir_;
		patternsType analysers_;
		patternsType patterns_;
		patternsType regexes_;
		std::string directory_;
		std::string outputPath_;
		filesType filesFound_;

		void resetVariables();

		//Action add and set variables;
		void addAnalyser(std::string analyser);
		void setDirectory(std::string directory);
		void setOutputPath(std::string outputPath);
		void addPattern(std::string pattern);
		void addRegex(std::string regex);
		void setSubDir(std::string subDir);

		filesType searchFiles();
		filesType convert(filesType filesFound);

		enum msgEnum
		{
			Analyser, Directory, OutputPath, Pattern, Regex, SubDir
		};
		std::unordered_map<std::string, msgEnum> msgMap_;
		void initializeMsgMap();

		void parseMsg(Msg& msgs);
	};
}