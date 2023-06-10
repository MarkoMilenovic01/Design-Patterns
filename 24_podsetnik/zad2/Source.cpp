#include<iostream>
#include<vector>
#include<tuple>

using namespace std;

class Slika {
	int sirina;
	int visina;
	vector<vector<vector<int>>> pikseli; // 3D niz, tj. 3 sirina x visina matrice piksela
	string putanja;
public:
	Slika(const string& putanja) {
		// recimo da ovde zaista učitamo sliku i da tom prilikom utvrdimo njene dimenzije.
		// Ovde ćemo zadati fiksno dimenzije i popuniti nasumičnim vrednostima.
		this->sirina = 1024;
		this->visina = 768;
		this->putanja = putanja;
		pikseli = vector<vector<vector<int>>>(3);
		for (int kanal = 0; kanal < 3; kanal++) {
			pikseli[kanal] = vector<vector<int>>(768);
			for (int i = 0; i < visina; i++) {
				pikseli[kanal][i] = vector<int>(1024);
				for (int j = 0; j < sirina; j++) {
					pikseli[kanal][i][j] = rand() % 256;
				}
			}
		}
	}
	string dajPutanju() const { return putanja; }
};

class Podsetnik;

class Dokument {
	string naslov;
	vector<tuple<Slika, int>> slike;
	string tekst;
public:
	Dokument(const string& naslov) : naslov(naslov){};
	void postaviSadrzaj(const string& sadrzaj) { this->tekst = sadrzaj; }
	void dodajSliku(Slika* slika, int pozicija) { this->slike.push_back(tuple<Slika, int>(*slika, pozicija)); }
	friend class Podsetnik; // Podsetnik moze da pristupa i privatnim/zasticenim atributima Dokumenta

	// Implementacije naredne dve metode date su nakon implementacije klase Podsetnik
	Podsetnik* kreirajPodsetnik() const;
	void rekonstruisiDokument(Podsetnik*);
	void opis() const {
		std::cout << naslov << std::endl;
		std::cout << tekst << std::endl;
		std::cout << "slike: " << std::endl;
		for (int i = 0; i < slike.size(); i++) {
			cout << "slika " << get<0>(slike[i]).dajPutanju() << " na poziciji " << get<1>(slike[i]) << "\n";
		}
	}
};

class Podsetnik {
private:
	vector<tuple<string, int>> slike;
	string tekst;
	string naslov;
public:
	Podsetnik(const Dokument* dokument) { // mozemo i ovako da kreiramo podsetnik (setite se da
		// smo klasu Podsetnik proglasili za prijatelja klase Dokuemnt!)
		this->tekst = dokument->tekst;
		this->naslov = dokument->naslov;
		for (int i = 0; i < dokument->slike.size(); i++)
			this->slike.push_back(tuple<string, int>(
				get<0>(dokument->slike[i]).dajPutanju(),
				get<1>(dokument->slike[i])
				)
			);
		/* Bitna napomena:
		Možda ste se do sada upitali, čitajući o projektnom uzorku i razmatrajući prvi
		zadatak (i verovatno sve primere dostupne na internetu), šta
		će nam zasebna klasa Podsetnik koja od atributa ima sve ono što ima i klasa
		Subjekt? Zašto ne bismo prosto čuvali kopije samog objekata Subjekt?
		operator=(const Subjekt& drugi) bi onda igrao ulogu metode vratiStaroStanje(),
		a kreiranje podsetnika vršili bismo konstruktorom kopije. Ukoliko atribute na taj
		način držimo u klasi Subjekat, problem sa (ne)narušavanjem inkapsulacije je rešen.

		Odgovor delimično leži u činjenici da bismo u složenijoj aplikaciji mogli imati
		izvedene klase subjekta i komplikovaniju komunikaciju kod koje je teško pozvati
		odgovarajuće operatore dodele vrednosti i konstruktor kopije.
		Drugi odgovor leži u činjenici da Podsetnici treba da budu što "lakši" objekti.
		Objekti koji će (uz možda dodatno izračunavanje) omogućiti rekonstrukciju objekta.
		Podsetnici bi trebalo da čuvaju minimum dovoljnih informacija za vraćanje u neko
		željeno prethodno stanje.
		Ovde to ilustrujemo na način da nećemo u podsetnicima čuvati RGB matrice slika,
		već samo njihove putanje.
		*/
	}
	friend class Dokument; // možemo i ovde iskoristiti pogodnosti prijateljskih klasa u C++-u.
};

void Dokument::rekonstruisiDokument(Podsetnik* p) {
	// Ovde ćemo iskoristiti činjenicu da smo Dokument proglasili prijateljem Podsetnika.
	this->tekst = p->tekst;
	this->naslov = p->naslov;
	this->slike.clear();

	int n = p->slike.size();
	for (int i = 0; i < n; i++) {
		this->slike.push_back(tuple<Slika, int>(Slika(get<0>(p->slike[i])), get<1>(p->slike[i])));
	}
}

Podsetnik* Dokument::kreirajPodsetnik() const {
	return new Podsetnik(this);
}


class Cuvar {
	Dokument* d; // Cuvar će imati pokazivač na Dokument i automatski ćemo kreirati i čuvati
	// podsetnik kada god pozovemo metodu snimi(). Slično tome, vršimo automatski pristup podsetnicima
	// i vraćanje stanja pozivom metode vratiPrethodnoStanje 
	vector<Podsetnik*> podsetnici;
public:
	Cuvar(Dokument* d) : d(d) {}
	void snimi() { podsetnici.push_back(d->kreirajPodsetnik()); }
	void vratiPrethodnoStanje(int indeks = -1) {
		// Omogućićemo vraćanje na bilo koje prethodno stanje. 
		// Za nenegativni indeks i pristupićemo i-tom mestu u vektoru podsetnici.
		// Za negativne indekse pristupaćemo podsetnicima sa kraja (-1 je poslednji podsetnik,
		// -2 pretposlednji itd). 
		if (indeks < 0) indeks = podsetnici.size() + indeks;
		if (indeks < 0 || indeks >= podsetnici.size()) throw runtime_error("Ne postoji podsetnik za prosledjeni indeks!");
		d->rekonstruisiDokument(podsetnici[indeks]);
	}
	~Cuvar() {
		for (int i = 0; i < podsetnici.size(); i++) {
			delete podsetnici[i];
		}
	}
};

int main() {
	Dokument dokument("test");
	dokument.postaviSadrzaj("Neki test sadrzaj\n");
	Cuvar cuvar(&dokument);
	cuvar.snimi();
	dokument.dodajSliku(new Slika("images/test.jpg"), 0);
	cuvar.snimi();
	dokument.dodajSliku(new Slika("images/test2.jpg"), 10);
	cuvar.snimi();
	dokument.postaviSadrzaj("Novi sadrzaj!");
	cuvar.snimi();

	for (int i = 0; i < 4; i++) {
		cuvar.vratiPrethodnoStanje(i);
		dokument.opis();
		cout << "\n\n";
	}
}