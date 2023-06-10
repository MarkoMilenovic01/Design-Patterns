#include<iostream>
#include<vector>
/*
Napisati klase Igrač i Igra za simulaciju neke video igre. 
Igra ima matricu polja i listu igrača. Na neko polje matrice može pasti prepreka.
Igrač se nalazi na određenom polju. Igrač ima svoju verziju matrice polja i svako polje može
biti neistraženo, čisto, zauzeto igračem ili prepreka. 
Kada na polje padne prepreka, svi igrači udaljeni jedno polje od datog polja treba da 
ažuriraju svoju veziju matrice. Postoji i brojač živih igrača (kao zasebna klasa).
Kada god se doda novi igrač brojač treba povećati za 1, a ukoliko prepreka padne na igrača, 
datog igrača treba ukloniti iz igre (i smanjiti brojač).
*/

class Igra;

enum class TipPolja { Igrac, Neistrazeno, Cisto, Prepreka };

// dimenzije matrice polja (mape) biće fiksne i unapred date
#define N 10 
#define M 20

class Posmatrac {
public:
	virtual void azuriraj(Igra*) = 0;

	//Kasnije u kodu biće nam potrebno da znamo da li pokazivač tipa Posmatrac pokazuje na objekat
	// tipa igrač ili ne. Opredelili smo se za prosto (ne baš najbolje) rešenje da svaka izvedena klasa
	// zna da li klasa igrač ili ne (samo ćemo u klasi igrač preklopiti metodu i vraćati true).
	// Biće nam potrebno da znamo da li je u pitanju klasa igrač da bismo izveli bezbednu konverziju
	// pokazivača osnovnog tipa u pokazivač izvedenog tipa. Bolji pristup je korišćenjem 
	// dynamic_cast<Igrac*>(pokazivac_tipa_posmatrac) i ispitivanjem da li je rezultat jednak null. 
	virtual bool jeIgrac() const { return false; }
	virtual ~Posmatrac() {}
};

// Da li je polje (i, j) validno u matrici n x m.
bool validnoPolje(int i, int j, int n, int m) {
	return i >= 0 && i < n && j >= 0 && j < m;
}

class Igrac : public Posmatrac {
	std::vector<std::vector<TipPolja>> mapa;
	int i, j; // pozicija igraca
public:
	Igrac(int i, int j) : i(i), j(j) {
		mapa = std::vector<std::vector<TipPolja>>(N);
		for (int i = 0; i < N; i++)
			mapa[i] = std::vector<TipPolja>(M);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				mapa[i][j] = TipPolja::Neistrazeno;
			}
		}
	}
	int pozicijaI() const { return i; }
	int pozicijaJ() const { return j; }
	bool jeIgrac() const override { return true; }
	void azuriraj(Igra* igra); // ovde je potrebno koristiti klasu Igra, a nismo je još uvek definisali,
	// tako da ćemo implementaciju ove metode dati kasnije...
};

class BrojacIgraca : public Posmatrac {
	int brIgraca = 0;
public:
	void azuriraj(Igra* igra); // ista stvar kao u slučaju igrača... implementacija je data nakon
	// definicije klase Igra.
};

class Igra {
	std::vector<std::vector<TipPolja>> mapa;
	std::vector<Posmatrac*> posmatraci;
public:
	Igra() {
		mapa = std::vector<std::vector<TipPolja>>(N);
		for (int i = 0; i < N; i++)
			mapa[i] = std::vector<TipPolja>(M);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				mapa[i][j] = TipPolja::Cisto;
			}
		}
		// Od posmatrača dodavaćemo uglavnom igrače, pa Igra ima metodu dodajIgrača.
		// Da bismo imali kao posmatrača i brojač igrača jedno od rešenja je da u
		// konstruktoru fiksno dodamo brojač igrača. 
		// Alternativa je da imamo metodu dodajPosmatrača, a da zatim proverimo da li je
		// dodati posmatrač zapravo igrač. Ako jeste, da odradimo logiku koja se tiče dodavanja
		// igrača, a da inače samo dodamo u listu posmatrača.
		posmatraci.push_back(new BrojacIgraca());
	}

	void dodajIgraca(Igrac* igrac) {
		posmatraci.push_back(igrac);
		mapa[igrac->pozicijaI()][igrac->pozicijaJ()] = TipPolja::Igrac; // zauzimamo polje.
		// Napomena: ovo je uprošćena ilustracija korišćenja posmatrača. Nismo gubili vreme
		// na proveru da li je dato polje već zauzeto...
		obavestiPosmatrace();
	}
	void obavestiPosmatrace() {
		for (auto posmatrac : posmatraci)
			posmatrac->azuriraj(this);
	}
	void postaviPrepreku(int i, int j) {
		if (mapa[i][j] == TipPolja::Igrac) {
			for (auto it = posmatraci.begin(); it != posmatraci.end(); it++) {
				if ((*it)->jeIgrac()) {
					Igrac* tmp = (Igrac*)(*it); // generalno opasna konverzija, ali mi smo sigurni
					// da je u pitanju zaista objekat tipa Igrac.
					// Konverzija je neophodna jer samo igrači imaju i, j poziciju.
					if (tmp->pozicijaI() == i && tmp->pozicijaJ() == j) {
						// Ako je na datoj poziciji igrač, brišemo ga iz igre.
						posmatraci.erase(it);
						break;
						
					}
				}
			}
		}
		mapa[i][j] = TipPolja::Prepreka;
		obavestiPosmatrace();
	}
	~Igra() {
		for (int i = 0; i < posmatraci.size(); i++)
			delete posmatraci[i];
		posmatraci.clear();
	}
	std::vector<Posmatrac*> dajPosmatrace() const { return posmatraci; }
	std::vector<std::vector<TipPolja>> dajMapu() const { return mapa; }
};

void Igrac::azuriraj(Igra* igra) {
	// Igrac treba da azurira svoju verziju mape, ali samo može da vidi polja mape koja
	// su susedna u odnosu na trenutnu poziciju.
	// Da u odnosu na trenutno polje ispitamo sva susedna polja čest pristup je preko niza
	// pomeraja. Da se pomerimo potreban je pomeraj po i i pomeraj po j, pa ćemo koristiti
	// dva niza od po onoliko elemenata koliko potencijalnih susednih polja imamo. 
	// na k-tom mestu u oba niza nalaze se takvi pomeraji da bi nas pomeranjem sa trenutne
	// pozicije za te pomeraje doveli do k-tog suseda.

	int brSusednihPolja = 8;
	auto mapaIgre = igra->dajMapu();

	int pomerajI[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int pomerajJ[] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	for (int pomeraj = 0; pomeraj < brSusednihPolja; pomeraj++) {
		int mapa_i = i + pomerajI[pomeraj];
		int mapa_j = j + pomerajJ[pomeraj];
		if (validnoPolje(mapa_i, mapa_j, N, M))
			mapa[mapa_i][mapa_j] = mapaIgre[mapa_i][mapa_j];
	}
}

void BrojacIgraca::azuriraj(Igra* igra) {
	std::vector<Posmatrac*> posmatraci = igra->dajPosmatrace();
	brIgraca = 0;
	for (int i = 0; i < posmatraci.size(); i++) {
		if (posmatraci[i]->jeIgrac())
			brIgraca++;
	}
	std::cout << "Broj igraca: " << brIgraca << std::endl;
}


int main() {
	Igra igra;
	igra.dodajIgraca(new Igrac(0, 0));
	igra.dodajIgraca(new Igrac(2, 5));
	igra.dodajIgraca(new Igrac(1, 1));
	igra.postaviPrepreku(1, 1);
	return 0;
}