#include<iostream>
#include<vector>

template<typename T>
class StackImplementator{
public:
	virtual void push(T) = 0;
	virtual T peek() const = 0;
	virtual void pop() = 0;
	virtual bool empty() const = 0;
	virtual int size() const = 0;
	virtual ~StackImplementator() {}
};

template<typename T>
class VectorStackImplementator : public StackImplementator<T> {
	//Napomena: VectorStackImplementator ujedno adaptira std::vector, prilagođava 
	// ga ciljnom interfejsu iz StackImplementator<T>
	std::vector<T> v;
public:
	void push(T elem) override {
		v.push_back(elem);
	}
	void pop() override {
		v.pop_back();
	}
	T peek() const override {
		return v.back();
	}
	bool empty() const override { return v.empty(); }
	int size() const override { return v.size(); }
};

template<typename T>
class ArrayStackImplementator : public StackImplementator<T> {
private:
	T* _array;
	int index;
	int capacity; 

	void resize() {
		T* new_array = new T[capacity * 2];
		for (int i = 0; i < capacity; i++) {
			new_array[i] = _array[i];
		}
		delete[] _array;
		_array = new_array;
		capacity *= 2;
	}
public:
	ArrayStackImplementator() : index(0), capacity(10) {
		_array = new T[capacity];
	}
	void pop() override {
		if (index > 0) index--;
		else std::cout << "Array is empty" << std::endl;
	}
	T peek() const override {
		if (index > 0) return _array[index - 1];
		std::cout << "Array is empty" << std::endl;
	}

	void push(T elem) override {
		if (index == capacity) resize();
		_array[index++] = elem;
	}
	int size() const override { return index; }
	bool empty() const override { return index == 0; }
};

//template<typename T, class Implementator=VectorStackImplementator<T>>
template<typename T, template<typename> class Implementator>
/*
Prethodnom linijom (koja nije u komentaru) saopštavamo da ćemo kao drugi parametar šablona
navesti takođe šablon i to takav da zavisi od jednog tipa. Dakle, nećemo navesti konkretnu
klasu. Ali tip od koga naš implementator treba da zavisi i tip T elemenata sa kojim ćemo raditi u 
steku su jedan isti tip. Tako da će konkretna klasa implementator biti Implementator<T>.
Na ovaj način prilikom instanciranja šablona zahtevamo od korisnika da navede liniju poput:
Stack<int, ArrayStackImplementator> - interno ćemo iskobinovati tip int i ArrayStackImplementator šablon
i doći do konkretne klase ArrayStackImplementator<int>. 
Naravno, ovo nije od suštinske važnosti za temu časa. 
Alternativa (linija template<typename T, class Implementator=VectorStackImplementator<T>)
zahteva od nas da kao klasu implementatora navedemo konkretan tip. Pa tako dozvoljava
da napišemo Stack<int, ArrayStackImplementator<float>>. Naravno, ovo se neće kompajlirati u slučaju
da ne možemo implicitno izvršiti konverziju iz int u float i obrnuto, ali još bolje je ako
možemo da se postaramo da takav problem bude nemoguć, što nam omogućuje prva varijanta (i dalje možemo
da napišemo neku "glupost", ali će VS odmah podvući datu liniju i dati nam do znanja da je nemoguće
kompajlirati dati kod, čak i pre nego što pokušamo da ga kompajliramo).
*/
class Stack {
private:
	Implementator<T> implementator;
public:

	int size() const { return implementator.size(); }
	bool empty() const { return implementator.empty(); }
	T peek() const { return implementator.peek(); }
	virtual void push(T elem) { implementator.push(elem); }
	virtual void pop() { implementator.pop(); }
};


template<typename T, template<typename> class Implementator>
class LimitedCapacityStack : public Stack<T, Implementator> {
	int capacity;
public:
	LimitedCapacityStack(int capacity): capacity(capacity) {}
	void push(T elem) {
		if (this->size() < capacity) {
			Stack<T, Implementator>::push(elem);
		}
		else { std::cout << "Stack overflow!" << std::endl; }
	}
};

int main() {
	LimitedCapacityStack<int, ArrayStackImplementator> s(2);
	s.push(1);
	s.push(12);
	s.push(-1);

	while (!s.empty()) {
		std::cout << s.peek() << std::endl;
		s.pop();
	}
}