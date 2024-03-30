#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"
#include "chrono"
#include "thread"
#include "unordered_map"
#include "PokerLogic.cpp"

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
        short pwLowRank[10] = {1,1,1,1,1,1,1,1,3,3};
        short pwMidRank[10] = {1,1,1,1,1,1,2,2,2,3};
        short pwHighRank[10] = {1,1,1,2,2,2,2,2,2,2};

        short pwLowRankRaised[10] = {1,1,1,3,3,3,3,3,3,3};
        short pwMidRankRaised[10] = {1,1,1,1,1,1,2,3,3,3};
        short pwHighRankRaised[10] = {1,1,1,2,2,2,2,2,2,3};
        
        int chips;
        string actionTaken = "";

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
                    actionTaken = "call";
                    return "call";
                case 2:
                    actionTaken = "raise";
                    return "raise";
                case 3:
                    actionTaken = "fold";
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
        string getLastActionTaken(){
            return actionTaken;
        }
        void resetActionTaken(){
            actionTaken = "";
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
        void updateChipTotal(int newTotal){
            chips = newTotal;
        }
        int getChipTotal(){
            return chips;
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

void createScreen(player player, vector<cpuOpponent*> cpuInPlay, vector<string> communityCards, int screenFlag){
    /*Screen Flag Atlas:
    1 - Main Menu
    2 - In Game
    3 - Settings Menu
    */ 

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
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|//||//|\n|__||__|";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|//||//|  |//||//|\n|__||__|  |__||__|";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|//||//|  |//||//|  "
                "|//||//|\n|__||__|  |__||__|  |__||__|";
                break;
        }
        
        cout << cpuHand << endl;

        cout << "\n\n";
        for (int i = 0; i < numOfCpu; i++){
                cout << "CPU" << i + 1 <<": " << cpuInPlay[i]->getChipTotal() << " - " << cpuInPlay[i]->getLastActionTaken() << endl; // previous action;
        }
        cout << "\n";

        //community cards
        cout << "Pot Total: " << endl; // add pot total variable
        cout << createCardSprite(communityCards);
        cout << "\n\n\n";

        //player hand
        cout << "Your Hand:" << endl;
        cout << createCardSprite(player.getHand()) << "\n";
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

vector<int> roundOfBetting(player *player1, vector<cpuOpponent*> cpusInPlay, vector<int> bets, vector<string> communityCards, int pot){
    // -99999 return = player folds
    string playerAction = "check"; 

    cout << bets.size();
    for (auto x:bets){
        cout << x << " ";
    }

    while (true){ // happens once, then until all values of bets are equal
        cout << "\nf";
        if (bets[0] != -99999){
            playerAction = getPlayerInput();
            if (playerAction == "raise"){
                int bet = stoi(getPlayerInput());
                
                if (bet > player1->getChipTotal()){
                    //cannot bet more chips than you have!  Go all in?
                    bet = player1->getChipTotal();
                }

                player1->updateChipTotal(player1->getChipTotal() - bet);
                bets[0] = bet;
            }
            if (playerAction == "call"){
                int prevValue = bets[0];
                bets[0] = *max_element(begin(bets), end(bets));

                player1->updateChipTotal(player1->getChipTotal() - (bets[0] - prevValue));
            }
            if (playerAction == "check"){
                if (bets.back() != 0){
                    cout << "cannot check, Calling.";
                }
            }
            if (playerAction == "fold"){
                bets[0] = -99999;
            }
        }
        cout << "\nplayer chips: " << player1->getChipTotal();

        for (int i = 0; i < cpusInPlay.size(); i++){
            if (bets[i+1] != -99999){
                vector<string> cpuhand = cpusInPlay[i]->getHand();
                cpuhand.insert(cpuhand.end(), communityCards.begin(), communityCards.end());

                int cpuhandRank = rankHand(cpuhand);
                cout << "\ncpu" << i+1 << " has hand ranking " << cpuhandRank;
                string cpuAction = cpusInPlay[i]->chooseAction(cpuhandRank, playerAction);

                //std::this_thread::sleep_for(std::chrono::seconds(1));

                if (cpuAction == "raise"){
                    cout << "\ncpu" << i+1 << " raises";
                    int random = rand() % 500;
                    int bet = cpuhandRank * 100 + random;

                    cout << "\nbet value is " << bet;
                    // if bet is above total chips, all in.
                    if (bet > cpusInPlay[i]->getChipTotal()){
                        bet = cpusInPlay[i]->getChipTotal();
                        cout << "\ncpu is all in.  Value is " << bet;
                    }

                    cpusInPlay[i]->updateChipTotal(cpusInPlay[i]->getChipTotal() - bet);
                    bets[i+1] = bet;
                    cout << "\ncpu" << i+1 << "'s chips remaining is " << cpusInPlay[i]->getChipTotal();
                }
                if (cpuAction == "call"){
                    cout << "\ncpu" << i+1 << " calls";
                    int prevValue = bets[i+1];
                    
                    bets[i+1] = *max_element(bets.begin(), bets.end());

                    cout << "\nbets vector value is " << bets[i+1];

                    cpusInPlay[i]->updateChipTotal(cpusInPlay[i]->getChipTotal() - (bets[i+1] - prevValue));
                    cout << "\ncpu" << i+1 << "'s chips remaining is " << cpusInPlay[i]->getChipTotal();
                }
                if (cpuAction == "fold"){
                    cout << "\ncpu" << i+1 << " folds";
                    bets[i+1] = -99999;
                }

            }
        }


        vector<int> betsCopy = bets;
        betsCopy.erase(remove(betsCopy.begin(), betsCopy.end(), -99999), betsCopy.end());
        cout << "\n" << betsCopy.size();
        if (equal(betsCopy.begin() + 1, betsCopy.end(), betsCopy.begin())){
            cout << "\nbets equal.";
            break;
        }
    }
    cout << "\nhand over";
    return bets;
}

int runHand(){
    srand((unsigned) time(NULL));

    deckOfCards deck1;
    deck1.populateDeck();
    
    player p1;
    player* p1Ptr = &p1;

    // Settings Variables
    int numOfCpu = 3; // 1 = 3 CPU , 2 = 2 CPU , 3 = 1 CPU
    int startingChips = 10000; 

    vector<string> communityCards = {};
    int pot = 0;

    // create vector holding all CPU in play, update chips
    vector<cpuOpponent*> cpuInPlay = {};
    cpuInPlay.resize(numOfCpu);

    for (int i = 0; i < numOfCpu; i++){
        cpuInPlay[i] = new cpuOpponent();
        cpuInPlay[i]->updateChipTotal(startingChips);
    }
    p1.updateChipTotal(startingChips);
    
    // create a vector of vectors of strings containing hands
    vector<vector<string>> hands(numOfCpu + 1);

    // deal cards    
    for (int i = 0; i < 2; i++){
        hands[0].push_back(deck1.drawCard());
        
        for (int j = 0; j < numOfCpu; j++){
            hands[j+1].push_back(deck1.drawCard());
        }
    }

    cout << "hands: ";
    for (auto x: hands){
        cout << ":";
        for (auto y: x)
            cout << y << " ";
    }

    p1.setHand(hands[0]);
    
    for (int i = 0; i < numOfCpu; i++){
        cpuInPlay[i]->setHand(hands[i+1]);
    }
    
    vector<int> bets = {0};
    for (int i = 0; i < cpuInPlay.size(); i++){
        bets.push_back(0);
    }
    
    cout << "\ncpu's in play: " << cpuInPlay.size() << endl;

    for (int i = 0; i < cpuInPlay.size(); i++){
        cout << i + 1 << ": ";
        for (auto x: cpuInPlay[i]->getHand()){
            cout << x << " ";
        }
        cout << endl;
    }
    
    // community cards
    for (int i = 0; i < 3; i++){
        communityCards.push_back(deck1.drawCard());
    }

    createScreen(p1, cpuInPlay, communityCards, 2);
    bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards, pot);
    bool roundOver = false;

    if (count(bets.begin(), bets.end(), -99999) >= numOfCpu){
        roundOver = true;
    }
    else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        createScreen(p1, cpuInPlay, communityCards, 2);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards, pot);
    }

    if (count(bets.begin(), bets.end(), -99999) >= numOfCpu){
        roundOver = true;
    }
    else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        createScreen(p1, cpuInPlay, communityCards, 2);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards, pot);
    }
    cout << "Round Over";

    // clean up memory, this should only be done upon game close
    for (auto& cpu : cpuInPlay) {
        delete cpu;
    }
    return 0;
}

int main(){
    runHand();
}
