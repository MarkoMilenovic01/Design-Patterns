#ifndef __GRADITELJ_H
#define __GRADITELJ_H

#include "Proizvod.h"

/* graditelj / builder proizvoda */
class Graditelj {
private:
    Proizvod proizvod;
public:
    static const int    DEFAULT_INT_SVOJSTVO;
    static const float  DEFAULT_FLOAT_SVOJSTVO;
    static const char   DEFAULT_CHAR_SVOJSTVO;

    Graditelj() {
        proizvod = Proizvod(DEFAULT_INT_SVOJSTVO,DEFAULT_FLOAT_SVOJSTVO,DEFAULT_CHAR_SVOJSTVO);
    }

    Graditelj& postaviIntSvojstvo(const int intSvojstvo) {
        proizvod.postaviIntSvojstvo(intSvojstvo);
        return *this;
    }
    Graditelj& postaviFloatSvojstvo(const float floatSvojstvo) {
        proizvod.postaviFloatSvojstvo(floatSvojstvo);
        return *this;
    }
    Graditelj& postaviCharSvojstvo(const char charSvojstvo) {
        proizvod.postaviCharSvojstvo(charSvojstvo);
        return *this;
    }

    Graditelj& postaviProizvod() {
        proizvod = Proizvod(DEFAULT_INT_SVOJSTVO, DEFAULT_FLOAT_SVOJSTVO, DEFAULT_CHAR_SVOJSTVO);
        return *this;
    }

    Proizvod napraviProizvod() {
        return proizvod;
    }
};

const int Graditelj::DEFAULT_INT_SVOJSTVO = 5;
const float Graditelj::DEFAULT_FLOAT_SVOJSTVO = 1.4141f;
const char Graditelj::DEFAULT_CHAR_SVOJSTVO = 'X';

#endif // __GRADITELJ_H
