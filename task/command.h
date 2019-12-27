#ifndef COMMAND_H
#define COMMAND_H 1

#include "document.h"

struct command {
	virtual void UnExecute() = 0;
	virtual ~command() = default;

protected:
	std::shared_ptr<document> doc_;
};

struct InsertCmd : command {
	InsertCmd(std::shared_ptr<document> &doc, size_t len, size_t cursor);

	void UnExecute() override;

private:
	size_t len_;
	size_t cursor_;
};

struct DeleteCmd : command {
	DeleteCmd(std::string str, size_t index, std::shared_ptr<document> &doc);

	void UnExecute() override;

private:
	std::string str_;
	size_t index_;
};

#endif //COMMAND_H