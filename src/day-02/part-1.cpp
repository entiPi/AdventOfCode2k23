#include <algorithm> // max
#include <iostream> // cin
#include <map>
#include <numeric> // accumulate
#include <string> // stoul
#include <vector>

using namespace std;

struct Game {
  size_t id{};
  map<string, size_t> amount_of;
};

int main() {
  vector<size_t> possible_games;

  while (cin.ignore(4) /* discard "Game" */, cin.good()) {
    Game current_game{};
    string amount, colour;

    cin >> current_game.id;
    cin.ignore(1); // discard ':'

    for (
        char ch = cin.get();
        ch != cin.eof() and ch != '\n';
        ch = cin.get()
        )
    {
      if (isspace(ch)) continue;
      else if (ch == ',' or ch == ';') {
          current_game.amount_of[colour] = max(stoul(amount), current_game.amount_of[colour]);
          amount.clear(), colour.clear();
      }
      else if (isdigit(ch)) amount += ch;
      else colour += ch;
    }
    current_game.amount_of[colour] = max(stoul(amount), current_game.amount_of[colour]);

    if (current_game.amount_of["red"] <= 12 and
        current_game.amount_of["green"] <= 13 and
        current_game.amount_of["blue"] <= 14)
    {
      possible_games.push_back(current_game.id);
    }
  }

  cout
    << accumulate(begin(possible_games), end(possible_games), 0u)
    << '\n';
}
