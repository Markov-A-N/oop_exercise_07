#include "document.h"

void document::Save(const std::string &name) const {
	std::ofstream file{name};
	if (!file) {
		throw std::runtime_error("File isn't opened\n");
	}
	file << cursor_ << " "; 
	file << buffer_;
}

void document::Load(const std::string &name) {
	std::ifstream file{name};
	if (!file) {
		throw std::runtime_error("File isn't opened\n");
	}
	file >> cursor_;
	buffer_.clear();
	file >> buffer_;
	name_ = name;
}

void document::Insert(std::string &str) {
	buffer_.insert(cursor_, str);
	cursor_ += str.size();
}

void document::InsertIndex(std::string &str, size_t index) {
	buffer_.insert(index, str);
	cursor_++;
}

void document::Delete() {
	buffer_.erase(cursor_ - 1, 1);
	cursor_--;
}

bool document::CursorLeft() {
	if (cursor_ == 0) {
		return false;
	}
	cursor_--;
	return true;
}

bool document::CursorRight() {
	if (cursor_ == buffer_.size()) {
		return false;
	}
	cursor_++;
	return true;
}

void document::Print() {
	std::cout << buffer_ << "\n";
}

void document::RemoveLast(size_t len, size_t cursor) {
	buffer_.erase(cursor, len);
	cursor_ = cursor;
}

size_t document::GetCursor() {
	return cursor_;
}

std::string document::GetElem(size_t index) {
	std::string str;
	str += buffer_[index];
	return str;
}

std::string document::GetName() {
	return name_;
}