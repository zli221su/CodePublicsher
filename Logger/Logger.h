#ifndef LOGGER_H
#define LOGGER_H
/////////////////////////////////////////////////////////////////////
// Logger.h - log text messages to std::ostream                    //
// ver 1.1                                                         //
//-----------------------------------------------------------------//
// Zhi Li                                                          //
// Spring 2019                                                     //
//-----------------------------------------------------------------//
// Source: Jim Fawcett (c) copyright 2015                          //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2017                            //
// Application: Several Projects, CSE687 - Object Oriented Design  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports logging for multiple concurrent clients to a
* single std::ostream.  It does this be enqueuing messages in a
* blocking queue and dequeuing with a single thread that writes to
* the std::ostream.
*
* It provides three logging classes, a non-template Logger class with
* instance methods, and two template classes StaticLogger<int> with static
* methods.
*
* In most of my code, I use the following definitions (under Log namespace):
*
*	using Debug = StaticLogger<1>;
*	using Demo = StaticLogger<2>;
*
* That allows a user to determine the kinds of outputs the program
* presents based on which of these loggers are started, e.g., by
* changing only a couple of lines of code.
*
* Build Process:
* --------------
* Required Files: Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
* Build Command: devenv logger.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.1 : 7 Apr 2019
* - One line demo add to "debug" Log mode
*
* --------------------
* ver 1.0 : 8 Mar 2019
* - first release
*
*/

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"


class Logger
{
public:
	Logger() {}
	void attach(std::ostream* pOut);
	void start();
	bool running();
	void pause(bool doPause);
	bool paused();
	void flush();
	void stop(const std::string& msg = "");
	void wait();
	void write(const std::string& msg);
	void writeLine(const std::string& msg);
	void title(const std::string& msg, char underline = '-');
	void Title(const std::string& msg, char underline = '=');
	~Logger();
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
private:
	std::thread* _pThr;
	std::vector<std::ostream*> streams_;
	Async::BlockingQueue<std::string> _queue;
	bool _ThreadRunning = false;
	bool _Paused = false;
};

template<int i>
class StaticLogger
{
public:
	static void attach(std::ostream* pOut) { _logger.attach(pOut); }
	static void start() { _logger.start(); }

	static void stop(const std::string& msg = "") { _logger.stop(msg); }
	static void write(const std::string& msg) { _logger.write(msg); }
	static void write(const std::vector<std::string>& msgs) { for (auto msg : msgs) _logger.writeLine(msg); }
	static void writeLine(const std::string& msg = "") { _logger.writeLine(msg); }
	static void warning(const std::string& msg, char surroundingLine = 'o') { _logger.Title(msg, surroundingLine); }
	static void Title(const std::string& msg, char surroundingLine = '=') { _logger.Title(msg, surroundingLine); }
	static void title(const std::string& msg, char underline = '=') { _logger.title(msg, underline); }
	static void subTitle(const std::string& msg, char underline = '-') { _logger.title(msg, underline); }
	static Logger& instance() { return _logger; }
	StaticLogger(const StaticLogger&) = delete;
	StaticLogger& operator=(const StaticLogger&) = delete;
private:
	static Logger _logger;
};

template<int i>
Logger StaticLogger<i>::_logger;

namespace Log {
	using Debug = StaticLogger<1>;
	using Demo = StaticLogger<2>;
	inline void startLoggers(std::string mode = "debug") {
		if (mode == "debug") {
			Debug::attach(&std::cout);
			Debug::start();
			Debug::Title("Debug Mode");
			Debug::writeLine("Messages may be redundant in Debug Mode!");
			Debug::writeLine("You can try the following attempts to switch to Demo Mode.");
			Debug::writeLine("      1. Check the \"Demo Mode\" CheckBox. (If using GUI interface.)");
			Debug::writeLine("      2. Add \" Mode demo \" to posted message string. (If sending messages to files who use Logger.h.)");
			Debug::writeLine("      3. Add \"demo\" to cmdLine. (If running c++ code directly.)");
		}
		if (mode == "demo") {
			Demo::attach(&std::cout);
			Demo::start();
			Debug::Title("Demo Mode");
		}
	}
	inline void stopLoggers() {
		Debug::stop("\n\n Stopping Debug Logger.");
		Demo::stop("\n\n Stopping Demo Logger.");
	}
}

namespace Logging
{
	template<int i>
	class StaticLogger
	{
	public:
		static void attach(std::ostream* pOut) { _logger.attach(pOut); }
		static void start() { _logger.start(); }
		static bool running() { return _logger.running(); }
		static void pause(bool doPause) { _logger.pause(doPause); }
		static void paused() { return _logger.paused(); }
		static void flush() { _logger.flush(); }
		static void stop(const std::string& msg = "") { _logger.stop(msg); }
		static void wait() { _logger.wait(); }
		static void write(const std::string& msg) { _logger.write(msg); }
		static void title(const std::string& msg, char underline = '-') { _logger.title(msg, underline); }
		static Logger& instance() { return _logger; }
		StaticLogger(const StaticLogger&) = delete;
		StaticLogger& operator=(const StaticLogger&) = delete;
	private:
		static Logger _logger;
	};

	template<int i>
	Logger StaticLogger<i>::_logger;

	using Rslt = Logging::StaticLogger<0>;
	using Demo = Logging::StaticLogger<1>;
	using Dbug = Logging::StaticLogger<2>;
}
#endif
