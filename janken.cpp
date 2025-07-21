#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <windows.h>

using namespace std;

const vector<char> MOVES = {'R', 'P', 'S'};

vector<char> playerHistory;
map<pair<char, char>, map<char, int>> patternCounts;

int playerWins = 0;
int markovWins = 0;
int ties = 0;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayHeader() {
    clearScreen();
    cout << "----------------------------------------" << endl;
    cout << "  Rock-Paper-Scissors (Markov Chain AI) " << endl;
    cout << "----------------------------------------" << endl;
    cout << "  You: " << playerWins << "  |  Markov: " << markovWins << "  |  Ties: " << ties << endl;
    cout << "----------------------------------------" << endl << endl;
}

char counterMove(char move) {
    if (move == 'R') return 'P';
    if (move == 'P') return 'S';
    return 'R';
}

char getFrequencyBasedMove() {
    map<char, int> freq;
    for (char move : playerHistory) {
        freq[move]++;
    }
    
    if (freq.empty()) {
        return MOVES[rand() % 3];
    }
    
    char mostCommon = max_element(freq.begin(), freq.end(), 
        [](const auto& a, const auto& b) { return a.second < b.second; })->first;
    return counterMove(mostCommon);
}

char getMarkovMove() {
    if (playerHistory.size() >= 4) {
        bool allSame = true;
        char lastMove = playerHistory.back();
        for (int i = playerHistory.size()-2; i >= max(0, (int)playerHistory.size()-4); i--) {
            if (playerHistory[i] != lastMove) {
                allSame = false;
                break;
            }
        }
        
        if (allSame) {
            return counterMove(lastMove);
        }
    }

    if (playerHistory.size() < 2) {
        return getFrequencyBasedMove();
    }

    char prev2 = playerHistory[playerHistory.size() - 2];
    char prev1 = playerHistory[playerHistory.size() - 1];
    auto key = make_pair(prev2, prev1);

    if (patternCounts[key].empty()) {
        return getFrequencyBasedMove();
    }

    char predictedMove = max_element(
        patternCounts[key].begin(),
        patternCounts[key].end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }
    )->first;

    return counterMove(predictedMove);
}

string getWinner(char player, char markov) {
    if (player == markov) {
        ties++;
        return "Tie!";
    }
    if ((player == 'R' && markov == 'S') || 
        (player == 'P' && markov == 'R') || 
        (player == 'S' && markov == 'P')) {
        playerWins++;
        return "You win!";
    }
    markovWins++;
    return "Markov wins!";
}

int main() {
    srand(time(0));

    while (true) {
        displayHeader();
        cout << "Enter R (Rock), P (Paper), or S (Scissors). Q to quit." << endl;
        cout << "Your move: ";
        
        char playerMove;
        cin >> playerMove;
        playerMove = toupper(playerMove);

        if (playerMove == 'Q') break;
        if (find(MOVES.begin(), MOVES.end(), playerMove) == MOVES.end()) {
            cout << "Invalid move! Try R, P, or S." << endl;
            Sleep(1000);
            continue;
        }

        char markovMove = getMarkovMove();
        string result = getWinner(playerMove, markovMove);

        displayHeader();
        cout << "You played: " << playerMove << endl;
        cout << "Markov chose: " << markovMove << endl;
        cout << result << endl << endl;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();

        if (playerHistory.size() >= 2) {
            char prev2 = playerHistory[playerHistory.size() - 2];
            char prev1 = playerHistory.back();
            auto key = make_pair(prev2, prev1);
            patternCounts[key][playerMove]++;
        }
        playerHistory.push_back(playerMove);
    }

    displayHeader();
    cout << "\nGame over! Final results:" << endl;
    cout << "You won: " << playerWins << " times" << endl;
    cout << "Markov won: " << markovWins << " times" << endl; 
    cout << "Ties: " << ties << " times" << endl;
    cout << "Markov learned " << patternCounts.size() << " of your patterns." << endl;
    
    return 0;
}