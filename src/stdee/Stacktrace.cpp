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

#include <stdee/Stacktrace.h>
#include <iostream>

namespace stdee {
namespace {

std::string getFile(const std::string& fileName) {
	const std::size_t fileLength = 50;
	std::string rv;

	if(fileName.size() > fileLength) {
		rv = "..." + fileName.substr(fileName.size()+3-fileLength);
	}
	else {
		rv = fileName;
	}
	while(rv.size() < fileLength) {
		rv += " ";
	}

	return rv;
}

std::string getLineNo(unsigned int lineNo) {
	const std::size_t lineNoLength = 6;
	std::string rv = std::to_string(lineNo);

	while(rv.size() < lineNoLength) {
		rv = " " + rv;
	}

	return rv;
}

}
void Stacktrace::add(const std::string& function, const std::string& file, unsigned int lineNo) {
	std::tuple<std::string, std::string, unsigned int> entry = std::make_tuple(function, file, lineNo);
	backtrace.push_back(entry);
}

void Stacktrace::dump() const {
	std::cerr << "\nStacktrace (file, lineno, function):" << std::endl;
	for (const auto& st : backtrace) {
		std::cerr << getFile(std::get<1>(st)) << " at " << getLineNo(std::get<2>(st)) << ": " << std::get<0>(st) << std::endl;
	}
}

} /* namespace stdee */
