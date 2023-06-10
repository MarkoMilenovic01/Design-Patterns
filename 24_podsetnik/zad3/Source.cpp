#include<iostream>
#include<vector>
#include<string>
#include<fstream>

enum class TipPolja {SLOBODNO, CILJ, ZID};
enum class SmerKretanja {LEVO, DESNO, GORE, DOLE};

std::ostream& operator << (std::ostream& s, TipPolja polje) {
	switch (polje) {
	case TipPolja::SLOBODNO:
		s << " ";
		break;
	case TipPolja::ZID:
		s << "#";
		break;
	default:
		s << "x";
	}
	return s;
}

class Podsetnik;

class Igra {
	std::vector<std::vector<TipPolja>> polja;
	int tren_pozicija_i;
	int tren_pozicija_j;
	int nivo;
	int n, m; // matrica polja biće formata n x m

	void ucitajMapu(std::string putanja) {
		std::ifstream mapa_fajl(putanja);
		mapa_fajl >> n;
		mapa_fajl >> m;
		polja = std::vector<std::vector<TipPolja>>(n);
		for (int i = 0; i < n; i++) {
			polja[i] = std::vector<TipPolja>(m);
			for (int j = 0; j < m; j++) {
				char polje;
				mapa_fajl >> polje;
				switch (polje) {
				case '0': // recimo da nam nule znače slobodna polja, a jedinice zauzeta, tj. prepreke/zidove
					polja[i][j] = TipPolja::SLOBODNO;
					break;
				case '1':
					polja[i][j] = TipPolja::ZID;
					break;
				case 'x':
					polja[i][j] = TipPolja::CILJ;
					break;
				default:
					throw std::runtime_error("Problem prilikom ucitavanja mape!");
				}
			}
		}
	}
public:
	Igra(int nivo): nivo(nivo) {
		tren_pozicija_i = 0;
		tren_pozicija_j = 0; // Recimo da uvek krece od polja 0, 0 
		// U folderu su ručno kreirani i dodati fajlovi 1.txt, 2.txt i 3.txt.
		std::string ime_fajla = std::to_string(nivo) + ".txt";
		ucitajMapu(ime_fajla);
	}
	void postaviPoziciju(int i, int j) {
		this->tren_pozicija_i = i;
		this->tren_pozicija_j = j;
	}

	void pomeriIgraca(SmerKretanja smer) {
		int novo_i = tren_pozicija_i, 
			novo_j = tren_pozicija_j;

		switch (smer) {
		case SmerKretanja::LEVO:
			novo_j--;
			break;
		case SmerKretanja::DESNO:
			novo_j++;
			break;
		case SmerKretanja::GORE:
			novo_i--;
			break;
		default:
			novo_i++;
		}
		if (novo_i >= 0 && novo_i < n && novo_j >= 0 && novo_j < m && polja[novo_i][novo_j] != TipPolja::ZID) {
			tren_pozicija_i = novo_i;
			tren_pozicija_j = novo_j;
		}
	}

	void stampa() {
		std::cout << "..........................................\n";
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (i == tren_pozicija_i && j == tren_pozicija_j) {
					std::cout << "I ";
				}
				else std::cout << polja[i][j];
			}
			std::cout << "\n";
		}
		std::cout << "..........................................\n\n";
	}

	const Podsetnik* kreirajPodsetnik() const;
	void rekonstruisiStanje(const Podsetnik*);
};

class Podsetnik {
	// Kao i u prethodnom primeru, Podsetnik neće čuvati sve atribute klase Igra, već
	// samo one od značaja za rekonstrukciju igre. 
private:
	int nivo;
	int pozicija_i;
	int	pozicija_j;
public:
	Podsetnik(int nivo, int i, int j): nivo(nivo), pozicija_i(i), pozicija_j(j) {}
	int dajI() const { return pozicija_i; }
	int dajJ() const { return pozicija_j; }
	int dajNivo() const { return nivo; }
};

const Podsetnik* Igra::kreirajPodsetnik() const {
	return new Podsetnik(nivo, tren_pozicija_i, tren_pozicija_j);
}

void Igra::rekonstruisiStanje(const Podsetnik* p) {
	this->nivo = p->dajNivo();
	this->tren_pozicija_i = p->dajI();
	this->tren_pozicija_j = p->dajJ();
	std::string ime_fajla = std::to_string(nivo) + ".txt";
	ucitajMapu(ime_fajla);
}

class Cuvar {
public:
	// Ovde je primenjena malo drugačija ideja organizacije klase Čuvar.
	// Naime, u memoriji (RAM memoriji) nećemo trajno čuvati podsetnike, već
	// će Čuvar biti zadužen za njihovo snimanje u fajl i učitavanje iz fajla. 
	void snimi(Igra* igra, std::string ime) { // ime je ime preko koga identifikujemo podsetnik,
		// tj. fajl u kome smo sačuvali relevantne informacije.
		const Podsetnik* p = igra->kreirajPodsetnik();
		ime = ime.find(".txt") == -1 ? ime + ".txt" : ime;
		std::ofstream fajl(ime);
		fajl << p->dajNivo() << " " << p->dajI() << " " << p->dajJ() << "\n";
		fajl.close();
		delete p;
	}
	void ucitajRanijuIgru(Igra* igra, std::string ime) {
		int i, j, nivo;
		ime = ime.find(".txt") == -1 ? ime + ".txt" : ime;
		std::ifstream fajl(ime);
		fajl >> nivo >> i >> j;
		Podsetnik p(nivo, i, j);
		igra->rekonstruisiStanje(&p);
		fajl.close();
	}
};

int main() {
	Cuvar cuvar;
	Igra igra(1);
	std::cout << "Za novu igru pritisnite 0, za ucitavanje stare 1\n";
	int izbor;
	std::cin >> izbor;

	/*
	Uz malo truda (koji nema veze sa projektnim uzorkom) u narednom kodu mozete implementirati
	složeniju logiku, sa beskonacnim petljama, ocitavanjem tastera "strelica" sa tastature, 
	iscrtavanjem mape posle svakog koraka i dodatnom mogućnošću da u svakom koraku prekinete igru,
	snimite ili pokrenete igru iz nekog snimljenog "podsetnika". 

	Ideje za dalje unapređenje (osim interaktivne main funkcije):
	Dodati veće mape i drugog igrača na drugom uglu mape. 
	Igrači bi mogli imati različite strategije za poteze. Različite automatizovane
	strategije, kao i strategiju koja traži unos sa konzole. Igra može tada biti
	medijator pri čemu igra saopštava igračima da li su nakon poteza pobedili/izgubili,
	dok igrači igri saopštavaju svoj smer kretanja u svakom potezu. 
	*/
	if (izbor == 0) {
		igra.pomeriIgraca(SmerKretanja::DESNO);
		igra.pomeriIgraca(SmerKretanja::DESNO);
		cuvar.snimi(&igra, "s1");
		igra.stampa();
		igra.pomeriIgraca(SmerKretanja::DESNO);
		igra.pomeriIgraca(SmerKretanja::DESNO);
		igra.pomeriIgraca(SmerKretanja::DOLE);
		igra.pomeriIgraca(SmerKretanja::DOLE);
		cuvar.snimi(&igra, "s2");
		igra.stampa();
	}
	else {
		std::cout << "Unesite ime fajla u kome je sacuvano stanje igre (sa ili bez .txt ekstenzije)\n";
		std::string ime;
		std::cin >> ime;
		cuvar.ucitajRanijuIgru(&igra, ime);
		igra.stampa();
	}
	return 0;
}