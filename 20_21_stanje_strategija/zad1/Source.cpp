#include<iostream>

class Osoba;

class Raspolozenje { // stanje
public:
	virtual void radi(Osoba*) = 0;
	virtual void zabavljajSe(Osoba*) = 0;
	virtual ~Raspolozenje() {}
};

class Osoba {
	std::string ime;
	std::string prezime;
	Raspolozenje* raspolozenje;
public:
	Osoba(const std::string& ime, const std::string& prezime);
	std::string dajIme() const { return ime; }
	std::string dajPrezime() const { return prezime; }
	void postaviRaspolozenje(Raspolozenje* raspolozenje) {
		this->raspolozenje = raspolozenje;
	}
	void radi() { raspolozenje->radi(this); }
	void zabavljajSe() { raspolozenje->zabavljajSe(this); }
	~Osoba() { delete raspolozenje; }
};

class DobroRaspolozenje : public Raspolozenje {
	void radi(Osoba* o) override; // U implementacijama koristićemo klasu LoseRaspolozenje,
	// koja je kasnije definisana, pa je i implementacija ovih metoda premeštena
	void zabavljajSe(Osoba* o) override;
};

class LoseRaspolozenje : public Raspolozenje {
	void radi(Osoba* o) override {
		std::cout << "Osoba " << o->dajIme() << " "
			<< o->dajPrezime() << " radi i lose je raspolozena!\n";
		if (rand() % 4 == 0) {
			o->postaviRaspolozenje(new DobroRaspolozenje());
			delete this; // Sasvim je u redu da objekat obriše sam sebe
		}
	}
	void zabavljajSe(Osoba* o) override {
		std::cout << "Osoba " << o->dajIme() << " "
			<< o->dajPrezime() << " se zabavlja i lose je raspolozena!\n";
		if (rand() % 10 < 7) {
			o->postaviRaspolozenje(new DobroRaspolozenje());
			delete this;
		}
	}
};

void DobroRaspolozenje::radi(Osoba* o) {
	std::cout << "Osoba " << o->dajIme() << " "
		<< o->dajPrezime() << " radi i dobro je raspolozena!\n";
	if (rand() % 2 == 0) {
		o->postaviRaspolozenje(new LoseRaspolozenje());
		delete this;
	}
}
void DobroRaspolozenje::zabavljajSe(Osoba* o) {
	std::cout << "Osoba " << o->dajIme() << " "
		<< o->dajPrezime() << " se zabavlja i dobro je raspolozena!\n";
	if (rand() % 10 == 0) {
		o->postaviRaspolozenje(new LoseRaspolozenje());
		delete this;
	}
}

Osoba::Osoba(const std::string& ime, const std::string& prezime) :
	ime(ime), prezime(prezime),
	raspolozenje(new DobroRaspolozenje()) {}


int main() {
	srand(time(0));
	Osoba* o = new Osoba("Pera", "Peric");
	for (int i = 0; i < 20; i++) {
		o->radi();
		o->zabavljajSe();
	}
	delete o;
	return 0;
}