#include<iostream>
#include<stack>
#include<unordered_map>
#include<cassert>
#include<unordered_set>
#include "logickiIzrazi.h"


LogickiIzraz* LogickiIzraz::parsiraj(const std::string& izraz_str) {
	// Parsiranje ćemo obaviti koristeći stek. 
	// Naime, kada idući s leva na desno mi prvi put naiđemo na neku funkciju (recimo AND),
	// mi u tom trenutku ne znamo koji su tačno operandi datog izraza (LogickoI u našem primeru).
	// Ono što možemo uraditi je da smestimo u stek logički izraz, bez operanada (postavljeni su na null),
	// a da operande postavimo onda kada ih budemo saznali. Sve ovo potižemo jednostavnim korišćenjem steka.
	// Kad god naiđemo na zatvorenu zagradu, potrebno je da iz steka "izbacimo" jedan ili dva izraza (u zavisnosti
	// od arnosti izraza) i proglasimo ih operandima izraza koji je u steku ispod njih. 
	// Da bismo znali koja je arnost neterminalnog izraza smeštenog u stek i koji je tačno tip izraza u pitanju,
	// koristimo još jedan stek u kome smeštamo uređeni par (funkcija, arnost).
	std::stack<LogickiIzraz*> logickiIzrazi;
	std::stack<std::pair<std::string, int>> oznakeOperacija;

	int i = 0;
	while (i < izraz_str.length()) {
		if (izraz_str[i] == '(' || izraz_str[i] == ' ' || izraz_str[i] == ',') {
			i++;
			continue;
		}

		// U narednim linijama ispitujemo da li treba smestiti neterminani izraz u stek
		if (i < izraz_str.length() - 2) {
			if (izraz_str.substr(i, 3) == "AND") {
				oznakeOperacija.push(std::make_pair("AND", 2));
				logickiIzrazi.push(new LogickoI(nullptr, nullptr));
				i += 3;
				continue;
			}
			if (izraz_str.substr(i, 2) == "OR") {
				oznakeOperacija.push(std::make_pair("OR", 2));
				logickiIzrazi.push(new LogickoIli(nullptr, nullptr));
				i += 2;
				continue;
			}
			if (izraz_str.substr(i, 3) == "NOT") {
				oznakeOperacija.push(std::make_pair("NOT", 1));
				logickiIzrazi.push(new Negacija(nullptr));
				i += 3;
				continue;
			}
			if (izraz_str.substr(i, 3) == "EQV") {
				oznakeOperacija.push(std::make_pair("EQV", 2));
				logickiIzrazi.push(new Ekvivalencija(nullptr, nullptr));
				i += 3;
				continue;
			}
		}
		// Zatim ispitujemo konstante
		if (i < izraz_str.length() - 3) {
			if (izraz_str.substr(i, 4) == "true") {
				logickiIzrazi.push(new Konstanta(true));
				i += 4;
				continue;
			}
		}
		if (i < izraz_str.length() - 4) {
			if (izraz_str.substr(i, 5) == "false") {
				logickiIzrazi.push(new Konstanta(false));
				i += 5;
				continue;
			}
		}

		if (izraz_str[i] == ')') {
			// Ako smo stigli do zatvorene zagrade, treba da pogledamo koji smo poslednji 
			// neterminalni izraz smestili u stek oznakeOperacija. 
			// Tj. koji smo uređeni par (oznaka, arnost) smestili i da u zavisnosti od 
			// arnosti izbcimo iz steka jedan ili dva operanda.
			// 
			// Pomoću oznake operacije znamo u koji pokazivač na izvedenu klasu na konvertujemo pokazivač 
			// osnovne klase iz steka logickiIzrazi da bismo mu "prikačili" operande.

			i++;
			std::pair<std::string, int> parOznakaArnost = oznakeOperacija.top();
			oznakeOperacija.pop();

			if (parOznakaArnost.second == 1) {
				LogickiIzraz* f = logickiIzrazi.top();
				logickiIzrazi.pop();
				// nakon što smo izbacili jedan operand, trenutno se na vrhu steka nalazi baš izraz koji 
				// predstavlja negaciju, a čiji smo operand upravo pronašli. 
				((Negacija*)logickiIzrazi.top())->postaviFormulu(f); // ova konverzija je bezbedna jer jedini
				// izraz arnosti 1 je negacija. 
			}
			else { // pošto podrazumevamo da radimo sa validnim izrazima koji su unarni ili binarni, činjenica da se
				// ne radi o unarnom izrazu implicira da je u pitanju binarni izraz.
				LogickiIzraz* f1 = logickiIzrazi.top();
				logickiIzrazi.pop();
				LogickiIzraz* f2 = logickiIzrazi.top();
				logickiIzrazi.pop();
				//Napomena: da smo imali i apstraktnu klasu BinarniLogickiIzraz ne bi bilo potrebe za ispitivanjima u narednim linijama.
				if (parOznakaArnost.first == "AND") {
					((LogickoI*)logickiIzrazi.top())->postaviLevi(f2); 
					((LogickoI*)logickiIzrazi.top())->postaviDesni(f1);
				}
				else if (parOznakaArnost.first == "OR") {
					((LogickoIli*)logickiIzrazi.top())->postaviLevi(f2);
					((LogickoIli*)logickiIzrazi.top())->postaviDesni(f1);
				}
				else {
					((Ekvivalencija*)logickiIzrazi.top())->postaviLevi(f2);
					((Ekvivalencija*)logickiIzrazi.top())->postaviDesni(f1);
				}
			}
			continue;
		}
		else {
		// Ako je kontrola toka došla do ovog dela, to znači da nismo naišli ni na novu operaciju,
		// niti na neki od znakova ' ', '(', ',', ')', niti na konstante true ili false.
		// Jedina opcija koja nam preostaje je da je u pitanju naziv promenljive. 
		// Mogli smo sebi olakšati posao pretpostavkom da su imena promenljivih sačinjena iz jednog 
		// karaktera, ali ovde ćemo implementirati slučaj u kome se ime promenljive sastoji od proizvoljnog 
		// broja karaktera. Pritom se u imenu smeju naći samo slova i cifre. 
			std::string imePromenljive = "";
			while (isalnum(izraz_str[i])) imePromenljive += izraz_str[i++];
			logickiIzrazi.push(new Promenljiva(imePromenljive));
		}
	}
	//Ako smo sve odradili dobro i prosleđeni izraz je validan,
	// na kraju se u steku nalazi tacno jedan ispravno formirani logički izraz.
	assert(logickiIzrazi.size() == 1 && oznakeOperacija.empty());
	LogickiIzraz* resenje = logickiIzrazi.top();
	logickiIzrazi.pop();
	return resenje;
}


