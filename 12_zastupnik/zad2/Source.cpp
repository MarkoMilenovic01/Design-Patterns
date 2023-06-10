#include<iostream>
#include<vector>

class InternetPage {};

class InternetService {
public:
	virtual InternetPage* getPage(const std::string& url) = 0;
	virtual ~InternetService() {}
};

class RealInternetService : public InternetService {
public:
	InternetPage* getPage(const std::string& url) override {
		std::cout << "Downloading page from " << url << std::endl;
		return new InternetPage();
	}
};

class SecuredInternetService : public InternetService {
private:
	std::vector<std::string> urls;
	RealInternetService* realInternetService;
public:
	SecuredInternetService(std::vector<std::string>& urls) : urls(urls),
	realInternetService(new RealInternetService()) {}
	~SecuredInternetService() { delete realInternetService; }
	InternetPage* getPage(const std::string& url) override {
		bool allowAccess = true;
		for (int i = 0; i < urls.size(); i++) {
			if (url.find(urls[i]) != -1) {
				allowAccess = false;
				break;
			}
		}
		if (allowAccess) {
			return realInternetService->getPage(url);
		}
		else {
			std::cout << "Cannot access page " << url << std::endl;
			return nullptr;
		}
	}
};

int main() {
	std::vector<std::string> blockedPages;
	blockedPages.push_back("abc.def");
	blockedPages.push_back("iop.jkl");
	InternetService* internetService = new SecuredInternetService(blockedPages);
	InternetPage* p1 = internetService->getPage("https://abc.def.tyu.com");
	InternetPage* p2 = internetService->getPage("https://qwe.ret.tyu.com");
	InternetPage* p3 = internetService->getPage("https://iop.jkl.tyu.com");

	return 0;
}