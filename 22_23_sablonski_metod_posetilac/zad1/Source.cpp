#include<iostream>

class A {
public:
	void sablonskiMetod() {
		korak1();
		int rezultat = korak2();
		if (rezultat % 2 == 0)
			korak3a();
		else
			korak3b();
	}
	virtual void korak1() {
		std::cout << "Podrazumevano ponasanje za korak1 iz klase A\n";
	};
	virtual int korak2() = 0;
	virtual void korak3a() = 0;
	virtual void korak3b() = 0;
	virtual ~A() {}
};

class B : public A {
public:
	int korak2() override {
		return rand() % 2;
	}
	void korak3a() override { std::cout << "Korak3a iz klase B\n"; }
	void korak3b() override { std::cout << "Korak3b iz klase B\n"; }
};

class C : public A {
public:
	int korak2() override {
		return 1;
	}
	void korak1() override { std::cout << "Preklopljeno ponasanje koraka 1 iz klase C\n"; }
	void korak3a() override { std::cout << "Korak3a iz klase C\n"; }
	void korak3b() override { std::cout << "Korak3b iz klase C\n"; }
};

int main() {
	A* b = new B();
	A* c = new C();

	b->sablonskiMetod();
	std::cout << "\n\n";
	c->sablonskiMetod();

	delete b;
	delete c;
	return 0;
}