std::string testiraj_logicke_izraze_bez_parsiranja(LogickiIzraz* izraz) {
	Kontekst k1;
	k1["x"] = true;
	k1["y"] = true;
	k1["z"] = false;

	Kontekst k2;
	k2["x"] = true;
	k2["y"] = false;
	k2["z"] = false;

	if (izraz==nullptr)
		izraz = new LogickoI(new LogickoIli(new LogickoI(new LogickoI(new Konstanta(true), new Negacija(new Promenljiva("z"))), new Promenljiva("x")), new Promenljiva("x")), new Promenljiva("y"));
	// Izraz iz linije iznad odgovara izrazu iz zadatka 1. Vidimo da je znatno teze kreirati izraz na ovaj nacin...
	// Mogući kompromis sa resenjem iz prethodnog zadatka bio bi ukoliko dodamo konstruktore za sve operatore koji ce
	// kao argumente uzimati i const char* i bool. Na primer, LogickoI(const char*, bool), LogickoI(const char*, LogickiIzraz*), itd... (9 konstuktora)
	// Ali bitna stvar koja nam može olakšati život u pristupu koji koristi dinamički polimorfizam je mogućnost kreiranja
	// logičkih izraza na osnovu parsiranja stringa.
	bool v1 = izraz->interpretiraj(k1);
	bool v2 = izraz->interpretiraj(k2);

	std::string izraz_str = izraz->toString();
	std::cout << "Vrednost izraza " << izraz_str << " u kontekstu: \n" << k1 << v1 << std::endl;
	std::cout << "Vrednost izraza " << izraz_str << " u kontekstu: \n" << k2 << v2 << std::endl;

	// Vratićemo kao rezultat stringovnu reprezentaciju izraza, da bismo kasnije lako testirali parsiranje
	// na upravo dobijenom stringu.
	return izraz_str;
}


