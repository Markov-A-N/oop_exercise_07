#ifndef DOCUMENT_H
#define DOCUMENT_H 1

#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct document {
	document(std::string &name) : name_(name), buffer_{}, cursor_{0} {}

	void Save(const std::string &name) const;

	void Load(const std::string &name);

	void Insert(std::string &str);

	void InsertIndex(std::string &str, size_t index);

	void Delete();

	bool CursorLeft();

	bool CursorRight();

	void Print();

	void RemoveLast(size_t len, size_t cursor);

	size_t GetCursor();

	std::string GetElem(size_t index);

	std::string GetName();

private:
	std::string name_;
	std::string buffer_;
	size_t cursor_;
};

#endif //DOCUMENT_H