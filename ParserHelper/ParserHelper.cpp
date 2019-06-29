/////////////////////////////////////////////////////////////////////////////////
// ParserHelper.cpp - Parse  by using Parser                                   //
// ver 1.0                                                                     //
// Zhi Li, CSE687 - Object Oriented Design, Spring 2019                        //
/////////////////////////////////////////////////////////////////////////////////

# include "ParserHelper.h"

namespace CodeAnalysis {
	// -------------< use parser to analyse a file >--------------
	void ParserHelper::initializeParser(const std::string& file) {
		std::string fileSpec;
		fileSpec = FileSystem::Path::getFullFileSpec(file);

		delete pConfigure_;
		pConfigure_ = new ConfigParseForCodeAnal;
		pParser_ = pConfigure_->Build();

		std::string name;
		if (pParser_)
		{
			name = FileSystem::Path::getName(file);
			if (!pConfigure_->Attach(fileSpec))
			{
				Log::Debug::writeLine("could not open file " + name);
				return;
			}
			Log::Debug::Title("Attaching " + fileSpec);
		}
		else
		{
			Log::Debug::title("Parser not built");
			return;
		}
		// save current package name
		Log::Debug::writeLine("\n Getting package name! fileName = " + name);
		Repository* pRepo = Repository::getInstance();
		pRepo->package() = name;
		// parse the package
		Log::Debug::title(" Parsing package! ");
		while (pParser_->next())
		{
			pParser_->parse();
		}
		Log::Debug::writeLine();
		// final AST operations
		Log::Debug::writeLine("Getting root node!");
		pGlobalScope_ = pRepo->getGlobalScope();

		complexityEval(pGlobalScope_);
	}

	Parser* ParserHelper::getParser() {
		return pParser_;
	}

	ASTNode* ParserHelper::getGlobalScope() {
		return pGlobalScope_;
	}

	// ------------< take care the pointer to parser >-------------
	ParserHelper::~ParserHelper() {
		delete pConfigure_;
	}
}

#ifdef TEST_PARSERHELPER

int main()
{
	using namespace CodeAnalysis;
	try {
		ParserHelper parserHelper;

		std::string file1 = "ParserHelper.cpp";
		Utilities::Title("Parsing " + file1);
		parserHelper.initializeParser(file1);
		auto root = parserHelper.getGlobalScope();
		TreeWalk(root);

		std::string file2 = "ParserHelper.h";
		Utilities::Title("Parsing " + file2);
		parserHelper.initializeParser(file2);
		root = parserHelper.getGlobalScope();
		TreeWalk(root);

	}
	catch (std::exception& ex) {
		Utilities::Title("std::exception caught\n\n" + std::string(ex.what()));
	}
	catch (...) {
		std::cout << "unknown exception";
	}
}

#endif