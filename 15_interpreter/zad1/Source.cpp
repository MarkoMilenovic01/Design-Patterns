#include<iostream>
#include"LogickiIzrazi.h"

/*
NAPOMENA: da ne bismo radili primer isti kao primer sa predavanja ili kao neki od primera koje je
radio Dejan Kolundžija, ovde smo se opredelili za pristup kod koga ćemo logičke izraze kreirati pozivima
funkcija AND, OR, NOT, a koje će kao rezultate vraćati odgovarajuće neterminalne logičke izraze. 
Koristeći šablone omogućićemo željeno ponašanje i veoma čitljiv kod (nigde u klijentu (main funkciji)
nećemo eksplicitno kreirati objekte klasa LogickiIzraz (nigde nećemo imati relativno nečitljiv kod poput
LogickiIzraz* izraz = new LogickoI(new LogickoIli(new Negacija(new Promenljiva("x")), new Konstanta(true)), new Promenljiva("y"));
Umesto toga, omogućićemo praktičniji i lepši zapis za kreiranje izraza poput:
auto izraz = AND(OR(NOT(Promenljiva("x")), Konstanta(true)), Promenljiva("y"));
Štaviše, ići ćemo i korak dalje da omogućimo kreiranje izraza još praktičnijom linijom poput:
auto izraz = AND(OR(NOT("x"), true), "y")
*/

// Funkcija AND kreiraće objekat tipa LogickoI. 
// Pogledati fajl LogickiIzrazi.h!
// LogickoI je šablon klase i zavisi od dva parametra: leva i desna formula. 
// Obe klase (formule) moraju biti takve da podržavaju poziv metode interpretiraj(Kontekst&) i operatora <<.
// Mi hoćemo da omogućimo poziv funkcije AND čak i kada se kao neki od operanada nalazi string ili bool.
// U slučaju argumenta tipa bool treba ga tretirati kao konstantu, a string kao promenljivu. 
// Ukoliko u vreme komajliranja imamo liniju tipa AND(true, "x") instanciraće se funckija
// AND<bool, const char*>, međutim ono što treba da vratimo kao rezultat funkcije NIJE tipa
// LogickoI<bool, const char*> - to bi bili nevalidni parametri šablona (ne postoji metoda interpretiraj
// za promenljivu tipa bool ili const char*).
// Očekivani tip rezultata bi zapravo bio LogickoI<Konstanta, Promenljiva>. 
// Isto tako mogli bismo pozvati AND sa AND(true, true) ili AND(OR("x", "y"), NOT("z")) pri čemu su
// odgovarajući tipovi rezultata LogickoI<Konstanta, Konstanta> i 
// LogickoI<LogickoIli<Promenljiva, Promenljiva>, Negacija<Promenljiva>>, a funkcije koje vrše poziv
// AND<bool, bool> i AND<LogickoIli<Promenljiva, Promenljiva>, Negacija<Promenljiva>>. 
// U drugom slučaju vidimo da je tip rezultata poziva funkcije AND<L, R> tipa LogickoI<L, R>, ali
// da to ne važi za sve slučajeve (važi samo kada su i L i R formule, a ne const char* ili bool).
// Tipičan, staromodni, način da razrešimo ovu situaciju je specijalizacijom šablona funkcije. 
// Da sami "ručno" ispišemo sve moguće funkcije za slučajeve kada je neki od operanada bool, kada je neki
// const char* ili formula. Dva operanda, tri mogućnosti za svaki, daju ukupno 9 različitih funkcija koje bismo
// morali da pišemo, tj. imali bismo četiri specijalizacije, četiri parcijalne specijalizacije i jedan šablon
// sa oba parametra.
// Srećom, od C++-a 14 kao rezultat funkcije možemo navesti ključnu reč auto ukoliko je moguće u vreme kompajliranja
// odrediti tip podatka koji se vraća kao rezultat funkcije. 
// OD C++-a 17 moguće je korišćenje konstantnih izraza za uslovno grananje. Konstantni izrazi se zapisuju sa
// constexpr(izraz), pri čemu izraz mora biti izraz čija je vrednost poznata u vreme kompajliranja. 
// Kombinovanjem constexpr za grananje i ključne reči auto, kao i utvrđivanjem da li je neki od tipova Leva i Desna
// bool ili const char* mi možemo imati različite grane koje vraćaju različite tipove podataka. To ne znači da će
// zaista jedna funkcija vraćati više tipova, već će se za svaku granu instancirati po jedna funkcija.
// Praktično, rezultat je isti kao da smo ispisali 9 funkcija (šablona/percijalnih i potpunih specijalizacija),
// ali imamo samo jedan šablon (doduše, ipak imamo grananje na 9 slučajeva, od toga ne možemo da pobegnemo).
template<typename Leva, typename Desna>
auto AND(const Leva leva, const Desna desna) {
	if constexpr(std::is_same<Leva, bool>::value) {
		// Ako je tip Leva bool, onda kao levi operand treba da postavimo konstantu, a zatim ispitamo 
		// sve moguće vrednosti za desni tip. I tako dalje za sve slučajeve...
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoI<Konstanta, Konstanta>(Konstanta(leva), Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoI<Konstanta, Promenljiva>(Konstanta(leva), Promenljiva(desna));
		}
		else {
			return LogickoI<Konstanta, Desna>(Konstanta(leva), desna);
		}
	}
	else if constexpr (std::is_same<Leva, const char*>::value) {
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoI<Promenljiva, Konstanta>(Promenljiva(leva), Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoI<Promenljiva, Promenljiva>(Promenljiva(leva), Promenljiva(desna));
		}
		else {
			return LogickoI<Promenljiva, Desna>(Promenljiva(leva), desna);
		}
	}
	else {
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoI<Leva, Konstanta>(leva, Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoI<Leva, Promenljiva>(leva, Promenljiva(desna));
		}
		else {
			// Naravno, ovaj opšti slučaj pokriva sve slučajeve kada ni Leva ni Desna nisu ni const char*, ni bool.
			return LogickoI<Leva, Desna>(leva, desna);
		}
	}
}


