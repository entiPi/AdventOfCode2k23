#include <algorithm> // copy, find_if
#include <iostream> // cin, cout
#include <ranges>
#include <regex> // regex, regex_match, smatch
#include <stdexcept> // runtime_error
#include <string>  // getline, stoul
#include <string_view>
#include <vector>

using namespace std;


struct RangeMap {
  size_t destination;
  size_t source;
  size_t length;

  bool is_part(size_t x) const { return source <= x and source + length >= x; }
  int offset() const { return destination - source; }
};


auto lookup(vector<RangeMap> const& map) {
  return [&map](size_t value) -> size_t {
    if (auto matching_range = find_if(begin(map), end(map), [&value](RangeMap const& range) { return range.is_part(value); });
        matching_range != end(map))
      return value + matching_range->offset();
    return value;
  };
}


void parse_list(string const& text, vector<size_t>& list) {
  istringstream iss{text};
  copy(
      istream_iterator<size_t>(iss),
      istream_iterator<size_t>(),
      back_inserter(list)
      );
}


void parse_map(vector<RangeMap>& map) {
  for (string line{}; getline(cin, line) and not line.empty();) {
    static regex re{"(\\d+) (\\d+) (\\d+)"};
    smatch m;
    if (not regex_match(line, m, re)) throw runtime_error("invalid map");
    map.emplace_back(stoul(m[1]), stoul(m[2]), stoul(m[3]));
  }
}


bool startswith(string const& base, string_view sub) {
  return base.substr(0, size(sub)) == sub;
}


int main() {
  vector<size_t> seeds;
  vector<RangeMap>
      seed_to_soil,
      soil_to_fertilizer,
      fertilizer_to_water,
      water_to_light,
      light_to_temperature,
      temperature_to_humidity,
      humidity_to_location;

  for (string line{}; getline(cin, line);) {
    if (startswith(line, "seeds:")) parse_list(line.substr(6), seeds);
    else if (startswith(line, "seed-to-soil")) parse_map(seed_to_soil);
    else if (startswith(line, "soil-to-fertilizer")) parse_map(soil_to_fertilizer);
    else if (startswith(line, "fertilizer-to-water")) parse_map(fertilizer_to_water);
    else if (startswith(line, "water-to-light")) parse_map(water_to_light);
    else if (startswith(line, "light-to-temperature")) parse_map(light_to_temperature);
    else if (startswith(line, "temperature-to-humidity")) parse_map(temperature_to_humidity);
    else if (startswith(line, "humidity-to-location")) parse_map(humidity_to_location);
  }

  auto const to_soil = views::transform(lookup(seed_to_soil));
  auto const to_fertilizer = views::transform(lookup(soil_to_fertilizer));
  auto const to_water = views::transform(lookup(fertilizer_to_water));
  auto const to_light = views::transform(lookup(water_to_light));
  auto const to_temperature = views::transform(lookup(light_to_temperature));
  auto const to_humidity = views::transform(lookup(temperature_to_humidity));
  auto const to_location = views::transform(lookup(humidity_to_location));

  auto const nearest_location = ranges::min(
      seeds
      | to_soil
      | to_fertilizer
      | to_water
      | to_light
      | to_temperature
      | to_humidity
      | to_location
      );
  cout << nearest_location << '\n';
}
