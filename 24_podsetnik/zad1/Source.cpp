#include<iostream>
#include<string>

class Memento {
	std::string state;
public:
	Memento(const std::string& state): state(state) {}
	std::string getState() const { return state; }
	void setState(const std::string& state) { this->state = state; } //seter ne mora da postoji
};

class Originator {
	std::string state;
public:
	Originator(const std::string& state): state(state) {}
	void setMemento(const Memento* m) { 
		std::cout << "Vracam objekat na ranije sacuvano stanje\n";
		this->state = m->getState();
	}
	Memento* createMemento() const { return new Memento(state); }
	void modifyState() { // Da naglasimo da stanje objekta može da se u toku životnog veka
		// objekta modifikuje (šta će nam inače Memento...)
		std::cout << "Modifikujem stanje objekta\n";
		state += std::to_string(rand() % 10); // prosto ćemo dopisati nasumičnu cifru
	}
	// Da opravdamo potrebu za korišćenjem Podsetnika, nećemo imati geter za stanje.
	// Ipak, da bismo kasnije u main-u štampali objekat i testirali vraćanje na prethodno stanje,
	// ovde ćemo dodati jedan "zamaskirani" geter koji daje opis objekta na način da upravo vraća
	// njegovo stanje. Imajte na umu da je ovde naglasak na komunikaciji klasa!
	std::string describe() const { return state; }
};

class Caretaker {
	const Memento* m; // Bitan detalj je da ne smemo dozvoliti modifikaciju podsetnika.
	// To smo mogli da postignemo i time što ne bismo imali setere u klasi Memento.
	// Ovde je implementiran najosnovniji slučaj gde Caretaker čuva samo jedan podsetnik.
public:
	void setMemento(const Memento* m) { 
		std::cout << "Cuvam novi podsetnik\n";
		this->m = m; 
	}
	const Memento* getMemento() const { return m; }
	~Caretaker() { delete m; }
};

int main() {
	Originator o("...");
	Caretaker caretaker;
	std::cout << "o.describe(): " << o.describe() << std::endl;
	o.modifyState();
	caretaker.setMemento(o.createMemento());
	std::cout << "o.describe(): " << o.describe() << std::endl;
	o.modifyState();
	std::cout << "o.describe(): " << o.describe() << std::endl;
	o.modifyState();
	std::cout << "o.describe(): " << o.describe() << std::endl;
	o.setMemento(caretaker.getMemento());
	std::cout << "o.describe(): " << o.describe() << std::endl;
	return 0;
}