#include <algorithm> // binary_search, copy_n, count_if
#include <iostream> // cin
#include <iterator> // istream_iterator
#include <vector>

using namespace std;

unsigned int points(unsigned int count) {
  if (count == 0) return 0u;

  unsigned int points{1u};
  while (--count > 0) points *= 2;
  return points;
}

int main() {
  unsigned int total_points{0u};

  while (cin.good()) {
    vector<unsigned int> winning_numbers, own_numbers;
    cin.ignore(10, ':');

    copy_n(
      istream_iterator<unsigned int>(cin),
      10,
      back_inserter(winning_numbers));
    sort(begin(winning_numbers), end(winning_numbers));

    cin.ignore(2);

    copy_n(
      istream_iterator<unsigned int>(cin),
      25,
      back_inserter(own_numbers));

    cin.ignore(2);

    auto is_winning = [&](auto const& own_number) {
        return binary_search(begin(winning_numbers), end(winning_numbers), own_number);
    };

    const auto amount_winning_numbers = count_if(begin(own_numbers), end(own_numbers), is_winning);
    total_points += points(amount_winning_numbers);
  }

  cout << total_points << '\n';
}
