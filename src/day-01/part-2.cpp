#include <iostream>  // cin, cout
#include <numeric>  // accumulate
#include <regex>
#include <string>  // getline, string, stoi
#include <vector>


char to_digit(std::string_view raw) {
  if (std::isdigit(raw[0]))
    return raw[0];

  if (raw == "one")   return '1';
  if (raw == "two")   return '2';
  if (raw == "three") return '3';
  if (raw == "four")  return '4';
  if (raw == "five")  return '5';
  if (raw == "six")   return '6';
  if (raw == "seven") return '7';
  if (raw == "eight") return '8';
  if (raw == "nine")  return '9';
  throw std::runtime_error("invalid digit");
}

int main() {
  using namespace std;

  vector<int> calibration_values;

  string number{2};
  for (string line{}; getline(cin, line);) {
    static const regex re{"\\d|one|two|three|four|five|six|seven|eight|nine"};

    static smatch match;
    if (not regex_search(line, match, re))
        throw std::runtime_error(line + ": first digit not found");
    number[0] = to_digit(match.str());

    for (
        auto subject = cbegin(line);
        regex_search(subject, cend(line), match, re);
        advance(subject, match.size())
        )
      number[1] = to_digit(match.str());

    calibration_values.push_back(stoi(number));
  }

  const auto total = accumulate(begin(calibration_values), end(calibration_values), 0);
  std::cout << total << '\n';
}
