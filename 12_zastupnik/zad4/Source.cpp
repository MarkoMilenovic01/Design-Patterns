#include<iostream>
#include<vector>

template<typename T>
class Vektor {
private:
	int n;
	std::vector<T> vec;
public:
	typedef T type; // Ovaj typedef je bitan da bismo ukoliko kao parametar šablona dobijemo Vektor<T> mogli
	// da dobijemo tip T kao Vektor<T>::type (imajte na umu da identifikator tipa Vektor<T> u šablonu može biti 
	// recimo X. Mi kao parametar nekog šablona vidimo X, ne vidimo T, a do T bismo došli sa X::type

	Vektor(int n) : n(n), vec(std::vector<T>(n)) {}
	Vektor& operator=(const Vektor& drugi) = default;

	// Savet: vratiti se na ovaj operator dodele nakon što proučite klase za binarni i unarni izraz
	template<typename T>
	Vektor& operator=(const T& drugi) {
		// Ukoliko se operator dodele vrednosti pozove na način da je sa desne strane vektor,
		// pozvaće se podrazumevani operator dodele - kao eksplicitno naveden par linija iznad
		// on ima prednost u odnosu na bilo koji instancirani operator na osnovu šablona. 
		// Ideja je dakle da se ovaj operator dodele vrednosti zove kad god se sa desne
		// strane nađe bilo koji tip za koji postoji metoda getSize() i za koji je preklopljen
		// operator [] tako da vraća tip T (u suprotnom kod neće moći da se kompajlira)
		// Ono na šta ovde ciljamo je da se ovaj operator dodele vrednosti poziva u slučajevima
		// kada sa desne strane imamo neki unarni ili binarni izraz.
		std::cout << "Poziv preklopljenog operatora dodele vrednosti" << std::endl;
		vec.clear();
		for (int i = 0; i < drugi.getSize(); i++)
			vec.push_back(drugi[i]);
		return *this;
	};
	int getSize() const { return n; }
	T operator[] (int i) const { // ovaj operator pozivaće se iz const funkcija i kao takav služiće
		// samo za očitavanje vrednosti iz vektora
		//std::cout << "Pozvan const [] operator" << std::endl;
		return vec[i]; 
	}
	T& operator[] (int i) { 
		// ovaj operator vratiće referencu i kao takav omogućava da menjamo elemente vektora,
		// tj. da vektoru na nekom mestu i dodelimo neku vrednost linijom poput v[i] = 10, gde je
		// v tipa Vektor<int>
		//std::cout << "Pozvan [] operator" << std::endl;
		return vec[i];
	}
};

template<typename T, template<typename> class OP, typename L, typename R>
class BinarniIzraz {
	/* Ovo je jedan zastupnika. Preko njega pristupamo rezultatu i prilikom pristupa mi vršimo izračunavanje
	rezultata. Prednost je što ovde nema potrebe za međurezultatima, a kako je u pitanju šablon klase,
	biće nam dozvoljeno jednostavno kombinovanje raznoraznih izraza dobijajući tako veoma složene izraze, 
	a za koje se tip određuje u vreme kompajliranja! 
	
	Razmotrimo ideju klase. 
	
	Tip T je tip vrednosti sa kojima na najnižem nivou radi naš binarni izraz.
	Na primer ako sabiramo vektore celih brojeva, onda je T = int. 

	OP je šablon koji zavisi od jednog argumenta koji mi ne prosleđujemo kada ovaj šablon navodimo.
	OP će biti binarna operacija koja će se primeniti nad operandima, a kao rezultat vratiće onaj
	tip koji toj binarnoj operaciji budemo prosledili. Kao parametar šablona imamo i tip T (već pomenut)
	pa će konkretna klasa operacije biti OP<T> (to ćemo navoditi kad god nam treba tip klase operacije).
	Bitno je da klasa OP<T> ima javnu statičku metodu evaluiraj koja će kao argumente uzeti dve vrednosti
	tipa T i vratiti takođe tip T. 

	L i R su tipovi levog i desnog operanda. Jedina ograničenja koje L i R treba da zadovolje su
	da imaju metodu getSize(), i da je nad njima moguće pozvati const operator[] koji vraća tip T.
	Lepota ovog pristupa je u tome što ukoliko ovoj klasi dodamo operator[] const takav da vraća T i
	metodu getSize(), onda će kao L ili R tip moći da se nađe i BinarniIzraz. Naravno, klasa Vektor
	već zadovoljava date uslove. Videćemo kasnije da je i klasa Unarni izraz implementirana na ovaj
	način tako da će kao levi ili desni operand moći da se nađe binarni izraz, unarni izraz ili vektor.
	*/
private:
	L leviOperand;
	R desniOperand;
public:
	typedef T type; // Ovo je neophodno da bismo ukoliko kao tip dobijemo BinarniIzraz u nekom šablonu mi mogli da 
	// "izvučemo" tip T preko ::type

