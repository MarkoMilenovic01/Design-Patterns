#include<iostream>

/*
5. Implementirati opšti oblik projektnog uzorka Adapter na osnovu UML dijagrama sa slike 2.
Dodati i klasu Adapter2 koja je implementirana kao klasni adapter
*/

class Target {
public:
	virtual void request() = 0; // Ciljni interfejs je takav da treba pozivati metodu request()
	virtual ~Target() {}
};

class Adaptee {
public:
	void specificRequest() { // Ali klasa koju imamo (a koju iz nekog razloga ne želimo ili ne možemo da 
		// modifikujemo ima metodu specificRequest()
		std::cout << "Executing specificRequest()" << std::endl;
	}
};

class ObjectAdapter : public Target {
	// i objektni i klasni adapter treba da nasleđuju Target (da bi implementirali željeni interfejs).
	// Kod objektnog adaptera imaćemo pokazivač, referencu ili ćemo agregirati objekat koji treba adaptirati,
	// a pozivom na request() samo ćemo "podmetnuti" objekat koji treba adaptirati tako da on obavi željenu
	// operaciju
private:
	Adaptee adaptee;
public:
	ObjectAdapter(const Adaptee& adaptee): adaptee(adaptee) {}
	void request() override {
		std::cout << "Using ObjectAdapter" << std::endl;
		adaptee.specificRequest();
	}
};

class ClassAdapter : public Target, private Adaptee {
public:
	// Kod klasnog adaptera potrebno je naslediti i klasu Adaptee, čime ostvarujemo da je svaki objekat
	// tipa ClassAdapter ujedno i Adaptee, pa kao takav ima i metodu specificRequest(). 
	// Ukoliko hoćemo da od korisnika sakrijemo interfejs iz Adaptee, potrebno je izvršiti privatno nasleđivanje.
	// Ukoliko hoćemo da naš adapter podržava i željeni i "sporni" interfejs, onda možemo izvršiti javno nasleđivanje
	void request() override {
		std::cout << "Using ClassAdapter" << std::endl;
		specificRequest(); // poziv metode iz osnovne klase Adaptee
	}
};

int main() {
	Target* a = new ObjectAdapter(Adaptee());
	Target* b = new ClassAdapter();
	a->request();
	b->request();
	return 0;
}