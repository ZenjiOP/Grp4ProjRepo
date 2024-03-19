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

// Card Sprite Creation
string createCardSprite(vector<string> handInfo){
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
    int cardBaseIndex;

    cardSprite += "\n";
    for (int i = 0; i < 6; i++){ // each line
        for (int j = 0; j < handInfo.size(); j++){ // each card
            char cardSuit = handInfo[j][1];
            string cardValue = ""; cardValue.push_back(handInfo[j][0]);
            cardBaseIndex = cardSuitIndex.find(cardSuit);

            string cardLine = cardBases[cardBaseIndex][i];
            if (i == 1 | i == 5) {cardLine.replace(i, 1, cardValue);}
            cardSprite.append(cardLine);
        }  
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

void createScreen(vector<string> playerHand, vector<string> communityCards, int screenFlag){
    /*Screen Flag Atlas:
    1 - Main Menu
    2 - In Game*/ 

    if (screenFlag == 1) {
        cout << "\n\n";
        // title
        cout << "$$$$$$$\\            $$\\                TWO CARD\n$$  __$$\\           $$ |\n$$ |  $$ | $$$$$$\\  $$ |  $$\\  $$$$$$\\"   
            << "   $$$$$$\\ \n$$$$$$$  |$$  __$$\\ $$ | $$  |$$  __$$\\ $$  __$$\\ \n$$  ____/ $$ /  $$ |$$$$$$  / $$$$$$$$ |$$ |  \\__|\n"
            << "$$ |      $$ |  $$ |$$  _$$<  $$   ____|$$ |\n$$ |      \\$$$$$$  |$$ | \\$$\\ \\$$$$$$$\\ $$ |\n\\__|       \\______/ \\"
            << "__|  \\__| \\_______|\\__|" << endl;
        cout << "\n                     GROUP 4\n\n		 1. START GAME\n		 2. SETTINGS\n		 3. QUIT\n\n\n\nENTER CHOICE: ";
    }

    if (screenFlag == 2) {
        cout << "CPU:" << endl;
        string cpuHand = " __  __\n|//||//|\n|__||__|";
        cout << cpuHand << endl;

        string spacer = "\n\n\n";
        cout << spacer;

        //community cards
        cout << "Pot Total: " << endl; // add pot total variable
        cout << createCardSprite(communityCards);
        cout << spacer;

        //player hand
        cout << "Your Hand:" << endl;
        cout << createCardSprite(playerHand) << "\n";
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << "999,999" << " |\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold.\n";
        cout << "What Will You Do? "; // return string for player input
    }
}

string getPlayerInput(){
    string playerInput;
    cin >> playerInput;
    // Based on input call functions or pass input to driver function

    return playerInput;
}

//Poker Logic, each function takes a vector of Strings in format "SV" (Suit/Value)
//Flush - remove card values, check if 5 suits are the same.  TODO: Save highest number for tiebreaking.
//Takes a vector<string> of community cards + one player hand and returns true if a straight is found.
bool isFlush(vector<string> hand){
    // reverse value and suit
    string tempHandArr[7];
    for (int i = 0; i < hand.size(); i++){
        tempHandArr[i] = hand[i][1];
    }
    // sort
    sort(begin(tempHandArr), end(tempHandArr));

    // check
    int flushCount = 0;
    for (int i = 0; i <=5; i++){
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

//Straight - iterate through the hand if (hand[i+1] == hand[i]-1). TODO: Save highest number in straight for tiebreaking.
//Takes a vector<string> of community cards + one player hand and returns true if a straight is found.
int isStraight(vector<string> hand){
    int tempHandArr[7];

    // convert to ints and handle face cards
    char cArr[5] = {'T','J','Q','K','A'};
	int sArr[5] = {10, 11, 12, 13, 14};
    
    for (int i = 0; i < hand.size(); i++) {
        tempHandArr[i] = hand[i][0] - '0';
        for (int j = 0; j < 5; j++) {
            if (hand[i][0] == cArr[j]) {
                tempHandArr[i] = sArr[j];
            }
        }
    }
    
    //sort
    sort(begin(tempHandArr), end(tempHandArr));

    //compare
    int straightCount = 0, royalStraightCount = 0, j = 0;
    for (int i = 0; i <=5; i++){
        // 2 if royal straight
        if (tempHandArr[i] == sArr[j]){
            j++;
            royalStraightCount++;
            if (royalStraightCount == 4){ return 2; }
        }
        // 1 if normal straight
        else if (tempHandArr[i] == tempHandArr[i+1]-1){
            straightCount++;
            if (straightCount == 4){ return 1; }
        }
        // 0 if none
        else {
            straightCount = 0;
        }
    }
    return 0;
}

//Of a Kind - iterate through hand if (hand[i] == hand[i+1]).  Check for 3 and 4 of a Kind.  TODO: Save Pair value for tiebreaking.
//Takes a vector<string> of community cards + one player hand and returns true if a match is found
int ofAKind(vector<string> hand){
    // 1 - Pair, 2 - Two Pair, 3 - Three of a Kind, 4 - Four of a Kind
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

    unordered_map<int, size_t> count;  // holds count of each encountered number.
    for (int i=0; i<7; i++){count[tempHandArr[i]]++;}  

    int pair = 0, threePair = 0, fourPair = 0;

    for (auto i : count) {
        if (i.second == 2) {
            pair++;
        }
        if (i.second == 3){
            threePair++;
        }
        if (i.second == 4){
            return 4;
        }
    }

    if (pair == 1 && threePair == 1){
        return 5;
    }
    else if (pair == 1){
        return 1;
    }
    else if (pair > 1){
        return 2;
    }
    else if (threePair == 1){
        return 3;
    }
    else if (pair == 1 && threePair == 1){
        return 5;
    }

    return false;
}

int rankHand(vector<string> hand){
    // 10:Royal Flush, 9:Straight Flush, 8:Four Of A Kind, 7:Full House, 6:Flush, 5:Straight, 4:Three of a Kind, 
        //3:Two Pair, 2:Pair, 1:High Card
    
    // Call Each Eval
    bool flush = isFlush(hand);
    int straight = isStraight(hand);
    int pairs = ofAKind(hand);

    if (flush){
        // royal
        if (straight == 2){
            return 10;
        }
        // straight
        if (straight == 1){
            return 9;
        }
        // flush
        return 6;
    }
    else if (straight != 0){
        // straight
        return 5;
    }
    switch (pairs){
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 4;
        case 4:
            return 8;
        case 5:
            return 7;
    }

    // high card
    return 1;
}

//Template play game function for testing.  Calls functions.  Should handle game operation.
void playGame(){
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

int main(){
    playGame();
}
