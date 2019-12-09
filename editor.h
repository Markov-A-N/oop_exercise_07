#ifndef EDITOR_H
#define EDITOR_H

#include "document.h"
#include "command.h"
#include <stack>

class Editor {
public:
	Editor() : Doc(nullptr), History() {};

	void CreateDocument(const std::string &name) {
		Doc = std::make_shared<Document>(name);
	}

	void InsertPrimitive(FigureType type, std::pair<double, double> *vertices) {
		std::shared_ptr<Command> command = std::shared_ptr<Command>(new InsertCommand(type, vertices));
		command->SetDocument(Doc);
		command->Execute();
		History.push(command);
	}

	void RemovePrimitive(int id) {
		try {
			std::shared_ptr<Command> command = std::shared_ptr<Command>(new RemoveCommand(id));
			command->SetDocument(Doc);
			command->Execute();
			History.push(command);
		} catch (std::exception &err) {
			std::cout << err.what() << "\n";
			throw;
		}
	}

	void SaveDocument(const std::string &filename) {
		Doc->Save(filename);
	}

	void LoadDocument(const std::string &filename) {
		Doc = std::make_shared<Document>(filename);
		Doc->Load(filename);
	}

	void Undo() {
		if (History.empty()) {
			throw std::logic_error("empty");
		}
		std::shared_ptr<Command> lastCommand = History.top();
		lastCommand->UnExecute();
		History.pop();
	}

	void PrintDocument() {
		Doc->Print();
	}

	bool DocumentExist() {
		return Doc != nullptr;
	}

	~Editor() = default;

private:
	std::shared_ptr<Document> Doc;
	std::stack<std::shared_ptr<Command>> History;
};

#endif //EDITOR_H