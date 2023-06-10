#include<iostream>
#include<limits.h>
#include<vector>

/*
Ovde ćemo imati jedno drugačije korišćenje posmatrača. 
Za neke strukure ćemo "prikačiti" funkcije kojim ćemo inkrementalno ažurirati 
vrednosti minimuma, maksimuma, itd. 
Pretpostavka zadatka je da ćemo samo dodavati elemente pa nam za ažuriranje
vrednosti ne treba cela struktura podataka, već samo novi element koji je dodat.
*/

class Funkcija {
public:
	virtual void azuriraj(int) = 0;
	virtual int f() const = 0; // vraća vrednost funkcije
	virtual std::string opis() const = 0; // služiće nam samo za ispis kasnije
};

class Suma : public Funkcija {
	int s = 0;
public:
	void azuriraj(int nova_vrednost) override {
		s += nova_vrednost;
	}
	std::string opis() const override { return "Suma"; }
	int f() const { return s; }
};

class Velicina : public Funkcija {
	int br = 0;
public:
	void azuriraj(int) override {
		br++;
	}
	std::string opis() const override { return "Velicina"; }
	int f() const { return br; }
};

class Min : public Funkcija {
	int min = INT_MAX;
public:
	void azuriraj(int nova_vrednost) override {
		min = nova_vrednost < min ? nova_vrednost : min;
	}
	std::string opis() const override { return "Min"; }
	int f() const { return min; }
};

class Max : public Funkcija {
	int max = INT_MIN;
public:
	void azuriraj(int nova_vrednost) override {
		max = nova_vrednost > max ? nova_vrednost : max;
	}
	std::string opis() const override { return "Max"; }
	int f() const { return max; }
};

class Kolekcija {
protected:
	//azurirajFunkcije ~ notify
	void azurirajFunkcije(int nova_vrednost) { for (auto f : funkcije) f->azuriraj(nova_vrednost); }
	std::vector<Funkcija*> funkcije;
public:
	virtual void dodajElement(int element) = 0;
	std::vector<Funkcija*> dajFunkcije() const { return funkcije; }
	void dodajFunkciju(Funkcija* f) { funkcije.push_back(f); }
};

#define MAX_VELICINA 100

class Stek : public Kolekcija {
	int vrh = 0;
	int niz[MAX_VELICINA];
public:
	void dodajElement(int element) override {
		niz[vrh++] = element;
		azurirajFunkcije(element);
	}
};

struct Cvor {
	Cvor* sledeci;
	int vrednost;
	Cvor(int vrednost, Cvor* sledeci=nullptr): vrednost(vrednost), sledeci(sledeci) {}
};

class JednostrukoPovezanaLista : public Kolekcija {
	Cvor* glava = nullptr;
public:
	void dodajElement(int element) override {
		if (!glava) { glava = new Cvor(element); }
		else {
			Cvor* tmp = glava;
			glava = new Cvor(element, tmp);
		}
		azurirajFunkcije(element);
	}
};

int main() {
	Kolekcija* k = new Stek();
	k->dodajFunkciju(new Velicina());
	k->dodajFunkciju(new Suma());
	k->dodajFunkciju(new Min());
	k->dodajFunkciju(new Max());

	k->dodajElement(1);
	k->dodajElement(11);
	k->dodajElement(-1);
	k->dodajElement(100);
	k->dodajElement(2);
	k->dodajElement(3);

	// Kad god je tip podataka dostupan u vreme kompajliranja, možemo koristiti ključnu reč 
	// auto umesto tipa (naročito praktično ako nam je naporno pišemo pun naziv tipa
	// poput std::vector<Funkcija*>
	auto funkcije = k->dajFunkcije();
	for (auto f : funkcije) {
		std::cout << "Vrednost funkcije " << f->opis() << " je " << f->f() << std::endl;
	}
}