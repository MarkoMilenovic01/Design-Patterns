#include<iostream>

class Subject {
public:
	virtual void doAction() = 0;
	virtual ~Subject() {}
};

class RealSubject : public Subject {
public:
	void doAction() override {
		std::cout << "RealSubject::doAction()" << std::endl;
	}
};

class Proxy : public Subject {
private:
	RealSubject* realSubject;
public:
	Proxy() { realSubject = new RealSubject(); }
	~Proxy() { delete realSubject; }

	void doAction() override {
		std::cout << "Proxy::doAction()" << std::endl;
		realSubject->doAction();
	}
};

int main() {
	Subject* subject = new Proxy();
	subject->doAction();
	return 0;
}