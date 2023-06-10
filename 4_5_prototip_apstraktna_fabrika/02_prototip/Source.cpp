#include<iostream>
#include<unordered_map>

/*
Implementirati klase:
a. IButton koja od atributa ima tekst, boju pozadine (zadaje se kao string oblika #rrggbb) i
čisto virtuelnu funkciju opis().
b. Button koja nasleđuje IButton i od dodatnih atributa ima širinu i visinu.
c. RadioButton koja nasleđuje IButton i od dodatnih atributa ima id (tipa string) 
grupe kojoj pripada.
d. CheckBox koja nasleđuje klasu IButton i od dodatnih atributa ima bool checked.
Omogućiti da kloniranje ovih klasa i dobijanje novih objekata pozivom metode/funkcije 
oblika getButton(ButtonType) koja će vratiti kopiju postojeg prototipa dugmeta datog tipa. 
getButton treba da prvo proveri da li postoji dugme tipa ButtonType i da izvrši kloniranje
ukoliko postoji, a ukoliko ne postoji da pre kloniranja napravi prototip datog tipa dugmeta.
*/

enum class ButtonType {BUTTON, RADIO_BUTTON, CHECK_BOX};

class IButton {
protected:
	std::string text;
	std::string backgroundColor;
	static std::unordered_map<ButtonType, IButton*> prototypes;
public:
	IButton(std::string text, std::string backgroundColor): 
		text(text), backgroundColor(backgroundColor) {}
	void setText(std::string text) {
		this->text = text;
	}
	void setBackgroundColor(std::string backgroundColor) {
		this->backgroundColor = backgroundColor;
	}
	virtual IButton* clone() = 0;
	virtual void describe() = 0;

	static IButton* getButton(ButtonType type);
};

std::unordered_map<ButtonType, IButton*> IButton::prototypes 
	= std::unordered_map<ButtonType, IButton*>();

class Button : public IButton {
protected:
	int width;
	int height;
public:
	Button(std::string text, std::string bgColor, int height, int width):
		IButton(text, bgColor), height(height), width(width) {}
	IButton* clone() {
		return new Button(this->text, this->backgroundColor, height, width);
	}
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void describe() {
		std::cout << "Button: " << text << ", bgColor = " << backgroundColor
			<< ", " << height << "x" << width << std::endl;
	}
};

class RadioButton : public IButton {
protected:
	std::string radioGroupId;
public:
	RadioButton(std::string text, std::string bgColor, std::string groupId) :
		IButton(text, bgColor), radioGroupId(groupId) {}
	IButton* clone() {
		return new RadioButton(this->text, this->backgroundColor, radioGroupId);
	}
	void setRadioGroupId(std::string id) {
		this->radioGroupId = id;
	}
	void describe() {
		std::cout << "RadioButton: " << text << ", bgColor = " << backgroundColor
			<< ", radioGroupId = " << radioGroupId << std::endl;
	}
};

class CheckBox : public IButton {
protected:
	bool checked;
public:
	CheckBox(std::string text, std::string bgColor, bool checked = false) :
		IButton(text, bgColor), checked(checked) {}
	IButton* clone() {
		return new CheckBox(this->text, this->backgroundColor, checked);
	}
	void setChecked(bool checked) {
		this->checked = checked;
	}
	void describe() {
		std::cout << "CheckBox: " << text << ", bgColor = " << backgroundColor
			<< ", checked = " << checked << std::endl;
	}
};

IButton* IButton::getButton(ButtonType type) {
	if (prototypes.find(type) == prototypes.end()) {
		switch (type) {
		case ButtonType::BUTTON:
			std::cout << "Kreiram prototip tipa Button" << std::endl;
			prototypes[type] = new Button("", "#BBBBBB", 10, 30);
			break;
		case ButtonType::CHECK_BOX:
			std::cout << "Kreiram prototip tipa CheckBox" << std::endl;
			prototypes[type] = new CheckBox("", "#BBBBBB");
			break;
		case ButtonType::RADIO_BUTTON:
			std::cout << "Kreiram prototip tipa RadioButton" << std::endl;
			prototypes[type] = new RadioButton("", "#BBBBBB", "");
			break;
		}
	}

	return prototypes[type]->clone();
}

int main() {
	Button* b1 = (Button*)IButton::getButton(ButtonType::BUTTON);
	b1->setText("Button 1");
	Button* b2 = (Button*)IButton::getButton(ButtonType::BUTTON);
	b2->setText("Button 2");
	b2->setWidth(60);

	b1->describe();
	b2->describe();
}