#include <iostream>
#include <string>
#include <cctype>
#include "command.h"
#include "document.h"
#include "editor.h"

int main() {
	editor editor_;
	std::string str;

	while (std::cin >> str) {
		if (islower(str[0])) {
			editor_.InsertInDocument(str);
		} else if (str == "Right" || str == "R") {
			editor_.CursorRightInDocument();
		} else if (str == "Left" || str == "L") {
			editor_.CursorLeftInDocument();
		} else if (str == "Print" || str == "P") {
			editor_.PrintDocument();
		} else if (str == "Create" || str == "C") {
			std::cout << "Input name of file: ";
			std::string fileName;
			std::cin >> fileName;
			editor_.CreateDocument(fileName);
		} else if (str == "Backspace" || str == "B") {
			editor_.DeleteInDocument();
		} else if (str == "Undo" || str == "U") {
			try {
				editor_.Undo();				
			} catch (std::logic_error &e) {
				std::cout << e.what();
			}
		} else if (str == "Load") {
			std::cout << "Input name of file: ";
			std::string fileName;
			std::cin >> fileName;
			editor_.LoadDocument(fileName); 
		} else if (str == "Save" || str == "S") {
			editor_.SaveDocument();
		} else if (str == "Exit" || str == "Quit" || str == "E" || str == "Q") {
			break;
		}
	}


	return 0;
}