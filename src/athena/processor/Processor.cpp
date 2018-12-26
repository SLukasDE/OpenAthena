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

#include <athena/processor/Processor.h>
#include <algorithm>

namespace athena {
namespace processor {

Processor::Processor() {
}

Processor::~Processor() {
	setParent(nullptr);
	while(!children.empty()) {
		children.back().get().setParent(nullptr);
	}
}

void Processor::setParent(Processor* processor) {
	if(processor == parent) {
		return;
	}

	if(parent) {
		for(auto iter = parent->children.begin(); iter != parent->children.end(); ++iter) {
			if(&iter->get() == this) {
				parent->children.erase(iter);
				break;
			}
		}
	}
	parent = processor;
	if(parent) {
		parent->children.push_back(std::ref(*this));
	}
}

void Processor::addRow(const Row& row) {
	// wenn das die erste Zeile ist, dann zeihe daraus erstmal die Informationen für die Spaltendefinition
	// und speichere sie in indexToColumns
	if(isFirstRow) {
		isFirstRow = false;
		indexToColumns.clear();
		for(unsigned int i=0; i<row.size(); ++i) {
			indexToColumns.insert(std::make_pair(i, std::cref(row[i].getColumn())));
		}
		init(row);
	}

	processRow(row);
}

void Processor::flush() {

}

void Processor::reset() {
	isFirstRow = true;
}

void Processor::init(const Row& row) {
}

void Processor::processRow(const Row& row) {
	write(row);
}

void Processor::write(const Row& row) {
	for(auto child : children) {
		child.get().addRow(row);
	}
}

} /* namespace processor */
} /* namespace athena */
