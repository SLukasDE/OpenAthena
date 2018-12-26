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

#include <athena/processor/OutputCSV.h>

namespace athena {
namespace processor {

OutputCSV::OutputCSV(std::ostream& outputStream)
: Processor(),
  outputStream(outputStream)
{
}

OutputCSV::~OutputCSV() {
}

void OutputCSV::init(const std::vector<Field>& row) {
	// Gebe erstmal den Header aus

	bool isFirstColumn = true;
	for(const auto& c : indexToColumns) {
		if(isFirstColumn) {
			isFirstColumn = false;
		}
		else {
			outputStream << ";";
		}

		outputStream << c.second.get().name;
	}
	outputStream << "\n";
}

void OutputCSV::processRow(const std::vector<Field>& row) {
//void OutputCSV::write(const std::vector<Field>& row) {
	for(unsigned int i=0; i<indexToColumns.size(); ++i) {
		if(i > 0) {
			outputStream << ";";
		}

		if(i < row.size() && row[i].isEmpty() == false) {
			outputStream << row[i].getText();
		}
	}
	outputStream << "\n";

	write(row);
}

} /* namespace processor */
} /* namespace athena */
