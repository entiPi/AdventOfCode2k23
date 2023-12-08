#include <algorithm> // min, max
#include <cctype> // isdigit
#include <iostream> // cin
#include <numeric> // accumulate
#include <string> // getline
#include <vector>

using namespace std;

int isdot(int ch) {
  return
    ch == '.' or
    ch == '#' or
    ch == '$' or
    ch == '%' or
    ch == '&' or
    ch == '+' or
    ch == '-' or
    ch == '/' or
    ch == '=' or
    ch == '@' or
    ch == '\n';
}
int isgearsymbol(int ch) { return ch == '*'; }

struct Coordinate { size_t x, y; };
struct Range { size_t first, last; };
struct Number {
  unsigned int value;
  Range x;
  size_t y;
};
struct Gear {
  Coordinate position;
  unsigned int ratio{1u};
  vector<unsigned int> adjacent_numbers{};
};

bool next_to(size_t lhs, size_t rhs) {
  const auto low = min(lhs, rhs);
  const auto high = max(lhs, rhs);

  return
    (low == 0 and high <= 1) or
    high - low <= 1;
}

auto is_adjacent(Coordinate const& gear, Number const& number) {
  if (not next_to(number.y, gear.y)) return false;
  return /* max(0, first-1) <= x <= last+1 */
    (max(1ul, number.x.first) - 1 <= gear.x) and
    (gear.x <= number.x.last + 1);
}


int main() {
  vector<Number> numbers;
  vector<Gear> gears;
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
       * +-------+---+---+--------------------+
       * | #     | p | c | action             |
       * +-------+---+---+--------------------+
       * | (1)   | . | 0 | captureNumber      |
       * | (2)   | 0 | 0 | captureNumber      |
       * | (3)   | * | 0 | captureNumber      |
       * | (4)   | . | * | addGear            |
       * | (5)   | * | * | addGear            |
       * | (6,7) | 0 | * | addGear, addNumber |
       * | (8)   | 0 | . | addNumber          |
       * |       | . | . | noop               |
       * |       | * | . | noop               |
       * +-------+---+---+--------------------+
       */

      if (isdigit(current)) /* (1,2,3) */ {
        number += current;
      }

      else if (isgearsymbol(current)) /* (4,5,6) */ {
        gears.emplace_back(parsing_position);

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

  /* numbers and gears are already sorted by y then x*/

  for (auto& gear : gears) {
    for (auto const& number : numbers) {
      if (is_adjacent(gear.position, number)) {
        gear.ratio *= number.value;
        gear.adjacent_numbers.push_back(number.value);
      }
    }
  }

  cout
    << accumulate(
           begin(gears),
           end(gears),
           0u,
           [](auto acc, Gear const& g) {
               return acc + (size(g.adjacent_numbers) == 2 ? g.ratio : 0);
           }
           )
    << '\n';
}
