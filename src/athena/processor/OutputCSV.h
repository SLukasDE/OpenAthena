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

#ifndef ATHENA_PROCESSOR_OUTPUTCSV_H_
#define ATHENA_PROCESSOR_OUTPUTCSV_H_

#include <athena/processor/Processor.h>
#include <iostream>

namespace athena {
namespace processor {

class OutputCSV: public Processor {
public:
	OutputCSV(std::ostream& outputStream);
	~OutputCSV();

protected:
	void init(const std::vector<Field>& row) override;
	void processRow(const std::vector<Field>& row) override;

private:
	std::ostream& outputStream;
};

} /* namespace processor */
} /* namespace athena */

#endif /* ATHENA_PROCESSOR_OUTPUTCSV_H_ */