	BinarniIzraz(L leviOperand, R desniOperand) : 
		leviOperand(leviOperand), desniOperand(desniOperand) {
		// kada kreiramo binarni izraz mi samo treba da upamtimo operande. Ništa ne računamo.
	}  

	T operator[] (int i) const {
		// Tek kada nad tipom binarni izraz mi pozovemo operator [] const mi ćemo izračunati vrednost
		// izraza za prosleđeno i.
		return OP<T>::evaluiraj(leviOperand[i], desniOperand[i]);
	}

	// Pretpostavka je da ćemo izraze graditi od operanada istih veličina. Mogli bismo da bacimo
	// exception ukoliko operandi nisu istih veličina, ali u ovom primeru mi prosto podrazumevamo
	// da je leviOperand.getSize() == desniOperand.getSize()
	int getSize() const { 
		return leviOperand.getSize(); // pod navedenom pretpostavkom, svejedno je koje getSize ćemo pozvati
	}
};

template<typename T, template <typename> class F, typename Argument>
class UnarniIzraz {
	/*
	Ideja je slična kao u slučaju binarnog izraza. T je tip  sa kojim radimo na najnižem nivou.
	F je šablon klase, a mi ćemo koristiti u klasi UnarniIzraz konkretnu klasu F<T>. 
	F<T> mora da ima javnu statičku metodu evaluiraj koja uzima jedan argument tipa T i vraća
	tip T. 
	Tip Argument mora biti takav da podržava poziv getSize() i operator [] const koji vraća tip T.
	*/
private:
	Argument argument;
public:
	typedef T type;
	UnarniIzraz(Argument argument) : argument(argument) {}
	int getSize() const { return argument.getSize(); }
	T operator[] (int i) const { return F<T>::evaluiraj(argument[i]); }
};


/* U nastavku sledi definicija par struktura (tačnije šablona struktura) koje imaju
javnu statičku metodu evaluiraj koja kao argument uzima dve vrednosti tipa T i vraća 
kao rezultat tip T

Napomena: u C++-u, razlika između strukture i klase je u podrazumevanom nivou pristupa ukoliko
ne navedemo odeljak private:, protected: ili public:.
Kod klase je to private:, a kod strukture public:
*/
template<typename T>
struct Saberi {
	static T evaluiraj(T a, T b) {
		return a + b;
	}
};

template<typename T>
struct Oduzmi {
	static T evaluiraj(T a, T b) {
		return a - b;
	}
};

template<typename T>
struct Pomnozi {
	static T evaluiraj(T a, T b) {
		return a * b;
	}
};

/* Dodajmo sada nekoliko šablona struktura sa javnom statičkom metodom evaluiraj koja uzima
jedan argument tipa T i vraća kao rezultat tip T. */
template<typename T>
struct SqrtF {
	static T evaluiraj(T a) {
		return sqrt(a); 
		// Podrazumevamo da je tip T takav da je u standardnoj biblioteci podržan poziv sqrt za dati tip
	}
};

