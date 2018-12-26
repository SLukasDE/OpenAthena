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

#ifndef ATHENA_FIELD_H_
#define ATHENA_FIELD_H_

#include <athena/Column.h>
#include <string>
#include <chrono>

namespace athena {

class Field {
public:
	Field(const Column& column);
	Field(const Column& column, const Field& value);
	Field(const Column& column, double value);
	Field(const Column& column, std::string value);
	Field(const Column& column, std::chrono::system_clock::time_point value);
	~Field();

	bool isEmpty() const;
	bool operator()() const;
	const Column& getColumn() const;

	double getNumber() const;
	void setNumber(double value);

	std::string getText() const;
	void setText(const std::string& value);

	std::chrono::system_clock::time_point getTimestamp() const;
	void setTimestamp(std::chrono::system_clock::time_point value);

private:
	const Column* column;
	bool empty = true;
	double number = 0;
	std::string text;
	std::chrono::system_clock::time_point timestamp;
};

inline bool operator< (const Field& lhs, const Field& rhs) {
	if(lhs.getColumn().type == rhs.getColumn().type) {
		switch(lhs.getColumn().type) {
		case Column::Type::number:
			return lhs.getNumber() < rhs.getNumber();
		case Column::Type::text:
			return lhs.getText() < rhs.getText();
		case Column::Type::timestamp:
			return lhs.getTimestamp() < rhs.getTimestamp();
		//default:
		case Column::Type::none:
			break;
		}
	}
	return lhs.getText() < rhs.getText();
}
inline bool operator> (const Field& lhs, const Field& rhs) { return rhs < lhs; }
inline bool operator<=(const Field& lhs, const Field& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Field& lhs, const Field& rhs) { return !(lhs < rhs); }

inline bool operator==(const Field& lhs, const Field& rhs) {
	if(lhs.getColumn().type == rhs.getColumn().type) {
		switch(lhs.getColumn().type) {
		case Column::Type::number:
			return lhs.getNumber() == rhs.getNumber();
		case Column::Type::text:
			return lhs.getText() == rhs.getText();
		case Column::Type::timestamp:
			return lhs.getTimestamp() == rhs.getTimestamp();
		//default:
		case Column::Type::none:
			break;
		}
	}
	return lhs.getText() == rhs.getText();
}
inline bool operator!=(const Field& lhs, const Field& rhs) { return !(lhs == rhs); }

} /* namespace athena */

#endif /* ATHENA_FIELD_H_ */
