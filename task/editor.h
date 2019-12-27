#ifndef EDITOR_H 
#define EDITOR_H 1

#include <stack>
#include <string>
#include <memory>
#include <iostream>

#include "command.h"
#include "document.h"

struct editor {

	editor() : doc_{nullptr}, history_{} {}

	void InsertInDocument(std::string &str);

	void DeleteInDocument();

	void CursorLeftInDocument();

	void CursorRightInDocument();

	void CreateDocument(std::string &name);

	bool DocumentExist();

	void SaveDocument();

	void LoadDocument(std::string &name);

	void PrintDocument();

	void Undo();

private:
	std::shared_ptr<document> doc_;
	std::stack<std::shared_ptr<command>> history_;
};

#endif //EDITOR_H