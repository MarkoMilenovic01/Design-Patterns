#include<iostream>

class BaseSingleton {
public:
	static BaseSingleton& getInstance() {
		static BaseSingleton instance; // ovim kreiramo statičku promenljivu, ali će se kreirati samo jednom,
		// bez obzira na to koliko puta zovemo getInstance()
		// Od standarda 11 C++ ovo je thread-safe. Već se koristi standard 20, tako da je teško naći kompajler koji
		// danas ne podržava bar verziju 11.
		return instance;
	}
	void use() {
		std::cout << "Using Singleton object" << std::endl;
	}
private:
	BaseSingleton() { std::cout << "Creating Singleton object" << std::endl; };
	BaseSingleton(const BaseSingleton&) = default;
	BaseSingleton& operator=(const BaseSingleton&) = default;
};

void reference_podsecanje() {
	int promenljiva = 5;
	int& r = promenljiva;
	std::cout << r << std::endl;
	std::cout << promenljiva << std::endl;

	r = 3; // ovom linijom menjamo i promenljivu promenljiva. Referenca je alijas, drugo ime, za istu promenljivu.
	std::cout << r << std::endl;
	std::cout << promenljiva << std::endl;
}


int main() {
	BaseSingleton& singleton = BaseSingleton::getInstance();
	singleton.use();
	BaseSingleton::getInstance().use(); // često Singleton koristimo na ovaj način, bez izvlačenja lokalne promenljive

	return 0;
}