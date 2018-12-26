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

#include <stdee/utility/CSV.h>
#include <utility>
#include <stdee/Logger.h>

namespace stdee {
namespace utility {

namespace {
std::map<char, std::string> initCharactersToQuote() {
	std::map<char, std::string> result;

	result.insert(std::make_pair('\\', "\\\\"));
	result.insert(std::make_pair('\n', "\\n"));
	result.insert(std::make_pair('\t', "\\t"));
	result.insert(std::make_pair('\r', "\\r"));
	return result;
}

std::map<char, ::std::string> charactersToQuote = initCharactersToQuote();

bool checkIsPrefix(std::string::const_iterator begin, std::string::const_iterator end, const std::string& prefix) {
	if(static_cast<std::size_t>(end-begin) < prefix.size()) {
		return false;
	}
	end = begin + prefix.size();
	auto mismatch = std::mismatch(begin, end, prefix.begin()).second;
	return mismatch == prefix.end();
}
}

std::string quote(const std::string& content) {
    std::string result;

    result.reserve(content.size());
    for(std::string::const_iterator it = content.begin(); it != content.end(); ++it) {
    	std::map<char, std::string>::const_iterator quoteResult = charactersToQuote.find(*it);

        if(quoteResult != charactersToQuote.end()) {
        	result += quoteResult->second;
        }
        else {
            result +=*it;
        }
    }

    return result;
}

std::string dequote(const ::std::string& content) {
    std::string result;

    result.reserve(content.size());
    bool quoteState = false;
    for(std::string::const_iterator it = content.begin(); it != content.end(); ++it) {
        if (*it == '\\' && !quoteState) {
        	/* setzte vorsorglich in den Quote-State */
        	quoteState = true;

        	/* pruefe, ob es eine fertige Uebersetzungsvorschrift gibt.
        	 * Falls ja, fuehre sie aus und beende den Quote-State wieder */
        	for(const auto& characterToQuote : charactersToQuote) {
        		if(checkIsPrefix(it, content.end(), characterToQuote.second)) {
        			it += characterToQuote.second.size() - 1;
        			result += characterToQuote.first;
                	quoteState = false;
        			break;
        		}
        	}
        } else if(quoteState) {
        	/* hier muessen besonders gequotete Zeichenfolgen uebersetzt werden */
        	/* aktuell wird das jedoch nicht gemacht, sondern einfach nur das naechste Zeichen genommen. */
        	result += *it;
            quoteState = false;
        }
        /* Normalfall, kein Quote... */
        else {
        	result += *it;
        }
    }

    return result;
}

CSV::CSV(char aSeparator)
: separator(aSeparator)
{
}

std::vector<std::string> CSV::lineToColumns(const std::string& line) const {
	std::vector<std::string> liste;
	std::string next;

    bool quote = false;

    for(auto const& c : line) {
        if(c == separator && !quote) {
            liste.push_back(dequote(next));
            next.clear();
            quote = false;
            continue;
        }

        next += c;
        if(c == '\\' && !quote) {
            quote = true;
        } else {
            quote = false;
        }
    }
    liste.push_back(dequote(next));
    next.clear();

    return liste;
}

std::string CSV::columnsToLine(const std::vector<std::string>& columns) const {
	std::string line;
	bool isFirstColumn = true;

	for(const auto& column : columns) {
		if(!isFirstColumn) {
			line += separator;
		}
		line += quote(column);
		isFirstColumn = false;
	}

	return line;
}

} /* namespace utility */
} /* namespace stdee */
