#include<iostream>

class Image {
protected:
	std::string path;
	int width; // recimo da slici želimo da zadamo širinu i visinu (bez obzira na stvarne dimenzije)
	int height;

public:
	Image() = default;
	Image(std::string path, int width, int height):
		path(path), width(width), height(height) {
		std::cout << "Creating image " << width << "x" << height << " from " << path << std::endl;
	}

	virtual int getWidth() { return width; }
	virtual int getHeight() { return height; }
	virtual void displayImage() {
		std::cout << "displaying image from " << path << std::endl; 
	}
};

class ImageProxy : public Image {
private:
	Image* img = nullptr;
	// Ideja je da sliku kreiramo samo onda kada bude bila zaista potrebna. Kao naslednik klase 
	// Slika, naš zastupnik u sebi već ima atribute slike tako da samim kreiranjem objekta ove 
	// klase mi već kreiramo i sliku kao objekat, ali ideja je ovde da u realnoj primeni slika
	// bude matrica ili 3D tenzor piksela koji bismo dobili tek nakon čitanja iz fajla, što je 
	// vremenski i memorijski skupa operacija. 
	// Tek kada nam slika bude bila zaista potrebna mi ćemo kreirati objekat tipa Image (koji 
	// pamtimo kao pokazivač u ovoj klasi i nema veze s tim što je ImageProxy nasleđena klasa od 
	// Image). Da bismo mogli da sliku kreiramo tek kada bude bila potrebna, mi moramo da negde
	// sačuvamo parametre potrebne za njeno kreiranje. 
	// Možemo dodati atribute imgPath, imgWidth i imgHeight klasi ImageProxy, a koji će biti
	// iskorišćeni samo prilikom kreiranja objekta tipa Image. Tako je prvobitno bilo urađeno
	// na vežbama uz poziv podrazumevanog konstruktora klase Image (konstruktor bez parametara).
	// Da to ne bi unosilo konfuziju (klasa Image ima kao atribute path, width i height, a nasleđena
	// klasa ImageProxy imgPath, imgWidth i imgHeight) ovde je materijal neznatno izmenjen da se
	// podaci potrebni za kreiranje slike pamte u postojećim atributima klase Image. To ima svoje
	// prednosti i mane. Prednost je to što nećemo imati 6 atributa od kojih koristmo samo 3,
	// a mana je što je sada možda ipak više konfuzno s obzirom na činjenicu da ćemo path, widht
	// i height atribute koristiti samo da bismo kreirali drugu sliku, i nećemo ih povezivati sa
	// činjenicom da ImageProxy kao naslednik od Image jeste i slika (zvanično). 
	// Napomena: oba pristupa su korektna. 
	
	// std::string imgPath; // na vežbama smo koristili ova tri atributa
	// int imgWidth;
	// int imgHeight;

	void createImage() {
		if (img) return;
		//img = new Image(imgPath, imgWidth, imgHeight);
		img = new Image(path, width, height);
	}
public:
	ImageProxy(std::string path, int width, int height)
		// : imgPath(path), imgWidth(width), imgHeight(height) {}
	{
		this->path = path;
		this->width = width;
		this->height = height;
		// Namerno ne zovemo konstruktor Image(path, width, height) da bismo naglasili da 
		// u nekoj realnoj primeni to moze biti "skup" konstruktor čiji poziv izbegavamo dok
		// god to ne bude bilo neophodno (lako bismo u tom konstruktoru mogli imati čitanje fajla
		// i kreiranje RGB matrica i resize itd)
	}

	// Suština pristupa je u sledećim preklopljenim metodama. 
	// Privtna metoda createImage() će kreirati objekat tipa Image ukoliko nije kreiran,
	// a metoda se poziva samo ukoliko se pozove neka od metoda getWidth(), getHeight() ili
	// displayImage(). Ako se nijedna od tih metoda ne bude pozvala, objekat img neće ni biti
	// kreiran.
	int getWidth() override {
		createImage();
		return img->getWidth();
	}
	int getHeight() override {
		createImage();
		return img->getHeight();
	}

	void displayImage() {
		createImage();
		img->displayImage();
	}

	~ImageProxy() { delete img; }
};


int main() {
	Image* im = new ImageProxy("bla bla.png", 100, 200);
	Image* im2 = new ImageProxy("bla.png", 10, 20);
	Image* im3 = new ImageProxy("bla3.png", 140, 240);
	im->displayImage(); // slike čiji su zastupnici im2 i im3 neće se nikada ni kreirati.
	return 0;
}