#include<iostream>
#include<ctime>
#include<fstream>

class MessageSender {
public:
	virtual void send(const std::string& message) {
		std::cout << "Sending message: " << message << std::endl;
	}
	virtual ~MessageSender() {}
};

class MessageDecorator : public MessageSender {
protected:
	MessageSender* decoratedObject;
public:
	MessageDecorator(MessageSender* decoratedObject): decoratedObject(decoratedObject) {}
	virtual void send(const std::string& message) = 0;
	~MessageDecorator() {
		delete decoratedObject;
	}
};

class CurrDatetimeDecorator : public MessageDecorator {
public:
	CurrDatetimeDecorator(MessageSender* decoratedObject): MessageDecorator(decoratedObject) {}
	void send(const std::string& message) override {
		time_t my_time = time(NULL);
		std::string vreme = std::ctime(&my_time);
		std::string new_message = message + "\n" + vreme;
		decoratedObject->send(new_message);
	}
};

class RecordHistoryDecorator : public MessageDecorator {
private:
	std::string filename;
public:
	RecordHistoryDecorator(MessageSender* decoratedObject, std::string filename) :
		MessageDecorator(decoratedObject), filename(filename) {}
	void send(const std::string& message) override {
		std::ofstream file(filename);
		file << message << std::endl;
		file.close();
		decoratedObject->send(message);
	}
};

class EncriptionDecorator : public MessageDecorator {
private:
	int shift;
public:
	EncriptionDecorator(MessageSender* decoratedObject, int shift) : MessageDecorator(decoratedObject), shift(shift) {}
	void send(const std::string& message) override {
		std::string encryptedMessage = "";
		for (int i = 0; i < message.size(); i++) {
			char currChar = message[i];
			currChar = (currChar + shift) % 256;
			encryptedMessage += currChar;
		}
		decoratedObject->send(encryptedMessage);
	}
};


/*
NAPOMENA: prethodni primer (sa ratnicima) smo i mogli rešiti tako što bi ratnik imao pokazivače
na razne delove opreme i naoružanja. Primer nije zavisio od redosleda dodavanja opreme.
Ukoliko nam je redosled bitan, onda je dekorator neophodan. To što je redosled bitan povlači za sobom
i da imamo odgovornost za to kako ćemo dekorisati neki objekat (u kom redosledu ćemo "ređati" dekoratore).

Na primer, sa našim dekoratorima iz ovog primera nije isto ukoliko prvo šifrujemo poruku, pa dodamo nešifrovan
tekst koji predstavlja datum i vreme, pa zatim na kraju upišemo u fajl ili prvo upišemo u fajl, pa dodamo datum i vreme,
pa šifrujemo itd. 
Ne postoji ispravan redosled, već je samo pitanje šta hoćemo.
*/
int main() {
	// U narednom primeru mi kreiramo naš sender objekat tako da na poruku doda datum i vreme, 
	// zatim sačuva tako dobijenu poruku u fajl, šifruje je i tek onda je šalje. 
	MessageSender* sender = new CurrDatetimeDecorator(
		new RecordHistoryDecorator(new EncriptionDecorator(new MessageSender(), 10), "history.txt"));
	sender->send("Test poruka");

	delete sender;
	return 0;
}