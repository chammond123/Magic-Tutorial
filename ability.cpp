#include "ability.h"

Ability::Ability(){};
Ability::Ability(std::function<void(Player*, Card*)> eff){}

// Activate method
void Ability::activate(Player *player, Card *target)
{
    if (effect) {
        effect(player, target);
    }
}

Ability Ability::addMana(int amount){
    return Ability( [amount](Player* player, Card*) {
        //Add mana to the player object
        // if (player) player->addMana(amount);
        // emit a signal to update the ui
    });
}

Ability Ability::buff(int amount){
    return Ability([amount](Player* , Card* target){

    });
}

Ability Ability::damageCreature(int amount){
    return Ability([amount](Player* , Card* target){
        if(target){
            //target->takeDamage(amount);
        }
    });
}

Ability Ability::damagePlayer(int amount){
    // add things
}

Ability Ability::heal(int amount){
    return Ability([amount](Player* player, Card*){
        if(player){
            // player->addHealth(amount);
        }
    });
}

Ability Ability::destroy(){
    return Ability([](Player*, Card* target){
        if(target){
            // target->destroy();
        }
    });
}


// void GameManager::activateCardAbility(Card& card, int abilityIndex, Player* player, Card* targetCard) {
//     if (abilityIndex >= 0 && abilityIndex < card.abilities.size()) {
//         Ability& ability = card.abilities[abilityIndex];
//         ability.activate(player, targetCard);
//     }
// }



