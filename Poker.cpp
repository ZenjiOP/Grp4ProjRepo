#include "Poker_Logic.h"
#include "Poker_Graphics.h"
#include "Poker_Classes.h"
#include "iostream"
#include "vector"
#include "algorithm"
#include "random"
#include "string"
#include "chrono"
#include "thread"
#include "map"

using namespace std;

vector<int> roundOfBetting(player *player1, vector<cpuOpponent*> cpusInPlay, vector<int> bets, vector<string> communityCards){
    // -99999 return = player folds
    string playerAction = "check"; 
    
    bool allBetsEqual = false;
    while (!allBetsEqual){ // happens once, then until all values of bets are equal
        // player turn
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
        
        // update action taken to blank
        for (int i = 0; i < cpusInPlay.size(); i++){
            if (bets[i+1] >= 0){
                cpusInPlay[i]->setActionTaken(" ");
            }
        }
        clearScreen(40);
        createScreen(player1, cpusInPlay, communityCards, 1);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // cpu turns
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

        // check if all bets are equal
        vector<int> betsNoFolds;
        for (size_t i = 0; i < bets.size(); i++){
            if(bets[i] >= 0) {
                betsNoFolds.push_back(bets[i]);
            }
        }

        int pot = 0;
        for (auto bet: bets){
            if (bet != -1 && bet < 0){
                bet = bet * -1;
            }
            pot += bet;
        }
        player1->setPotVal(pot); 
        
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
    
    for (auto cpu: cpuInPlay) // set last action taken to blank
        cpu->setActionTaken(" ");
    

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
    for (int bet: betsNoFolds)
        total += bet;

    if (winner == 0) {
        p1Ptr->updateChipTotal(p1Ptr->getChipTotal() + total);
    } else {
        cpuInPlay[winner - 1]->updateChipTotal(cpuInPlay[winner - 1]->getChipTotal() + total);
        cpuInPlay[winner - 1]->setActionTaken("won");
    }

    // Display end of Hand screen
    clearScreen(40);
    createScreen(p1Ptr, cpuInPlay, communityCards, 2);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    if (winner != 0)
        cpuInPlay[winner - 1]->setActionTaken(" ");
    
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
        if (input == "1"|| input == "start" || input == "Start"){ // play game -------------------------------------------
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
            
        } else if (input == "2" || input == "settings" || input == "Settings"){ //settings --------------------------------------
            
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
                    clearScreen(40);
                    createMenuScreen(settings, 2);
                
                } else if (input == "2") {
                    cout << "Please Enter new starting Chips (Max 200,000): ";
                    input = getPlayerInput();
                    while (stoi(input) > 200000 || stoi(input) <= 0){
                        cout << "Unrecognized Input\033[F\033[2K";
                        cout << "Please Enter new starting Chips (Max 200,000): ";
                        input = getPlayerInput();
                    }
                    settings->setStartingChips(stoi(input));
                    clearScreen(40);
                    createMenuScreen(settings, 2);
                } else if (input == "3") {
                    cout << "Please Enter new small Blind (Max 2,000): ";
                    input = getPlayerInput();
                    while (stoi(input) > 2000 || stoi(input) <= 0){
                        cout << "Unrecognized Input\033[F\033[2K";
                        cout << "Please Enter new small Blind (Max 2,000): ";
                        input = getPlayerInput();
                    }
                    settings->setSmallBlindValue(stoi(input));
                    clearScreen(40);
                    createMenuScreen(settings, 2);
                } else if (input != "back") {
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
