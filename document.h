#ifndef DOCUMENT_H
#define DOCUMENT_H 1

#include <fstream>
#include <list>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <utility>
#include "figures.h"

class Document {
public:
	Document() : Id(1), Name(""), Buffer(0), triangleFactory(),
		squareFactory(), rectangleFactory() {};

	Document(std::string name) : Id(1), Name(std::move(name)), Buffer(0),
		triangleFactory(), squareFactory(), rectangleFactory() {};

	~Document() = default;

	void Rename(const std::string &newName) {
		Name = newName;
	} 

	void Save(const std::string &filename) {
		SerializeImpl(filename);
	}

	void Load(const std::string &filename) {
		DeserializeImpl(filename);
	}

	void Print() {
		std::for_each(Buffer.begin(), Buffer.end(), [](std::shared_ptr<Figure>
			shape) {
			shape->Print(std::cout) << "\n";
		});
	}

	void RemovePrimitive(int id) {
		auto it = std::find_if(Buffer.begin(), Buffer.end(),
			[id](std::shared_ptr<Figure> shape) -> bool {
				return id == shape->getId();
			});

		if (it == Buffer.end()) {
			throw std::logic_error("Figure with this id doesn't exist");
		}

		Buffer.erase(it);
	}

	void InsertPrimitive(FigureType type, std::pair<double, double> *
															vertices) {
		switch (type) {
			case TRIANGLE:
				Buffer.push_back(triangleFactory.FigureCreate(vertices, 
																Id++));
				break;
			case SQUARE:
				Buffer.push_back(squareFactory.FigureCreate(vertices, 
																Id++));
				break;
			case RECTANGLE:
				Buffer.push_back(rectangleFactory.FigureCreate(vertices, 
																Id++));
				break;
		}
	}

private:
	int Id;
	std::string Name;
	std::list<std::shared_ptr<Figure>> Buffer;
	TriangleFactory triangleFactory;
	SquareFactory squareFactory;
	RectangleFactory rectangleFactory;

	friend class InsertCommand;
	friend class RemoveCommand;

	void SerializeImpl(const std::string &filename) const {
		std::ofstream os(filename, std::ios::binary | std::ios::out);
		if (!os) {
			throw std::runtime_error("File is not opened");
		}
		size_t nameLen = Name.size();
		os.write((char *) &nameLen, sizeof(nameLen));
		os.write((char *) Name.c_str(), nameLen);
		for (const auto &shape : Buffer) {
			shape->Serialize(os);
		}
	}

	void DeserializeImpl(const std::string &filename) {
		std::ifstream is(filename, std::ios::binary | std::ios::in);
		if (!is) {
			throw std::runtime_error("File is not opened");
		}
		size_t nameLen;
		is.read((char *) &nameLen, sizeof(nameLen));
		char *name = new char[nameLen + 1];
		name[nameLen] = 0;
		is.read(name, nameLen);
		Name = std::string(name);
		delete [] name;
		FigureType type;
		while (true) {
			is.read((char *) &type, sizeof(type));
			if (is.eof()) {
				break;
			}
			switch (type) {
				case TRIANGLE:
					Buffer.push_back(triangleFactory.FigureCreate());
					break;
				case SQUARE:
					Buffer.push_back(squareFactory.FigureCreate());
					break;
				case RECTANGLE:
					Buffer.push_back(rectangleFactory.FigureCreate());
					break;
			}
			Buffer.back()->Deserialize(is);
		}
		Id = Buffer.size();
	}

	std::shared_ptr<Figure> GetFigure(int id) {
		if (id > Id || id == 0) {
			throw std::runtime_error("Invalid id");
		}
		auto it = std::find_if(Buffer.begin(), Buffer.end(),
			[id](std::shared_ptr<Figure> shape) -> bool {
			return id == shape->getId();
		});
		return *it;
	}

	int GetPos(int id) {
		auto it = std::find_if(Buffer.begin(), Buffer.end(),
			[id](std::shared_ptr<Figure> shape) -> bool {
				return id == shape->getId();
			});
		return std::distance(Buffer.begin(), it);
	}

	void InsertPrimitive(int pos, std::shared_ptr<Figure> figure) {
		auto it = Buffer.begin();
		std::advance(it, pos);
		Buffer.insert(it, figure);
	}

	void RemoveLastPrimitive() {
		if (Buffer.empty()) {
			throw std::logic_error("Document is empty");
		}
		Buffer.pop_back();
	}
};

#endif //DOCUMENT_H