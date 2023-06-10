#include<iostream>
#include<vector>
#include<tuple>

class GrafickiElement {
public:
	virtual void iscrtaj(int x, int y, int ugao) = 0;
	virtual ~GrafickiElement() {}
};

class Drvo : public GrafickiElement {
protected:
	std::string putanjaDoTeksture;
public:
	Drvo(const std::string& putanja): putanjaDoTeksture(putanja) {}
};

class Hrast : public Drvo {
public:
	Hrast() : Drvo("models/hrast.obj") {}
	void iscrtaj(int x, int y, int ugao) override {
		std::cout << "Crtam hrast " << putanjaDoTeksture << " [" << x << ", " << y << "], ugao = " << ugao << std::endl;
	}
};

class Smrca : public Drvo {
public:
	Smrca() : Drvo("models/smrca.obj") {}
	void iscrtaj(int x, int y, int ugao) override {
		std::cout << "Crtam smrcu " << putanjaDoTeksture << " [" << x << ", " << y << "], ugao = " << ugao << std::endl;
	}
};

class Zivotinja : public GrafickiElement {
protected:
	std::string putanjaDoTeksture;
public:
	Zivotinja(const std::string& putanja) : putanjaDoTeksture(putanja) {}
};

class Medved : public Zivotinja {
public:
	Medved() : Zivotinja("models/medved.obj") {}
	void iscrtaj(int x, int y, int ugao) override {
		std::cout << "Crtam medveda " << putanjaDoTeksture << " [" << x << ", " << y << "], ugao = " << ugao << std::endl;
	}
};

class Vuk : public Zivotinja {
public:
	Vuk() : Zivotinja("models/vuk.obj") {}
	void iscrtaj(int x, int y, int ugao) override {
		std::cout << "Crtam vuka " << putanjaDoTeksture << " [" << x << ", " << y << "], ugao = " << ugao << std::endl;
	}
};

class Ekosistem { 
	// Ova klasa igraće ulogu fabrike živih organizama, ali ne smemo da "mešamo" razne
	// organizme i zato će biti organizovana kao apstraktna fabrika. Konkretne fabrike kreiraće
	// odgovarajuće organizme (tj. organizovaće njihovo deljenje, jer su to fabrike muva).
protected:
	Ekosistem() {};
	Ekosistem& operator=(const Ekosistem& drugi) = default;
	Ekosistem(const Ekosistem& drugi) = default;
public:
	virtual Zivotinja* dajZivotinju() = 0;
	virtual Drvo* dajDrvo() = 0;
	virtual ~Ekosistem() {}
};

class EkosistemListopadneSume : public Ekosistem {
private:
	EkosistemListopadneSume() = default;
	EkosistemListopadneSume& operator=(const EkosistemListopadneSume& drugi) = default;
	EkosistemListopadneSume(const EkosistemListopadneSume& drugi) = default;

	// Pošto će sve instance klase Medved (isto važi i za Hrast) imati isto interno stanje, 
	// nema potrebe da imamo mapiranje id-jeva ili internih stanja u objekte. Samo jedan objekat
	// klase Medved je dovoljan, pa ćemo te muva-objekte čuvati prosto kao dva pokazivača. 
	Medved* medved = nullptr;
	Hrast* hrast = nullptr;
public:
	static Ekosistem& getInstance() {
		static EkosistemListopadneSume instance;
		return instance;
	}

	Zivotinja* dajZivotinju() override {
		if (!medved) medved = new Medved();
		return medved;
	}

	Drvo* dajDrvo() override {
		if (!hrast) hrast = new Hrast();
		return hrast;
	}

	~EkosistemListopadneSume() {
		delete medved;
		delete hrast;
	}
	
};


class EkosistemCetinarskeSume : public Ekosistem {
private:
	EkosistemCetinarskeSume() = default;
	EkosistemCetinarskeSume& operator=(const EkosistemCetinarskeSume& drugi) = default;
	EkosistemCetinarskeSume(const EkosistemCetinarskeSume& drugi) = default;

	Vuk* vuk = nullptr;
	Smrca* smrca = nullptr;
public:
	static Ekosistem& getInstance() {
		static EkosistemCetinarskeSume instance;
		return instance;
	}

	Zivotinja* dajZivotinju() override {
		if (!vuk) vuk = new Vuk();
		return vuk;
	}

	Drvo* dajDrvo() override {
		if (!smrca) smrca = new Smrca();
		return smrca;
	}

	~EkosistemCetinarskeSume() {
		delete smrca;
		delete vuk;
	}
};