// Sada sve isto radimo za šablon funkcije OR koja treba da kreira odgovarajući objekat LogickoIli<...,...>
template<typename Leva, typename Desna>
auto OR(const Leva leva, const Desna desna) {
	if constexpr (std::is_same<Leva, bool>::value) {
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoIli<Konstanta, Konstanta>(Konstanta(leva), Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoIli<Konstanta, Promenljiva>(Konstanta(leva), Promenljiva(desna));
		}
		else {
			return LogickoIli<Konstanta, Desna>(Konstanta(leva), desna);
		}
	}
	else if constexpr (std::is_same<Leva, const char*>::value) {
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoIli<Promenljiva, Konstanta>(Promenljiva(leva), Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoIli<Promenljiva, Promenljiva>(Promenljiva(leva), Promenljiva(desna));
		}
		else {
			return LogickoIli<Promenljiva, Desna>(Promenljiva(leva), desna);
		}
	}
	else {
		if constexpr (std::is_same<Desna, bool>::value) {
			return LogickoIli<Leva, Konstanta>(leva, Konstanta(desna));
		}
		else if constexpr (std::is_same<Desna, const char*>::value) {
			return LogickoIli<Leva, Promenljiva>(leva, Promenljiva(desna));
		}
		else {
			return LogickoIli<Leva, Desna>(leva, desna);
		}
	}
}

// Za negaciju imamo znatno manje posla... Samo 3 slučaja.
template<typename Formula>
auto NOT(Formula formula) {
	if constexpr (std::is_same<Formula, bool>::value) {
		return Negacija<Konstanta>(Konstanta(formula));
	}
	else if constexpr (std::is_same<Formula, const char*>::value) {
		return Negacija<Promenljiva>(Promenljiva(formula));
	} 
	else { return Negacija<Formula>(formula); }
}


int main() {
	Kontekst k1;
	k1["x"] = true;
	k1["y"] = true;
	k1["z"] = false;

	Kontekst k2;
	k2["x"] = true;
	k2["y"] = false;
	k2["z"] = false;

	// Prelaskom mišem preko ključne reči auto Visual Studio će vam pokazati kog je tačno tipa 
	// promenljiva izraz. U slučaju linije ispod, odgovarajući tip je:
	// LogickoI<LogickoIli<LogickoI<LogickoI<Konstanta, Negacija<Promenljiva>>, Promenljiva>, Promenljiva>, Promenljiva>,
	auto izraz = AND(OR(AND(AND(true, NOT("z")), "x"), "x"), "y");

	bool v1 = izraz.interpretiraj(k1);
	bool v2 = izraz.interpretiraj(k2);

	std::cout << "Vrednost izraza " << izraz << " u kontekstu: \n" << k1 << v1 << std::endl;
	std::cout << "Vrednost izraza " << izraz << " u kontekstu: \n" << k2 << v2 << std::endl;
}