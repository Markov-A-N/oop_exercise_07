#include <iostream>
#include <string>
#include "editor.h"

void menu() {
	std::cout << "\nMenu\n";
	std::cout << "Create\n";
	std::cout << "Load\n";
	std::cout << "Save <fileName>\n";
	std::cout << "Add <figureType>\n";
	std::cout << "Remove <figure ID>\n";
	std::cout << "Undo\n";
	std::cout << "Print\n\n";
}

void create(Editor &editor) {
	std::string cmd;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::string filename;
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error &err) {
				std::cout << err.what() << "\n";
			}
		}
		std::cout << "Enter name of new project\n";
	}
	std::cin >> cmd;
	editor.CreateDocument(cmd);
	std::cout << "Document " << cmd << " is created\n";
}

void save(Editor &editor) {
	if (!editor.DocumentExist()) {
		throw std::runtime_error("Document does not exist"); 
	}
	std::string filename;
	std::cin >> filename;

	try {
		editor.SaveDocument(filename);
		std::cout << "Document save if file " << filename << "\n";
	} catch (std::runtime_error &err) {
		std::cout << err.what() << "\n";
	}
}

void load(Editor &editor) {
	std::string cmd;
	std::string filename;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error& err) {
				std::cout << err.what() << "\n";
			}
		}
		std::cin >> filename;
		try {
			editor.LoadDocument(filename);
			std::cout << "Document loaded from file " << filename << "\n";
		} catch (std::runtime_error& err) {
			std::cout << err.what() << "\n";
		}
	}
}

void add(Editor &editor) {
	if (!editor.DocumentExist()) {
		throw std::runtime_error("Document does not exist"); 
	}
	char type;
	std::cin >> type;
	if (type == 'T') {
		//std::shared_ptr<std::pair<double, double>> vertices = std::make_shared<std::pair<double, double>>(new std::pair<double, double>[3])
		std::pair<double, double> *vertices = new std::pair<double, double>[3];
		for (int i = 0; i < 3; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		try {
			editor.InsertPrimitive(TRIANGLE, vertices);
			delete [] vertices;
			vertices = nullptr;
		} catch (std::logic_error &err) {
			std::cout << err.what() << "\n";
			delete [] vertices;
			vertices = nullptr;
			return;
		}
	}
	else if (type == 'S') {
		std::pair<double, double> *vertices = new std::pair<double, double>[4];
		for (int i = 0; i < 4; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		try {
			editor.InsertPrimitive(SQUARE, vertices);
			delete [] vertices;
			vertices = nullptr;
		} catch (std::logic_error &err) {
			std::cout << err.what() << "\n";
			delete [] vertices;
			vertices = nullptr;
			return;
		}
	}
	else if (type == 'R') {
		std::pair<double, double> *vertices = new std::pair<double, double>[4];
		for (int i = 0; i < 4; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		try {	
			editor.InsertPrimitive(RECTANGLE, vertices);
			delete [] vertices;
			vertices = nullptr;
		} catch (std::logic_error &err) {
			std::cout << err.what() << "\n";
			delete [] vertices;
			vertices = nullptr;
			return;
		}
	}
	else {
		std::cout << "Primitive isn't added\n";
		std::cin.clear();
		std::cin.ignore(30000, '\n');
		return;
	}
	std::cout << "Primitive is added\n";
}

/*void remove(Editor &editor) {
	if (!editor.DocumentExist()) {
		throw std::runtime_error("Document does not exist"); 
	}
	int id;
	
	std::cin >> id;

	try {
		editor.RemovePrimitive(id);
	} catch (std::exception &err) {;
		return;
	}
	std::cout << "Primitive with " << id << " is removed\n";
}*/

void remove(Editor &editor) {
	if (!editor.DocumentExist()) {
		std::cout << "Document does not exist\n";
	}
	int id;
	std::cin >> id;
	if (id <= 0) {
		std::cout << "Invalid id\n";
		return;
	} 
	try {
		editor.RemovePrimitive(id);
	} catch (std::exception &e) {
		return;
	}
	std::cout << "Remove primitive at id: " << id << "\n";
}

int main(int argc, char **argv) {
	Editor editor;
	std::string cmd;

	while(std::cin >> cmd) {
		if (cmd == "Menu") {
			menu();
		} 
		else if (cmd == "Create") {
			create(editor);
		}
		else if (cmd == "Save") {
			try {	
				save(editor);
			} catch (std::runtime_error &err) {
				std::cout << err.what() << "\n\n";
			}
		}
		else if (cmd == "Load") {
			try {
				load(editor);
			} catch (std::runtime_error &err) {
				std::cout << err.what() << "\n\n";
			}
		}
		else if (cmd == "Add") {
			try {
				add(editor);
			} catch (std::runtime_error &err) {
				std::cout << err.what() << "\n\n";
			}
		}
		else if (cmd == "Remove") {
			try {
				remove(editor);
			} catch (std::exception &err) {
				std::cout << err.what() << "\n";
			}
		}
		else if (cmd == "Undo") {
			try {
				editor.Undo();
				std::cout << "OK\n";
			} catch (std::logic_error &err) {
				std::cout << err.what() << "\n\n";
			}
		}
		else if (cmd == "Print") {
			if (!editor.DocumentExist()) {
				std::cout << "Document does not exist" << "\n\n";
				continue;
			}
			editor.PrintDocument();
		}
		else {
			std::cin.clear();
			std::cin.ignore(30000, '\n');
		}
		std::cout << "\n";
	}

	return 0;
}