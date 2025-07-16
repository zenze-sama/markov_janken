#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const vector<char> MOVES = {'R', 'P', 'S'};

vector<char> playerHistory;
map<pair<char, char>, map<char, int>> patternCounts;

int playerWins = 0;
int markovWins = 0;
int ties = 0;

char getMarkovMove() {
    if (playerHistory.size() < 2) {
        return MOVES[rand() % 3];
    }

    char prev2 = playerHistory[playerHistory.size() - 2];
    char prev1 = playerHistory[playerHistory.size() - 1];
    auto key = make_pair(prev2, prev1);

    if (patternCounts[key].empty()) {
        return MOVES[rand() % 3];
    }

    char predictedMove = max_element(
        patternCounts[key].begin(),
        patternCounts[key].end(),
        [](const auto& a, const auto& b) { return a.second < b.second; }
    )->first;

    if (predictedMove == 'R') return 'P';
    if (predictedMove == 'P') return 'S';
    return 'R';
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