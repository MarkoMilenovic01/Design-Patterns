#include<iostream>

#define MAX_VELICINA 100
template<typename T>
class Niz;

/*
U ovom zadatku primenjujemo znatno drugačiji pristup. Naime, implementiraćemo iterator za klasu Niz u 
skladu sa standardima standardne biblioteke. Samim tim, neće biti dinamičkog polimorfizma.
for (TIP elem : kolekcija) se prevodi u kod poput:
for(Iterator it = kolekcija.begin(); it != kolekcija.end(); it++) {
	TIP elem = *it;
}
Dakle, da bi foreach petlja mogla da se kompajlira, potrebno je da kolekcija ima metode
begin i end koje vraćaju kao tip iterator. Da iterator ima preklopljene operatore preinkrementa, postinkrementa,
dereferenciranja i poređenja po jednakosti i nejednakosti. 
*/

template<typename T>
class IteratorNiza {
	Niz<T>* niz;
	int tr_pozicija = 0;
public:
	IteratorNiza(Niz<T>* niz, int tr_pozicija=0): niz(niz), tr_pozicija(tr_pozicija) {}
	IteratorNiza<T>& operator++() {
		tr_pozicija++;
		return *this;
	}
	IteratorNiza<T> operator++(int) {
		IteratorNiza<T> tmp = *this;
		tr_pozicija++;
		return tmp;
	}
	T operator*() {
		return niz->niz[tr_pozicija];
	}
	bool operator==(const IteratorNiza<T>& drugi) const {
		return this->niz->niz == drugi.niz->niz && this->tr_pozicija == drugi.tr_pozicija;
	}
	bool operator!=(const IteratorNiza<T>& drugi) const {
		return !(*this == drugi);
	}
};

template<typename T>
class Niz {
private:
	T niz[MAX_VELICINA];
	int tr_velicina = 0;
public:
	void dodajNaKraj(const T elem) { niz[tr_velicina++] = elem; }
	int velicina() const { return tr_velicina; }
	bool prazan() const { return tr_velicina == 0; }
	IteratorNiza<T> begin() { return IteratorNiza<T>(this); }
	IteratorNiza<T> end() { return IteratorNiza<T>(this, tr_velicina); }
	friend class IteratorNiza<T>;
};

int main() {
	using T = int;
	Niz<T>niz;
	niz.dodajNaKraj(1);
	niz.dodajNaKraj(2);
	niz.dodajNaKraj(3);
	niz.dodajNaKraj(4);
	for (T elem : niz) {
		std::cout << elem << std::endl;
	}
	return 0;
}