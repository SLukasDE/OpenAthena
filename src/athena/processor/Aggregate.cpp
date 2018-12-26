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

#include <athena/processor/Aggregate.h>
#include <stdexcept>
#include <algorithm>

namespace athena {
namespace processor {


Aggregate::DefEntry::DefEntry(unsigned int destinationIndex, std::string name, Aggregate::Type aggType)
: column(destinationIndex, name, Column::Type::none),
  aggType(aggType),
  keyField(column)
{ }


Aggregate::Aggregate()
: Processor()
{
}

Aggregate::~Aggregate() {
}


void Aggregate::addDefinition(const std::string& columnName, Aggregate::Type aggType) {
	std::unique_ptr<DefEntry> defEntry(new DefEntry(definition.size(), columnName, aggType));
	definition.push_back(std::move(defEntry));
}

void Aggregate::flush() {
	Processor::Row newRow;

	for(auto& defEntry: definition) {
		if(defEntry->aggType == Type::none) {
			newRow.push_back(defEntry->keyField);
		}
		else {
			newRow.push_back(aggregate(defEntry->aggType, defEntry->column, defEntry->valueFields));
			defEntry->valueFields.clear();
		}
	}

	write(newRow);
}


const char* Aggregate::typeToString(Aggregate::Type type) {
	switch(type) {
	case Type::avg:
		return "AVG";
	case Type::count:
		return "COUNT";
	case Type::first:
		return "FIRST";
	case Type::last:
		return "LAST";
	case Type::max:
		return "MAX";
	case Type::min:
		return "MIN";
	case Type::sum:
		return "SUM";
	default:
		break;
	}
	return "(none)";
}

Aggregate::Type Aggregate::stringToType(const std::string& agg) {
	if(agg == "COUNT") {
		return Type::count;
	}
	else if(agg == "MAX") {
		return Type::max;
	}
	else if(agg == "MIN") {
		return Type::min;
	}
	else if(agg == "AVG") {
		return Type::avg;
	}
	else if(agg == "SUM") {
		return Type::sum;
	}
	else if(agg == "FIRST") {
		return Type::first;
	}
	else if(agg == "LAST") {
		return Type::last;
	}

	return Type::none;
}

Column::Type Aggregate::getAggregateType(Type aggregateType, Column::Type valueType) {
	switch(aggregateType) {
	case Type::count: {
		return Column::Type::number;
	}
	case Type::min:
	case Type::max:
	case Type::first:
	case Type::last:
	case Type::none:
		return valueType;
	case Type::avg:
		if(valueType == Column::Type::text) {
			return Column::Type::none;
		}
		return valueType;
	case Type::sum:
		if(valueType == Column::Type::timestamp) {
			return Column::Type::none;
		}
		return valueType;
	default:
		break;
	}

	throw std::out_of_range("unknown aggregate function");
	return Column::Type::none;
}

Field Aggregate::aggregate(Type type, const Column& column, const std::vector<Field>& values) {
	switch(type) {
	case Type::count: {
		return Field(column, static_cast<double>(values.size()));
	}
	case Type::max: {
		auto iter = std::max_element(values.begin(), values.end());
		if(iter == values.end()) {
			//throw std::out_of_range("cannot find maximum element from empty container");
			return Field(column);
		}
		return Field(column, *iter);
	}
	case Type::min: {
		auto iter = std::min_element(values.begin(), values.end());
		if(iter == values.end()) {
			//throw std::out_of_range("cannot find minimum element from empty container");
			return Field(column);
		}
		return Field(column, *iter);
	}
	case Type::avg:
		if(values.empty() == false) {
			switch(values[0].getColumn().getType()) {
			case Column::Type::number: {
				double value = 0;
				for(auto iter = values.begin(); iter != values.end(); ++iter) {
					value += iter->getNumber();
				}
				value /= values.size();
				return Field(column, value);
			}
			case Column::Type::none:
			case Column::Type::timestamp:
			case Column::Type::text:
				return Field(column);
			}
		}
		return Field(column);
	case Type::sum:
		if(values.empty() == false) {
			switch(values[0].getColumn().getType()) {
			case Column::Type::text: {
				std::string value;
				for(auto iter = values.begin(); iter != values.end(); ++iter) {
					value += iter->getText();
				}
				return Field(column, value);
			}
			case Column::Type::number: {
				double value = 0;
				for(auto iter = values.begin(); iter != values.end(); ++iter) {
					value += iter->getNumber();
				}
				return Field(column, value);
			}
			case Column::Type::none:
			case Column::Type::timestamp:
				return Field(column);
			}
		}
		return Field(column);
	case Type::first: {
		auto iter = values.begin();
		if(iter == values.end()) {
			//throw std::out_of_range("cannot find first element from empty container");
			return Field(column);
		}
		return Field(column, *iter);
	}
	case Type::last: {
		auto iter = values.rbegin();
		if(iter == values.rend()) {
			//throw std::out_of_range("cannot find last element from empty container");
			return Field(column);
		}
		return Field(column, *iter);
	}
	default:
		break;
	}

	throw std::out_of_range("unknown aggregate function");
//	return Field(column);
}

void Aggregate::reset() {
	for(auto& defEntry: definition) {
		defEntry->valueFields.clear();
	}
	Processor::reset();
}

void Aggregate::init(const Row& row) {
	for(auto& defEntry: definition) {
		unsigned int i=0;
		for(; i<row.size(); ++i) {
			if(row[i].getColumn().getName() != defEntry->column.getName()) {
				continue;
			}
			Column::Type valueType = getAggregateType(defEntry->aggType, row[i].getColumn().getType());
			defEntry->column = Column(defEntry->column.index, defEntry->column.name, valueType);

			defEntry->index = i;
			if(defEntry->aggType == Aggregate::Type::none) {
				defEntry->keyField = Field(defEntry->column, row[i]);
			}
//			else {
//				defEntry->valueFields.push_back(Field(defEntry->column, row[i]));
//			}
			break;
		}
		if(i>=row.size()) {
			throw std::runtime_error("Defined column \"" + defEntry->column.getName() + "\" in row not found.");
		}
	}
}

void Aggregate::processRow(const Row& row) {
	// Check if Key-Values have been changed.
	// if yes, then flush the current aggregation values.
	for(const auto& defEntry: definition) {
		if(defEntry->aggType == Aggregate::Type::none) {
			if(defEntry->keyField != row[defEntry->index]) {
				flush();
				break;
			}
		}
	}

	for(auto& defEntry: definition) {
		if(defEntry->aggType == Aggregate::Type::none) {
			defEntry->keyField = Field(defEntry->column, row[defEntry->index]);
		}
		else {
			defEntry->valueFields.push_back(row[defEntry->index]);
		}
	}
}

} /* namespace processor */
} /* namespace athena */
