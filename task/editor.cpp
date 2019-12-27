#include "editor.h"
#include "document.h"
#include "command.h"

void editor::InsertInDocument(std::string &str) {
	if (doc_ == nullptr) {
		std::cout << "No document\n";
		return;
	}
	std::shared_ptr<command> cmd = std::shared_ptr<command>(new InsertCmd(doc_, str.size(), doc_->GetCursor()));
	doc_->Insert(str);
	history_.push(cmd);
}

void editor::DeleteInDocument() {
	if (doc_ == nullptr) {
		std::cout << "No document\n";
		return;
	}
	size_t index = doc_->GetCursor();
	if (index == 0) {
		std::cout << "Empty document\n";
		return;
	}
	std::shared_ptr<command> cmd = std::shared_ptr<command>(new DeleteCmd(doc_->GetElem(index), index, doc_));
	doc_->Delete();
	history_.push(cmd);
}

void editor::CursorLeftInDocument() {
	if (doc_ == nullptr) {
		std::cout << "No document\n";
		return;
	}
	doc_->CursorLeft();
}

void editor::CursorRightInDocument() {
	if (doc_ == nullptr) {
		std::cout << "No document\n";
		return;
	}
	doc_->CursorRight();
}

void editor::CreateDocument(std::string &name) {
	doc_ = std::make_shared<document>(name);
}

bool editor::DocumentExist() {
	return doc_ != nullptr;
}

void editor::SaveDocument() {
	if (doc_ == nullptr) {
		std::cout << "No document\n";
		return;
	}
	std::string name = doc_->GetName();
	doc_->Save(name);
}

void editor::LoadDocument(std::string &name) {
	try {
		doc_ = std::make_shared<document>(name);
		doc_->Load(name);
		while (!history_.empty()) {
			history_.pop();
		}
	} catch (std::runtime_error &err) {
		std::cout << err.what();
	} 
}

void editor::PrintDocument() {
	doc_->Print();
}

void editor::Undo() {
	if (history_.empty()) {
		throw std::logic_error("History is empty\n");
	}
	std::shared_ptr<command> last = history_.top();
	last->UnExecute();
	history_.pop();
}