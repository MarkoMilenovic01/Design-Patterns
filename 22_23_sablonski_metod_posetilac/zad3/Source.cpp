#include<iostream>
#include<vector>
#include<queue>

class Graf {
	/*Jedan "programerski" primer. 
	Graf može biti implementiran koristeći liste susedstva ili
	koristeći matricu susedstva. Bez obzira na to kako implementirali graf, 
	algoritmi pretrage u širinu i u dubinu mogu se implementirati pod pretpostavkom da
	je implementirana metoda koja će za prosleđeni čvor vratiti sve njegove susede. 
	Naše algortime (BFS i DFS) implementiraćemo u osnovnoj klasi, a izvedenim klasama prepustićemo
	da implementiraju "korak" dobijanja suseda čvora.
	*/
protected:
	int n;
	void DFS(int trenCvor, bool* poseceniCvorovi) { 
		// Rekurzivna metoda, kojoj nećemo pristupati od spolja
		poseceniCvorovi[trenCvor] = true;
		std::cout << trenCvor << std::endl;
		std::vector<int> susedi = susediOd(trenCvor);
		for (int u : susedi) {
			if (!poseceniCvorovi[u])
				DFS(u, poseceniCvorovi);
		}
	}
public:
	Graf(int n) : n(n) {}
	virtual void dodajGranu(int _od, int _do, bool usmerena = true) = 0;
	virtual std::vector<int> susediOd(int v) = 0; // izvedene klase preklopiće ovu metodu
	void BFS(int s) {
		bool* poseceniCvorovi = new bool[n];
		for (int i = 0; i < n; i++) {
			poseceniCvorovi[i] = false;
		}
		std::queue<int> red;
		red.push(s);
		poseceniCvorovi[s] = true;

		while (!red.empty()) {
			int v = red.front();
			red.pop();
			std::cout << v << std::endl;
			std::vector<int> susedi = susediOd(v); // Ovde će se pozvati implementacija iz izvedene klase
			for (int u : susedi) {
				if (!poseceniCvorovi[u]) {
					poseceniCvorovi[u] = true;
					red.push(u);
				}
			}
		}
		delete[]poseceniCvorovi;
	}
	void DFS(int s) {
		bool* poseceniCvorovi = new bool[n];
		for (int i = 0; i < n; i++) {
			poseceniCvorovi[i] = false;
		}
		DFS(s, poseceniCvorovi);
		delete[]poseceniCvorovi;
	}
	virtual ~Graf() {}
};

class GrafMatSusedstva : public Graf {
private:
	std::vector<std::vector<int>> matSusedstva;
public:
	GrafMatSusedstva(int n) : Graf(n) {
		matSusedstva = std::vector<std::vector<int>>(n);
		for (int i = 0; i < n; i++)
			matSusedstva[i] = std::vector<int>(n);
	}
	void dodajGranu(int _od, int _do, bool usmerena = true) override {
		matSusedstva[_od][_do] = 1;
		if (!usmerena) {
			matSusedstva[_do][_od] = 1;
		}
	}
	std::vector<int> susediOd(int v) override {
		std::vector<int> susedi;
		for (int u = 0; u < n; u++) {
			if (matSusedstva[v][u]) susedi.push_back(u);
		}
		return susedi;
	}
};

class GrafListaSusedstva : public Graf {
private:
	std::vector<std::vector<int>> listeSusedstva;
public:
	GrafListaSusedstva(int n) : Graf(n) {
		listeSusedstva = std::vector<std::vector<int>>(n);
	}
	void dodajGranu(int _od, int _do, bool usmerena = true) override {
		listeSusedstva[_od].push_back(_do);
		if (!usmerena) {
			listeSusedstva[_do].push_back(_od);
		}
	}
	std::vector<int> susediOd(int v) override {
		return listeSusedstva[v];
	}
};

void testirajGraf(Graf* g) {
	g->dodajGranu(0, 1, false);
	g->dodajGranu(0, 4, false);
	g->dodajGranu(1, 2, false);
	g->dodajGranu(1, 3, false);
	g->dodajGranu(1, 5, false);
	g->dodajGranu(5, 6, false);
	g->dodajGranu(6, 7, false);
	g->dodajGranu(3, 4, false);
	g->dodajGranu(3, 5, false);
	g->dodajGranu(5, 8, false);
	g->dodajGranu(8, 9, false);

	std::cout << "BFS:" << std::endl;
	g->BFS(0);
	std::cout << "\n\n\n";
	g->DFS(0);
}

int main() {
	Graf* g1 = new GrafMatSusedstva(10);
	Graf* g2 = new GrafMatSusedstva(10);
	testirajGraf(g2);

	delete g1;
	delete g2;
	return 0;
}