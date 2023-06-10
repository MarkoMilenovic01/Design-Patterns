#include<iostream>

class Receiver {
public:
	void methodA() { std::cout << "methodA" << std::endl; }
	void methodB(std::string arg) { std::cout << "methodB\n\targ:" << arg << std::endl; }
	void methodC(int a, int b) { std::cout << "methodC\n\ta: " << a << "\n\tb: " << b << std::endl; }
};

class CommandBase {
protected:
	Receiver* receiver; // Ukoliko će sve komande da se izvršavaju nad istim tipom Receiver
	// (ili bar nad tipovima izvedenim iz iste klase Receiver),
	// apstraktna komanda može imati pokazivač (ili referencu) na klasu Receiver.
	// U opštem slučaju, konkretne komande mogu raditi sa različitim tipovima "primaoca" koji nemaju zajedničku
	// roditeljsku klasu
public:
	CommandBase(Receiver* receiver): receiver(receiver) {}
	virtual void execute() = 0;
	virtual ~CommandBase() {}
};

class CommandA : public CommandBase {
public:
	using CommandBase::CommandBase; // koristićemo isti konstruktor kakav ima i CommandBase. 
	// Da ne bismo sami kucali takav konstruktor naveli smo datu liniju
	void execute() override { receiver->methodA(); }
};

class CommandB : public CommandBase {
private:
	std::string arg;
public:
	CommandB(Receiver* r, const std::string& arg): CommandBase(r), arg(arg) {}
	void execute() override { receiver->methodB(arg); }
};

class CommandC : public CommandBase {
	int a, b; // Čuvamo i paramtere koje treba proslediti odgovarajućoj metodi
public:
	CommandC(Receiver* r, int a, int b): CommandBase(r), a(a), b(b) {}
	void execute() override { receiver->methodC(a, b); }
};

class Invoker {
private:
	CommandBase* command;
public:
	Invoker(CommandBase* command=nullptr):command(command) {}
	// Invoker može imati samo jednu komandu ili više njih. Primer kada Invoker ima samo jednu komandu je 
	// u slučaju da je Invoker dugme čijim pritiskom treba da se izvrši data komanda. 
	void setCommand(CommandBase* command) { this->command = command; }
	void executeCommand() {
		command->execute();
	}
};

int main() {
	Receiver* r = new Receiver();
	CommandBase* k1 = new CommandA(r);
	CommandBase* k2 = new CommandB(r, "bla bla");
	CommandBase* k3 = new CommandC(r, 1, 2);

	Invoker inv;
	inv.setCommand(k1);
	inv.executeCommand();

	inv.setCommand(k2);
	inv.executeCommand();

	inv.setCommand(k3);
	inv.executeCommand();

	return 0;
}