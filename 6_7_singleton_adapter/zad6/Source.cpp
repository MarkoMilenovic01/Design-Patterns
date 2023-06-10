#include<iostream>

/*
6.	Kreirati klasu ShapeDrawerV1 i dve metode drawRectangle i drawCircle.
Pretpostavimo da hoćemo da izmenimo naš softver novim klasama koje bi se drugačije koristile, 
ali da zbog postojećih korisnika želimo da zadržimo i staru implementaciju. 
Kreirati nezavisnu apstraktnu klasu ShapeDrawerV2 koja ima jednu čistu virtuelnu 
funkciju draw(ShapeType). Pritom, stara implementacija korektno radi i nećemo da pravimo novu 
implementaciju "od nule", već da korisitmo kod koji smo već implementirali. 
Problem ćemo rešiti kreirajući adapter kojim se omogućuje korišćenje
stare klase ShapeDrawerV1, ali komuniciranjem preko novog interfejsa iz klase ShapeDrawerV2.
*/

class ShapeDrawerV1 {
public:
	void drawRectangle() {
		std::cout << "Drawing rectangle..." << std::endl;
	}
	void drawCircle() {
		std::cout << "Drawing circle..." << std::endl;
	}
};

enum class ShapeType {RECTANGLE, CIRCLE};

class ShapeDrawerV2 {
public:
	virtual void draw(ShapeType) = 0;
	virtual ~ShapeDrawerV2() {}
};

class ShapeDrawerAdapter : public ShapeDrawerV2, private ShapeDrawerV1 {
public:
	void draw(ShapeType type) override {
		std::cout << "ShapeDrawerAdapter: " << std::endl;
		if (type == ShapeType::CIRCLE)
			drawCircle();
		else
			drawRectangle();
	}
};

int main() {
	ShapeDrawerV2* drawer = new ShapeDrawerAdapter();
	drawer->draw(ShapeType::CIRCLE);
	std::cout << std::endl;
	drawer->draw(ShapeType::RECTANGLE);
	return 0;
}