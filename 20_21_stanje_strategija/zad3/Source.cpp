#include<iostream>

class IStrategija {
public:
	virtual void metoda() = 0;
	~IStrategija() {}
};

class StrategijaA : public IStrategija {
public:
	void metoda() override { std::cout << "Metoda strategije A\n"; }
};

class StrategijaB : public IStrategija {
public:
	void metoda() override { std::cout << "Metoda strategije B\n"; }
};

class Kontekst {
private:
	IStrategija* strategija;
public:
	Kontekst(IStrategija* strategija = nullptr):strategija(strategija) {}
	void postaviStrategiju(IStrategija* strategija) { 
		this->strategija = strategija; 
	}
	void metoda() { this->strategija->metoda(); }
	~Kontekst() { }
};

int main() {
	Kontekst k;
	IStrategija* s1 = new StrategijaA();
	IStrategija* s2 = new StrategijaB();

	k.postaviStrategiju(s1);
	k.metoda();
	k.postaviStrategiju(s2);
	k.metoda();

	delete s1;
	delete s2;

	return 0;
}