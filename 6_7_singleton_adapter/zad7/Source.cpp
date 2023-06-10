#include<iostream>
#include<vector>
/*
7.	Adapter se (u izmenjenom obliku) može koristiti i da omogući korišćenje potpuno nezavisnih 
klasa zajedničkim interfejsom.
Kreirati apstraktnu klasu Target sa čisto virtuelnom metodom print(std::string) i tri 
nezavisne klase A, B i C sa metodama printA(std::string), printB(std::string), 
printC(std::string) (svaka metoda u svojoj klasi). Kreirati šablonsku klasu PrintAdapter 
koja kao parameter šablona ima klasu koju treba da adaptira. 
PrintAdapter implementirati kao objektni adapter, a prilikom kreiranja proslediti kao argument 
pokazivač na metodu koju treba da izvrši pozivom na print. 
Testirati različite tipove adaptera u main funkciji.
*/

class Target {
public:
	virtual void print(const std::string&) = 0;
	virtual ~Target(){}
};

class A {
public:
	void printA(const std::string& message) {
		std::cout << "ClassA: " << message << std::endl;
	}
};


class B {
public:
	void printB(const std::string& message) {
		std::cout << "ClassB: " << message << std::endl;
	}
};


class C {
public:
	void printC(const std::string& message) {
		std::cout << "ClassC: " << message << std::endl;
	}
};

template<class Adaptee>
class PrintAdapter : public Target {
private:
	void (Adaptee::* method)(const std::string&);
	Adaptee* adaptee;
public:
	PrintAdapter(Adaptee* adaptee, void (Adaptee::* method)(const std::string&)):
		method(method), adaptee(adaptee) {}
	void print(const std::string& message) override {
		(adaptee->*method)(message);
	}
	~PrintAdapter() {
		delete adaptee;
	}
};

int main() {
	std::vector<Target*> printers; 
	printers.push_back(new PrintAdapter<A>(new A(), &A::printA));
	printers.push_back(new PrintAdapter<B>(new B(), &B::printB));
	printers.push_back(new PrintAdapter<C>(new C(), &C::printC));

	for (int i = 0; i < printers.size(); ++i) {
		printers[i]->print("message...");
		std::cout << std::endl;
	}
	return 0;
}