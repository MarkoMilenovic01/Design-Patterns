#include<iostream>
#include<unordered_map>

class Banka;

class Racun {
protected:
	std::string brRacuna; 
	float trenStanje;
	Banka* banka;
public:
	float dajTrenutnoStanje() const { return trenStanje; }
	std::string dajBrojRacuna() const { return brRacuna; }
	Racun(Banka* banka, const std::string& brRacuna, int trenStanje);

	virtual void izvrsiTransackiju(const std::string& naRacun, int iznos) = 0;
	void primiUplatu(std::string saRacuna, float iznos) {
		std::cout << "Sa racuna " << saRacuna << " uplaceno " << iznos << " " << this->valuta() << "\n";
		trenStanje += iznos; 
	}
	virtual std::string valuta() const = 0;
};

class Banka {
	std::unordered_map<std::string, Racun*> racuni;
	std::vector<std::string> blokiraniRacuni;
public:
	virtual float provizija(float iznos) const = 0; // Banka je apstraktna klasa zbog postojanja bar jedne
	// čisto virtuelne metode. To što metoda provizija nije implementirana u klasi Banka, ne znači da tu metodu
	// ne možemo da koristimo unutar klase! To je jako bitna osobina dinamičkog polimorfizma. Kada iz klase banka
	// pozovemo provizija (ili tačnije this->provizija()), pozvaće se ona implementacija koja odgovara tipu na 
	// koji this zaista ukazuje! 
	// Ovu osobinu OOP-a i dinamičkog polimorfizma koristićemo najviše kod projektnog uzorka šablonski metod.

	void dodajRacun(Racun* r) {
		racuni[r->dajBrojRacuna()] = r;
	}
	float izvrsiTransackiju(std::string saRacuna, std::string naRacun, int iznos) {
		for (int i = 0; i < blokiraniRacuni.size(); i++)
			if (blokiraniRacuni[i] == saRacuna) return 0;
		if (racuni.find(saRacuna) == racuni.end() || racuni.find(naRacun) == racuni.end()) return 0;
		Racun* sa = racuni[saRacuna];
		Racun* na = racuni[naRacun];
		float iznosUplate;
		if (sa->valuta() == na->valuta()) {
			iznosUplate = iznos;
		}
		else {
			if (sa->valuta() == "din") {
				// treba da dinarima kupimo evre, a promenljiva iznos tretira se kao
				// iznos u dinarima
				iznosUplate = iznos / 119.0f;
			}
			else {
				iznosUplate = iznos * 117;
			}
		}
		na->primiUplatu(saRacuna, iznosUplate);
		float ukupniIznos = iznos + provizija(iznos);  // ovde pozivamo metodu provizija. Kako je ovo apstraktna klasa,
		// sigurni smo da će this ukazivati na objekat neke izvedene, konkretne, klase (jer inače ne bi objekat
		// ni postojao) tako da će biti pozvana ona implementacija koja odgovara tipu datog objekta.
		if (ukupniIznos <= sa->dajTrenutnoStanje())
			return ukupniIznos;
		else 
			return 0;
	}
};

class BankaA : public Banka {
public:
	// Napomena 1: da primer učinimo zanimljivijim mogli smo da dodamo složeniju logiku obračunavanja
	// provizije kod neke banke. Recimo, ako je iznos u jednom opsegu jedna provizija, a u nekom drugom druga itd.

	// Napomena 2: Obično nije dobar stil programiranja da imamo nove klase samo zbog jedne različite metode,
	// koja čak i nije suštinski bitna za datu klasu (za banku je najbitnije da ima račune i da ume da izvršava
	// transakcije). Za ovu namenu, bolje bi bilo imati zasebnu klasu koja računa samo proviziju, tzv. strategiju
	// obračuna, a da svaka banka ima svoju strategiju i prepušta strategiji obračun provizije.
	// Ovde je odrađeno nasleđivanje pre svega da bismo demonstrirali da možemo imati više tipova posrednika (banaka),
	// kao i više tipova kolega (računa).
	float provizija(float iznos) const override { return 0.01 * iznos; }
};

class BankaB : public Banka {
public:
	float provizija(float iznos) const override { return 0.02 * iznos; }
};

class DinRacun : public Racun {
public:
	using Racun::Racun;
	std::string valuta() const override { return "din"; }
	void izvrsiTransackiju(const std::string& naRacun, int iznos) {
		float ukupniIznos = banka->izvrsiTransackiju(brRacuna, naRacun, iznos);
		if (ukupniIznos == 0) {
			std::cout << "Neuspesni pokusaj transfera " << iznos << " dinara na racun " << naRacun << "\n";
		}
		else {
			std::cout << "Uspesno prebaceno " << iznos << " dinara na racun " << naRacun << "\n";
			std::cout << "Sa računa umanjen iznos od " << ukupniIznos << " " << this->valuta() << std::endl;
			trenStanje -= ukupniIznos;
		}
	}
};

class EurRacun : public Racun {
public:
	using Racun::Racun;
	std::string valuta() const override { return "eur"; }
	void izvrsiTransackiju(const std::string& naRacun, int iznos) {
		float ukupniIznos = banka->izvrsiTransackiju(brRacuna, naRacun, iznos);
		if (ukupniIznos == 0) {
			std::cout << "Neuspesni pokusaj transfera " << iznos << " evra na racun " << naRacun << "\n";
		}
		else {
			std::cout << "Uspesno prebaceno " << iznos << " evra na racun " << naRacun << "\n";
			std::cout << "Sa računa umanjen iznos od " << ukupniIznos << " " << this->valuta() << std::endl;
			trenStanje -= ukupniIznos;
		}
	}
};

Racun::Racun(Banka* banka, const std::string& brRacuna, int trenStanje) :
	banka(banka), brRacuna(brRacuna), trenStanje(trenStanje) {
	banka->dodajRacun(this); // Čim kreiramo račun (a znajući za koju je banku), 
	// povezujemo ga sa datom bankom. Loše bi bilo da nezavisno od kreiranja računa mi
	// možemo, a ne moramo, da povežemo račun sa bankom. Ovo je jedan način da budemo sigurni
	// da će računi biti ispravno vezani. 
	
	// Još bolja opcija je da banke budu ujedno i fabrike računa i da banke kreiraju račune postavljajući
	// sebe kao banku za račun koji su kreirale i dodajući račun u svoju internu listu.
	// Tada bi banke mogle da kreiraju račune na različite načine dajući specifične identifikatore (kako
	// se inače u bankama i radi - prve tri cifre računa su id banke).
}

int main() {
	Banka* b = new BankaA();
	Racun* r1 = new DinRacun(b, "123", 1241);
	Racun* r2 = new EurRacun(b, "321", 241);
	r2->izvrsiTransackiju("123", 100);
	return 0;
}
