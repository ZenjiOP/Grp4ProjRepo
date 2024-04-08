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
        void setBlindBet(int blindVal){
            blindBet = blindVal;
        }      
        string getBlindBet(){
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
        int getBlindBetInt(){
            return blindBet;
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
        int blindBet = 0; // 0 None, 1 Small, 2 Big
    public:
        void setHand(vector<string> newHand){
            hand = newHand;
        }
        vector<string> getHand(){
            return hand;
        }
        void setBlindBet(int blindVal){
            blindBet = blindVal;
        }      
        string getBlindBet(){
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
        int getBlindBetInt(){
            return blindBet;
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
        int smallBlindValue = 250;
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
        void setSmallBlindValue(int smallBlindValue){
            this->smallBlindValue = smallBlindValue;
        }
        int getSmallBlindValue(){
            return smallBlindValue;
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
    1 - In Game
    2 - In Game, CPU Hand shown
    */ 
    if (screenFlag == 1) { // In Game
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|//||//|\n|__||__|\n[" + cpuInPlay[0]->getBlindBet() + "]";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|//||//|  |//||//|\n|__||__|  |__||__|\n"
                "[" + cpuInPlay[0]->getBlindBet() + "]     [" + cpuInPlay[1]->getBlindBet() + "]";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|//||//|  |//||//|  "
                "|//||//|\n|__||__|  |__||__|  |__||__|\n[" + cpuInPlay[0]->getBlindBet() + "]     "
                "[" + cpuInPlay[1]->getBlindBet() + "]     [" + cpuInPlay[2]->getBlindBet() + "]";
                break;
        }
        
        cout << cpuHand << endl;

        cout << "\n";
        for (int i = 0; i < numOfCpu; i++){
                cout << "CPU" << i + 1 <<": " << cpuInPlay[i]->getChipTotal() << " - " << cpuInPlay[i]->getLastActionTaken() << endl; // previous action;
        }
        cout << "\n";

        //community cards
        cout << "Pot Total: "; // add pot total variable
        cout << createCardSprite(communityCards);
        cout << "\n";

        //player hand
        cout << "Your Hand:";
        cout << createCardSprite(player->getHand());
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " | [" + player->getBlindBet() + "]\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold. What Will You Do?" << endl;
    }

    if (screenFlag == 2) { // End of Hand / CPU Card Show
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] + "|\n|__||__|\n[   ]";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[0]->getHand()[1] + "||" + cpuInPlay[1]->getHand()[1] + "|\n|__||__|  |__||__|\n[   ]     [   ]";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[0]->getHand()[1] + "||" + cpuInPlay[1]->getHand()[1] + "|  |" + cpuInPlay[2]->getHand()[1] + "||" 
                + cpuInPlay[2]->getHand()[1] + "|\n|__||__|  |__||__|  |__||__|\n[   ]     [   ]     [   ]";
                break;
        }
        
        cout << cpuHand << endl;

        cout << "\n";
        for (int i = 0; i < numOfCpu; i++){
                cout << "CPU" << i + 1 <<": " << cpuInPlay[i]->getChipTotal() << " - " << cpuInPlay[i]->getLastActionTaken() << endl; // previous action;
        }
        cout << "\n";

        //community cards
        cout << "Pot Total: "; // add pot total variable
        cout << createCardSprite(communityCards);
        cout << "\n";

        //player hand
        cout << "Your Hand:";
        cout << createCardSprite(player->getHand());
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " | [   ]\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold. What Will You Do?" << endl;
    }
}

void createMenuScreen(settingsHolder *settings, int menuFlag){
    // Title
    if (menuFlag == 1) { 
        cout << "\n\n";
        cout << "				        TWO CARD\n\033[1;33m$$$$$$$\\            $$\\            \033[0mTEXAS HOLD'EM\033[1;33m\n$$  "
            "__$$\\           $$ |\n$$ |  $$ | $$$$$$\\  $$ |  $$\\  $$$$$$\\   $$$$$$\\ \n$$$$$$$  |$$  __$$\\ $$ | $$  |$$  __$$\\ "
            "$$  __$$\\ \n$$  ____/ $$ /  $$ |$$$$$$  / $$$$$$$$ |$$ |  \\__|\n$$ |      $$ |  $$ |$$  _$$<  $$   ____|$$ |\n$$ |   "
            "   \\$$$$$$  |$$ | \\$$\\ \\$$$$$$$\\ $$ |\n\\__|       \\______/ \\__|  \\__| \\_______|\\__|\033[0m" << endl;
        cout << "\n                     GROUP 4\n\n		 1. START GAME\n		 2. SETTINGS\n		 3. QUIT\n\n\n\nENTER CHOICE: ";
    }
    // Settings
    if (menuFlag == 2){
        cout << "\nCURRENT SETTINGS:";
        cout << "\n\n1: Number Of CPU Opponents(Max 3): " << settings->getNumberOfCpu();
        cout << "\n\n2: Starting Chips(Max 200,000): " << settings->getStartingChips();
        cout << "\n\n3: Small Blind Value(Max 2,000): " << settings->getSmallBlindValue();
        
        for (int i = 0; i < 8; i++)
            cout << "\n";
        
        cout << "Enter the number of a setting you'd like to modify, or back to return to menu: " << endl;
    }
}

string getPlayerInput(){
    string playerInput;
    cin >> playerInput;

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
                
                transform(playerAction.begin(), playerAction.end(), playerAction.begin(), [](unsigned char c) {
                    return tolower(c);});

                if (playerAction == "raise"){
                    try {
                        cout << "\033[2KHow much?: ";
                        int bet = stoi(getPlayerInput());
                        bet = min(bet, player1->getChipTotal()); // if bet > chip total, bet = chip total
                    
                        player1->updateChipTotal(player1->getChipTotal() - bet);
                        bets[0] += bet;
                        break;
                    }
                    catch (const exception& e){
                        cout << "\033[FUnrecognized Input\033[F\033[2K";
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
                
                cout << "Unrecognized Input\033[F\033[2K";
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
                createScreen(player1, cpusInPlay, communityCards, 1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        vector<int> betsNoFolds;
        for (size_t i = 0; i < bets.size(); i++){
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

int runHand(settingsHolder *settings, player *p1Ptr, vector<cpuOpponent*> cpuInPlay){
    deckOfCards deck1;
    deck1.populateDeck();

    vector<string> communityCards = {};
    
    // create a vector of vectors of strings containing hands
    vector<vector<string>> hands(cpuInPlay.size() + 1);

    // deal "hole" cards
    for (int i = 0; i < 2; i++){
        hands[0].push_back(deck1.drawCard());
        
        for (int j = 0; j < cpuInPlay.size(); j++){
            hands[j+1].push_back(deck1.drawCard());
        }
    }
    p1Ptr->setHand(hands[0]);
    
    for (int i = 0; i < cpuInPlay.size(); i++)
        cpuInPlay[i]->setHand(hands[i+1]);
    
    vector<int> bets = {0};
    // forced blinds and creation of bets vector
    bets[0] += settings->getSmallBlindValue() * p1Ptr->getBlindBetInt(); // will mult by 0, 1 for small, 2 for big
    p1Ptr->updateChipTotal(p1Ptr->getChipTotal()- bets[0]);
    for (int i = 0; i < cpuInPlay.size(); i++) {
        bets.push_back(0);
        bets[i + 1] += settings->getSmallBlindValue() * cpuInPlay[i]->getBlindBetInt(); // will mult by 0, 1 for small, 2 for big 
        cpuInPlay[i]->updateChipTotal(cpuInPlay[i]->getChipTotal() - bets[i + 1]);
    }
    // round of betting to call blinds/fold
    clearScreen(40);
    createScreen(p1Ptr, cpuInPlay, communityCards, 1);
    bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);

    // community cards
    for (int i = 0; i < 3; i++)
        communityCards.push_back(deck1.drawCard());

    clearScreen(40);
    createScreen(p1Ptr, cpuInPlay, communityCards, 1);
    bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    bool roundOver = false;

    if (count_if(bets.begin(), bets.end(), [](int bet) { return bet < 0; }) >= cpuInPlay.size()){
        roundOver = true;
    } else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        clearScreen(40);
        createScreen(p1Ptr, cpuInPlay, communityCards, 1);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    }

    if (count_if(bets.begin(), bets.end(), [](int bet) { return bet < 0; }) >= cpuInPlay.size()){
        roundOver = true;
    } else if (!roundOver){
        communityCards.push_back(deck1.drawCard());
        clearScreen(40);
        createScreen(p1Ptr, cpuInPlay, communityCards, 1);
        bets = roundOfBetting(p1Ptr, cpuInPlay, bets, communityCards);
    }
    cout << "Hand Over - ranking hands";

    map<string, int> handRanks;
    
    // Rank player's hand
    vector<string> playerHand;
    
    playerHand = p1Ptr->getHand();
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
    
    // Update chip totals
    vector<int> betsNoFolds;
    for (size_t i = 0; i < bets.size(); i++){
        if (bets[i] != -1 && bets[i] < 0){
            betsNoFolds.push_back(bets[i] * - 1);
        } else if (bets[i] > 0) {
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
    } else {
        cpuInPlay[winner - 1]->updateChipTotal(cpuInPlay[winner - 1]->getChipTotal() + total);
    }

    // Display end of Hand screen
    clearScreen(40);
    createScreen(p1Ptr, cpuInPlay, communityCards, 2);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}

int runGame(){
    srand((unsigned) time(NULL));

    settingsHolder s1;
    settingsHolder* settings = &s1;

    
    string input;
    while (true) {
        clearScreen(40);
        createMenuScreen(settings, 1);
        
        input = getPlayerInput();
        if (input == "1"){ // play game -------------------------------------------
            player p1;
            player* player1 = &p1;

            // Settings Variables
            int numOfCpu = settings->getNumberOfCpu(); 
            int startingChips = settings->getStartingChips(); 

            // create vector holding all CPU in play, update chips
            vector<cpuOpponent*> cpuInPlay = {};
            cpuInPlay.resize(numOfCpu);

            for (int i = 0; i < numOfCpu; i++){
                cpuInPlay[i] = new cpuOpponent();
                cpuInPlay[i]->updateChipTotal(startingChips);
            }
            p1.updateChipTotal(startingChips);

            // tracker for blinds
            int blindCounter = 0;
            
            // main gameplay loop
            while (player1->getChipTotal() > 0 && !cpuInPlay.empty()){
                player1->setBlindBet(0); // reset blindBet flag
                for (int i = 0; i < cpuInPlay.size(); i++){
                    cpuInPlay[i]->setBlindBet(0); // reset blindBet flags

                    if (cpuInPlay[i]->getChipTotal() == 0){
                        cpuInPlay.erase(cpuInPlay.begin() + i); 
                    }
                }

                if (blindCounter > cpuInPlay.size())
                    blindCounter = 0;

                if (blindCounter == 0){
                    player1->setBlindBet(1);
                    cpuInPlay[0]->setBlindBet(2);
                } else {
                    cpuInPlay[blindCounter - 1]->setBlindBet(1);
                    if (blindCounter == cpuInPlay.size()){
                        player1->setBlindBet(2);
                    } else {
                        cpuInPlay[blindCounter]->setBlindBet(2);
                    }
                }
                blindCounter++;
                


                runHand(settings, player1, cpuInPlay);    
            }

            // clean up memory
            for (auto& cpu : cpuInPlay) {
                delete cpu;
            }
            
        } else if (input == "2"){ //settings --------------------------------------
            
            clearScreen(40);
            createMenuScreen(settings, 2);

            while (input != "back"){
                input = getPlayerInput();
                
                if (input == "1"){
                    cout << "Please Enter new number of CPUs (Max 3): ";
                    input = getPlayerInput();
                    while (input != "1" && input != "2" && input != "3"){
                        cout << "Unrecognized Input\033[F\033[2K";
                        cout << "Please Enter new number of CPUs (Max 3): ";
                        input = getPlayerInput();
                    }
                    settings->setNumberOfCpu(stoi(input));
                    clearScreen(2);
                
                }/* else if (input == "2") {
                    cout << "Please Enter new starting Chips (Max 200,000): ";
                    input = getPlayerInput();
                    while (input > 200000 || input <= 0){
                        cout << "Unrecognized Input\033[F\033[2K";
                        cout << "Please Enter new starting Chips (Max 200,000): ";
                        input = getPlayerInput();
                    }
                } else if (input == "3") {
                    cout << "Please Enter new small Blind (Max 2,000): ";
                    input = getPlayerInput();
                    while (input > 2000 || input <= 0){
                        cout << "Unrecognized Input\033[F\033[2K";
                        cout << "Please Enter new small Blind (Max 2,000): ";
                        input = getPlayerInput();
                    }
                } */else if (input != "back") {
                    cout << "Unrecognized Input\033[F\033[2K";
                }
            }

        } else if (input == "3"){
            return 0;
        }
        
        if (input != "back") {
            cout << "Unrecognized Input\033[F\033[2K";
        }
    }
}

int main(){
    runGame();
}
