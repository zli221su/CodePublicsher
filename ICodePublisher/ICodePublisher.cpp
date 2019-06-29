



#include "ICodePublisher.h"
#include "../CodePublisher/CodePublisher.h"


ICodePublisher* CodePublisherFactory::createCodePublisher() {
	return new CodePublish::CodePublisher();
}

#ifdef TEST_ICODEPUBLISHER

int main() {
	ICodePublisher* pCp;
	CodePublisherFactory factory;
	pCp = factory.createCodePublisher();


	getchar();
	getchar();
}

#endif