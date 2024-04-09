#ifndef POKER_CLASSES_H_
#define POKER_CLASSES_H_

#include <string>
#include <vector>

using namespace std;

class deckOfCards{
    private:
        int numberOfCards = 52;
        const string suits = "HCSD", faceCards = "TJQKA"; // T = 10
        vector<string> deck;
    public:
        void populateDeck();
        string drawCard();
        void printDeck();
};

class cpuOpponent{
    private:
        // action probability weights.  1 - Check/Call, 2 - Raise, 3 - Fold
        // action response takes player's last input as a param in addition to hand rank
        short pwLowRank[10] = {1,1,1,1,1,1,1,1,1,3};
        short pwMidRank[10] = {1,1,1,1,1,1,2,2,2,3};
        short pwHighRank[10] = {1,1,1,2,2,2,2,2,2,2};

        short pwLowRankRaised[10] = {1,1,1,3,3,3,3,3,3,3};
        short pwMidRankRaised[10] = {1,1,1,1,1,1,2,3,3,3};
        short pwHighRankRaised[10] = {1,1,1,2,2,2,2,2,2,3};
        
        int chips;
        int prevRaise = 0;
        int blindBet = 0; // 0 None, 1 Small, 2 Big
        string actionTaken = "";
    
        vector<string> hand = {};
    public:
        string chooseAction(int handRanking, string lastPlayerAction);
        void setHand(vector<string> newHand);
        vector<string> getHand();
        void updateChipTotal(int newChipTotal);
        int getChipTotal();
        void setBlindBet(int blindVal);    
        string getBlindBet();
        int getBlindBetInt();
        string getLastActionTaken();
        void setPrevRaise(int raise);
        void setActionTaken(string newAction);
};

class player{
    private:
        vector<string> hand = {};
        int chips;
        int blindBet = 0; // 0 None, 1 Small, 2 Big
        int pot = 0;
    public:
        void setHand(vector<string> newHand);
        vector<string> getHand();
        void setBlindBet(int blindVal);    
        string getBlindBet();
        int getBlindBetInt();
        void updateChipTotal(int newTotal);
        int getChipTotal();
        void setPotVal(int potVal);
        int getPot();
};

class settingsHolder{
    private:
        int numberOfCpu = 3;
        int startingChips = 10000;
        int smallBlindValue = 250;
    public:
        void setNumberOfCpu(int numberOfCpu);
        int getNumberOfCpu();
        void setStartingChips(int startingChips);
        int getStartingChips();
        void setSmallBlindValue(int smallBlindValue);
        int getSmallBlindValue();
};

#endif // POKER_CLASSES_H_
