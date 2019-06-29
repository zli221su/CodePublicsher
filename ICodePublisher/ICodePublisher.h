#pragma once







#include <string>
#include <vector>
#include "../CppCommWithFileXfer/Message/Message.h"


class ICodePublisher
{
public:
	virtual ~ICodePublisher() = 0 {};
	virtual std::vector<std::string> procMsg(MsgPassingCommunication::Message recieved) = 0;
};
extern "C" {

	struct CodePublisherFactory {
		ICodePublisher* createCodePublisher();
	};
}

