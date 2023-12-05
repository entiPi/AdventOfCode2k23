#include <algorithm> // max
#include <iostream> // cin
#include <map>
#include <numeric> // accumulate
#include <string> // stoul
#include <vector>

using namespace std;

int main() {
  vector<size_t> powers;

  while (cin.ignore(9, ':') /* discard "Game XXX:" */, cin.good()) {
    map<string, size_t> amount_of;
    string amount, colour;

    for (
        char ch = cin.get();
        ch != cin.eof() and ch != '\n';
        ch = cin.get()
        )
    {
      if (isspace(ch)) continue;
      else if (ch == ',' or ch == ';') {
          amount_of[colour] = max(stoul(amount), amount_of[colour]);
          amount.clear(), colour.clear();
      }
      else if (isdigit(ch)) amount += ch;
      else colour += ch;
    }
    amount_of[colour] = max(stoul(amount), amount_of[colour]);

    powers.push_back(
        amount_of["red"]
        * amount_of["green"]
        * amount_of["blue"]
        );
  }

  cout
    << accumulate(begin(powers), end(powers), 0u)
    << '\n';
}
