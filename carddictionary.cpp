// In carddictionary.cpp
#include "carddictionary.h"

// Definition of the static member
std::map<QString, Card> cardDictionary::dict;

void cardDictionary::addCard(Card card) {
    giveAbility(&card);
    dict[card.name] = card;
}

Card cardDictionary::getCard(QString cardName) {
    return dict[cardName];
}

void cardDictionary::giveAbility(Card* c){
    if (c->name == "Divination"){
        c->setAbility(Ability::drawCards(2));
    }

    if (c->name == "Mountain"){
        c->setAbility(Ability::addMana(1, ManaType::RED));
    }

    if (c->name == "Island"){
        c->setAbility(Ability::addMana(1, ManaType::BLUE));
    }

    if (c->name == "Black Lotus"){
        c->setAbility(Ability::addMana(3, ManaType::ANY));
    }

    if (c->name == "Shock"){
        c->setAbility(Ability::damageTarget(2));
        c->needsTarget = true;
    }

    if (c->name == "Lightning Bolt"){
        c->setAbility(Ability::damageTarget(3));
        c->needsTarget = true;
    }

    if (c->name == "Counterspell"){
        c->setAbility(Ability::counter());
        c->needsTarget = true;
    }

    if (c->name == "Fervor"){
        c->setAbility(Ability::bypassSummonSickness());
    }

}
