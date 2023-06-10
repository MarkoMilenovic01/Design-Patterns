#include<iostream>
#include<vector>

class TitleFile {
protected:
	//Napomena: u odnosu na rešenje sa časa, izdvojen je naziv fajla kao atribut
	// za TitleFile. Sada parseFile ne uzima argumente, već simulira parsiranje fajla
	// datog atributom filename.
	std::string filename;
public:
	std::string getFilename() { return filename; }
	TitleFile(const std::string& filename) : filename(filename) {}
	virtual std::vector<std::tuple<int, int, std::string>*> parseFile() = 0;
	// Recimo da simuliramo ove klase tako što će parseFile da vrati uređene
	// trojke(od, do, subtitle) kojim se definiše šta se prikazuje 
	// od koje do koje milisekunde
	virtual ~TitleFile() {}
};

class SrtTitle : public TitleFile {
public:
	// Za klasu SrtTitle bio bi nam potreban konstruktor sa jednim parametrom
	// (const std::string&) samo da bismo taj string prosledili konstruktoru osnovne klase
	// Umesto da gubimo vreme i pisemo takav konstruktor, mozemo samo dozvoliti klasi 
	// SrtTitle da koristi kontruktor iz TitleFile navodeci:
	using TitleFile::TitleFile;

	std::vector<std::tuple<int, int, std::string>*> parseFile() {
		std::cout << "Opening file " + filename + " with srt subtitle..." << std::endl;
		return std::vector<std::tuple<int, int, std::string>*>();
	}
};

class SubTitle : public TitleFile {
public:
	using TitleFile::TitleFile;

	std::vector<std::tuple<int, int, std::string>*> parseFile() {
		std::cout << "Opening file " + filename + " with sub subtitle..." << std::endl;
		return std::vector<std::tuple<int, int, std::string>*>();
	}
};

class TxtTitle : public TitleFile {
public:
	using TitleFile::TitleFile;

	std::vector<std::tuple<int, int, std::string>*> parseFile() {
		std::cout << "Opening file " + filename + " with txt subtitle..." << std::endl;
		return std::vector<std::tuple<int, int, std::string>*>();
	}
};

class TitleDisplay {
protected:
	int fontSize = 14;
	std::string fontStyle = "Times New Roman";
	TitleFile* titleFile;
	std::vector<std::tuple<int, int, std::string>*> subtitles;

	void parseFile() {
		// Parsiranje fajla prepuštamo implementatoru i ova metoda se ponaša
		// potpuno isto bilo da se radi o "običnoj" ili "finijoj" apstrakciji
		// To što je metoda privatna ne treba da zbuni. Bitno je da smo razdvojili
		// apstrakciju i implementaciju.
		std::cout << "Parsing " << titleFile->getFilename() << std::endl;
		subtitles = titleFile->parseFile();
	};
public:
	TitleDisplay(TitleFile* titleFile): 
		titleFile(titleFile) {
		parseFile();
	}
	virtual void displaySubtitle() {
		std::cout << "Showing basic subtitles" << std::endl;
	}
};

class PremiumTitleDisplay : public TitleDisplay {
public:
	// Bez obzira na dodatke koje ova klasa poseduje, parsiranje fajla i dalje ostaje isto i vrši
	// ga implementator
	PremiumTitleDisplay(TitleFile* file, int fontSize, const std::string& fontStyle) : TitleDisplay(file) {
		this->fontSize = fontSize;
		this->fontStyle = fontStyle;
	}
	void displaySubtitle() override {
		std::cout << "Showing premium subtitle with fontSize = " << fontSize;
		std::cout << ", fontStyle = " << fontStyle << std::endl;
	}
};

int main() {
	TitleDisplay* t1 = new TitleDisplay(new SrtTitle("title.srt"));
	TitleDisplay* t2 = new PremiumTitleDisplay(new TxtTitle("title2.txt"), 14, "Arial");

	t1->displaySubtitle();
	t2->displaySubtitle();
	
	return 0;
}
