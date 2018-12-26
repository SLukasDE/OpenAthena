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

#include <athena/InputCSV.h>
#include <stdee/utility/CSV.h>
#include <athena/Column.h>
#include <athena/Field.h>
#include <string>
#include <memory>

namespace athena {

InputCSV::InputCSV(std::istream& inputStream)
: inputStream(inputStream)
{
}

InputCSV::~InputCSV() {
}

void InputCSV::addProcessor(athena::processor::Processor& processor) {
	processors.push_back(&processor);
}

void InputCSV::run() {
	stdee::utility::CSV csv(';');
    std::vector<std::unique_ptr<athena::Column>> columns;

    while(!inputStream.eof()) {
    	std::string line;
        getline(inputStream, line);

        std::vector<std::string> stringColumns = csv.lineToColumns(line);

        if(columns.empty()) {
            for(unsigned int i=0; i<stringColumns.size(); ++i) {
            	columns.push_back(std::unique_ptr<athena::Column>(new athena::Column(i, stringColumns[i], athena::Column::Type::text)));
            }
        }
        else {
            athena::processor::Processor::Row row;
            for(unsigned int i=0; i<std::min(stringColumns.size(), columns.size()); ++i) {
            	row.push_back(athena::Field(*columns[i].get(), stringColumns[i]));
            }
            for(auto processor : processors) {
            	processor->addRow(row);
            }
        }
    }

    for(auto processor : processors) {
    	processor->flush();
    }
}

} /* namespace athena */
