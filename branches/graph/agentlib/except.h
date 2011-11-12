#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <strstream>
#include <string>

using namespace std;

class baseException
{
	public:
		baseException(const string& str = ""):
		    msgString(str)
		{
			if (msgString == "")
				msgString = "Unspecified exception";
		}

		string what() const
		{
			return msgString;
		}

	protected:
		string msgString;
};

// error in graph class
class graphError: public baseException
{
	public:
		graphError(const string& msg = ""):
			baseException(msg)
		{}
};

#endif
