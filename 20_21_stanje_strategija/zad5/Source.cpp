#include<iostream>
#include<string>
#include<fstream>

class Osoba;

class EksportStrategija {
public:
	virtual void eksportuj(Osoba*) = 0;
	virtual ~EksportStrategija() {}
};

class Osoba {
	std::string ime;
	std::string prezime;
	int godRodjenja;
	EksportStrategija* eksportStrategija;
public:
	Osoba(const std::string& ime, const std::string prezime,
		int godRodjenja);
	void postaviEksportStrategiju(EksportStrategija* s) {
		this->eksportStrategija = s;
	}
	std::string dajIme() const { return ime; }
	std::string dajPrezime() const { return prezime; }
	int dajGodinuRodjenja() const { return godRodjenja; }
	void eksportuj() { this->eksportStrategija->eksportuj(this); }
};

class CsvEksport : public EksportStrategija {
public:
	void eksportuj(Osoba* o) {
		std::string ime_fajla = o->dajIme() + "_" + o->dajPrezime() + \
			"_" + std::to_string(o->dajGodinuRodjenja()) + ".csv";
		std::ofstream fajl(ime_fajla);
		fajl << "ime, prezime, godRodjenja\n" << o->dajIme() << ", ";
		fajl << o->dajPrezime() << ", " << o->dajGodinuRodjenja() << "\n";
		fajl.close();
	}
};

class JsonEksport : public EksportStrategija {
public:
	void eksportuj(Osoba* o) {
		std::string ime_fajla = o->dajIme() + "_" + o->dajPrezime() + \
			"_" + std::to_string(o->dajGodinuRodjenja()) + ".json";
		std::ofstream fajl(ime_fajla);
		fajl << "{\n\t\"ime\": \"" << o->dajIme() << "\",\n\t\"prezime\": \"";
		fajl << o->dajPrezime() << "\",\n\t\"godRodjenja\": " << o->dajGodinuRodjenja() << "\n}\n";
		fajl.close();
	}
};

class XMLEksport : public EksportStrategija {
public:
	void eksportuj(Osoba* o) {
		std::string ime_fajla = o->dajIme() + "_" + o->dajPrezime() + \
			"_" + std::to_string(o->dajGodinuRodjenja()) + ".xml";
		std::ofstream fajl(ime_fajla);
		fajl << "<xml encoding=\"utf-8\" version=\"1.0\">\n\t";
		fajl << "<ime>" << o->dajIme() << "</ime>\n\t<prezime>";
		fajl << o->dajPrezime() << "</prezime>\n\t<godRodjenja>" << o->dajGodinuRodjenja();
		fajl << "</godRodjenja>\n</xml>\n";
		fajl.close();
	}
};

Osoba::Osoba(const std::string& ime, const std::string prezime,
	int godRodjenja) : ime(ime), prezime(prezime),
	godRodjenja(godRodjenja) {
	eksportStrategija = new CsvEksport();
}

int main() {
	Osoba* o = new Osoba("Pera", "Peric", 1992);
	o->eksportuj();
	o->postaviEksportStrategiju(new JsonEksport());
	o->eksportuj();
	o->postaviEksportStrategiju(new XMLEksport());
	o->eksportuj();
	delete o;
}