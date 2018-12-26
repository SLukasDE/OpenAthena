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

#ifndef ATHENA_PROCESSOR_PROCESSOR_H_
#define ATHENA_PROCESSOR_PROCESSOR_H_

#include <vector>
#include <athena/Field.h>
#include <map>
#include <functional>

namespace athena {
namespace processor {

class Processor {
public:
	using Row = std::vector<Field>;

	Processor();
	virtual ~Processor();

	void setParent(Processor* processor);

	// for every element in row is "row[i].getIndex() = i"
	void addRow(const Row& row);

	void flush();

protected:
	virtual void reset();

	virtual void init(const Row& row);
	virtual void processRow(const Row& row);
	void write(const Row& row);

	std::map<unsigned int, std::reference_wrapper<const Column>> indexToColumns;

private:
	Processor* parent = nullptr;
	std::vector<std::reference_wrapper<Processor>> children;
	bool isFirstRow = true;
};

} /* namespace processor */
} /* namespace athena */

#endif /* ATHENA_PROCESSOR_PROCESSOR_H_ */
