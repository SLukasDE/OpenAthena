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

#ifndef ATHENA_PROCESSOR_AGGREGATE_H_
#define ATHENA_PROCESSOR_AGGREGATE_H_

#include <athena/processor/Processor.h>
#include <athena/processor/Aggregate.h>
#include <tuple>
#include <string>
#include <vector>
#include <memory>

namespace athena {
namespace processor {

class Aggregate: public Processor {
public:
	enum class Type {
		none, count, max, min, avg, sum, first, last
	};

	struct DefEntry {
		DefEntry(unsigned int destinationIndex, std::string name, Aggregate::Type aggType);

		// Column for keyField and valueFields
		Column column;

		// index of input row that is responsible for this instance
		unsigned int index = 0;

		// if aggType is "none", then keyField is used,
		// if aggType is "min", "max", ... then valueFields is used
		Aggregate::Type aggType;
		Field keyField;
		std::vector<Field> valueFields;
	};

	Aggregate(/* ideal: Parse-String, z.B. std::string str="LOGIN=COUNT;ARTIKEL=MAX"*/);
	~Aggregate();

	void addDefinition(const std::string& columnName, Aggregate::Type aggType);

	template<typename T> // T = container<std::tuple<std::string, athena::utility::Aggregate::Type> >
	void setDefinition(const T& t);

	void flush();

	static const char* typeToString(Type type);
	static Type stringToType(const std::string& agg);

	// calculates the result type of aggregation of values with given type.
	static Column::Type getAggregateType(Type aggregateType, Column::Type valueType);

	static Field aggregate(Type type, const Column& column, const std::vector<Field>& values);

protected:
	void reset() override;

	void init(const Row& row) override;
	void processRow(const Row& row) override;

private:
	std::vector<std::unique_ptr<DefEntry>> definition;
};

template<typename T>
void Aggregate::setDefinition(const T& t) {
	reset();

	definition.clear();
	for(const auto& v : t) {
		addDefinition(std::get<0>(v), std::get<1>(v));
	}
}

} /* namespace processor */
} /* namespace athena */

#endif /* ATHENA_PROCESSOR_AGGREGATE_H_ */
