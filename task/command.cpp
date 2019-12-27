#include "command.h"

InsertCmd::InsertCmd(std::shared_ptr<document> &doc, size_t len, size_t cursor) {
	doc_ = doc;
	len_ = len;
	cursor_ = cursor;
}

void InsertCmd::UnExecute() {
	doc_->RemoveLast(len_, cursor_);	
}

DeleteCmd::DeleteCmd(std::string str, size_t index, std::shared_ptr<document> &doc) {
	str_ = str;
	index_ = index;
	doc_ = doc;
}

void DeleteCmd::UnExecute() {
	doc_->InsertIndex(str_, index_);
}