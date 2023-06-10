#include<iostream>
#include <unordered_map>

class Automat;

class IStanje {
public:
	virtual char prelaz(Automat*, char) = 0; // bitove ćemo uzimati kao karaktere '0' i '1'
	// i isto tako vraćati izlaze (kao karaktere)
	virtual char opis() const = 0; // Nije neophodna metoda. Sluzi nam samo da nam
	// za stanje A vrati 'A', za B 'B' itd.
	virtual ~IStanje() {}
};

// Automat koji detektuje sekvencu 1101 (uz dozvoljeno preklapanje sekvenci) dat je na 
// slici automat.png (u folderu je zad2)

class Automat {
private:
	IStanje* stanje;
public:
	Automat();
	std::string prihvatiSekvencu(const std::string& ulaz) {
		std::string izlaz = "";
		for (int i = 0; i < ulaz.size(); i++) {
			std::cout << "Trenenutno stanje: " << stanje->opis() << std::endl;
			izlaz += stanje->prelaz(this, ulaz[i]);
		}
		std::cout << "Trenenutno stanje: " << stanje->opis() << std::endl;
		return izlaz;
	}
	void postaviStanje(IStanje* novoStanje) { stanje = novoStanje; }
};

/*
Imaćemo po jednu klasu za svako stanje (kao i ranije). Bitno je sledeće zapažanje: 
metoda prelaz svakog od konkretnih stanja biće oblika (uz pretpostavku da su ulazi samo '0' i '1'): 
	if (ulaz == '0') {
		// pređi u jedno stanje i vrati jedan izlaz
	}
	else {
		// pređi u neko drugo stanje i vrati neki drugi izlaz
	}

S obzirom na to, mogli bismo napisati četiri izvedene klase i "ručno" ispisati četiri puta
skoro isti kod, ili možemo iskoristiti šablon!
Naravno, sve ovo se može napisati i bez šablona, uz malo više koda i pomalo dosadno.
Prvo ćemo uvesti klasu OznačenoStanje koja predstavlja stanje koje ima oznaku, a oznaku
ćemo slati kao parametar šablona. Ova klasa nije neophodna ukoliko ne želimo da imamo metodu opis!
*/
template<char oznaka>
class OznacenoStanje : public IStanje {
public:
	char opis() const override { return oznaka; }
};

// Naredna šablonska klasa je izuzetno bitna. Pomoću nje omogući ćemo "šablonske" prelaze
// koje smo opisali u komentaru desetak linija iznad. Klasu ćemo izvesti iz klase
// OznacenoStanje<oznaka>, pa je kao parametar šablona potrebno proslediti oznaku.
// Osim oznake (koja nije neophodna ako ne implementiramo neobaveznu metodu opsi), 
// parametri šablona koje šaljemo, a koji su od suštinske važnosti za 
// omogućavanje prelaza su oznaka narednog stanja ako na ulazu dobijemo nulu, izlaz
// ukoliko dobijemo nulu, kao i naredno stanje i izalz ako dobijemo jedinicu. 
template<char oznaka, char prelaz_0, char izlaz_0, char prelaz_1, char izlaz_1>
class StanjeT : public OznacenoStanje<oznaka> {
	// Ovu metodu implementiraćemo kasnije, jer ćemo koristiti klase Stanje<oznaka> koje
	// nismo definisali još uvek
	char prelaz(Automat*, char) override;
};

template<char oznaka>
class Stanje; // Ovo nam treba samo kao šablon. Nebitno nam je da ima bilo kakve veze sa 
// prethodno napisanim klasama. Ovaj šablon koristićemo samo za specijalizacije, a pomoću
// tih specijalizacija definisaćemo stanja A, B, C, D (tj. zvaćemo ih Stanje<'A'> itd.)