class Suma : public GrafickiElement {
protected:
	int sirina;
	int duzina;
	// Drveće ćemo pamtiti u matrici. Pozicija svakog drveta pamti se implicitno kao
	// indeks reda i kolone u matrici, ali moramo negde zapamtiti i rotaciju (setimo se,
	// x, y i rotacija čine eksterno stanje). Zato ćemo kreirati zasebnu matricu u kojoj će
	// na mestu i, j biti rotacija drveta na mestu i, j. 
	// Kod korišćenja projektnog uzorka muva ovakav tip vođenja računa o eksternom stanju je 
	// skoro uvek neophodan. Lepši način da se čuvanje eksternog stanja postigne je da kreiramo
	// neku wrapper klasu koja će imati kao atribute eksterno stanje i pokazivač na deljeni objekat.
	// Suština uzorka je u deljenju objekata, a to kako ćemo pamtiti eksterno stanje je na klijentu.
	// Da ne bismo komplikovali primer, ovde je eksterno stanje pamćeno u zasebnim vektorima/matricama.
	std::vector<std::vector<Drvo*>> drvece;
	std::vector<std::vector<int>> rotacijaDrveca;
	// Za životinje takođe treba sačuvati eksterno stanje. Kako će životinja biti manje, 
	// čuvaćemo ih u vektoru, a njihovo eksterno stanje čuvaćemo u zasebnom vektoru kao 
	// uređenu trojku x, y, rotacija.
	std::vector<Zivotinja*> zivotinje;
	std::vector<std::tuple<int, int, int>> pozicijaUgaoZivotinje;

public:
	Suma() = default;
	Suma(int duzina, int sirina) : duzina(duzina), sirina(sirina) {
		drvece = std::vector<std::vector<Drvo*>>(duzina);
		rotacijaDrveca = std::vector<std::vector<int>>(duzina);
		for (int i = 0; i < duzina; i++) {
			drvece[i] = std::vector<Drvo*>(sirina);
			rotacijaDrveca[i] = std::vector<int>(sirina);
		}
	}
	void iscrtaj(int x, int y, int ugao) {
		for (int i = 0; i < drvece.size(); i++) {
			for (int j = 0; j < drvece[i].size(); j++) {
				drvece[i][j]->iscrtaj(x + j, y + i, (ugao + rotacijaDrveca[i][j]) % 360);
			}
		}
		for (int i = 0; i < zivotinje.size(); i++) {
			// elemente n-torke (engl. tuple) uzimamo sa std::get<index>(tuple).
			// Potrebno je uključiti biblioteku <tuple>
			// Naravno, kao parametar šablona, indeks mora biti poznat u vreme kompajliranja
			// (ili hardkodovan ili konstantni izraz (constexpr))
			int poz_x = std::get<0>(pozicijaUgaoZivotinje[i]);
			int poz_y = std::get<1>(pozicijaUgaoZivotinje[i]);
			int _ugao = std::get<2>(pozicijaUgaoZivotinje[i]);

			zivotinje[i]->iscrtaj(x + poz_x, y + poz_y, ugao + _ugao);
		}
	}
	/* Bitno: šuma ne sme da briše drveće i životinje u destruktoru! To su deljeni objekti!
	   Time se nismo bavili na času, ali da budemo 100% sigurni mogli bismo da destruktore za 
	   drveće i životinje učinimo privatnim/protected i proglasimo fabrike za prijatelje tih klasa
	   tako da samo fabrike mogu da brišu date objekte. 
	*/
};

class ListopadnaSuma : public Suma {
public:
	typedef EkosistemListopadneSume EkosistemSume; // Ideja ovog typedef-a je da kada god nam
	// treba fabrika mi identifikujemo datu klasu sa EkosistemSume (nebitan detalj). 
	// Sa istim ovakvim typedef-om u klasi CetinarskaSuma postizemo da konstruktori obe sume izgledaju
	// identicno. 
	// Moguće alternativne implementacije kojim bismo izbegli kopiranje istog koda su: 
	// 1. Da imamo pokazivač na fabriku i protected konstruktor u klasi Suma koji uzima kao 
	// argumente duzinu, visinu i pokazivac na fabriku, а da iz izvedenih klasa mi pozivamo taj
	// konstruktor prosleđivajući pokazivač na fabriku. 
	// 2. Da izvedene šume budu šablonske klase sa tipom ekosistema kao parametrom šablona
	// 3. (ružno rešenje) da popunjavanje šume izdvojimo kao zasebnu funkciju (ili statičku metodu)
	// kojoj bismo slali fabriku i ostale relevantne vrednosti. 
	ListopadnaSuma(int duzina, int sirina): Suma(duzina, sirina) {
		
		for (int i = 0; i < duzina; i++) {
			for (int j = 0; j < sirina; j++) {
				drvece[i][j] = EkosistemSume::getInstance().dajDrvo();
				rotacijaDrveca[i][j] = rand() % 360;
			}
		}
		int brZivotinja = duzina * sirina / 5;
		for (int i = 0; i < brZivotinja; i++) {
			zivotinje.push_back(EkosistemSume::getInstance().dajZivotinju());
			pozicijaUgaoZivotinje.push_back(
				std::make_tuple<int, int, int>(rand() % sirina, rand() % duzina, rand() % 360));
		}
	}
};

class CetinarskaSuma : public Suma {
public:
	typedef EkosistemCetinarskeSume EkosistemSume; 
	CetinarskaSuma(int duzina, int sirina) : Suma(duzina, sirina) {

		for (int i = 0; i < duzina; i++) {
			for (int j = 0; j < sirina; j++) {
				drvece[i][j] = EkosistemSume::getInstance().dajDrvo();
				rotacijaDrveca[i][j] = rand() % 360;
			}
		}
		int brZivotinja = duzina * sirina / 5;
		for (int i = 0; i < brZivotinja; i++) {
			zivotinje.push_back(EkosistemSume::getInstance().dajZivotinju());
			pozicijaUgaoZivotinje.push_back(
				std::make_tuple<int, int, int>(rand() % sirina, rand() % duzina, rand() % 360));
		}
	}
};

int main() {
	Suma* suma = new CetinarskaSuma(5, 5);
	suma->iscrtaj(0, 0, 0);
	return 0;
}
