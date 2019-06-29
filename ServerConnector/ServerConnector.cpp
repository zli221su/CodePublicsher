/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                //
/////////////////////////////////////////////////////////////////////////

#include "ServerConnector.h"
#include "../FileSystem/FileSystem.h"
#include "../Process/Process.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< return name of every file on path >----------------------------

Files Server::getFiles(const Repository::SearchPath& path)
{
	return Directory::getFiles(path);
}
//----< return name of every subdirectory on path >--------------------

Dirs Server::getDirs(const Repository::SearchPath& path)
{
	return Directory::getDirectories(path);
}

namespace MsgPassingCommunication
{
	// These paths, global to MsgPassingCommunication, are needed by 
	// several of the ServerProcs, below.
	// - should make them const and make copies for ServerProc usage

	std::string sendFilePath;
	std::string saveFilePath;

	//----< show message contents >--------------------------------------

	template<typename T>
	void show(const T& t, const std::string& msg)
	{
		std::cout << "\n  " << msg.c_str();
		for (auto item : t)
		{
			std::cout << "\n    " << item.c_str();
		}
	}
	//----< test ServerProc simply echos message back to sender >--------

	std::function<std::vector<Msg>(Msg)> echo = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply = msg;
		reply.to(msg.from());
		reply.from(msg.to());
		replies.push_back(reply);
		return replies;
	};
	//----< getFiles ServerProc returns list of files on path >----------

	std::function<std::vector<Msg>(Msg)> getFiles = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("getFiles");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = path;
			Files files = Server::getFiles(searchPath);
			size_t count = 0;
			for (auto item : files)
			{
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				reply.attribute("file" + countStr, item);
			}
		}
		else
		{
			std::cout << "\n  getFiles message did not define a path attribute";
		}
		replies.push_back(reply);
		return replies;
	};
	//----< getDirs ServerProc returns list of directories on path >-----

	std::function<std::vector<Msg>(Msg)> getDirs = [](Msg msg) {
		std::vector<Msg> replies;
		Msg reply;
		reply.to(msg.from());
		reply.from(msg.to());
		reply.command("getDirs");
		std::string path = msg.value("path");
		if (path != "")
		{
			std::string searchPath = path;
			Files dirs = Server::getDirs(searchPath);
			size_t count = 0;
			for (auto item : dirs)
			{
				if (item != ".." && item != ".")
				{
					std::string countStr = Utilities::Converter<size_t>::toString(++count);
					reply.attribute("dir" + countStr, item);
				}
			}
		}
		else
		{
			std::cout << "\n  getDirs message did not define a path attribute";
		}
		replies.push_back(reply);
		return replies;
	};

	// ---------< Helper function - assemble reply message >-----------
	void assembleSendFileReply(Msg& reply, std::string& fileName) {
		reply.command("sendFile");
		reply.attribute("sendingFile", fileName);
		reply.attribute("fileName", fileName);
	}

	//----< analyze code on current server path >--------------------------
	std::function<std::vector<Msg>(Msg)> publish = [](Msg msg) {
		std::vector<Msg> replies;
		std::vector<std::string> result;
		msg.attribute("OutputPath", sendFilePath);
		ICodePublisher* pCp = nullptr;
		CodePublisherFactory factory;
		pCp = factory.createCodePublisher();
		int i = 0;
		result = pCp->procMsg(msg);

		if (result[0] != "filesFound" && result[0] != "filesConverted") {
			std::cout << "\n No returning files.";
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			replies.push_back(reply);
			return replies;
		}
		else {
			size_t i = 1;
			// sending files to client folder
			for (; i < result.size() && result[i].find("filesFound") == std::string::npos; i++) {
				Msg reply;
				reply.to(msg.from());
				reply.from(msg.to());
				assembleSendFileReply(reply, result[i]);
				replies.push_back(reply);
			}
			// Correct, check 04/23/2019
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("filesFound");
			for (size_t count = 1; (i + count) < result.size(); count++) {
				reply.attribute("fileFound" + std::to_string(count), result[i + count]);
			}
			replies.push_back(reply);
		}
		delete pCp;
		return replies;
	};
}

