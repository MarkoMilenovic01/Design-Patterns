#include<iostream>

class Ratnik {
protected:
	int napad;
	int odbrana;
public:
	Ratnik(): napad(1), odbrana(1) {}
	Ratnik(int napad, int odbrana): napad(napad), odbrana(odbrana) {}
	virtual int zadajUdarac() const = 0;
	virtual void primiUdarac(int jacinaUdarca) = 0;
	virtual ~Ratnik() {}
};

/*
Recimo da imamo dve konkretne klase, OdbrambeniRatnik i RatnikZaNapad. 

*/
class OdbrambeniRatnik : public Ratnik {
public:
	OdbrambeniRatnik(int napad, int odbrana) : Ratnik(napad, odbrana) {}
	int zadajUdarac() const override { return napad; }
	void primiUdarac(int jacinaUdarca) {// Recimo da odbrambeni ratnik ublažava udarac
		jacinaUdarca -= odbrana;
		jacinaUdarca = 0.9 * jacinaUdarca;
		jacinaUdarca = std::max(0, jacinaUdarca);
		std::cout << "Primljen udarac jacine " << jacinaUdarca << std::endl;
	}
};

class RatnikZaNapad : public Ratnik {
public:
	RatnikZaNapad(int napad, int odbrana) : Ratnik(napad, odbrana) {}
	int zadajUdarac() const override { return napad * 1.1; } //ratnik za napad zadaje jači udarac
	void primiUdarac(int jacinaUdarca) {
		jacinaUdarca -= odbrana;
		jacinaUdarca = std::max(0, jacinaUdarca);
		std::cout << "Primljen udarac jacine " << jacinaUdarca << std::endl;
	}
};

class RatnikSaOpremom : public Ratnik {
protected:
	Ratnik* dekorisaniRatnik;
public:
	RatnikSaOpremom(Ratnik* dekorisaniRatnik): dekorisaniRatnik(dekorisaniRatnik) {}
	// Možemo da implementiramo metode zadajUdarac i primiUdarac, a da ih u izvedenim dekoratorima
	// prekolopimo ukolikoo je to potrebno (motivacija je to što su u pitanju dve metode, a nema potrebe 
	// da svaka nasleđena klasa preklapa svaku metodu (obično prosleđivanje poziva metoda (tj. da dekorator 
	// ne radi ništa) je dovoljno za neke metode nekih dekoratora). 
	int zadajUdarac() const override { return dekorisaniRatnik->zadajUdarac(); }
	void primiUdarac(int jacinaUdarca) override { dekorisaniRatnik->primiUdarac(jacinaUdarca); }
};

class RatnikSaMacem : public RatnikSaOpremom {
private:
	int jacinaMaca;
public:
	RatnikSaMacem(Ratnik* dekorisaniRatnik, int jacinaMaca): RatnikSaOpremom(dekorisaniRatnik), jacinaMaca(jacinaMaca) {}
	int zadajUdarac() const override {
		int jacinaOsnovnogUdarca = dekorisaniRatnik->zadajUdarac();
		return jacinaMaca + jacinaOsnovnogUdarca;
	}
	// primiUdarac ne preklapamo već prepuštamo da se pozove implementacija iz klase 
	// RatnikSaOpremom (a ta metoda samo prosleđuje poziv dekorisanom objektu). 
};

class RatnikSaStitom : public RatnikSaOpremom {
private:
	int jacinaStita;
public:
	RatnikSaStitom(Ratnik* dekorisaniRatnik, int jacinaStita): 
		RatnikSaOpremom(dekorisaniRatnik), jacinaStita(jacinaStita) {}
	void primiUdarac(int jacinaUdarca) override {
		jacinaUdarca -= jacinaStita;
		dekorisaniRatnik->primiUdarac(jacinaUdarca);
	}
	// ovde nismo preklopili zadajUdarac.. 
};

class RatnikSaOklopom : public RatnikSaOpremom {
private:
	int jacinaOklopa;
public:
	RatnikSaOklopom(Ratnik* dekorisaniRatnik, int jacinaOklopa) :
		RatnikSaOpremom(dekorisaniRatnik), jacinaOklopa(jacinaOklopa) {}
	// Recimo da je jak oklop težak i da zbog toga onaj ko ima jak (težak) oklop
	// ulaže veći napor da zada udarac čineći udarac slabijim. 
	// Ovde ćemo zato preklopiti i primiUdarac i zadajUdarac. 
	// Na primerima ove dve metode vidimo i da kod dekoratora, u zavisnosti od ponašanja
	// koje hoćemo da postignemo, možemo da prvo izvršimo poziv metode dekorisanog objekta
	// a da onda "dodamo nešto" ili da prvo izvršimo neku modifikaciju, a zatim pozovemo
	// metodu dekorisanog objekta. 
	void primiUdarac(int jacinaUdarca) override {
		jacinaUdarca -= jacinaOklopa;
		dekorisaniRatnik->primiUdarac(jacinaUdarca);
	}
	int zadajUdarac() const override {
		int jacinaOsnovnogUdarca = dekorisaniRatnik->zadajUdarac();
		return jacinaOsnovnogUdarca - 0.1 * jacinaOklopa;
	}
};

int main() {
	Ratnik* r = new OdbrambeniRatnik(10, 8);
	Ratnik* r2 = new RatnikSaStitom(new RatnikSaOklopom(new RatnikSaMacem(new RatnikZaNapad(10, 10), 5), 10), 4);
	
	std::cout << "Odbrambeni ratnik zadaje udarac jacine: " << r->zadajUdarac() << std::endl;
	std::cout << "Odbrambeni ratnik prima udarac: " << std::endl;
	r->primiUdarac(10);

	std::cout << "Ratnik sa opremom zadaje udarac jacine: " << r2->zadajUdarac() << std::endl;
	std::cout << "Ratnik sa opremom ratnik prima udarac: " << std::endl;
	r2->primiUdarac(10);

	return 0;
}