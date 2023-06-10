#include<iostream>

class Posetilac;

class Element {
protected:
	int stanje;
public:
	Element(int stanje): stanje(stanje) {}
	int dajStanje() const { return stanje; }
	virtual void osnovnaFunkcionalnost() = 0; // ova metoda je tu da naglasimo da klase iz
	// hijerarhije Element već nešto "rade", a da su posetioci tu da pruže dodatne funkcionalnosti.

	// Naredna metoda je imenovana izvrsiNovuOperaciju da bi bolje asocirala na ono čemu služi. Na
	// UML-u i u terminologiji projektnog uzorka Posetilac ta metoda se često naziva prihvati (engl. accept),
	// što ne deluje previše informativno.
	virtual void izvrsiNovuOperaciju(Posetilac*) = 0;
	virtual ~Element() {}
};

class KonkretniElementA : public Element {
public:
	using Element::Element;
	void osnovnaFunkcionalnost() override { std::cout << "Osnovna funkcionalnost A" << std::endl; }
	void izvrsiNovuOperaciju(Posetilac*) override; // Zbog redosleda definisanja klasa, 
	// ova implementacija biće data naknadno, van klase.
};

class KonkretniElementB : public Element {
public:
	using Element::Element;
	void osnovnaFunkcionalnost() override { std::cout << "Osnovna funkcionalnost B" << std::endl; }
	void izvrsiNovuOperaciju(Posetilac*) override; // implementacija je data naknadno.
};

class Posetilac {
public:
	// Ovo je bitno: posetilac mora da zna kako da "poseti" svaku konkretnu klasu. 
	// Ovde je u kodu ostala originalna terminologija "poseti" (engl. visit), mada bi opet
	// informativnije bilo da smo te metode nazvali "izvrši" i sl. 
	virtual void poseti(KonkretniElementA*) = 0;
	virtual void poseti(KonkretniElementB*) = 0;
	virtual ~Posetilac() {}
};

class NovaOperacija1 : public Posetilac {
public:
	void poseti(KonkretniElementA* a) override {
		std::cout << "Izvrsavam novu operaciju 1 nad elementom klase KonkretniElementA sa stanjem " << a->dajStanje() << std::endl;
	}
	void poseti(KonkretniElementB* a) override {
		std::cout << "Izvrsavam novu operaciju 1 nad elementom klase KonkretniElementB sa stanjem " << a->dajStanje() << std::endl;
	}
};

class NovaOperacija2 : public Posetilac {
public:
	void poseti(KonkretniElementA* a) override {
		std::cout << "Izvrsavam novu operaciju 2 nad elementom klase KonkretniElementA sa stanjem " << a->dajStanje() << std::endl;
	}
	void poseti(KonkretniElementB* a) override {
		std::cout << "Izvrsavam novu operaciju 2 nad elementom klase KonkretniElementB sa stanjem " << a->dajStanje() << std::endl;
	}
};

// Cena toga što dodajemo nove funkcionalnosti van hijerarhije naših postojećih klasa je to 
// što postojeće klase treba samo da izvrše sledeću jednostavnu implementaciju metode 
// izvrsiNovuOperaciju (ili accept())
void KonkretniElementA::izvrsiNovuOperaciju(Posetilac* p) {
	p->poseti(this);
}

void KonkretniElementB::izvrsiNovuOperaciju(Posetilac* p) {
	p->poseti(this);
}
// Primećujemo da je implementacija metode izvrsiNovuOperaciju ista u obe izvedene klase.
// Zašto nismo mogli da onda implementaciju prebacimo u roditeljsku klasu, Element?
// Odgovor leži u tome što je u klasi Element this tipa Element*. Koju tačno metodu 
// posetioca treba pozvati ako izvršimo poziv posetilac->poseti(element), 
// gde je element tipa Element*? Ne možemo znati. Upravo zato je nepohodno da u izvedenim klasama
// dodamo kod za "posećivanje". U izvedenoj klasi this je tipa IzvedenaKlasa*, a za izvedene klase
// imamo metode poseti i nema dileme koju treba pozvati. 

int main() {
	Element* e1 = new KonkretniElementA(123);
	Element* e2 = new KonkretniElementB(321);
	Posetilac* p1 = new NovaOperacija1();
	Posetilac* p2 = new NovaOperacija2();
	e1->izvrsiNovuOperaciju(p1);
	e1->izvrsiNovuOperaciju(p2);
	e2->izvrsiNovuOperaciju(p1);
	e2->izvrsiNovuOperaciju(p2);

	delete e1;
	delete e2;
	delete p1;
	delete p2;

	return 0;
}