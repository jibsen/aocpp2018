//
// Advent of Code 2018, day 4, part two
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Event = std::pair<long long, std::string>;
using SleepSchedule = std::vector<std::pair<int, int>>;

bool get_timestamp(long long &ts)
{
	int year, month, day, hour, minute;

	if (scanf("[%d-%d-%d %d:%d] ", &year, &month, &day, &hour, &minute) == 5) {
		ts = year  * 100000000LL
		   + month * 1000000LL
		   + day   * 10000LL
		   + hour  * 100LL
		   + minute;

		return true;
	}

	return false;
}

std::vector<Event> read_events()
{
	std::vector<Event> events;
	std::string line;
	long long ts = 0;

	while (get_timestamp(ts) && std::getline(std::cin, line)) {
		events.emplace_back(ts, line);
	}

	return events;
}

std::map<int, SleepSchedule> get_guard_sleep_schedules(const std::vector<Event> &events)
{
	std::map<int, SleepSchedule> schedules;

	int guard = -1;
	std::vector<int> minutes;

	for (const auto &[ts, line] : events) {
		if (auto p = line.find('#'); p != std::string::npos) {
			guard = std::stoi(line.substr(p + 1));
			continue;
		}

		minutes.push_back(ts % 100);

		if (minutes.size() == 2) {
			schedules[guard].push_back({minutes[0], minutes[1]});
			minutes.clear();
		}
	}

	return schedules;
}

std::pair<int, int> minute_most_asleep(const SleepSchedule &schedule)
{
	std::array<int, 60> sleep_freq = {};

	for (const auto &period : schedule) {
		for (int i = period.first; i < period.second; ++i) {
			sleep_freq[i]++;
		}
	}

	int max_minute = std::distance(sleep_freq.begin(), std::max_element(sleep_freq.begin(), sleep_freq.end()));

	return {max_minute, sleep_freq[max_minute]};
}

int main()
{
	auto events = read_events();

	std::sort(events.begin(), events.end());

	auto schedules = get_guard_sleep_schedules(events);

	int max_freq = std::numeric_limits<int>::min();
	int result = -1;

	for (const auto &[guard, schedule] : schedules) {
		auto [max_minute, freq] = minute_most_asleep(schedule);

		if (freq > max_freq) {
			max_freq = freq;
			result = guard * max_minute;
		}
	}

	std::cout << result << '\n';

	return 0;
}
