#include<iostream>

class GUIAplikacija {
protected:
	virtual void iscrtajZaglavlje() = 0;
	virtual void iscrtajMeni() = 0;
	virtual void iscrtajOsnovniProstor() = 0;
public:
	void iscrtaj() {
		iscrtajZaglavlje();
		iscrtajMeni();
		iscrtajOsnovniProstor();
		std::cout << "\n";
	}
	~GUIAplikacija() {}
};

class MobilnaAplikacija : public GUIAplikacija {
protected:
	void iscrtajZaglavlje() override { 
		std::cout << "Iscrtavam zaglavlje za prikaz na mobilnom telefonom\n";
	}
	void iscrtajMeni() override {
		std::cout << "Iscrtavam meni za prikaz na mobilnom telefonom\n";
	}
	void iscrtajOsnovniProstor() override {
		std::cout << "Iscrtavam osnovni prostor za prikaz na mobilnom telefonom\n";
	}
};

class DesktopAplikacija : public GUIAplikacija {
protected:
	void iscrtajZaglavlje() override {
		std::cout << "Iscrtavam zaglavlje za prikaz na desktop racunaru\n";
	}
	void iscrtajMeni() override {
		std::cout << "Iscrtavam meni za prikaz na desktop racunaru\n";
	}
	void iscrtajOsnovniProstor() override {
		std::cout << "Iscrtavam osnovni prostor za prikaz na desktop racunaru\n";
	}
};

int main() {
	GUIAplikacija* aplikacija1 = new MobilnaAplikacija();
	GUIAplikacija* aplikacija2 = new DesktopAplikacija();
	aplikacija1->iscrtaj();
	aplikacija2->iscrtaj();

	delete aplikacija1;
	delete aplikacija2;
	return 0;
}