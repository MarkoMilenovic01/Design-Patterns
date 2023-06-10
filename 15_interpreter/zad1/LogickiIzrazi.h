#pragma once
#include<iostream>
#include<unordered_map>

class Kontekst {
private:
	std::unordered_map<std::string, bool> vrednosti;
public:
	bool vrednost(std::string kljuc) {
		return vrednosti[kljuc];
	}
	bool& operator[] (std::string kljuc) { return vrednosti[kljuc]; }
	friend std::ostream& operator << (std::ostream& stream, Kontekst k) {
		for (auto it = k.vrednosti.begin(); it != k.vrednosti.end(); it++) {
			stream << it->first << ": " << it->second << "\n";
		}
		stream << std::endl;
		return stream;
	}
};

class LogickiIzraz {
public:
	virtual bool interpretiraj(Kontekst&) const = 0;
	virtual ~LogickiIzraz() {}
	virtual void ispisi() const = 0; // Napomena: ova metoda ne traži se zadatkom i njena implementacija nije neophodna.
	// Dodata je samo kao dodatni primer za vežbu, a i iz praktičnih razloga - da bismo mogli da prosto ispišemo logički
	// izraz.. 
};

class Konstanta : public LogickiIzraz {
private:
	bool vrednost;
public:
	Konstanta(bool vrednost) : vrednost(vrednost) {}
	bool interpretiraj(Kontekst&) const override { return vrednost; }
	void ispisi() const override { std::cout << vrednost; }
	// zarad lakog ispisivanja preklopljen je i operator << 
	friend std::ostream& operator << (std::ostream& stream, const Konstanta& k) { stream << k.vrednost; return stream; }
};

class Promenljiva : public LogickiIzraz {
private:
	std::string ime;
public:
	Promenljiva(const std::string& ime) : ime(ime) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return kontekst.vrednost(ime);
	}
	friend std::ostream& operator << (std::ostream& stream, const Promenljiva& p) { stream << p.ime; return stream; }
	void ispisi() const override { std::cout << ime; }
};


/* Kod narednih neterminalnih izraza (LogickoIli, LogickoI, Negacija) ideja je slična kao kod
odloženog izračunavanja. Koristićemo statički polimorfizam. 
Levi operand i desni operand logičkog veznika može biti bilo koja formula,
tačnije, po bilo koja klasa sa metodom interpretiraj(Kontekst) i preklopljenim operatorom << 
*/
template<typename LevaFormula, typename DesnaFormula>
class LogickoIli : public LogickiIzraz {
private:
	LevaFormula leva;
	DesnaFormula desna;
public:
	LogickoIli(const LevaFormula& leva, const DesnaFormula& desna) :
		leva(leva), desna(desna) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return leva.interpretiraj(kontekst) || desna.interpretiraj(kontekst);
	}
	friend std::ostream& operator << (std::ostream& stream, const LogickoIli& ili) {
		stream << "OR(" << ili.leva << ", " << ili.desna << ")";
		return stream;
	}

	void ispisi() const override {
		std::cout << *this;
	}
};

template<typename LevaFormula, typename DesnaFormula>
class LogickoI : public LogickiIzraz {
private:
	LevaFormula leva;
	DesnaFormula desna;
public:
	LogickoI(const LevaFormula& leva, const DesnaFormula& desna) :
		leva(leva), desna(desna) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return leva.interpretiraj(kontekst) && desna.interpretiraj(kontekst);
	}
	friend std::ostream& operator << (std::ostream& stream, const LogickoI& i) {
		stream << "AND(" << i.leva << ", " << i.desna << ")";
		return stream;
	}

	void ispisi() const override {
		std::cout << *this;
	}
};

template<typename Formula>
class Negacija : public LogickiIzraz {
private:
	Formula formula;
public:
	Negacija(const Formula& formula) : formula(formula) {}
	bool interpretiraj(Kontekst& kontekst) const override {
		return !formula.interpretiraj(kontekst);
	}
	friend std::ostream& operator << (std::ostream& stream, const Negacija& n) {
		stream << "NOT(" << n.formula << ")";
		return stream;
	}

	void ispisi() const override {
		std::cout << *this;
	}
};