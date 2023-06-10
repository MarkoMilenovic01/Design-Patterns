#include<iostream>
#include<fstream>

enum class LogLevel {DEBUG, INFO, WARNING, ERROR};

/* Tipičan primer korišćenja lanca odgovornosti je kod zahteva koji sami po sebi već imaju
 "prirodno" pridodat nivo važnosti. To je slučaj sa logovanjem, pri čemu poruke koje logujemo
  imaju različit nivo značaja. Greške ćemo možda ispisivati i u fajl i u konzolu ili ćemo ih 
  možda bojiti, slati na mejl... Upozoranja bismo možda mogli da samo štampamo itd. 
  */

class Logger {
protected:
	Logger* next = nullptr;
public:
	virtual void log(const std::string& message, LogLevel level) = 0;
	void setNext(Logger* logger) { this->next = logger; }

	// setLast metoda ne mora da postoji. Kako će različiti handler-i činiti povezanu listu
	// praktičniji način za kreiranje liste je da krenemo od prvog čvora (handlera) i uzastopno dodajemo na kraj.
	// Kreiranje liste sa više od dva elemnta pomoću setNext je malo zahtevnije. 
	void setLast(Logger* logger) {
		Logger* tmp = this;
		while (tmp->next != NULL) tmp = tmp->next;
		tmp->next = logger;
	}
	virtual ~Logger() { delete next; }
};

// Odradićemo implementaciju tako da debug i info logger ispisuju sadržaj u konzolu, dok warning i error ispisuju i u fajl.

class DebugLogger : public Logger {
public:
	void log(const std::string& message, LogLevel level) override {
		if (level == LogLevel::DEBUG) {
			std::cout << "DEBUG: " << message << std::endl;
			return;
		}
		if (next) {
			next->log(message, level);
		}
	}
};

class InfoLogger : public Logger {
public:
	void log(const std::string& message, LogLevel level) override {
		if (level == LogLevel::INFO) {
			std::cout << "INFO: " << message << std::endl;
			return;
		}
		if (next) {
			next->log(message, level);
		}
	}
};

class WarningLogger : public Logger {
private:
	std::string filename;
public:
	WarningLogger(const std::string& filename): filename(filename) {}
	void log(const std::string& message, LogLevel level) override {
		if (level == LogLevel::WARNING) {
			std::cout << "WARNING: " << message << std::endl;
			std::fstream file(filename, std::ios_base::app);
			// Drugi argument je mod otvaranja fajla. app je od append, tj. otvaramo fajl u 
			// modu za dopisivanje sadržaja.
			file << "WARNING: " << message << std::endl;
			file.close();
			return;
		}
		if (next) {
			next->log(message, level);
		}
	}
};

class ErrorLogger : public Logger {
private:
	std::string filename;
public:
	ErrorLogger(const std::string& filename) : filename(filename) {}
	void log(const std::string& message, LogLevel level) override {
		if (level == LogLevel::ERROR) {
			std::cout << "ERROR: " << message << std::endl;
			std::fstream file(filename, std::ios_base::app);
			file << "ERROR: " << message << std::endl;
			file.close();
			return;
		}
		if (next) {
			next->log(message, level);
		}
	}
};


int main() {
	Logger* logger = new DebugLogger();
	logger->setLast(new InfoLogger());
	logger->setLast(new WarningLogger("warnings.txt"));
	logger->setLast(new ErrorLogger("errors.txt"));

	logger->log("Poruka 1", LogLevel::INFO);
	logger->log("Poruka 2", LogLevel::ERROR);
	logger->log("Problem 3", LogLevel::ERROR);
	logger->log("Upozorenje 1", LogLevel::WARNING);

	return 0;
}