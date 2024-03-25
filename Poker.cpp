#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"
#include "unordered_map"
#include "PokerLogic.cpp"
#include "Graphics.cpp"

using namespace std;

class deckOfCards{
    private:
        int numberOfCards = 52;
        const string suits = "HCSD", faceCards = "TJQKA"; // T = 10, included for formatting later
        vector<string> deck;
    public:
        void populateDeck(){
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
        string drawCard(){
            if (deck.size() == 0){
                populateDeck();
            }
            string drawnCard = deck.back();
            deck.pop_back();
            return drawnCard;
        }

        void printDeck(){
            cout << "Number of Cards: " << deck.size() << "\n";
            for (int i = 0; i < deck.size(); i++)
                cout << deck[i] << " ";
            cout << "\n";
        }
};

class cpuOpponent{
    private:
        // action probability weights.  1 - Check/Call, 2 - Raise, 3 - Fold
        // action response takes player's last input as a param in addition to hand rank
        short pwLowRank[10] = {1,1,1,1,1,1,1,3,3,3};
        short pwMidRank[10] = {1,1,1,1,1,1,2,2,2,3};
        short pwHighRank[10] = {1,1,1,2,2,2,2,2,2,2};

        short pwLowRankRaised[10] = {1,1,1,3,3,3,3,3,3,3};
        short pwMidRankRaised[10] = {1,1,1,1,1,1,2,3,3,3};
        short pwHighRankRaised[10] = {1,1,1,2,2,2,2,2,2,3};
        
        int chips;

        vector<string> hand = {};

        // a random number for aggressiveness in constructor?
    public:
        // ex. cpuActionTaken = cpu1.chooseAction(rankHand(cpuHand + communityCards), playerInput);
        string chooseAction(int handRanking, string lastPlayerAction){
            short actionChosen;

            if (lastPlayerAction == "raise"){
                if (handRanking < 3){
                    int random = rand() % 10;
                    actionChosen = pwLowRankRaised[random];
                }
                else if (handRanking >= 3 && handRanking <= 6){
                    int random = rand() % 10;
                    actionChosen = pwMidRankRaised[random];
                }
                else if (handRanking > 6){
                    int random = rand() % 10;
                    actionChosen = pwHighRankRaised[random];
                }
            }
            else {
                if (handRanking < 3){
                    int random = rand() % 10;
                    actionChosen = pwLowRank[random];
                }
                else if (handRanking >= 3 && handRanking <= 6){
                    int random = rand() % 10;
                    actionChosen = pwMidRank[random];
                }
                else if (handRanking > 6){
                    int random = rand() % 10;
                    actionChosen = pwHighRank[random];
                }
            }

            switch (actionChosen){
                case 1:
                    return "call";
                case 2:
                    return "raise";
                case 3:
                    return "fold";
            }
            return "NULL";
        }
        void setHand(vector<string> newHand){
            hand = newHand;
        }
        vector<string> getHand(){
            return hand;
        }
        void updateChipTotal(int newChipTotal){
            chips = newChipTotal; 
        }
        int getChipTotal(){
            return chips;
        }      
};

class player{
    private:
        vector<string> hand = {};
        int chips;
    public:
        void setHand(vector<string> newHand){
            hand = newHand;
        }
        vector<string> getHand(){
            return hand;
        }
        void updateChips(int newTotal){
            chips = newTotal;
        }
        int getChips(){
            return chips;
        }
};


//Template play game function for testing.  Calls functions.  Should handle game operation.
void playGame(){
    srand((unsigned) time(NULL));

    deckOfCards deck1;
    deck1.populateDeck();

    vector<string> testHand = {};
    // for game evaluations, simply add both the player hand vector and the community cards vector together then pass to eval functions
    cout << "Drawing 7 cards\n";
    for (int i = 0; i < 7; i++){
        auto card = deck1.drawCard();
        testHand.push_back(card);
    }

    cout << "Test Hand Vector: ";
    cout << createCardSprite(testHand);

    vector<string> testHand2 = {"4D", "2C", "8S", "AD", "TD", "6C", "3S"};

    for (auto x : testHand) 
        cout << x << " "; 

    cout << "\nFlush:" << isFlush(testHand);
    cout << "\nStraight:" << isStraight(testHand);
    cout << "\nOf A Kind:" << ofAKind(testHand) << endl;
    cout << "Hand Ranking: " << rankHand(testHand) << endl;

    //testing createScreen
    vector<string> testPlayerHand = {"AS", "AH"};
    vector<string> testCommunityCards = {"5H", "6C", "5S", "KC", "4D"}; 
    createScreen(testPlayerHand, testCommunityCards, 1);
}

int main(){
    playGame();
}
