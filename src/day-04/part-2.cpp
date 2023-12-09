#include <algorithm> // copy, count_if
#include <iostream> // cin
#include <numeric> // accumulate
#include <string> // stoul
#include <vector>

using namespace std;

int main() {
  vector<unsigned int> copies;
  auto copies_of_number = [&copies](size_t index) -> decltype(copies)::reference {
      if (size(copies) < index) copies.resize(index);
      return copies[--index];
  };

  unsigned int current_card;
  for (string card_number{}; getline(cin, card_number, ':');) {
    vector<unsigned int> winning_numbers, own_numbers;

    current_card = stoul(card_number.substr(4));
    ++copies_of_number(current_card);

    copy(
      istream_iterator<unsigned int>(cin),
      istream_iterator<unsigned int>(), /*10,*/
      back_inserter(winning_numbers));
    sort(begin(winning_numbers), end(winning_numbers));
    cin.clear();

    cin.ignore(2);

    copy(
      istream_iterator<unsigned int>(cin),
      istream_iterator<unsigned int>(), /*25,*/
      back_inserter(own_numbers));
    cin.clear();

    auto is_winning = [&](auto const& own_number) {
        return binary_search(begin(winning_numbers), end(winning_numbers), own_number);
    };

    const auto new_copies = count_if(begin(own_numbers), end(own_numbers), is_winning);
    for (auto copy{0u}; copy < copies_of_number(current_card); ++copy)
      for (auto offset{1}; offset <= new_copies; ++offset)
        ++copies_of_number(current_card + offset);
  }

  const auto total_cards = accumulate(
      begin(copies),
      ++find(begin(copies), end(copies), current_card),
      0u
      );

  cout << total_cards << '\n';
}
