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

#include <athena/Field.h>
#include <stdee/Time.h>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace athena {

Field::Field(const Column& column)
: column(&column)
{
}

Field::Field(const Column& column, const Field& value)
: column(&column)
{
	switch(column.type) {
	case Column::Type::none:
		break;
	case Column::Type::number:
		setNumber(value.getNumber());
		break;
	case Column::Type::text:
		setText(value.getText());
		break;
	case Column::Type::timestamp:
		setTimestamp(value.getTimestamp());
		break;
	default:
		throw std::invalid_argument("Invalid value, unknown type.");
	}
}

Field::Field(const Column& column, double value)
: column(&column)
{
	setNumber(value);
}

Field::Field(const Column& column, std::string value)
: column(&column)
{
	setText(value);
}

Field::Field(const Column& column, std::chrono::system_clock::time_point value)
: column(&column)
{
	setTimestamp(value);
}

Field::~Field() {
}

bool Field::isEmpty() const {
	return empty;
}

bool Field::operator()() const {
	return isEmpty() == false;
}

const Column& Field::getColumn() const {
	return *column;
}



double Field::getNumber() const {
	if(isEmpty()) {
		return 0;
	}

	switch(column->type) {
	case Column::Type::none:
		break;
	case Column::Type::number:
		return number;
	case Column::Type::text:
		return std::stod(text);
		// return boost::lexical_cast<double>(text);
	case Column::Type::timestamp:
		throw std::invalid_argument("Cannot convert timestamp to number.");
		//throw std::runtime_error("Cannot convert timestamp to number.");
		//throw std::out_of_range("Cannot convert timestamp to number.");
	}
	return 0;
}

void Field::setNumber(double value) {
	switch(column->type) {
	case Column::Type::none:
		return;
	case Column::Type::number:
		number = value;
		break;
	case Column::Type::text:
		text = std::to_string(value);
		break;
	case Column::Type::timestamp:
		throw std::invalid_argument("Cannot convert number to timestamp.");
		//throw std::runtime_error("Cannot convert number to timestamp.");
		//throw std::out_of_range("Cannot convert number to timestamp.");
	}

	empty = false;
}

std::string Field::getText() const {
	if(isEmpty()) {
		return "";
	}

	switch(column->type) {
	case Column::Type::none:
		break;
	case Column::Type::number:
		return std::to_string(number);
	case Column::Type::text:
		return text;
	case Column::Type::timestamp:
		return stdee::Time::toString<std::chrono::system_clock>(timestamp);
	}
	return "";
}

void Field::setText(const std::string& value) {
	switch(column->type) {
	case Column::Type::none:
		return;
	case Column::Type::number:
		number = std::stod(value);
		break;
	case Column::Type::text:
		text = value;
		break;
	case Column::Type::timestamp:
		// timestamp = stdee::Time::fromString<std::chrono::system_clock>("2018-08-04 12:20:00");
		timestamp = stdee::Time::fromString<std::chrono::system_clock>(value);
	}

	empty = false;
}

std::chrono::system_clock::time_point Field::getTimestamp() const {
	if(isEmpty()) {
		return std::chrono::time_point<std::chrono::system_clock>::min();
	}

	switch(column->type) {
	case Column::Type::none:
		break;
	case Column::Type::number:
		throw std::invalid_argument("Cannot convert number to timestamp.");
	case Column::Type::text:
		return stdee::Time::fromString<std::chrono::system_clock>(text);
	case Column::Type::timestamp:
		return timestamp;
	}
	return std::chrono::time_point<std::chrono::system_clock>::min();
}

void Field::setTimestamp(std::chrono::system_clock::time_point value) {
	switch(column->type) {
	case Column::Type::none:
		return;
	case Column::Type::number:
		throw std::invalid_argument("Cannot convert timestamp to number.");
	case Column::Type::text:
		text = stdee::Time::toString<std::chrono::system_clock>(value);
		break;
	case Column::Type::timestamp:
		timestamp = value;
	}

	empty = false;
}

} /* namespace athena */
