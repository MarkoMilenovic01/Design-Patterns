#include<iostream>

class IComponent {
public:
	virtual void operation() = 0;
	virtual ~IComponent() {}
};

class ConcreteComponent : public IComponent {
public:
	void operation() override {
		std::cout << "Doing operation from ConcreteComponent" << std::endl;
	}
};

class Decorator : public IComponent {
protected:
	IComponent* decoratedObject;
public:
	Decorator(IComponent* decoratedObject): decoratedObject(decoratedObject) {}
};

class ConcreteDecorator1 : public Decorator {
public:
	ConcreteDecorator1(IComponent* decoratedObject): Decorator(decoratedObject) {};
	void operation() override {
		std::cout << "Calling operation from ConcreteDecorator1" << std::endl;
		this->decoratedObject->operation();
	}
};

class ConcreteDecorator2 : public Decorator {
public:
	ConcreteDecorator2(IComponent* decoratedObject) : Decorator(decoratedObject) {};
	void operation() override {
		std::cout << "Calling operation from ConcreteDecorator2" << std::endl;
		this->decoratedObject->operation();
	}
};

int main() {
	IComponent* component1 = new ConcreteDecorator2(new ConcreteComponent());
	IComponent* component2 = new ConcreteDecorator1(new ConcreteDecorator2(new ConcreteComponent()));
	component1->operation();
	std::cout << std::endl << std::endl;
	component2->operation();
	return 0;
}