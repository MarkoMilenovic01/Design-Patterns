#include<iostream>
#include<unordered_map>
#include<string>

class Flyweight {
public:
	Flyweight() = default;
	virtual void operation(std::string& extrinsicState) = 0;
	virtual ~Flyweight() {}
};

class ConcreteFlyweight : public Flyweight {
	std::string intrinsicState;
public:
	ConcreteFlyweight(const std::string& intrinsicState) : intrinsicState(intrinsicState) {
		std::cout << "Created new ConcreteFlyweight object with intrisicState == " << intrinsicState << std::endl;
	}
	void operation(std::string& extrinsicState) override {
		std::cout << "Executing operaton from ConcreteFlyweight." << std::endl;
		std::cout << "\tintrinsicState:  " << intrinsicState << std::endl;
		std::cout << "\textrinsicState:  " << extrinsicState << std::endl << std::endl;
	}
};

class UnsharedConcreteFlyweight : public Flyweight {
	std::string allState;
public:
	UnsharedConcreteFlyweight(const std::string& allState) : allState(allState) {
		std::cout << "Created new UnsharedConcreteFlyweight object with allState == " << allState << std::endl;
	}
	void operation(std::string& extrinsicState) override {
		std::cout << "Executing operaton from ConcreteFlyweight." << std::endl;
		std::cout << "\tintrinsicState:  " << allState << std::endl;
		std::cout << "\textrinsicState:  " << extrinsicState << std::endl << std::endl;
	}
};

class FlyweightFactory {
private:
	FlyweightFactory() {}
	FlyweightFactory& operator=(const FlyweightFactory&) = default;
	FlyweightFactory(const FlyweightFactory&) = default;
public:
	static FlyweightFactory& getInstance() {
		static FlyweightFactory instance;
		return instance;
	}
	Flyweight* getFlyweight(const std::string& intrinsicState) {
		if (exsistingFlyweights.find(intrinsicState) == exsistingFlyweights.end()) {
			exsistingFlyweights[intrinsicState] = new ConcreteFlyweight(intrinsicState);
		}
		return exsistingFlyweights[intrinsicState];
	}

private:
	std::unordered_map<std::string, Flyweight*> exsistingFlyweights;
};

int main() {
	std::vector<Flyweight*> flyweights;

	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("a"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("b"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("c"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("a"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("a"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("b"));
	flyweights.push_back(FlyweightFactory::getInstance().getFlyweight("a"));

	flyweights.push_back(new UnsharedConcreteFlyweight("d"));
	
	int br = 0;
	for (auto it = flyweights.begin(); it != flyweights.end(); ++it) {
		std::string tmp = std::to_string(br);
		(*it)->operation(tmp);
		br++;
	}

	return 0;
}