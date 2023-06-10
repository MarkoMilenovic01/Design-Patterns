#include<iostream>

class Implementator {
public:
	virtual void implementation() = 0;
	virtual ~Implementator(){};
};

class ConcreteImplementator1 : public Implementator {
public:
	void implementation() override {
		std::cout << "Concrete implementation 1" << std::endl;
	}
};

class ConcreteImplementator2 : public Implementator {
public:
	void implementation() override {
		std::cout << "Concrete implementation 2" << std::endl;
	}
};

class Abstraction {
protected:
	Implementator* implementator;
public:
	Abstraction(Implementator* implementator) : 
		implementator(implementator) {}
	virtual void function() {
		implementator->implementation();
	}
	virtual ~Abstraction() {
		delete implementator;
	}
};

class RefinedAbstraction : public Abstraction {
public:
	RefinedAbstraction(Implementator* implementator):
		Abstraction(implementator) {}
	virtual void function() {
		std::cout << "RefinedAbstraction: " << std::endl;
		implementator->implementation();
	}
};

int main() {
	Abstraction* a1 = new Abstraction(new ConcreteImplementator2());
	Abstraction* a2 = new RefinedAbstraction(new ConcreteImplementator1());

	a1->function();
	a2->function();

	return 0;
}