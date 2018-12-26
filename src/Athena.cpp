//============================================================================
// Name        : Athena.cpp
// Author      : Sven Lukas
// Version     : 0.1
// License     : MIT License
//============================================================================
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
#include <athena/processor/Aggregate.h>
#include <athena/Column.h>-
#include <athena/Field.h>
#include <athena/InputCSV.h>
#include <stdee/Logger.h>
#include <vector>
#include <iostream>
#include <fstream>


int main(int argc, const char *argv[]) {
	if(argc < 2) {
	    athena::InputCSV inputCSV(std::cin);
		athena::processor::OutputCSV processor(std::cout);
		inputCSV.addProcessor(processor);
	    inputCSV.run();
	}
	else if(argc == 2) {
		std::ifstream stream;
	    stream.open(argv[1], std::ios::in);

	    if(!stream) {
	        stdee::logger.error << "Couldn't open file \"" << argv[1] << "\"\n";
	        return -1;
	    }

	    athena::InputCSV inputCSV(stream);
		athena::processor::OutputCSV processor(std::cout);
		inputCSV.addProcessor(processor);
	    inputCSV.run();
	    stream.close();
	}
	else {
        stdee::logger.error << "Invalid parameter\n";
        return -1;
	}

//	athena::processor::Aggregate processor1;
//	processor1.addDefinition("TS", athena::processor::Aggregate::Type::none);
//	processor1.addDefinition("Name", athena::processor::Aggregate::Type::none);
//	processor1.addDefinition("Age", athena::processor::Aggregate::Type::sum);

    return 0;
}
