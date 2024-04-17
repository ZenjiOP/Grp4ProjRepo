// Written by JHoulihan
#include "Poker_Classes.h"
#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"

using namespace std;

void deckOfCards::populateDeck(){
    // populate deck with cards, CALL FIRST
    for (int i = 0; i < 4; i++){
        for (int j = 2; j < 10; j++){
            deck.push_back(to_string(j) + suits[i]);
        }
        for (int j = 0; j <= 4; j++){
            string cardToInsert = "";
            cardToInsert.push_back(faceCards[j]); cardToInsert.push_back(suits[i]);
            deck.push_back(cardToInsert);
        }
    }
    // shuffle
    auto rd = random_device {}; 
    auto rng = default_random_engine { rd() };
    shuffle(begin(deck), end(deck), rng);
}

// returns a 2 character string in format "AS" (AceSpades / ValueSuit)
string deckOfCards::drawCard(){
    if (deck.size() == 0){
        populateDeck();
    }
    string drawnCard = deck.back();
    deck.pop_back();
    return drawnCard;
}

void deckOfCards::printDeck(){
    cout << "Number of Cards: " << deck.size() << "\n";
    for (int i = 0; i < deck.size(); i++)
        cout << deck[i] << " ";
    cout << "\n";
}


string cpuOpponent::chooseAction(int handRanking, string lastPlayerAction){
    short actionChosen;

    if (lastPlayerAction == "raise"){
        if (handRanking < 3){
            int random = rand() % 10;
            actionChosen = pwLowRankRaised[random];
        } else if (handRanking >= 3 && handRanking <= 6){
            int random = rand() % 10;
            actionChosen = pwMidRankRaised[random];
        } else if (handRanking > 6){
            int random = rand() % 10;
            actionChosen = pwHighRankRaised[random];
        }
    } else {
        if (handRanking < 3){
            int random = rand() % 10;
            actionChosen = pwLowRank[random];
        } else if (handRanking >= 3 && handRanking <= 6){
            int random = rand() % 10;
            actionChosen = pwMidRank[random];
        } else if (handRanking > 6){
            int random = rand() % 10;
            actionChosen = pwHighRank[random];
        }
    }

    switch (actionChosen){
        case 1:
            actionTaken = "call";
            prevRaise = 0;
            return "call";
        case 2:
            actionTaken = "raise";
            return "raise";
        case 3:
            actionTaken = "fold";
            prevRaise = 0;
            return "fold";
    }
    return "NULL";
}

void cpuOpponent::setHand(vector<string> newHand){
    hand = newHand;
}

vector<string> cpuOpponent::getHand(){
    return hand;
}

void cpuOpponent::updateChipTotal(int newChipTotal){
    chips = newChipTotal; 
}

int cpuOpponent::getChipTotal(){
    return chips;
}

void cpuOpponent::setBlindBet(int blindVal){
    blindBet = blindVal;
}   

string cpuOpponent::getBlindBet(){
    switch (blindBet){
        case 0:
            return "   ";
        case 1:
            return "SML";
        case 2:
            return "BIG";
    }
    return "   ";
}

int cpuOpponent::getBlindBetInt(){
    return blindBet;
}

string cpuOpponent::getLastActionTaken(){
    if (prevRaise != 0)
        return actionTaken + " - " + to_string(prevRaise);
    return actionTaken;
}

void cpuOpponent::setPrevRaise(int raise){
    prevRaise = raise;
}

void cpuOpponent::setActionTaken(string newAction){
    actionTaken = newAction;
}


void player::setHand(vector<string> newHand){
    hand = newHand;
}

vector<string> player::getHand(){
    return hand;
}

void player::setBlindBet(int blindVal){
    blindBet = blindVal;
}

string player::getBlindBet(){
    switch (blindBet){
        case 0:
            return "   ";
        case 1:
            return "SML";
        case 2:
            return "BIG";
    }
    return "   ";
}

int player::getBlindBetInt(){
    return blindBet;
}

void player::updateChipTotal(int newTotal){
    chips = newTotal;
}

int player::getChipTotal(){
    return chips;
}

void player::setPotVal(int potVal){
    pot = potVal;
}

int player::getPot(){
    return pot;
}

void settingsHolder::setNumberOfCpu(int numberOfCpu){
    this->numberOfCpu = numberOfCpu;
}

int settingsHolder::getNumberOfCpu(){
    return numberOfCpu;
}

void settingsHolder::setStartingChips(int startingChips){
    this->startingChips = startingChips;
}

int settingsHolder::getStartingChips(){
    return startingChips;
}

void settingsHolder::setSmallBlindValue(int smallBlindValue){
    this->smallBlindValue = smallBlindValue;
}

int settingsHolder::getSmallBlindValue(){
    return smallBlindValue;
}