// Ovo je suština i praktičnost pristupa sa šablonima. Nećemo pisati klase Stanje<'A'>, 
// Stanje<'B'> itd. Samo ćemo izvršiti nasleđivanje iz odgovarajuće klase StanjeT
// i osloniti se na implementaciju koja je data u klasi StanjeT.
// Prvi parametar šablona StanjeT uvek je simbol koji navodimo kao parametar šablona
// za Stanje. Ostale parametre čitamo sa slike automat.png
template<>
class Stanje<'A'> : public StanjeT<'A', 'A', '0', 'B', '0'> {};
template<>
class Stanje<'B'> : public StanjeT<'B', 'A', '0', 'C', '0'> {};
template<>
class Stanje<'C'> : public StanjeT<'C', 'D', '0', 'C', '0'> {};
template<>
class Stanje<'D'> : public StanjeT<'D', 'A', '0', 'B', '1'> {};

class FabrikaStanja { // Fabrika muva
private:
	FabrikaStanja() = default;
	FabrikaStanja& operator=(const FabrikaStanja&) = default;
	FabrikaStanja(const FabrikaStanja&) = default;
	std::unordered_map<char, IStanje*>stanja;
public:
	static FabrikaStanja& dajFabriku() {
		static FabrikaStanja fabrika;
		return fabrika;
	}

	/*
	// Standardna implementacija metode dajObjekat
	static IStanje* dajStanje(char oznaka) {
		FabrikaStanja& fabrika = FabrikaStanja::dajFabriku();
		if (fabrika.stanja.find(oznaka) == fabrika.stanja.end()) {
			switch (oznaka) {
			case 'A':
				fabrika.stanja[oznaka] = new Stanje<'A'>();
				break;
			case 'B':
				fabrika.stanja[oznaka] = new Stanje<'B'>();
				break;
			case 'C':
				fabrika.stanja[oznaka] = new Stanje<'C'>();
				break;
			case 'D':
				fabrika.stanja[oznaka] = new Stanje<'D'>();
				break;
			default:
				fabrika.stanja[oznaka] = nullptr;
			}
		}
		return fabrika.stanja[oznaka];
	}*/

	/* 
	// Kad već radimo sa šablonima možemo iskoristiti i sledeću (kompaktniju) implementaciju:
	template<char oznaka>
	static IStanje* dajStanje() {
		FabrikaStanja& fabrika = dajFabriku();
		if (fabrika.stanja.find(oznaka) == fabrika.stanja.end())
			fabrika.stanja[oznaka] = new Stanje<oznaka>();
		return fabrika.stanja[oznaka];
	}*/

	// A može i još kraće (onda nam ne treba uopšte klasa fabrika, već bi ovo mogla da bude
	// statička metoda klase Automat).
	// Setite se, kreiraće se po jedna statička metoda za svaku oznaku i po jedan objekat
	// tipa Stanje<oznaka>!
	template<char oznaka>
	static IStanje* dajStanje() {
		static Stanje<oznaka> stanje;
		return &stanje;
	}
};

template<char oznaka, char prelaz_0, char izlaz_0, char prelaz_1, char izlaz_1>
char StanjeT<oznaka, prelaz_0, izlaz_0, prelaz_1, izlaz_1>::prelaz(Automat* a, char ulaz) {
	// Ovaj šablon nam reguliše prelaze za sva stanja. Stanja smo definisali nasleđivanjem 
	// instance šablona!
	if (ulaz == '0') {
		a->postaviStanje(FabrikaStanja::dajStanje<prelaz_0>());
		return izlaz_0;
	}
	else {
		a->postaviStanje(FabrikaStanja::dajStanje<prelaz_1>());
		return izlaz_1;
	}
}

Automat::Automat() { stanje = new Stanje<'A'>(); }

int main() {
	Automat a;
	auto izlaznaSekvenca = a.prihvatiSekvencu("11011011010000011111101");
	std::cout << "\nIzlazna sekvenca: \n" << izlaznaSekvenca << std::endl;
	return 0;
}

