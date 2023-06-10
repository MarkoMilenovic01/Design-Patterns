/* primer iz knjige Design Patterns Explained Simply */

#include <iostream>
#include <vector>

#define USE_FACTORY_METHOD false

using namespace std;

/* imamo apstraktnu klasu zivotinja i konkretne nasledne Pas, Riba, Ptica, prilikom izvrsenja programa
   pitani smo da odaberemo koje cemo sve tipove zivotinja da kreiramo i nakon toga bude prikazano sta
   rade te zivotinje koje smo kreirali.
   U primeru cemo razmotriti slucaj kada koristimo factory method sablon i 
   slucaj u kome u glavnom programu direktno kreiramo zeljene objekte
   (da li ce se koristiti factory method zavisi od vrednosti oznake USE_FACTORY_METHOD)
   */
class Zivotinja {
public:
    virtual void mrdaj() = 0;
    // staticka metoda napraviZivotinju igra ulogu fabricke metode
    // za USE_FACTORY_METHOD postavljeno na false, mozemo zamisliti da ova 
    // metoda i ne postoji
    static Zivotinja* napraviZivotinju(int odabir);
};

class Pas: public Zivotinja {
public:
    void mrdaj() {
        cout << "Pas: trcim po parku..." << endl;
    }
};
class Riba: public Zivotinja {
public:
    void mrdaj() {
        cout << "Riba: plivam po dubinama okeana..." << endl;
    }
};
class Ptica: public Zivotinja {
public:
    void mrdaj() {
        cout << "Ptica: letim nebom visoko..." << endl;
    }
};

/* implementacija fabrickog metoda */
Zivotinja* Zivotinja::napraviZivotinju(int odabir) {
    if (odabir == 1) {
        return new Pas();
    }
    else if (odabir == 2) {
        return new Riba();
    }
    else {
        return new Ptica();
    }
}

int main() {
    vector<Zivotinja*> uloge;
    int odabir;
    while (true) {
        cout << "Odaberi ulogu (1-3) ili startuj simulaciju (0)\nPas(1), Riba(2), Ptica(3), Startuj(0): ";
        cin >> odabir;
        if (odabir == 0) {
            break;
        }
        if (USE_FACTORY_METHOD) {
            uloge.push_back(Zivotinja::napraviZivotinju(odabir));
        }
        else {
            if (odabir == 1) {
                uloge.push_back(new Pas());
            }
            else if (odabir == 2) {
                uloge.push_back(new Riba());
            }
            else {
                uloge.push_back(new Ptica());
            }
        }
    }
    for (int i = 0; i < uloge.size(); i++) {
        uloge[i]->mrdaj();
    }
    for (int i = 0; i < uloge.size(); i++) {
        delete uloge[i];
    }
    return 0;
}
