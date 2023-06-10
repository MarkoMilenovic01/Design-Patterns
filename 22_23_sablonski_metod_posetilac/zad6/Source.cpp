#include<iostream>
#include<vector>

class Operacija;

/* Napomena u vezi primera:
Ispravnija i lakša implementacija binarnog stabla je da prosto imamo klasu čvor koja 
ima pokazivače na levog i desnog sledbenika, kao i polje za podatak koji u datom čvoru čuvamo.
Klasa BinarnoStablo bi onda imala samo pokazivač na jedan čvor koji bi bio koren stabla.
Ovde ćemo implementirati čvor tako da nema sledbenike, a binarno stablo tako da ima bar jedan
podatak (implicitni čvor) i pokazivače na dva sledbenika koji mogu biti nova stabla ili čvorovi.
Implementacija nije baš najbolja upravo zbog tog ograničenja da kreiranjem stabla se obavezujemo
da nešto smestimo u njega, ali ovde je naglasak na obilasku stabla i korišćenju posetioca.
*/

class IElement {
protected:
	int podatak;
public:
	IElement(int podatak) : podatak(podatak) {}
	virtual int vrednost() const { return podatak; };
	virtual int izvrsiOperaciju(Operacija*) = 0;
	virtual ~IElement() {}
};

class Cvor : public IElement {
public:
	Cvor(int podatak) : IElement(podatak) {}
	int vrednost() const override { return podatak; }
	virtual int izvrsiOperaciju(Operacija*) override;
};

class BinarnoStablo : public IElement {
	IElement* levi;
	IElement* desni;
public:
	using IElement::IElement;
	void postaviLevi(IElement* levi) { this->levi = levi; }
	void postaviDesni(IElement* desni) { this->desni = desni; }
	IElement* dajLevi() const { return levi; }
	IElement* dajDesni() const { return desni; }

	virtual int izvrsiOperaciju(Operacija*) override; // ovde će operacije vraćati vrednost.
	// Ograničenje koje imamo je da sve operacije (bar one koje ćemo zvati iz metode izvrsiOperaciju
	// moraju da vraćaju isti tip vrednosti!
};

class Operacija {
public:
	virtual int izvrsi(Cvor*) = 0;
	virtual int izvrsi(BinarnoStablo*) = 0;
	virtual std::string opis() const = 0; // neobavezna metoda. Treba nam kasnije za ispis.
	virtual ~Operacija() {}
};

class Suma : public Operacija {
public:
	int izvrsi(Cvor* c) { return c->vrednost(); }
	int izvrsi(BinarnoStablo* stablo) {
		// Ovde ćemo videti praktičnost double dispatch pristupa. Naime, ono što nam je potrebno
		// da bismo izračunali sumu elemenata u binarnom stablu je da saberemo vrednost korena (u ovoj
		// lošoj implementaciji svako binarno stablo ima bar koren i njegovoj vrednosti pristupamo preko
		// stablo->vrednost()) i sume unutar levog podstabla i desnog podstabla. 
		// Kako da izračunamo sumu levog podstabla na primer? 
		// Sledeća linija koda se ne može kompajlirati:
		// izvrsi(stablo->dajLevi());
		// Zašto? Zato što je tip podataka koji metoda dajLevi() vraća IElement* i mi zahtevamo
		// time da se pozove metoda izvrsi nad tipom IElement*, a takva metoda nije definisana. 
		// Kako da saznamo koji se tačno izvedeni tip krije iza IElement*? Jedna opcija je da pomoću
		// dynamic_cast-a isprobavamo razne mogućnosti, što je malo nepraktično. 
		// Međutim, bitno zapažanje je da to nije neophodno. Možemo prepustiti kontrolu toka ponovo metodi 
		// izvrsiOperaciju koji god da je tip u pitanju. Kada se ta metoda bude pozvala nad 
		// objektom koji dobijemo kao rezultat poziva dajLevi(), izvršiće se implementacija koja 
		// već "zna" koji je tačno tip u pitanju i pozvaće se zatim odgovarajuća metoda izvsi iz ove
		// klase. 
		// Savet: ukoliko imate problema sa razumevanjem opisane komunikacije, postavite breakpoint-e
		// i ispratite kontrolu toka kod ovih poziva!
		
		return stablo->vrednost() + stablo->dajLevi()->izvrsiOperaciju(this) + \
			stablo->dajDesni()->izvrsiOperaciju(this);
	}
	std::string opis() const override { return "Suma"; }
};

class Velicina : public Operacija {
public:
	int izvrsi(Cvor* c) { return 1; }
	int izvrsi(BinarnoStablo* stablo) {
		return 1 + stablo->dajLevi()->izvrsiOperaciju(this) + \
			stablo->dajDesni()->izvrsiOperaciju(this);
	}
	std::string opis() const override { return "Velicina"; }
};

class Minimum : public Operacija {
public:
	int izvrsi(Cvor* c) { return c->vrednost(); }
	int izvrsi(BinarnoStablo* stablo) {
		int a = stablo->vrednost();
		int b = stablo->dajLevi()->izvrsiOperaciju(this);
		int c = stablo->dajDesni()->izvrsiOperaciju(this);
		a = std::min(a, b);
		a = std::min(a, c);
		return a;
	}
	std::string opis() const override { return "Minimum"; }
};

class Maksimum : public Operacija {
public:
	int izvrsi(Cvor* c) { return c->vrednost(); }
	int izvrsi(BinarnoStablo* stablo) {
		int a = stablo->vrednost();
		int b = stablo->dajLevi()->izvrsiOperaciju(this);
		int c = stablo->dajDesni()->izvrsiOperaciju(this);
		a = std::max(a, b);
		a = std::max(a, c);
		return a;
	}
	std::string opis() const override { return "Maksimum"; }
};

int Cvor::izvrsiOperaciju(Operacija* o) {
	return o->izvrsi(this);
};
int BinarnoStablo::izvrsiOperaciju(Operacija* o) {
	return o->izvrsi(this);
}


int main() {
	BinarnoStablo s(10);
	BinarnoStablo s1(5);
	s1.postaviLevi(new Cvor(1));
	s1.postaviDesni(new Cvor(2));
	s.postaviLevi(&s1);
	s.postaviDesni(new Cvor(14));

	std::vector<Operacija*> operacije;
	operacije.push_back(new Velicina());
	operacije.push_back(new Suma());
	operacije.push_back(new Minimum());
	operacije.push_back(new Maksimum());
	for (auto o : operacije) {
		std::cout << o->opis() << ": " << s.izvrsiOperaciju(o) << std::endl;
		delete o;
	}
}