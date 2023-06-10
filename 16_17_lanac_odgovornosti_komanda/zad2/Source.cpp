#include<iostream>
#include<unordered_set>
#include<unordered_map>

/*
U ovom primeru videćemo situaciju u kojoj redosled handler-a ima uticaj na uspešnost i efikasnost razrešenja
zahteva. Ovde zahtev neće imati oznaku kojom odlučujemo da li određeni handler može da ga obradi, već će
svaki handler na osnovu svoje logike i samog zahteva odlučiti da li može da obradi dati zahtev.
*/

class WebPage {
public:
	std::string url;
	WebPage(const std::string& url): url(url) {}
};

class InternetServiceHandler {
protected:
	InternetServiceHandler* next;
public:
	virtual WebPage* getPage(const std::string& url, const std::string& ip) {
		// Često se osnovna klasa handler-a implementira tako da metoda za obradu zahteva nije apstraktna, već
		// da ima zadatak samo da prosledi zahtev dalje. Sada je zadatak izvedenih klasa da obrade zahtev ukoliko mogu, 
		// a u suprotnom treba da proslede zahtev osnovnoj klasi. Tako u izvedenoj klasi potpuno "krijemo" da uopšte postoje
		// drugi handler-i, tj. sledeći handler.
		if (next) { return next->getPage(url, ip); }
		return nullptr;
	}
	void setNext(InternetServiceHandler* handler) { this->next = handler; }
	virtual ~InternetServiceHandler() { delete next; }
};

class DownloadPageInternetServiceHandler : public InternetServiceHandler {
public:
	WebPage* getPage(const std::string& url, const std::string& ip) { 
		return new WebPage(url); 
	}
};

class RestrictedAccessInternetServiceHandler : public InternetServiceHandler {
protected:
	std::unordered_set<std::string> ips;
public:
	RestrictedAccessInternetServiceHandler(std::unordered_set<std::string>& ips): ips(ips) {}
	WebPage* getPage(const std::string& url, const std::string& ip) {
		if (ips.find(ip) == ips.end()) { 
			return InternetServiceHandler::getPage(url, ip);
		}
		else {
			return nullptr;
		}
	}
};

class CacheInternetServiceHandler : public InternetServiceHandler {
protected:
	std::unordered_map<std::string, WebPage*> cache;
public:
	WebPage* getPage(const std::string& url, const std::string& ip) {
		if (cache.find(url) == cache.end()) {
			WebPage* page = InternetServiceHandler::getPage(url, ip);
			if (page != NULL)
				cache[url] = page;
			return page;
		} else {
			std::cout << "Vracam kesiran sadrzaj sa adrese " << url << std::endl;
			return cache[url];
		}
	}
};

int main() {
	std::unordered_set<std::string>ips;
	ips.insert("12.32.43.54");
	ips.insert("12.32.43.154");

	InternetServiceHandler* handler1 = new RestrictedAccessInternetServiceHandler(ips);
	InternetServiceHandler* handler2 = new CacheInternetServiceHandler();
	InternetServiceHandler* handler3 = new DownloadPageInternetServiceHandler();
	handler2->setNext(handler3);
	handler1->setNext(handler2);

	WebPage* p1 = handler1->getPage("www.nesto.com", "12.32.43.154");
	if (p1) {
		std::cout << "Uspesno preuzet sadrzaj sa adrese " << p1->url << std::endl;
	}
	else {
		std::cout << "Neuspesno preuzet sadrzaj sa adrese www.nesto.com i preko ip adrese 12.32.43.154" << std::endl;
	}

	WebPage* p2 = handler1->getPage("www.nesto.com", "12.32.43.1");
	if (p2) {
		std::cout << "Uspesno preuzet sadrzaj sa adrese " << p2->url << std::endl;
	}
	else {
		std::cout << "Neuspesno preuzet sadrzaj sa adrese www.nesto.com i preko ip adrese 12.32.43.154" << std::endl;
	}

	WebPage* p3 = handler1->getPage("www.nesto.com", "12.32.43.1");
	if (p3) {
		std::cout << "Uspesno preuzet sadrzaj sa adrese " << p3->url << std::endl;
	}
	else {
		std::cout << "Neuspesno preuzet sadrzaj sa adrese www.nesto.com i preko ip adrese 12.32.43.154" << std::endl;
	}
	return 0;
}