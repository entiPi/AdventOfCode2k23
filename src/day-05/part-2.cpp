#include <algorithm> // copy, find_if
#include <experimental/iterator> // make_ostream_joiner
#include <filesystem> // path
#include <iostream> // cin, cout
#include <limits> // numeric_limits
#include <ranges>
#include <regex> // regex, regex_match, smatch
#include <source_location>
#include <sstream>
#include <stdexcept> // runtime_error
#include <string>  // getline, stoul
#include <string_view>
#include <vector> // vector, erase_if

using namespace std;
using std::filesystem::path;


struct Range {
  size_t start;
  size_t length;

  size_t front() const { return start; }
  size_t back() const { return start + length - 1; }

  bool operator==(Range const&) const = default;
  auto operator<=>(Range const&) const = default;
};


struct RangeMap {
  Range source() const { return Range{source_, length_}; }
  Range destination() const { return Range{destination_, length_}; }

  bool is_part(size_t x) const { return source_ <= x and source_ + length_ >= x; }
  int offset() const { return destination_ - source_; }
  size_t length() const { return length_; }

  bool operator==(RangeMap const&) const = default;
  auto operator<=>(RangeMap const& other) const { return this->source_ <=> other.source_; }

  private:
  size_t destination_;
  size_t source_;
  size_t length_;
};


ostream& operator<<(ostream& os, Range const& r) { return os << '[' << r.front() << '-' << r.back() << ']'; }
ostream& operator<<(ostream& os, RangeMap const& m) { return os << m.source() << " -> " << m.destination(); }
ostream& operator<<(ostream& os, vector<RangeMap> const& v) {
  os << '{';
  copy(begin(v), end(v), experimental::make_ostream_joiner(os, ", "));
  return os << '}';
}


auto lookup(vector<RangeMap> const& map) {
  return [&map](size_t value) -> size_t {
    if (auto matching_range = find_if(begin(map), end(map), [&value](RangeMap const& range) { return range.is_part(value); });
        matching_range != end(map))
      return value + matching_range->offset();
    return value;
  };
}


void parse_ranges(string const& text, vector<Range>& list) {
  istringstream iss{text};
  for (size_t start{}, length{}; iss.good();) {
    iss >> start >> length;
    list.emplace_back(start, length);
  }
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


struct Assertion : runtime_error { using runtime_error::runtime_error; };
void assert_eq(vector<RangeMap> const& lhs, vector<RangeMap> const& rhs, source_location loc = source_location::current()) {
  if (lhs != rhs) {
    stringstream ss;
    ss
      << "[FAIL] "
      << " Assertion " << path{loc.file_name()}.filename().native() << ':' << loc.line() << ": "
      << lhs << " == " << rhs << '\n';
    throw Assertion{ss.str()};
  }
}


bool intersects(Range const& lhs, Range const& rhs) {
  auto const& low = min(lhs, rhs);
  auto const& high = max(lhs, rhs);

  return high.front() <= low.back();
}


vector<RangeMap> flatMap(RangeMap const& R1, RangeMap const& R2) {
  vector<RangeMap> result;

  if (
      R1.destination().front() <= R2.source().front() and
      R1.destination().back() <= R2.source().back()
     )
  {
    // [  R1.destination ( R2.source )       ]
    // +------ (1) ------+
    //                   +--- (2) ---+
    //                               +- (3) -+
  }

//   else if (
//       R1.destination().front() <= R2.source.front() and
//       R1.destination().back() <= R2.source().back()
//      )
//   {
//     // [  R1.destination (       ]  R2.source )
//     // +------ (1) ------+
//     //                   +- (2) -+
//     //                           +---- (3) ---+
//
//     RangeMap O1{R1.destination().front(), R1.source().front(), max(R2.source().front() - R1.destination().front() - 1, 0)};
//     RangeMap O2{R2.destination().front(), R2.source().front(), R1.length() - O1.length()};
//     RangeMap O3{R2.destination().front() + O2.length(), R1.destination().back()+1, max()
//   }

  return result;
}


// Reduce RangeMaps from consecutive layers to RangeMaps on single layer.
vector<RangeMap> flatten(vector<RangeMap> R1, vector<RangeMap> R2) {
  vector<RangeMap> result;

  // search for overlaps in R1.destination_range and R2.source_range
  for (RangeMap const& r2 : R2) {
    vector<RangeMap> replacement_maps;
    erase_if(R1, [&](RangeMap const& r1){
        bool const does_intersect = intersects(r1.destination(), r2.source());
        if (does_intersect) {
          BAD R2.erase(r2);  // invalidates iterators
          for (auto&& replacement_map : flatMap(r1, r2))
            replacement_maps.push_back(replacement_map);
        }
        return does_intersect;
    });
    copy(begin(replacement_maps), end(replacement_maps), back_inserter(result));
  }

  sort(begin(result), end(result));
  return result;
}


void run_tests() {
  assert_eq(flatten({}, {}), {});
  assert_eq(flatten({}, {{2, 1, 1}}), {{2, 1, 1}});
  assert_eq(flatten({{2, 1, 1}}, {}), {{2, 1, 1}});
  assert_eq(flatten({{4, 3, 1}}, {{2, 1, 1}}), {{2, 1, 1}, {4, 3, 1}});

  assert_eq(flatten({{2, 1, 1}}, {{1, 2, 1}}), {{1, 2, 1}});

  cout << "All Tests Passed\n";
}


int main() {
//  vector<Range> seeds;
//  vector<RangeMap>
//      seed_to_soil,
//      soil_to_fertilizer,
//      fertilizer_to_water,
//      water_to_light,
//      light_to_temperature,
//      temperature_to_humidity,
//      humidity_to_location;
//
//  for (string line{}; getline(cin, line);) {
//    if (startswith(line, "seeds:")) parse_ranges(line.substr(6), seeds);
//    else if (startswith(line, "seed-to-soil")) parse_map(seed_to_soil);
//    else if (startswith(line, "soil-to-fertilizer")) parse_map(soil_to_fertilizer);
//    else if (startswith(line, "fertilizer-to-water")) parse_map(fertilizer_to_water);
//    else if (startswith(line, "water-to-light")) parse_map(water_to_light);
//    else if (startswith(line, "light-to-temperature")) parse_map(light_to_temperature);
//    else if (startswith(line, "temperature-to-humidity")) parse_map(temperature_to_humidity);
//    else if (startswith(line, "humidity-to-location")) parse_map(humidity_to_location);
//  }

  run_tests();
}