/*
Slede implementacije dve neobavezne funkcije kojim ćemo ispitivati za prosleđeni string kojim
specificiramo logički izraz, da li je dati logički izraz tautologija (prva funkcija) ili kontradikcija (druga
funkcija). 
*/
bool tautologija(std::string izraz_str) {
	LogickiIzraz* izraz = LogickiIzraz::parsiraj(izraz_str);
	Kontekst k;
	std::unordered_set<std::string> promenljive;
	izraz->sakupiPromenljive(promenljive); // ovim ćemo prikupiti sve promenljive od kojih izraz zavisi.
	std::vector<std::string> prom_vec;// zgodno je da sada promenljive prebacimo u vektor, da bismo
	// im pristupali operatorom []
	for (auto it = promenljive.begin(); it != promenljive.end(); it++) {
		prom_vec.push_back(*it);
	}

	promenljive.clear();
	int n = prom_vec.size();
	// Za izraz od n promenljivih imamo ukupno 2^n različitih konteksta, tj. različitih valuacija formule.
	// Umesto da pišemo funkciju koja će nam dati sve varijacije n-te klase od 2 elementa (za sve moguće vrednosti
	// true/false, iskoristićemo činjenicu da znamo koliko ima datih varijacija (2^n) i da svaku od varijacija možemo
	// dobiti kao binarni zapis broja iz skupa {0, 1, ... , 2^n - 1}, tj. na osnovu decimalnog indeksa. 
	std::vector<int> bitovi(n);
	int br_valuacija = pow(2, n);
	for (int i = 0; i < br_valuacija; i++) {
		for (int& bit : bitovi)
			bit = 0; // postavimo na početku sve bitove na nule
		int kopija = i;
		int pozicija = n - 1;
		while (kopija > 0) {//pretvaramo broj u binarni
			bitovi[pozicija--] = kopija % 2;
			kopija /= 2;
		}
		for (int i_prom = 0; i_prom < n; i_prom++) {
			// vrednost promenljive sa indeksom i_prom postavljamo na true ili false oslanjajući se na 
			// implicitnu konverziju int -> bool (bit 1 se konvertuje u true, 0 u false).
			// Promenljiva k predstavlja kontekst.
			k[prom_vec[i_prom]] = bitovi[i_prom];
		}
		bool v = izraz->interpretiraj(k);
		if (!v) return false;
	}
	return true;
}

/*Tautologija je tačna u svim valuacijama, a kontradikcija netačna u svim valuacijama.
 Posledica definicije je da je formula F konstradikcija akko je !F tautologija.
 */
bool kontradikcija(std::string formula) {
	return tautologija("NOT(" + formula + ")");
}

int main() {
	std::string izraz_str = testiraj_logicke_izraze_bez_parsiranja(nullptr);// funkcija će osim testiranja vratiti i 
	// stringovnu reprezentaciju izraza. 
	// U nastavku ćemo parsirati dobijeni string i interpretirati ga. Upoređivanjem ćemo se utvrditi da 
	// naša statička metoda parsiraj zaista ispravno parsira dati string
	LogickiIzraz* izraz = LogickiIzraz::parsiraj(izraz_str);
	std::string izraz_str2 = testiraj_logicke_izraze_bez_parsiranja(izraz);

	if (izraz_str == izraz_str2) {
		std::cout << "\nParsirani izraz odgvoara originalnom izrazu!" << std::endl;
	}

	bool dodatniTestovi = true;
	if (dodatniTestovi) {
		std::cout << "\n\n**************************************\n";
		std::vector<std::string> formule;
		formule.push_back("AND(a, NOT(a))");
		formule.push_back("OR(a, NOT(a))");
		formule.push_back("AND(a, b)");
		formule.push_back("EQV( NOT( AND( AND(a, b), c) ), AND( AND(a, b), c) )");
		formule.push_back("EQV( NOT(AND(a, b)), OR(NOT(a), NOT(b)))");

		for (int i = 0; i < formule.size(); i++) {
			std::string formula = formule[i];
			if (tautologija(formula)) {
				std::cout << "Formula " << formula << " je tautologija!";
			}
			else if (kontradikcija(formula)) {
				std::cout << "Formula " << formula << " je kontradikcija!";
			}
			else {
				std::cout << "Formula " << formula << " nije ni kontradikcija, ni tautologija.";
			}
			std::cout << std::endl;
		}
	}
 	return 0;
}