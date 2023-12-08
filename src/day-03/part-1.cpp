#include <algorithm> // min, max
#include <cctype> // isdigit
#include <iostream> // cin
#include <numeric> // accumulate
#include <string> // getline
#include <vector>

using namespace std;

int isdot(int ch) { return ch == '.' or ch == '\n'; }
int issymbol(int ch) { return not (isdot(ch) or isdigit(ch)); }

struct Coordinate { size_t x, y; };
struct Range { size_t first, last; };
struct Number {
  unsigned int value;
  Range x;
  size_t y;
};

bool next_to(size_t lhs, size_t rhs) {
  const auto low = min(lhs, rhs);
  const auto high = max(lhs, rhs);

  return
    (low == 0 and high <= 1) or
    high - low <= 1;
}

auto is_adjacent(Number const& number) { return
    [&](auto const& symbol) {
      if (not next_to(number.y, symbol.y)) return false;
      return /* max(0, first-1) <= x <= last+1 */
        (max(1ul, number.x.first) - 1 <= symbol.x) and
        (symbol.x <= number.x.last + 1);
    };
}


int main() {
  vector<Number> numbers;
  vector<Coordinate> symbols;
  Coordinate parsing_position{};

  auto add_number = [&](string& n, Coordinate const& c) {
    numbers.emplace_back(stoul(n), Range{c.x-size(n), c.x-1}, c.y);
    n.clear();
  };


  for (
      string line{};
      getline(cin, line);
      ++parsing_position.y, parsing_position.x = 0
      )
  {
    string number;
    for (char previous{'.'}; char const& current : line) {

      /*
       * +-------+---+---+----------------------+
       * | #     | p | c | action               |
       * +-------+---+---+----------------------+
       * | (1)   | . | 0 | captureNumber        |
       * | (2)   | 0 | 0 | captureNumber        |
       * | (3)   | * | 0 | captureNumber        |
       * | (4)   | . | * | addSymbol            |
       * | (5)   | * | * | addSymbol            |
       * | (6,7) | 0 | * | addSymbol, addNumber |
       * | (8)   | 0 | . | addNumber            |
       * |       | . | . | noop                 |
       * |       | * | . | noop                 |
       * +-------+---+---+----------------------+
       */

      if (isdigit(current)) /* (1,2,3) */ {
        number += current;
      }

      else if (issymbol(current)) /* (4,5,6) */ {
        symbols.push_back(parsing_position);

        if (isdigit(previous)) /* (7) */ {
          add_number(number, parsing_position);
        }
      }

      else if (isdigit(previous) and isdot(current)) /* (8) */{
        add_number(number, parsing_position);
      }

      ++parsing_position.x;
      previous = current;
    }

    if (not empty(number)) {
      add_number(number, parsing_position);
    }
  }

  /* numbers and symbols are already sorted by y then x*/

  erase_if(numbers, [&](auto const& number){ return
      none_of(begin(symbols), end(symbols), is_adjacent(number));
      });

  cout
    << accumulate(
           begin(numbers),
           end(numbers),
           0u,
           [](auto acc, Number const& n) { return acc + n.value; }
           )
    << '\n';
}
