#include<iostream>

// Podsetnik: ideja ovog pristupa kao i svih narednih je da je životni vek statičkih atributa cela aplikacija. 
class BaseSingleton {
public:
	static BaseSingleton* getInstance() { // bitno je da bude static, da bismo mogli da pozivamo funkciju nezavisno od toga postoje li objekti date klase
		if (!instance) // ideja pristupa je da samo kod prvog poziva kreiramo statički atribut instance
			instance = new BaseSingleton();
		return instance; 
	}
	void use() {
		std::cout << "Using Singleton object" << std::endl;
	}
private:
	// Ovim osiguravamo ne samo da ne mozemo da pozovemo konstruktor van klase, već da ne možemo
	// da napravimo ni kopiju jednom kreiranog objekta
	BaseSingleton() { std::cout << "Creating Singleton object" << std::endl; };
	BaseSingleton(const BaseSingleton&) = default;
	BaseSingleton& operator=(const BaseSingleton&) = default;

	static BaseSingleton* instance;
};

BaseSingleton* BaseSingleton::instance = nullptr;

int main() {
	BaseSingleton* singleton = BaseSingleton::getInstance();
	singleton->use();
	BaseSingleton::getInstance()->use();
	return 0;
}