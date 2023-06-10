#include<iostream>
#include<fstream>
#include<unordered_map>

class JsonElement {
public:
	virtual void exportToFile(std::ofstream&, int) = 0;
	virtual ~JsonElement() {}
};

template<typename T>
class SimpleJsonValue: public JsonElement {
	// Ideja je da ovim šablonom klase pokrijemo tipove string, null, bool, broj.
	// Ovo će biti listovi kompozita.
	T value;
public:
	SimpleJsonValue(T value): value(value) {}
	void exportToFile(std::ofstream& file, int indentLevel) override {
		//constexpr je oznaka za izraz čija je vrednost poznata u vreme
		// kompajliranja. Rezultat ovog grananja u instanciranim klasama neće biti
		// kod koji zaista sadrži grananje, već će svaka konkretna klasa imati samo jednu
		// liniju koda
		// std::is_same proverava da li su dva prosleđena tipa ista.
		if constexpr(std::is_same<T, std::string>::value)
			file << "\"" << value << "\"";
		else if constexpr(std::is_same<T, std::nullptr_t>::value)
			file << "null";
		else if constexpr(std::is_arithmetic<T>::value) 
			file << value;
		else if constexpr(std::is_same<T, bool>::value)
			file << ((bool)value ? "true" : "false");
	}
};

class JsonObject : public JsonElement {
	// Jedna od kompozit klasa. Svaki json objekat ima parove ključ-vrednost.
	// Vrednosti su Json komponente (ovde nazvane JsonElement) koje mogu biti 
	// proste (listovi) ili kompozitne (drugi json objekti ili liste json objekata).
	std::unordered_map<std::string, JsonElement*> values;
public:
	void addElement(std::string key, JsonElement* value) {
		values[key] = value;
	}
	void exportToFile(std::ofstream& file, int indentLevel) override {
		// indentLevel je tu da bismo sve ugnježdene json elemente štampali za po
		// jedan tabulator više uvučeno. 
		std::string indentation = "";
		for (int i = 0; i < indentLevel; ++i)
			indentation += "\t";
		file << indentation << "{" << std::endl;
		indentation += "\t";
		int size = values.size();
		int i = 0;
		for (auto it = values.begin(); it != values.end(); it++) {
			file << indentation << "\"" << it->first << "\": ";
			it->second->exportToFile(file, indentLevel + 1);
			if (i == size - 1)
				file << std::endl;
			else
				file << "," << std::endl;
			i++;
		}
		for (int i = 0; i < indentLevel; i++)
			file << "\t";
		file << "}";
	}
	~JsonObject() {
		for (auto it = values.begin(); it != values.end(); ++it) {
			delete it->second;
		}
		values.clear();
	}
};

class JsonArray : public JsonElement {
	// Druga kompozit klasa. Niz moze sadrzati elemente razlicitog tipa, pa i druge nizove
	// ili Json objekte.
	std::vector<JsonElement*> jsonArray;
public:
	JsonArray(std::initializer_list<JsonElement*> initList) {
		// korisna stvar od C++ 11 je lista za inicijalizaciju.
		// Omogucava da u konstruktoru prosledimo niz elemenata zadat između
		// viticastih zagrada. Videti korišćenje u main funkciji. 
		// Liste za inicijalizaciju su se toliko "odomaćile" da mnoge strukture,
		// uključujući i std::vector, mogu da se kreiraju na osnovu prosleđene liste. 
		jsonArray = std::vector<JsonElement*>(initList);
	}

	void addElement(JsonElement* newElement) { this->jsonArray.push_back(newElement); }
	void exportToFile(std::ofstream& file, int indentLevel) override {
		file << "[" << std::endl;
		std::string indentation;
		for (int i = 0; i < indentLevel+1; ++i) {
			indentation += "\t";
		}

		for (int i = 0; i < jsonArray.size(); ++i) {
			file << indentation;
			jsonArray[i]->exportToFile(file, indentLevel + 1);
			std::string comma = i < jsonArray.size() - 1 ? "," : "";
			file << comma << std::endl;
		}
		for (int i = 0; i < indentLevel; i++)
			file << "\t";
		file << "]";
	}

	~JsonArray() {
		for (auto it = jsonArray.begin(); it != jsonArray.end(); it++)
			delete* it;
		jsonArray.clear();
	}
};

int main() {
	std::ofstream file("test.json");
	JsonObject* o1 = new JsonObject();
	o1->addElement("a", new SimpleJsonValue<int>(32));
	o1->addElement("b", new SimpleJsonValue<bool>(true));
	o1->addElement("c", new SimpleJsonValue<std::nullptr_t>(NULL));
	JsonObject* o2 = new JsonObject();
	o2->addElement("jsonObj", o1);
	o2->addElement("testArray", new JsonArray({
		new SimpleJsonValue<bool>(false),
		new SimpleJsonValue<int>(93),
		new SimpleJsonValue<std::string>("test_string")
		}));
	o2->addElement("d", new SimpleJsonValue<std::string>("bla bla"));

	o2->exportToFile(file, 0);
}