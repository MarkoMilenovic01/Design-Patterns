#include<iostream>

class BaseSingleton {
public:
	static BaseSingleton& getInstance() {
		static BaseSingleton instance;
		return instance;
	}
	virtual void use() {
		std::cout << "Using Singleton object" << std::endl;
	}
protected:
	// hoćemo da dozvolimo poziv ovih konstruktora/operatora iz izvedenih klasa. 
	BaseSingleton() { std::cout << "Creating BaseSingleton object" << std::endl; };
	BaseSingleton(const BaseSingleton&) = default;
	BaseSingleton& operator=(const BaseSingleton&) = default;
};

class DerivedSingleton1 : public BaseSingleton {
public:
	static BaseSingleton& getInstance() {
		static DerivedSingleton1 instance;
		return instance;
	}
	void use() override {
		std::cout << "Using DerivedSingleton1 object" << std::endl;
	}
protected:
	DerivedSingleton1() { std::cout << "Creating DerivedSingleton1 object" << std::endl; };
	DerivedSingleton1(const DerivedSingleton1&) = default;
	DerivedSingleton1& operator=(const DerivedSingleton1&) = default;
};

class DerivedSingleton2 : public BaseSingleton {
public:
	static BaseSingleton& getInstance() {
		static DerivedSingleton2 instance;
		return instance;
	}
	void use() override {
		std::cout << "Using DerivedSingleton2 object" << std::endl;
	}
protected:
	DerivedSingleton2() { std::cout << "Creating DerivedSingleton2 object" << std::endl; };
	DerivedSingleton2(const DerivedSingleton2&) = default;
	DerivedSingleton2& operator=(const DerivedSingleton2&) = default;
};

int main() {
	DerivedSingleton1::getInstance().use();
	// korišćenje referenci dozovljava polimorfno ponašanje, pa će se pozvati use() iz klase DerivedSingleton1, iako
	// getInstance vraća referencu na BaseSingleton
	DerivedSingleton2::getInstance().use();
	DerivedSingleton1::getInstance().use();
	DerivedSingleton2::getInstance().use();
	return 0;
}