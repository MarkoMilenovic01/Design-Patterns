#include<iostream>
#include<sstream>
#include<vector>
#include<fstream>

class Osoba {
protected:
	std::string ime;
	std::string prezime;
	std::string mesto;
	int godRodjenja;

public:
	Osoba() = default;
	Osoba(std::string ime, std::string prezime, int godRodjenja, std::string mesto):
		ime(ime), prezime(prezime), mesto(mesto), godRodjenja(godRodjenja) {}
	friend std::ostream& operator << (std::ostream& stream, const Osoba& osoba);
	friend std::istream& operator >> (std::istream& stream, Osoba& osoba);
};

std::ostream& operator << (std::ostream& stream, const Osoba& osoba) {
	stream << osoba.ime << "," << osoba.prezime << "," << osoba.godRodjenja << "," << osoba.mesto;
	return stream;
}

std::istream& operator >> (std::istream& stream, Osoba& osoba) {
	stream >> osoba.ime >> osoba.prezime >> osoba.godRodjenja >> osoba.mesto;
	return stream;
}

class Student : public Osoba {
protected:
	float prosecnaOcena;
public:
	Student() = default;
	Student(std::string ime, std::string prezime, int godRodjenja, std::string mesto, float prosecnaOcena):
		Osoba(ime, prezime, godRodjenja, mesto), prosecnaOcena(prosecnaOcena) {}

	friend std::ostream& operator << (std::ostream&, const Student&);
	friend std::istream& operator >> (std::istream&, Student&);
};

std::ostream& operator << (std::ostream& stream, const Student& student) {
	Osoba tmp = student;
	stream << tmp << "," << student.prosecnaOcena;
	return stream;
}

std::istream& operator >> (std::istream& stream, Student& student) {
	stream >> student.ime >> student.prezime >> student.godRodjenja >> student.mesto >> student.godRodjenja;
	return stream;
}

void test_cin_cout() {
	Osoba o1("pera", "peric", 1990, "Nis");
	Osoba o2;
	Student s("zika", "zikic", 1991, "Nis", 6.69f);
	std::cin >> o2;
	std::cout << o1 << std::endl;
	std::cout << o2 << std::endl;
	std::cout << s << std::endl;
}

int main(int argc, char ** argv) {
	std::string filename;
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		filename = "studenti.txt";
	}

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Problem sa otvaranjem fajla!" << std::endl;
		exit(1);
	}
	
	std::string linija;
	std::stringstream studentLinija;
	std::vector<Student> studenti;
	char separator = ',';
	while (!file.eof()) {
		std::getline(file, linija); 
		studentLinija.clear();
		studentLinija << linija;
		
		std::string ime, prezime, mesto;
		int godina;
		float prosek;
		std::string tmp;

		std::getline(studentLinija, ime, separator);
		std::getline(studentLinija, prezime, separator);
		std::getline(studentLinija, tmp, separator);
		godina = std::stoi(tmp);
		std::getline(studentLinija, mesto, separator);
		std::getline(studentLinija, tmp, separator);
		prosek = std::stof(tmp);
		studenti.push_back(Student(ime, prezime, godina, mesto, prosek));	
	}

	std::ofstream outputFile("izlaz.txt");
	for (int i = 0; i < studenti.size(); ++i) {
		Student s = studenti[i];
		std::cout << s << std::endl;
		outputFile << s << std::endl;
	}

	/*for (Student s : studenti) {
		std::cout << s << std::endl;
		outputFile << s << std::endl;
	}*/
	file.close();
	outputFile.close();
	return 0;
}