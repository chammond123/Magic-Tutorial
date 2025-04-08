#include "zone.h"
#include "card.h"
#include <algorithm>
#include <chrono>

Zone::Zone() {

    // Initialize random engine
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    randomEngine = std::default_random_engine(seed);



}

void Zone::intialize(const QVector<Card>& cards) {

}

void Zone::setVisibility(bool visibility) {

}

void Zone::shuffle() {


}


