#include<iostream>

#define MAX_VELICINA 100

template<typename T>
class Iterator;

template<typename T>
class Kolekcija {
public:
	virtual void dodajNaKraj(const T elem) = 0;
	virtual int velicina() const = 0;
	virtual bool prazna() const { return velicina() == 0; };
	// Apstraktna kolekcija (Aggregate) ima apstraktnu metodu za kreiranje apstraktnog iteratora.
	// Konkretne kolekcije će preklopiti metodu i kreirati odgovarajući konkretni iterator.
	virtual Iterator<T>* iterator() = 0;
	virtual ~Kolekcija() {}
};

template<typename T>
class Iterator {
public:
	virtual T element() const = 0; // metoda za vraćanje trenutnog elementa
	virtual void sledeci() = 0;
	virtual bool kraj() const = 0;
	virtual ~Iterator() {}
};

template<typename T> class Niz;

template<typename T>
class IteratorNiza : public Iterator<T> {
private:
	Niz<T>* niz;
	int tr_pozicija;
public:
	IteratorNiza(Niz<T>* niz, int tr_pozicija=0): niz(niz),
		tr_pozicija(tr_pozicija) {}
	T element() const override { return niz->niz[tr_pozicija]; }
	void sledeci() override { tr_pozicija++; }
	bool kraj() const { return tr_pozicija == niz->velicina(); }
};

template<typename T>
class Niz : public Kolekcija<T> {
private:
	T niz[MAX_VELICINA];
	int tr_velicina = 0;
public:
	void dodajNaKraj(const T elem) override { niz[tr_velicina++] = elem;}
	int velicina() const override { return tr_velicina; }
	Iterator<T>* iterator() override { return new IteratorNiza<T>(this); }
	friend class IteratorNiza<T>; // Najčešće (kao što je to ovde slučaj) želimo da iteratoru damo
	// pristup svim atributima/metodama klase.
};

template<typename T>
struct Cvor {
	T podatak;
	Cvor<T>* sledeci;
	Cvor(T podatak, Cvor<T>* sledeci = nullptr) : podatak(podatak),
		sledeci(sledeci) { }
};

template<typename T>
class JednostrukoPovezanaLista;

template<typename T>
class IteratorListe : public Iterator<T> {
private:
	JednostrukoPovezanaLista<T>* lista;
	Cvor<T>* tr_cvor;
public:
	IteratorListe(JednostrukoPovezanaLista<T>* lista) : lista(lista) {
		tr_cvor = lista->glava;
	}
	T element() const override { return tr_cvor->podatak; }
	void sledeci() override { tr_cvor=tr_cvor->sledeci; }
	bool kraj() const { return tr_cvor == nullptr; }
};

template<typename T>
class JednostrukoPovezanaLista : public Kolekcija<T> {
private:
	Cvor<T>* glava = nullptr;
public:
	void dodajNaKraj(const T elem) override {
		if (glava == nullptr) {
			glava = new Cvor<T>(elem);
			return;
		}
		Cvor<T>* pom = glava;
		while (pom->sledeci) pom = pom->sledeci;
		pom->sledeci = new Cvor<T>(elem);
	}
	int velicina() const override {
		if (!glava) return 0;
		int vel = 0;
		Cvor<T>* tmp = glava;
		while (tmp) {
			tmp = tmp->sledeci;
			vel++;
		}
		return vel;
	}
	Iterator<T>* iterator() override { return new IteratorListe<T>(this); }
	~JednostrukoPovezanaLista() {
		if (glava) {
			Cvor<T> *tren = glava;
			Cvor<T>* sled;
			while (tren) {
				sled = tren->sledeci;
				delete tren;
				tren = sled;
			}
		}
	}
	friend class IteratorListe<T>;
};

template<typename T>
void testirajKolekciju(Kolekcija<T>* k) {
	// Ovde vidimo praktičnost projektnog uzorka. Dobili smo pokazivač apstraktnog tipa,
	// ali svakako, iako ne znamo o kom tipu se tačno radi, mi možemo da iteriramo kroz
	// datu kolekciju. 
	Iterator<T>* it = k->iterator();
	for (; !it->kraj(); it->sledeci()) {
		std::cout << it->element() << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	using T = int;
	Kolekcija<T>* niz = new Niz<T>();
	niz->dodajNaKraj(1);
	niz->dodajNaKraj(2);
	niz->dodajNaKraj(3);

	Kolekcija<T>* lista = new JednostrukoPovezanaLista<T>();
	lista->dodajNaKraj(1);
	lista->dodajNaKraj(2);
	lista->dodajNaKraj(3);

	testirajKolekciju(niz);
	testirajKolekciju(lista);

	delete lista;
	delete niz;
}
