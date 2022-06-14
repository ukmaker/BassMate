#ifndef BM_SELECT_WRAPPERS_H
#define BM_SELECT_WRAPPERS_H
#include "VS1053/Voice.h"
#include "Widgets/WrapperSelectWidget.h"



using namespace bassmate;

class VoiceWrapper : public Wrapper<Voice> {

    public:

    VoiceWrapper() {}

    const char *getText(Voice arg) {
        return arg.name;
    }

    bool equals(Voice a, Voice b) {
        return a.id == b.id;
    }

};

class FamilyWrapper : public Wrapper<Family> {

    public:

    FamilyWrapper() {}

    const char *getText(Family arg) {
        return arg.name;
    }
    bool equals(Family a, Family b) {
        return a.id == b.id;
    }

};

class BankWrapper : public Wrapper<Bank> {

    public:

    BankWrapper() {}

    const char *getText(Bank arg) {
        return arg.name;
    }
    bool equals(Bank a, Bank b) {
        return a.id == b.id;
    }

};


#endif