template<typename T>
struct SinF {
	static T evaluiraj(T a) {
		return sin(a);
		// Podrazumevamo da je tip T takav da je u standardnoj biblioteci podržan poziv sin za dati tip
	}
};


// Unarne izraze za funkciju SqrtF::evaluiraj() kreiraćemo šablonskom funkcijom Sqrt
template<typename Argument>
UnarniIzraz<typename Argument::type, SqrtF, Argument> 
Sqrt(const Argument& arg) {
	// sa typename Argument::type izvlačimo tip sa kojim data klasa Argument radi. Zato smo u 
	// klasama UnarniIzraz, BinarniIzraz i Vektor dodali typedef T type
	// Ideja je da Argument može biti bilo unarni izraz, binarni izraz ili vektor.
	return UnarniIzraz<typename Argument::type, SqrtF, Argument>(arg);
}

template<typename Argument>
UnarniIzraz<typename Argument::type, SinF, Argument>
Sin(const Argument& arg) {
	return UnarniIzraz<typename Argument::type, SinF, Argument>(arg);
}


// Kako da kreiramo binarne izraze? Najlepši način koji omogućuje pisanje čitljivog i 
// efikasnog koda je preklapanjem operatora
template<typename L, typename R>
BinarniIzraz<typename L::type, Saberi, L, R> 
operator+(const L& levi, const R& desni) {
	/* Pretpostavka je da je L::type == R::type. Mogli smo to proveriti, ali to bi možda 
	bilo previše za ovaj kurs...
	Suština ove ideje je da L i R mogu biti (nezavisno jedno od drugog) bilo binarni izraz, 
	unarni izraz ili vektor. 
	*/
	return BinarniIzraz<typename L::type, Saberi, L, R>(levi, desni);
}

template<typename L, typename R>
BinarniIzraz<typename L::type, Oduzmi, L, R>
operator-(const L& levi, const R& desni) {
	return BinarniIzraz<typename L::type, Oduzmi, L, R>(levi, desni);
}

template<typename L, typename R>
BinarniIzraz<typename L::type, Pomnozi, L, R>
operator*(const L& levi, const R& desni) {
	return BinarniIzraz<typename L::type, Pomnozi, L, R>(levi, desni);
}

int main() {
	Vektor<float> v(10);
	Vektor<float> v2(10);

	for (int i = 0; i < 10; i++) {
		v[i] = i;
		v2[i] = i * i;
	}

	Vektor<float> rezultujuciVektor(10);
	// Ukoliko je tip neke promenljive poznat u vreme kompajliranja, umesto pisanja datog tipa
	// može se iskoristiti ključna reč auto. To je posebno pogodno za tipove podataka koji zavise
	// od instanciranja šablona i koji su često slabo čitljivi. 
	// Na primer, tip podataka za izraz u liniji ispod je: 
	// BinarniIzraz<float, Oduzmi, BinarniIzraz<float, Saberi, Vektor<float>, UnarniIzraz<float, SqrtF, BinarniIzraz<float, Pomnozi, Vektor<float>, Vektor<float>>>>, UnarniIzraz<float, SinF, UnarniIzraz<float, SqrtF, Vektor<float>>>>
	auto objekatOdlozenogIzracunavanja = v + Sqrt(v2 * v) - Sin(Sqrt(v2));
	// U liniji ispod ćemo pozvati operator dodele vrednosti klase Vektor<float> na način da je sa desne strane
	// tip podataka različit od Vektor<float>, pa će se u toj liniji izvršiti konstruktor koji postavlja elemente vektorra
	// jedan po jedan koristeći operator [] čime će se zapravo pokrenuti i izvršiti izračunavanje izraza.
	rezultujuciVektor = objekatOdlozenogIzracunavanja;

	for (int i=0; i < v.getSize(); i++)
		std::cout << "(v + Sqrt(v2 * v) - Sin(Sqrt(v2)))[" << i << "] = " << rezultujuciVektor[i] << std::endl;
	return 0;
}