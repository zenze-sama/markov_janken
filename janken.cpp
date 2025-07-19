#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>

using namespace std;

const vector<char> MOVES = {'R', 'P', 'S'};

vector<char> playerHistory;
map<pair<char, char>, map<char, int>> patternCounts;

int playerWins = 0;
int markovWins = 0;
int ties = 0;

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

bool isRepeatingPattern() {
    if (playerHistory.size() < 3) return false;
    char prev1 = playerHistory.back();
    char prev2 = playerHistory[playerHistory.size() - 2];
    char prev3 = playerHistory[playerHistory.size() - 3];
    return (prev1 == prev2 && prev2 == prev3);
}

char getAntiHumanMove() {
    char repeatedMove = playerHistory.back();
    vector<char> options;
    for (char move : MOVES) {
        if (move != repeatedMove) {
            options.push_back(move);
        }
    }
    return counterMove(options[rand() % options.size()]);
}

char getMarkovMove() {
    if (playerHistory.size() >= 3 && isRepeatingPattern()) {
        return getAntiHumanMove();
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

    cout << "Rock-Paper-Scissors (Markov Chain Version)" << endl;
    cout << "Enter R (Rock), P (Paper), or S (Scissors). Q to quit." << endl;

    while (true) {
        cout << "\nYour move: ";
        char playerMove;
        cin >> playerMove;
        playerMove = toupper(playerMove);

        if (playerMove == 'Q') break;
        if (find(MOVES.begin(), MOVES.end(), playerMove) == MOVES.end()) {
            cout << "Invalid move! Try R, P, or S." << endl;
            continue;
        }

        char markovMove = getMarkovMove();
        cout << "Markov chose: " << markovMove << endl;

        string result = getWinner(playerMove, markovMove);
        cout << result << endl;

        if (playerHistory.size() >= 2) {
            char prev2 = playerHistory[playerHistory.size() - 2];
            char prev1 = playerHistory.back();
            auto key = make_pair(prev2, prev1);
            patternCounts[key][playerMove]++;
        }
        playerHistory.push_back(playerMove);
    }

    cout << "\nGame over! Final results:" << endl;
    cout << "You won: " << playerWins << " times" << endl;
    cout << "Markov won: " << markovWins << " times" << endl; 
    cout << "Ties: " << ties << " times" << endl;
    cout << "Markov learned " << patternCounts.size() << " of your patterns." << endl;
    
    return 0;
}