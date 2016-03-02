#ifndef MESSAGE_FINISHED_PARSING_H
#define MESSAGE_FINISHED_PARSING_H

#include <sstream>
#include <iomanip>

#include "data/ErrorCountInfo.h"
#include "utility/messaging/Message.h"
#include "utility/messaging/type/MessageStatus.h"

class MessageFinishedParsing
	: public Message<MessageFinishedParsing>
{
public:
	MessageFinishedParsing(size_t fileCount, size_t totalFileCount, float parseTime, ErrorCountInfo errorCount)
		: fileCount(fileCount)
		, totalFileCount(totalFileCount)
		, parseTime(parseTime)
		, errorCount(errorCount)
	{
	}

	static const std::string getStaticType()
	{
		return "MessageFinishedParsing";
	}

	virtual void dispatch()
	{
		MessageStatus(getStatusStr(), errorCount.total > 0).dispatch();

		Message<MessageFinishedParsing>::dispatch();
	}

	std::string getStatusStr() const
	{
		std::stringstream ss;
		ss << "Finished analysis: ";
		ss << fileCount << "/" << totalFileCount << " files, ";
		ss << std::setprecision(2) << std::fixed << parseTime << " seconds, ";
		ss << errorCount.total << " error" << (errorCount.total > 1 ? "s" : "");
		if (errorCount.fatal > 0)
		{
			ss << " (" << errorCount.fatal << " fatal)";
		}
		return ss.str();
	}

	virtual void print(std::ostream& os) const
	{
		os << getStatusStr();
	}

	size_t fileCount;
	size_t totalFileCount;
	float parseTime;
	ErrorCountInfo errorCount;
};

#endif // MESSAGE_FINISHED_PARSING_H
