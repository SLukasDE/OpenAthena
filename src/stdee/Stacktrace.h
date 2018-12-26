// The MIT License (MIT)
//
// Copyright (c) 2018 Sven Lukas
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Our apologies.  When the previous paragraph was written, lowercase had not yet
// been invented (that would involve another several millennia of evolution).
// We did not mean to shout.

#ifndef STDEE_STACKTRACE_H_
#define STDEE_STACKTRACE_H_

#ifndef BOOST_STACKTRACE_USE_ADDR2LINE
#define BOOST_STACKTRACE_USE_ADDR2LINE
#endif

#include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>
#include <vector>
#include <tuple>
#include <string>

namespace stdee {

class Stacktrace;

typedef boost::error_info<stdee::Stacktrace, stdee::Stacktrace> StacktraceType;

class Stacktrace {
public:
	inline Stacktrace();
	inline Stacktrace(const char* functionName, const char* fileName, unsigned int lineNo);

	void add(const std::string& function, const std::string& file, unsigned int lineNo);
	void dump() const;

private:
	std::vector<std::tuple<std::string, std::string, unsigned int>> backtrace;
};


inline Stacktrace::Stacktrace()
{
	boost::stacktrace::stacktrace st;
	bool isFirst = true;
	for (boost::stacktrace::frame frame : st) {
		if(isFirst) {
			isFirst = false;
			continue;
		}
		add(frame.name(), frame.source_file(), frame.source_line());
	}
}

inline Stacktrace::Stacktrace(const char* functionName, const char* fileName, unsigned int lineNo)
{
	boost::stacktrace::stacktrace st;
	bool isFirst = true;
	bool isSecond = true;

	for (boost::stacktrace::frame frame : st) {
		if(isFirst) {
			isFirst = false;
			continue;
		}
		if(isSecond) {
			isSecond = false;
			//add(functionName, fileName, lineNo);
			add(frame.name(), frame.source_file(), lineNo);
			continue;
		}
		add(frame.name(), frame.source_file(), frame.source_line());
	}
}

template <class E>
void throwAndAddStacktrace(const E& e) {
    throw boost::enable_error_info(e) << stdee::StacktraceType(stdee::Stacktrace(__func__, __FILE__, __LINE__));
}

} /* namespace stdee */

#endif /* STDEE_STACKTRACE_H_ */
