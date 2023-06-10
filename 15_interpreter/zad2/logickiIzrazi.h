#pragma once
#include<unordered_set>
#include<unordered_map>
#include<iostream>

class Kontekst {
private:
	std::unordered_map<std::string, bool> vrednosti;
public:
	bool vrednost(std::string kljuc) {
		return vrednosti[kljuc];
	}
	friend std::ostream& operator << (std::ostream& stream, Kontekst k) {
		for (auto it = k.vrednosti.begin(); it != k.vrednosti.end(); it++) {
			stream << it->first << ": " << it->second << "\n";
		}
		stream << std::endl;
		return stream;
	}
	bool& operator[] (std::string kljuc) { return vrednosti[kljuc]; }
};

class LogickiIzraz {
public:
	virtual bool interpretiraj(Kontekst&) const = 0;
	virtual ~LogickiIzraz() {}
	virtual std::string toString() const = 0; // Napomena: ova metoda ne traži se zadatkom i njena implementacija nije neophodna.
	// Dodata je samo kao dodatni primer za vežbu, a i iz praktičnih razloga - da bismo mogli da prosto ispišemo logički
	// izraz.. 
	virtual void sakupiPromenljive(std::unordered_set<std::string>& promenljive) = 0;
	// Dodata metoda koja treba da prođe kroz logički izraz i vrati nam (kao skup) sve promenljive od kojih
	// dati izraz zavisi. Neobavezna metoda naravno. Dodata zarad vežbe i za primer provere da li je neka
	// formula tautologija/kontradikcija.
	// Skup je struktura podataka koja nam omogućuje da čuvamo elemente bez dupliranja. 

	static LogickiIzraz* parsiraj(const std::string&);
};

class Konstanta : public LogickiIzraz {
private:
	bool vrednost;
public:
	Konstanta(bool vrednost) : vrednost(vrednost) {}
	bool interpretiraj(Kontekst&) const override { return vrednost; }
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) {}
	std::string toString() const override { return vrednost ? "true" : "false"; }
};

class Promenljiva : public LogickiIzraz {
private:
	std::string ime;
public:
	Promenljiva(const std::string& ime) : ime(ime) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return kontekst.vrednost(ime);
	}
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) { promenljive.insert(ime); }
	std::string toString() const override { return ime; }
};


// U ovoj verziji oslanjamo se na dinamički polimorfizam
class LogickoIli : public LogickiIzraz {
private:
	LogickiIzraz* leva;
	LogickiIzraz* desna;
public:
	LogickoIli(LogickiIzraz* leva, LogickiIzraz* desna) :
		leva(leva), desna(desna) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return leva->interpretiraj(kontekst) || desna->interpretiraj(kontekst);
	}
	// Seteri su nam potrebni zbog parsiranja kasnije...
	void postaviLevi(LogickiIzraz* levi) { this->leva = levi; }
	void postaviDesni(LogickiIzraz* desni) { this->desna = desni; }
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) {
		leva->sakupiPromenljive(promenljive);
		desna->sakupiPromenljive(promenljive);
	}

	std::string toString() const override { return "OR(" + leva->toString() + ", " + desna->toString() + ")"; }

	~LogickoIli() { delete leva; delete desna; }
};

class LogickoI : public LogickiIzraz {
private:
	LogickiIzraz* leva;
	LogickiIzraz* desna;
public:
	LogickoI(LogickiIzraz* leva, LogickiIzraz* desna) :
		leva(leva), desna(desna) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return leva->interpretiraj(kontekst) && desna->interpretiraj(kontekst);
	}
	void postaviLevi(LogickiIzraz* levi) { this->leva = levi; }
	void postaviDesni(LogickiIzraz* desni) { this->desna = desni; }
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) {
		leva->sakupiPromenljive(promenljive);
		desna->sakupiPromenljive(promenljive);
	}
	std::string toString() const override { return "AND(" + leva->toString() + ", " + desna->toString() + ")"; }

	~LogickoI() { delete leva; delete desna; }
	//Napomena: umseto što LogickoI i LogickoIli imaju identične konstruktore, po dva identična setera,
	// identičan destruktor i skoro identičnu implementaciju metode ispisi(),
	// mogli smo obe klase izvesti iz klase BinarniLogickiIzraz, ali ovde je data lošija implementacija, 
	// da ne bismo zbunjivali uvođenjem nove klase i da bi kod bio što sličniji kodu iz prethodnog zadatka.
};


// Zbog primera sa tautologijama, bilo je zgodno da uvedemo i ekvivalenciju kao logički izraz.
// DA bismo kompaktno zapisali neke poznate tautologije...
class Ekvivalencija : public LogickiIzraz {
private:
	LogickiIzraz* leva;
	LogickiIzraz* desna;
public:
	Ekvivalencija(LogickiIzraz* leva, LogickiIzraz* desna) :
		leva(leva), desna(desna) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return leva->interpretiraj(kontekst) == desna->interpretiraj(kontekst);
	}
	void postaviLevi(LogickiIzraz* levi) { this->leva = levi; }
	void postaviDesni(LogickiIzraz* desni) { this->desna = desni; }
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) {
		leva->sakupiPromenljive(promenljive);
		desna->sakupiPromenljive(promenljive);
	}
	std::string toString() const override { return "EQV(" + leva->toString() + ", " + desna->toString() + ")"; }

	~Ekvivalencija() { delete leva; delete desna; }
	//Napomena: umseto što LogickoI i LogickoIli imaju identične konstruktore, po dva identična setera,
	// identičan destruktor i skoro identičnu implementaciju metode ispisi(),
	// mogli smo obe klase izvesti iz klase BinarniLogickiIzraz, ali ovde je data lošija implementacija, 
	// da ne bismo zbunjivali uvođenjem nove klase i da bi kod bio što sličniji kodu iz prethodnog zadatka.
};


class Negacija : public LogickiIzraz {
private:
	LogickiIzraz* formula;
public:
	Negacija(LogickiIzraz* formula) : formula(formula) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return !formula->interpretiraj(kontekst);
	}
	void postaviFormulu(LogickiIzraz* formula) { this->formula = formula; }
	std::string toString() const override { return "NOT(" + formula->toString() + ")"; }
	void sakupiPromenljive(std::unordered_set<std::string>& promenljive) { formula->sakupiPromenljive(promenljive); }
	~Negacija() { delete formula; }

};