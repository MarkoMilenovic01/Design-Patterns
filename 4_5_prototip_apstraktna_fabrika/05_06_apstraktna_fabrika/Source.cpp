#include<iostream>

/*
5. Kreirati apstraktne klase Stolica, Sto i Polica (sa čisto virtuelnom metodom opis()) i 
konkretne klase DrvenaStolica, DrveniSto, DrvenaPolica, ModernaStolica, ModerniSto, ModernaPolica
kao i fabrike za proizvode ove dve familije proizvoda.
6. Napraviti klasu Soba koja ima stolicu, sto i policu. Sobu treba kreirati objekat 
klase Dizajner na način:
Soba s = dizajner.kreirajSobu(stilSobe);
Možemo imati dva graditelja koja će kreirati sobu postavljajući nameštaj iz određene 
fabrike (jedan graditelj koristi jednu fabriku, drugi drugu). Dizajner bi igrao ulogu
upravljača/direktora. Ukoliko konkretne graditelje kreiramo na osnovu stila sobe (enum ili string)
onda imamo takođe i primenu fabričkog metoda koji će kreirati graditelja.
Pitanje: da li bismo mogli da kloniramo sobu? Postoje li neke prepreke za to?
*/

class ISto {
public:
	virtual void opis() = 0;
	virtual ~ISto() {};
};

class DrveniSto : public ISto {
public:
	void opis() override {
		std::cout << "Drveni sto" << std::endl;
	}
};

class ModerniSto : public ISto {
public:
	void opis() override {
		std::cout << "Moderni sto" << std::endl;
	}
};

class IStolica {
public:
	virtual void opis() = 0;
	virtual ~IStolica() {}
};

class DrvenaStolica : public IStolica {
public:
	void opis() override {
		std::cout << "Drvena stolica" << std::endl;
	}
};

class ModernaStolica : public IStolica {
public:
	void opis() override {
		std::cout << "Moderna stolica" << std::endl;
	}
 };

class IPolica {
public:
	virtual void opis() = 0;
	virtual ~IPolica() {}
};

class DrvenaPolica : public IPolica {
public:
	void opis() override {
		std::cout << "Drvena polica" << std::endl;
	}
};

class ModernaPolica : public IPolica {
public:
	void opis() override {
		std::cout << "Moderna polica" << std::endl;
	}
};

class IFabrikaNamestaja {
public:
	virtual IStolica* napraviStolicu() = 0;
	virtual ISto* napraviSto() = 0;
	virtual IPolica* napraviPolicu() = 0;
	virtual ~IFabrikaNamestaja() {}
};

class FabrikaDrvenogNamestaja : public IFabrikaNamestaja {
public:
	IStolica* napraviStolicu() override { return new DrvenaStolica(); }
	ISto* napraviSto() override { return new DrveniSto(); }
	IPolica* napraviPolicu() override { return new DrvenaPolica(); }
};

class FabrikaModernogNamestaja : public IFabrikaNamestaja {
public:
	IStolica* napraviStolicu() override { return new ModernaStolica(); }
	ISto* napraviSto() override { return new ModerniSto(); }
	IPolica* napraviPolicu() override { return new ModernaPolica(); }
};


class Soba {
private:
	ISto* sto;
	IPolica* polica;
	IStolica* stolica;
public:
	void setSto(ISto* sto) { this->sto = sto; }
	void setStolica(IStolica* stolica) { this->stolica = stolica; }
	void setPolica(IPolica* polica) { this->polica = polica; }
	void opis() {
		std::cout << "Opis sobe: " << std::endl;
		sto->opis();
		stolica->opis();
		polica->opis();
	}
};

enum class StilSobe {DRVENA, MODERNA};

class IGraditeljSobe {
protected:
	Soba* soba;
public:
	IGraditeljSobe& kreirajPraznuSobu() {
		soba = new Soba();
		return *this;
	}
	virtual IGraditeljSobe& postaviSto() = 0;
	virtual IGraditeljSobe& postaviStolicu() = 0;
	virtual IGraditeljSobe& postaviPolicu() = 0;
	virtual ~IGraditeljSobe() {}
	Soba* dajSobu() { return soba; }
	static IGraditeljSobe* kreirajGraditelja(StilSobe stil);
};

class GraditeljDrveneSobe: public IGraditeljSobe {
private:
	FabrikaDrvenogNamestaja fabrika;
public:
	IGraditeljSobe& postaviSto() override {
		soba->setSto(fabrika.napraviSto());
		return *this;
	}
	IGraditeljSobe& postaviStolicu() override {
		soba->setStolica(fabrika.napraviStolicu());
		return *this;
	}
	IGraditeljSobe& postaviPolicu() override {
		soba->setPolica(fabrika.napraviPolicu());
		return *this;
	}
};

class GraditeljModerneSobe : public IGraditeljSobe {
private:
	FabrikaModernogNamestaja fabrika;
public:
	IGraditeljSobe& postaviSto() override {
		soba->setSto(fabrika.napraviSto());
		return *this;
	}
	IGraditeljSobe& postaviStolicu() override {
		soba->setStolica(fabrika.napraviStolicu());
		return *this;
	}
	IGraditeljSobe& postaviPolicu() override {
		soba->setPolica(fabrika.napraviPolicu());
		return *this;
	}
};

IGraditeljSobe* IGraditeljSobe::kreirajGraditelja(StilSobe stil) {
	if (stil == StilSobe::DRVENA) return new GraditeljDrveneSobe();
	return new GraditeljModerneSobe();
}

class Dizajner {
public:
	Soba* kreirajSobu(StilSobe stil) {
		IGraditeljSobe* graditelj = IGraditeljSobe::kreirajGraditelja(stil);
		graditelj->kreirajPraznuSobu().postaviPolicu().postaviSto().postaviStolicu();
		Soba* soba = graditelj->dajSobu();
		delete graditelj;
		return soba;
	}
};

int main() {
	Dizajner dizajner;
	Soba* s1 = dizajner.kreirajSobu(StilSobe::DRVENA);
	Soba* s2 = dizajner.kreirajSobu(StilSobe::MODERNA);
	s1->opis();
	std::cout << std::endl;
	s2->opis();

	return 0;
}
