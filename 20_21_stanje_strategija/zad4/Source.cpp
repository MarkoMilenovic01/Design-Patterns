#include<iostream>
#include<chrono>
#define MAX_VREDNOST 10

class AlgoritamSortiranja {
public:
	virtual void sortiraj(int* niz, int n) = 0;
	virtual ~AlgoritamSortiranja() {}
};

class SortiranjePrebrojavanjem : public AlgoritamSortiranja {
public:
	void sortiraj(int* niz, int n) override {
		int br[MAX_VREDNOST];
		for (int i = 0; i < n; i++)
			br[niz[i]] += 1;
		int i = 0;
		for (int vrednost = 0; vrednost < MAX_VREDNOST; vrednost++) {
			for (int b = 0; b < br[vrednost]; b++) {
				niz[i++] = vrednost;
			}
		}
	}
};

class SortiranjeSelekcijom : public AlgoritamSortiranja {
public:
	void sortiraj(int* niz, int n) override {
		for (int i = 0; i < n; i++) {
			int min = niz[i];
			int indeks_minimuma = i;
			for (int j = i + 1; j < n; j++) {
				if (niz[j] < min) {
					min = niz[j];
					indeks_minimuma = j;
				}
			}
			int tmp = niz[i];
			niz[i] = min;
			niz[indeks_minimuma] = tmp;
		}
	}
};

class Niz {
	int* niz;
	int n;
	AlgoritamSortiranja* algoritam;
public:
	Niz(int n): n(n), niz(new int[n]()) {}
	void postaviAlgoritamSortiranja(AlgoritamSortiranja* algo) {
		this->algoritam = algo;
	}
	int& operator[] (int indeks) { return niz[indeks]; }
	int operator[] (int indeks) const { return niz[indeks]; }
	void sortiraj() { algoritam->sortiraj(niz, n); }
	int velicina() const { return n; }
	~Niz() { 
		delete[] niz;
		niz = nullptr;
	}
};

int main() {
	Niz a(100000);
	Niz b(100000);

	for (int i = 0; i < a.velicina(); i++) {
		a[i] = rand() % MAX_VREDNOST;
		b[i] = rand() % MAX_VREDNOST;
	}

	auto start = std::chrono::system_clock::now();
	a.postaviAlgoritamSortiranja(new SortiranjeSelekcijom());
	a.sortiraj();
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Brzina sortiranja selekcijom: " << elapsed << " mikrosekundi\n";
	
	start = std::chrono::system_clock::now();
	b.postaviAlgoritamSortiranja(new SortiranjePrebrojavanjem());
	b.sortiraj();
	end = std::chrono::system_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Brzina sortiranja prebrojavanjem: " << elapsed << " mikrosekundi\n";

	return 0;
}