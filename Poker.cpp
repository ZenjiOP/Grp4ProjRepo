#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"
#include "chrono"
#include "thread"
#include "map"
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
        int prevRaise = 0;
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
            if (prevRaise != 0)
                return actionTaken + " - " + to_string(prevRaise);
            return actionTaken;
        }
        void setPrevRaise(int raise){
            prevRaise = raise;
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

class settingsHolder{
    private:
        int numberOfCpu = 3;
        int startingChips = 10000;
    public:
        void setNumberOfCpu(int numberOfCpu){
            this->numberOfCpu = numberOfCpu;
        }
        int getNumberOfCpu(){
            return numberOfCpu;
        }
        void setStartingChips(int startingChips){
            this->startingChips = startingChips;
        }
        int getStartingChips(){
            return startingChips;
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
        {"|A \033[1;33m.\033[0m  |"},
        {"| \033[1;33m/ \\\033[0m |"},
        {"| \033[1;33m\\ /\033[0m |"},
        {"|  \033[1;33mv\033[0m  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A \033[1;32m_\033[0m  |"},
        {"| \033[1;32m( )\033[0m |"},
        {"|\033[1;32m(_'_)\033[0m|"},
        {"|  \033[1;32m|\033[0m  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A\033[1;31m_ _\033[0m |"},
        {"|\033[1;31m( v )\033[0m|"},
        {"| \033[1;31m\\ /\033[0m |"},
        {"|  \033[1;31mv\033[0m  |"},
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

string createStringFromInt(int input){
    string result = to_string(input);
    if (result.size() > 3)
        result.insert(result.size() - 3, ",");
    for (int i = result.size(); i < 7; i++)
        result.insert(0, " ");
    return result;
}

void createScreen(player *player, vector<cpuOpponent*> cpuInPlay, vector<string> communityCards, int screenFlag){
    /*Screen Flag Atlas:
    1 - Main Menu
    2 - In Game
    3 - In Game, CPU Hand shown
    */ 

    if (screenFlag == 1) { // Title
        cout << "\n\n";
        cout << "				   TEXAS HOLD'EM\n\033[1;33m$$$$$$$\\            $$\\                \033[0mTWO CARD\033[1;33m\n$$  "
            "__$$\\           $$ |\n$$ |  $$ | $$$$$$\\  $$ |  $$\\  $$$$$$\\   $$$$$$\\ \n$$$$$$$  |$$  __$$\\ $$ | $$  |$$  __$$\\ "
            "$$  __$$\\ \n$$  ____/ $$ /  $$ |$$$$$$  / $$$$$$$$ |$$ |  \\__|\n$$ |      $$ |  $$ |$$  _$$<  $$   ____|$$ |\n$$ |   "
            "   \\$$$$$$  |$$ | \\$$\\ \\$$$$$$$\\ $$ |\n\\__|       \\______/ \\__|  \\__| \\_______|\\__|\033[0m" << endl;
        cout << "\n                     GROUP 4\n\n		 1. START GAME\n		 2. SETTINGS\n		 3. QUIT\n\n\n\nENTER CHOICE: ";
    }

    if (screenFlag == 2) { // In Game
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
        cout << createCardSprite(player->getHand()) << "\n";
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " |\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold.\n";
        cout << "What Will You Do? "; // return string for player input
    }

    if (screenFlag == 3) { // End of Hand / CPU Card Show
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] + "|\n|__||__|";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[0]->getHand()[1] + "||" + cpuInPlay[1]->getHand()[1] + "|\n|__||__|  |__||__|";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[0]->getHand()[1] + "||" + cpuInPlay[1]->getHand()[1] + "|  |" + cpuInPlay[2]->getHand()[1] + "||" 
                + cpuInPlay[2]->getHand()[1] + "|\n|__||__|  |__||__|  |__||__|";
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
        cout << createCardSprite(player->getHand()) << "\n";
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " |\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold.\n";
        cout << "What Will You Do? "; // return string for player input
    }
}

void createSettingsScreen(settingsHolder *settings){
    // Settings
    cout << "\n\n\n\nCURRENT SETTINGS:";
    cout << "\n\n1: Number Of CPU Opponents(Max 3): " << settings->getNumberOfCpu();
    cout << "\n\n2: Starting Chips: " << settings->getStartingChips();
    
    for (int i = 0; i < 21; i++)
        cout << "\n";
    
    cout << "Enter the number of a setting you'd like to modify,\n Or back to return to menu: ";
}

string getPlayerInput(){
    string playerInput;
    cin >> playerInput;
    // Based on input call functions or pass input to driver function

    return playerInput;
}

vector<int> roundOfBetting(player *player1, vector<cpuOpponent*> cpusInPlay, vector<int> bets, vector<string> communityCards){
    // -99999 return = player folds
    string playerAction = "check"; 
    
    bool allBetsEqual = false;
    while (!allBetsEqual){ // happens once, then until all values of bets are equal
        if (bets[0] >= 0){
            while (true){
                playerAction = getPlayerInput();
                
                if (playerAction == "raise"){
                    try {
                        int bet = stoi(getPlayerInput());
                        bet = min(bet, player1->getChipTotal()); // if bet > chip total, bet = chip total
                    
                        player1->updateChipTotal(player1->getChipTotal() - bet);
                        bets[0] += bet;
                        break;
                    }
                    catch (const exception& e){
                        cout << "Unrecognized Input";
                        continue;
                    }
                } else if (playerAction == "call" || playerAction == "check"){
                    int prevValue = bets[0];
                    bets[0] = *max_element(begin(bets), end(bets));

                    player1->updateChipTotal(player1->getChipTotal() - (bets[0] - prevValue));
                    break;
                } else if (playerAction == "fold"){
                    int prevValue = bets[0];
                    if (prevValue == 0)
                        prevValue++;
                    bets[0] = prevValue * -1;
                    break;
                }
                
                cout << "Unrecognized Input\n";
            }
        }

        for (int i = 0; i < cpusInPlay.size(); i++){
            if (bets[i+1] >= 0){
                vector<string> cpuhand = cpusInPlay[i]->getHand();
                cpuhand.insert(cpuhand.end(), communityCards.begin(), communityCards.end());

                int cpuhandRank = rankHand(cpuhand);
                string cpuAction = cpusInPlay[i]->chooseAction(cpuhandRank, playerAction);

                if (cpuAction == "raise"){
                    int random = rand() % 500;
                    int bet = cpuhandRank * 100 + random;
                    bet = max(bet, bets[i] + cpuhandRank + random);
                    
                    bet = min(bet, cpusInPlay[i]->getChipTotal());// if bet is above total chips, all in.

                    cpusInPlay[i]->updateChipTotal(cpusInPlay[i]->getChipTotal() - bet);
                    cpusInPlay[i]->setPrevRaise(bet);
                    bets[i+1] += bet;
                } else if (cpuAction == "call"){
                    int prevValue = bets[i+1];
                    
                    bets[i+1] = *max_element(bets.begin(), bets.end());

                    cpusInPlay[i]->updateChipTotal(cpusInPlay[i]->getChipTotal() - (bets[i+1] - prevValue));
                } else if (cpuAction == "fold"){
                    int prevValue = bets[i+1];
                    if (prevValue == 0)
                        prevValue++;
                    bets[i+1] = prevValue * -1;
                } 

                clearScreen(40);
                createScreen(player1, cpusInPlay, communityCards, 2);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        cout<<"\n Bets: ";
        for (int x: bets)
            cout << x << " ";

        vector<int> betsNoFolds;
        for (size_t i = 1; i < bets.size(); i++){
            if(bets[i] >= 0) {
                betsNoFolds.push_back(bets[i]);
            }
        } 

        allBetsEqual = false;
        if (adjacent_find(betsNoFolds.begin(), betsNoFolds.end(), not_equal_to<int>()) == betsNoFolds.end())
            allBetsEqual = true;
    }
    return bets;
}

int runHand(){
    srand((unsigned) time(NULL));

    deckOfCards deck1;
    deck1.populateDeck();
    
    settingsHolder s1;
    settingsHolder* settings = &s1;

    player p1;
    player* p1Ptr = &p1;

    // Settings Variables
    int numOfCpu = settings->getNumberOfCpu(); 
    int startingChips = settings->getStartingChips(); 

    vector<string> communityCards = {};

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

    // tracker for blinds
    int blindCounter = 1;
    int* blindCounterPtr = &blindCounter;

    // deal cards    
    for (int i = 0; i < 2; i++){
        hands[0].push_back(deck1.drawCard());
        
        for (int j = 0; j < numOfCpu; j++){
            hands[j+1].push_back(deck1.drawCard());
        }
    }

    p1.setHand(hands[0]);
    
    for (int i = 0; i < numOfCpu; i++)
        cpuInPlay[i]->setHand(hands[i+1]);
    
    vector<int> bets = {0};
    for (int i = 0; i < cpuInPlay.size(); i++)
        bets.push_back(0); 

    // community cards
    for (int i = 0; i < 3; i++)
        communityCards.push_back(deck1.drawCard());

    clearScreen(40);
    createScreen(p1Ptr, cpuInPlay, communityCards, 2);
    bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    bool roundOver = false;

    if (count_if(bets.begin(), bets.end(), [](int bet) { return bet < 0; }) >= numOfCpu){
        roundOver = true;
    } else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        clearScreen(40);
        createScreen(p1Ptr, cpuInPlay, communityCards, 2);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    }

    if (count_if(bets.begin(), bets.end(), [](int bet) { return bet < 0; }) >= numOfCpu){
        roundOver = true;
    } else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        clearScreen(40);
        createScreen(p1Ptr, cpuInPlay, communityCards, 2);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    }
    cout << "Hand Over - ranking hands";

    map<string, int> handRanks;
    
    // Rank player's hand
    vector<string> playerHand;
    
    playerHand = p1.getHand();
    playerHand.insert(playerHand.end(), communityCards.begin(), communityCards.end());
    
    handRanks["player"] = (bets[0] >= 0) ? rankHand(playerHand) : 0; // if player folded, rank 0

    int winner = 0;

    // Rank CPU hands
    for (int i = 0; i < cpuInPlay.size(); i++){
        vector<string> cpuHand;
    
        cpuHand = cpuInPlay[i]->getHand();
        cpuHand.insert(cpuHand.end(), communityCards.begin(), communityCards.end());
    
        handRanks["cpu " + to_string(i+1)] = (bets[i + 1] >= 0) ? rankHand(cpuHand) : 0; // if cpu folded, rank 0

        if (handRanks["cpu " + to_string(i+1)] > handRanks["player"])
            winner = i+1;
    }

    // Debug
    map<string, int>::iterator it = handRanks.begin();
    while (it != handRanks.end()) {
        cout << "Key: " << it->first
             << ", Value: " << it->second << endl;
        ++it;
    }
    cout << "\nwinner: " << winner;
    
    // Update chip totals
    vector<int> betsNoFolds;
    for (size_t i = 0; i < bets.size(); i++){
        if(bets[i] >= 0) {
            betsNoFolds.push_back(bets[i]);
        }
    } 
    int total = 0;
    cout << "\nbets: ";
    for (int bet: betsNoFolds){
        total += bet;
        cout << bet << " ";
    }    
    if (winner == 0) {
        p1Ptr->updateChipTotal(p1Ptr->getChipTotal() + total);
    }
    else{
        cpuInPlay[winner - 1]->updateChipTotal(cpuInPlay[winner - 1]->getChipTotal() + total);
    }

    // Debug
    cout <<"\nplayer: "<< p1Ptr->getChipTotal();
    for (auto i : cpuInPlay)
        cout << "\n" << i->getChipTotal();

    // clean up memory, this should only be done upon game close
    for (auto& cpu : cpuInPlay) {
        delete cpu;
    }
    return 0;
}

int main(){
    runHand();
}