using namespace MsgPassingCommunication;

void demoProject4() {
	using namespace std;
	cout << "\n (Requirement #3) This Client can continue working before a reply from the CodePublisher server is sent back.";
	cout << "\n (Requirement #4) This GUI clearly show the remote directories and by pressing \"publish\" button, conversion results will be returned for displaying.";
	cout << "\n (Requirement #4) Converted files are shown in \"Converted Files\" tag.";
	cout << "\n (Requirement #5) The client and server are communicating by sending \"Messages\" (Instances of Message class)";
	cout << "\n (Requirement #6) Converted files are all sent back to client automaticaly after conversion (publish).";
	cout << "\n (Requirement #7) Two client are opened in auto demo (localhost:8081 and localhost:8082). Server url is set to localhost:8080.";
	cout << "\n (Requirement #8) Arguments received for Client is formated as:";
	cout << "\n           [client url] [server url] [remote path] [sendFile location for Client (optional)] [saveFile location for Client (optional)] [\"AutoDemo\"(optional)]";
	cout << "\n (Requirement #8) Arguments received for Server is formated as:";
	cout << "\n           [server url] [sendFile location for Server (optional)] [saveFile location for Server (optional)]";
}


int main(int argc, char* argv[])
{
	//Log::startLoggers("debug"); // Uncomment this line if you wants to see more information

	SetConsoleTitleA("CodePublisher Server Console");

	std::cout << "\n  CodePublisher Demo";
	std::cout << "\n ==========================";
	std::cout << "\n";

	sendFilePath = FileSystem::Directory::createOnPath("../ServerFilesFolders/SendFiles");
	saveFilePath = FileSystem::Directory::createOnPath("../ServerFilesFolders/SaveFiles");

	// set up server url
	const MsgPassingCommunication::EndPoint* pServerEndpoint;
	if (argc >= 2) {
		std::string serverUrl = argv[1];
		int colonPosition = serverUrl.find_first_of(':');
		std::string serverMachineAdress = serverUrl.substr(0, colonPosition - 0);
		int serverPort = std::stoi(serverUrl.substr(colonPosition + 1, serverUrl.length() - (colonPosition + 1)));
		pServerEndpoint = new MsgPassingCommunication::EndPoint(serverMachineAdress, serverPort);

		sendFilePath = argc >= 3 ? FileSystem::Directory::createOnPath(argv[2]) : sendFilePath;
		saveFilePath = argc >= 4 ? FileSystem::Directory::createOnPath(argv[3]) : saveFilePath;
	}
	else {
		pServerEndpoint = &serverEndPoint;
	}
	Server server(*pServerEndpoint, "ServerConnector");

	// may decide to remove Context
	MsgPassingCommunication::Context* pCtx = server.getContext();
	pCtx->saveFilePath = saveFilePath;
	pCtx->sendFilePath = sendFilePath;

	server.start();

	demoProject4();

	server.addMsgProc("echo", echo);
	server.addMsgProc("getFiles", getFiles);
	server.addMsgProc("getDirs", getDirs);
	server.addMsgProc("publish", publish);
	server.addMsgProc("serverQuit", echo);

	server.processMessages();

	Msg msg(serverEndPoint, serverEndPoint);  // send to self
	msg.name("msgToSelf");

	/////////////////////////////////////////////////////////////////////
	// Additional tests here, used during development
	//
	//msg.command("echo");
	//msg.attribute("verbose", "show me");
	//server.postMessage(msg);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	//msg.command("getFiles");
	//msg.remove("verbose");
	//msg.attributes()["path"] = storageRoot;
	//server.postMessage(msg);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	//msg.command("getDirs");
	//msg.attributes()["path"] = storageRoot;
	//server.postMessage(msg);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout << "\n  press enter to exit\n";
	std::cin.get();
	std::cout << "\n";

	msg.command("serverQuit");
	server.postMessage(msg);
	server.stop();
	return 0;
}

