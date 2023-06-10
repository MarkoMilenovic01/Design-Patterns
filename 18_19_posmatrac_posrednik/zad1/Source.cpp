#include<iostream>
#include<vector>

class Subject;

class Observer {
public:
	virtual void update(Subject*) = 0;
	virtual ~Observer() {}
};

class Subject {
protected:
	int a = 0;
	std::vector<Observer*> observers;
public:
	// Iako je u pitanju apstraktni subjekat, metode attach, detach i notify ćemo implementirati,
	// jer u opštem slučaju nema tu mesta za drugačije implementacije...
	// Za slučaj da treba preklopiti ponašanje u izvedenim klasama, proglasićemo ih za virtuelne
	virtual void attach(Observer* observer) {
		observers.push_back(observer);
	}
	virtual void detach(Observer* observer) {
		for (auto it = observers.begin(); it != observers.end(); it++) {
			if (*it == observer) {
				observers.erase(it);
				break;
			}
		}
	}
	virtual void notify() {
		for (auto it = observers.begin(); it != observers.end(); it++) {
			(*it)->update(this);
		}
	}
	int getState() const { return a; }
	void setState(int a) {
		this->a = a;
		notify(); // obaveštavamo posmatrače da je došlo do promene
	}
	// Kad već radimo nasleđivanje (koje nije neophodno, već zavisi od primene do primene),
	// i želimo da imamo apstraktnog subjekta, dodaćemo jednu čistu virtuelnu funkciju f
	// Naravvno, u realnim primenama, subject često osim toga što je "posmatran" ima i neku
	// praktičnu ulogu u našem sistemu i neke metode kojima obavlja svoje poslove.
	virtual void f() = 0;

	virtual ~Subject() {
		for (int i = 0; i < observers.size(); i++)
			delete observers[i];
		observers.clear();
	}
};

class ConcreteSubject : public Subject {
public:
	void f() override {
		this->a++;
		notify(); // obaveštavamo posmatrače da je došlo do promene
	}
};

class ConcreteObserver : public Observer {
	std::string name;
public:
	ConcreteObserver(const std::string& name): name(name) {}
	void update(Subject* s) override {
		std::cout << "ConcreteObserver (name = " << name << "), Subject state = " << s->getState() << std::endl;
	}
};


int main() {
	Subject* s = new ConcreteSubject();
	s->attach(new ConcreteObserver("a"));
	s->attach(new ConcreteObserver("b"));
	s->attach(new ConcreteObserver("c"));
	s->f();
	s->f();
	s->setState(40);
	delete s;

	return 0;
}
