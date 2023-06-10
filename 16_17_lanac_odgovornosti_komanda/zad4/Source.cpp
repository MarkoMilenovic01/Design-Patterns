#include<iostream>
#include<fstream>
#include<sstream>
#include<queue>
#include<stack>

class Dokument {
private:
	std::string sadrzaj;
	std::string ime_fajla;
public:
	Dokument(const std::string& ime_fajla): ime_fajla(ime_fajla) {
		std::ifstream fajl(ime_fajla);
		if (fajl.is_open()) { // Ako fajl već postoji, pročitaćemo ga.
			std::stringstream stream;
			stream << fajl.rdbuf();
			sadrzaj = stream.str();
			fajl.close();
		}
		else 
			sadrzaj = "";
	}
	void snimi() {
		std::ofstream fajl(ime_fajla);
		fajl << sadrzaj;
		fajl.close();
	}
	void umetniZnak(char c, int pos) {
		sadrzaj.insert(pos, 1, c);
	}
	void dodajZnak(char c) {
		sadrzaj += c;
	}
	std::string uzmiSadrzaj() const { return sadrzaj; }
	void postaviSadrzaj(std::string sadrzaj) { this->sadrzaj = sadrzaj; }
};

class Komanda {
protected:
	Dokument* d;
public:
	Komanda(Dokument* d): d(d) {}
	virtual void izvrsi() = 0;
	virtual void vrati() = 0; // omogućićemo i undo, tj. poništenje date komande, tj. vraćanje...
	virtual ~Komanda() {}
};

class Snimi : public Komanda {
public:
	using Komanda::Komanda;
	void izvrsi() override { d->snimi(); }
	void vrati() override { }; // ovde undo neće moći da radi ništa 
};

class DodajKarakter : public Komanda {
private:
	char c;
public:
	DodajKarakter(Dokument* d, char c): Komanda(d), c(c) {}
	void izvrsi() override { d->dodajZnak(c); }
	void vrati() override {
		std::string sadrzaj = d->uzmiSadrzaj();
		sadrzaj = sadrzaj.substr(0, sadrzaj.size() - 1);
		d->postaviSadrzaj(sadrzaj);
	}
};

class UmetniKarakter : public Komanda {
private:
	char c;
	int pos;
public:
	UmetniKarakter(Dokument* d, char c, int pos) : Komanda(d), c(c), pos(pos) {}
	void izvrsi() override { d->umetniZnak(c, pos); }
	void vrati() override {
		// Na baš najefikasniji način da iz stringa izbacimo jedan karakter, ali efikasnost nam ovde nije u fokusu.
		std::string sadrzaj = d->uzmiSadrzaj();
		sadrzaj = sadrzaj.substr(0, pos) + sadrzaj.substr(pos + 1);
		d->postaviSadrzaj(sadrzaj);
	}
};

class Pokretac { // Invoker
private:
	std::queue<Komanda*> komande; // Zgodna struktura za smeštanje komandi koje tek treba pokrenuti je queue.
	// Dodajemo komande i izvršavamo ih kasnije u redosledu dodavanja... 

	// Da omogućimo undo i redo koristićemo dva steka. Kada god izvršimo neku komandu dodaćemo je na undoStek,
	// koji će nam služiti da uz pomoć njega implementiramo undo operaciju. Takođe, kad god pozovemo redo potrebno 
	// je da dodamo komandu u undo stek. 
	// U redoStek dodajemo komandu kad god pozovemo undo. Obično je ponašanje redo operacije takvo da se ona 
	// onemogućuje ukoliko nakon undo izvršimo neku potpuno novu komandu.
	std::stack<Komanda*> undoStek;
	std::stack<Komanda*> redoStek;
public:
	void dodajKomandu(Komanda* komanda) {
		komande.push(komanda);
	}
	void izvrsiKomandu(Komanda* komanda) {
		komanda->izvrsi();
		undoStek.push(komanda);
		while (redoStek.size() > 0) { redoStek.pop(); } // redo može da se izvrši nakon redo ili nakon undo, ali ne
		// i nakon "običnog" izvršenja neke komande
	}
	void izvrsiKomande() {
		while (komande.size() > 0) {
			Komanda* k = komande.front();
			komande.pop();
			izvrsiKomandu(k);
		}
	}
	void undo() {
		if (undoStek.size() > 0) {
			Komanda* k = undoStek.top();
			undoStek.pop();
			k->vrati();
			redoStek.push(k);
		}
	}
	void redo() {
		if (redoStek.size() > 0) {
			Komanda* k = redoStek.top();
			redoStek.pop();
			k->izvrsi();
			undoStek.push(k);
		}
	}
};

int main() {
	Dokument* d = new Dokument("test.txt");
	Pokretac p;
	p.dodajKomandu(new DodajKarakter(d, 'a'));
	p.dodajKomandu(new DodajKarakter(d, 'b'));
	p.dodajKomandu(new DodajKarakter(d, 'd'));
	p.dodajKomandu(new DodajKarakter(d, 'e'));
	p.dodajKomandu(new DodajKarakter(d, 'f'));
	p.dodajKomandu(new UmetniKarakter(d, 'c', 2));
	p.izvrsiKomande();
	p.izvrsiKomandu(new Snimi(d));
	std::cout << "Sadrzaj dokumenta: " << d->uzmiSadrzaj() << std::endl;

	for (int i = 0; i < 7; i++) {
		p.undo();
		std::cout << "Sadrzaj dokumenta: " << d->uzmiSadrzaj() << std::endl;
	}
	return 0;
}


