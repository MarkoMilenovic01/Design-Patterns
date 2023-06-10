#include<iostream>
#include<fstream>

class Operacija;

class Osoba {
	std::string ime, prezime;
public:
	Osoba(const std::string& ime, const std::string& prezime): ime(ime), prezime(prezime) {}
	std::string dajIme() const { return ime; }
	std::string dajPrezime() const { return prezime; }
	virtual void izvrsiOperaciju(Operacija*) = 0;
	virtual ~Osoba() {}
};

class Zaposleni : public Osoba {
private:
	std::string posao;
public:
	Zaposleni(
		const std::string& ime,
		const std::string& prezime,
		const std::string& posao) : Osoba(ime, prezime), posao(posao) {}
	void izvrsiOperaciju(Operacija*) override;
	std::string dajPosao() const { return posao; }
};

class Nezaposleni : public Osoba {
public:
	using Osoba::Osoba;
	void izvrsiOperaciju(Operacija*) override;
};

// U ovom primeru ćemo raditi sa referencama umesto sa pokazivačima.
class Operacija {
public:
	virtual void izvrsi(Zaposleni&) = 0;
	virtual void izvrsi(Nezaposleni&) = 0;
	virtual ~Operacija() {}
};

class Rad : public Operacija {
public:
	void izvrsi(Zaposleni& o) override{
		std::cout << o.dajIme() << " " << o.dajPrezime() << " radi na poslu " << o.dajPosao() << std::endl;
	}
	void izvrsi(Nezaposleni& o) override {
		std::cout << o.dajIme() << " " << o.dajPrezime() << " je nezaposlen i trazi posao..." << std::endl;
	}
};

class Odmor : public Operacija {
public:
	void izvrsi(Zaposleni& o) override {
		std::cout << o.dajIme() << " " << o.dajPrezime() << " odmara od posla " << o.dajPosao() << std::endl;
	}
	void izvrsi(Nezaposleni& o) override {
		std::cout << o.dajIme() << " " << o.dajPrezime() << " je nezaposlen i odmara od trazenja posla" << std::endl;
	}
};

class EksportUFajl : public Operacija {
private:
	std::string imeFajla; // Ako je za izvršenje operacije potreban neki argument, možemo ga 
	// sačuvati kao atribut 
public:
	EksportUFajl(const std::string& imeFajla) : imeFajla(imeFajla) {}
	void izvrsi(Zaposleni& o) override {
		std::ofstream fajl(imeFajla);
		fajl << "Zaposleni: " << o.dajIme() << ", " << o.dajPrezime() << ", " << o.dajPosao() << std::endl;
		fajl.close();
	}
	void izvrsi(Nezaposleni& o) override {
		std::ofstream fajl(imeFajla);
		fajl << "Nezaposleni: " << o.dajIme() << ", " << o.dajPrezime() << std::endl;
		fajl.close();
	}
};

void Nezaposleni::izvrsiOperaciju(Operacija* o) {
	o->izvrsi(*this);
}
void Zaposleni::izvrsiOperaciju(Operacija* o) {
	o->izvrsi(*this);
}

int main() {
	Zaposleni pera("Pera", "Peric", "prodavac");
	Nezaposleni mika("Mika", "Mikic");
	Operacija* r = new Rad();
	Operacija* o = new Odmor();
	Operacija* e1 = new EksportUFajl("pera.txt");
	Operacija* e2 = new EksportUFajl("mika.txt");

	pera.izvrsiOperaciju(r);
	pera.izvrsiOperaciju(o);
	pera.izvrsiOperaciju(e1);
	mika.izvrsiOperaciju(r);
	mika.izvrsiOperaciju(o);
	mika.izvrsiOperaciju(e2);

	delete e1;
	delete e2;
	delete r;
	delete o;

	return 0;
}