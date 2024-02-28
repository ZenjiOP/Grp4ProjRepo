#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"
#include "unordered_map"

using namespace std;

class deckOfCards{
    private:
        int numberOfCards = 52;
        const string suits = "HCSD", faceCards = "TJQKA"; // T = 10, included for formatting later
        vector<string> deck;
    public:
        void populateDeck(){
            // populate deck with cards
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

        string drawCard(){
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

// Card Sprite Creation, to Render Cards this will likely need to be adjusted
string createCardSprite(string cardInfo){
    // replace 1,1 and 5,6
    // 10 index problem fix
    string cardBases[4][6] = {
        {{" _____ "},
        {"|A .  |"},
        {"| /.\\ |"},
        {"|(_._)|"},
        {"|  |  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A .  |"},
        {"| / \\ |"},
        {"| \\ / |"},
        {"|  v  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A _  |"},
        {"| ( ) |"},
        {"|(_'_)|"},
        {"|  |  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A_ _ |"},
        {"|( v )|"},
        {"| \\ / |"},
        {"|  v  |"},
        {"|____A|"}}
    };
    
    string cardSprite = "", cardSuitIndex = "SDCH";
    string cardValue = ""; cardValue.push_back(cardInfo[0]);
    char cardSuit = cardInfo[1];
    int cardBaseIndex;
    cardBaseIndex = cardSuitIndex.find(cardSuit);

    //if (cardValue == "T") {cardValue = "10";}

    for (int i = 0; i < 6; i++){
        string cardLine = cardBases[cardBaseIndex][i];
        if (i == 1 | i == 5) {cardLine.replace(i, 1, cardValue);}
        cardSprite.append(cardLine);
        cardSprite += "\n";
    }   

    return cardSprite;
}

// clears screen, for display later.
void clearScreen(int numOfLines){
    for (int i = 0; i < numOfLines; i++){
        // VT100 Escape Codes: "[2k":Clear line "[A":Move up a line
        cout << "\33[2K\r\033[A"; // delete line, move up a line
    }
    cout << "\x1b[2K"; // delete current line
}

//Poker Logic, each function takes a vector of Strings in format "SV" (Suit/Value)
//Flush - remove card values, check if all suits are the same.  Save highest number for tiebreaking.
bool isFlush(vector<string> hand){
    // reverse value and suit
    string tempHandArr[7];
    for (int i = 0; i < hand.size(); i++){
        tempHandArr[i] = hand[i][1];
    }
    // sort
    sort(begin(tempHandArr), end(tempHandArr));
    cout << "\nsorted: ";
    for (int i = 0; i < 7; i++){cout << tempHandArr[i] << " ";}
    cout << "\n";
    // check
    int flushCount = 0;
    for (int i = 0; i <=5; i++){
        cout << tempHandArr[i] << " " << tempHandArr[i+1] << " | " << flushCount << "\n";
        if (tempHandArr[i] == tempHandArr[i+1]){
            flushCount++;
            if (flushCount == 4){ return true; }
        }
        else {
            flushCount = 0;
        }
    }
    return false;
}

//Straight - iterate through the hand if (hand[i+1] == hand[i]-1). Save highest number in straight for tiebreaking.
bool isStraight(vector<string> hand){
    int tempHandArr[7];

    // convert to ints and handle face cards
    char cArr[5] = {'T','J','Q','K','A'};
	string sArr[5] = {"10","11","12","13","14"};
    
    for (int i = 0; i < hand.size(); i++) {
        tempHandArr[i] = hand[i][0] - '0';
        for (int j = 0; j < 5; j++) {
            if (hand[i][0] == cArr[j]) {
                tempHandArr[i] = stoi(sArr[j]);
            }
        }
    }
    
    //sort
    sort(begin(tempHandArr), end(tempHandArr));
    cout << "\nsorted: ";
    for (int i = 0; i < 7; i++){cout << tempHandArr[i] << " ";}
    cout << "\n";
    
    //compare
    int straightCount = 0;
    for (int i = 0; i <=5; i++){
        cout << tempHandArr[i] << " " << tempHandArr[i+1] << " | " << straightCount << "\n";
        if (tempHandArr[i] == tempHandArr[i+1]-1){
            straightCount++;
            if (straightCount == 4){ return true; }
        }
        else {
            straightCount = 0;
        }
    }
    return false;
}

//Of a Kind - iterate through hand if (hand[i] == hand[i+1]).  Check for 3 and 4 of a Kind.  Save Pair value for tiebreaking.
bool ofAKind(vector<string> hand){
    int tempHandArr[7];

    // convert to ints and handle face cards
    char cArr[5] = {'T','J','Q','K','A'};
	string sArr[5] = {"10","11","12","13","14"};
    
    for (int i = 0; i < hand.size(); i++) {
        tempHandArr[i] = hand[i][0] - '0';
        for (int j = 0; j < 5; j++) {
            if (hand[i][0] == cArr[j]) {
                tempHandArr[i] = stoi(sArr[j]);
            }
        }
    }

    //sort
    sort(begin(tempHandArr), end(tempHandArr));
    cout << "\nsorted: ";
    for (int i = 0; i < 7; i++){cout << tempHandArr[i] << " ";}
    cout << "\n";

    unordered_map<int, size_t> count;  // holds count of each encountered number 
    for (int i=0; i<7; i++){count[tempHandArr[i]]++;}  

    cout << count.size() << endl;
    int twoPair = 0, threePair = 0;

    for (auto i : count) {
        if (i.second == 2) {
            twoPair++;
        }
        if (i.second == 3){
            threePair++;
        }
    }

    if (twoPair != 0 | threePair != 0){
        cout << "\nTwo of a Kind: " << twoPair;
        cout << "\nThree of a Kind: " << threePair;
        cout << "\n";
        return true;
    }

    return false;
}

//Template play game function for testing.  Calls functions.  Should handle game operation.
void playGame(){
    deckOfCards deck1;
    deck1.populateDeck();

    deck1.printDeck();
    cout << "Drawing 5 cards\n";
    for (int i = 0; i < 5; i++){
        auto card = deck1.drawCard();
        cout << card << "\n";
        cout << createCardSprite(card) << "\n";
    }
    cout << "\n";
    deck1.printDeck();

    cout << createCardSprite("TS") << "\n";

    // testing flush detection with 7 cards.
    vector<string> testHand = {"4D","4H","5D","4C","5S","5H","7C"};
    cout << "Test Hand Vector: ";

    for (auto x : testHand) 
        cout << x << " "; 

    cout << isFlush(testHand);
    cout << isStraight(testHand);
    cout << ofAKind(testHand);
}

int main(){
    playGame();
}
