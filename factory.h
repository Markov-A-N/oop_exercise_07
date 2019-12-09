#ifndef FACTORY_H
#define FACTORY_H 1

class Factory {
public:
	using Vertex = std::pair<double, double>;
	virtual std::shared_ptr<Figure> FigureCreate() const = 0;
	virtual std::shared_ptr<Figure> FigureCreate(Vertex *vertices, int id) 
					const = 0;
};

class TriangleFactory : public Factory {
public:
	std::shared_ptr<Figure> FigureCreate() const override {
		std::shared_ptr<Figure> res = std::make_shared<Triangle>();
		return res;
		//return std::make_shared<Figure>(Triangle{});
		//return std::shared_ptr<Figure>(new Triangle{});
	}

	std::shared_ptr<Figure> FigureCreate(Vertex *vertices, int id) const
															override {
		std::shared_ptr<Figure> res = std::make_shared<Triangle>(vertices[0], vertices[1], vertices[2], id);
		return res;
		//return std::make_shared<Figure>(Triangle{vertices[0], vertices[1],
		//		vertices[2], id});
		//return std::shared_ptr<Figure>(new Triangle{vertices[0], vertices[1],
		//		vertices[2], id});
	}
};

class SquareFactory : public Factory {
public:
	std::shared_ptr<Figure> FigureCreate() const override {
		std::shared_ptr<Figure> res = std::make_shared<Square>();
		return res;
		//return std::shared_ptr<Figure>(new Square{});
	}

	std::shared_ptr<Figure> FigureCreate(Vertex *vertices, int id) const
															override {
		std::shared_ptr<Figure> res = std::make_shared<Square>(vertices[0], vertices[1], vertices[2], vertices[3], id);
		return res;
		//return std::shared_ptr<Figure>(new Square{vertices[0], vertices[1],
		//		vertices[2], vertices[3], id});
	}
};

class RectangleFactory : public Factory {
public:
	std::shared_ptr<Figure> FigureCreate() const override {
		std::shared_ptr<Figure> res = std::make_shared<Rectangle>();
		return res;
		//return std::shared_ptr<Figure>(new Rectangle{});
	}

	std::shared_ptr<Figure> FigureCreate(Vertex *vertices, int id) const
															override {
		std::shared_ptr<Figure> res = std::make_shared<Rectangle>(vertices[0], vertices[1], vertices[2], vertices[3], id);
		return res;

		//return std::shared_ptr<Figure>(new Rectangle{vertices[0], vertices[1],
		//		vertices[2], vertices[3], id});
	}
};

#endif //FAACTORY_H