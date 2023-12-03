#include <algorithm>  // reverse
#include <cctype>  // isdigit
#include <iostream>  // cin, cout
#include <numeric>  // accumulate
#include <ranges>
#include <string>  // getline, string, stoi
#include <vector>

using namespace std;

int main() {
  vector<int> calibration_values;

  string number{2, 'x'};
  for (string line{}; getline(cin, line);) {
    int (*is_digit)(int) = isdigit;

    auto first_digit = line
      | views::filter(is_digit)
      | views::take(1);

    auto last_digit = line
      | views::reverse
      | views::filter(is_digit)
      | views::take(1);

    number[0] = first_digit.front();
    number[1] = last_digit.front();

    calibration_values.push_back(stoi(number));
  }

  const auto total = accumulate(begin(calibration_values), end(calibration_values), 0);
  std::cout << total << '\n';
}
