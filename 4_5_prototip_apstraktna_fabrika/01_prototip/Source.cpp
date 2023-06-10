#include<iostream>

/*
Implementirati opšti oblik projektnog uzorka Prototip na osnovu UML dijagrama sa slike 1.
*/
class Prototype {
protected:
	int a;// pretpostavimo da je svojstvo a neko svojstvo za koje je potrebno izvrsiti
	// kompleksnu racunicu, ali da će svaka instanca klase doći do istog rezultata.
	// ideja je da tu činjenicu iskoristimo tako što ćemo samo jednom proračunavati svojstvo a!
	std::string b;
public:
	Prototype() {}
	Prototype(int a, std::string b): a(a), b(b) {
		std::cout << "Kreiram prototip i vrsim kompleksnu racunicu" << std::endl;
	}
	void setB(std::string b) {
		this->b = b;
	}
	virtual Prototype* clone() = 0;
	virtual void describe() = 0;
};

class ConcretePrototype1 : public Prototype {
public:
	ConcretePrototype1() : Prototype() {}
	ConcretePrototype1(int a, std::string b): Prototype(a, b) {
		std::cout << "Kreiram konkretni prototip 1 i vrsim kompleksnu racunicu" << std::endl;
	}
	virtual void describe() {
		std::cout << "Konkretni prototip tipa 1: " << this->b << std::endl;
	}
	virtual Prototype* clone() {
		ConcretePrototype1* clone = new ConcretePrototype1();
		clone->a = this->a;
		clone->b = this->b;
		return clone;
	}
};

class ConcretePrototype2 : public Prototype {
public:
	ConcretePrototype2() : Prototype() {}
	ConcretePrototype2(int a, std::string b) : Prototype(a, b) {
		std::cout << "Kreiram konkretni prototip 2 i vrsim kompleksnu racunicu" << std::endl;
	}
	virtual void describe() {
		std::cout << "Konkretni prototip tipa 2: " << this->b << std::endl;
	}
	virtual Prototype* clone() {
		ConcretePrototype2* clone = new ConcretePrototype2();
		clone->a = this->a;
		clone->b = this->b;
		return clone;
	}
};

int main() {
	Prototype* a, * b, * c, * d;
	a = new ConcretePrototype1(1, "a");
	b = new ConcretePrototype2(2, "b");
	// Kreiraćemo konkretne prototipove samo jednom pozivajući potencijalno skupi konstruktor,
	// a zatim ćemo ostale objekte dobijati kloniranjem
	c = a->clone();
	c->setB("c");
	d = b->clone();
	d->setB("d");

	a->describe();
	b->describe();
	c->describe();
	d->describe();

	return 0;
}