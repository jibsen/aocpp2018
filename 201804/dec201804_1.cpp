//
// Advent of Code 2018, day 4, part one
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <numeric>
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

int minute_most_asleep(const SleepSchedule &schedule)
{
	std::array<int, 60> sleep_freq = {};

	for (const auto &period : schedule) {
		for (int i = period.first; i < period.second; ++i) {
			sleep_freq[i]++;
		}
	}

	return std::distance(sleep_freq.begin(), std::max_element(sleep_freq.begin(), sleep_freq.end()));
}

int main()
{
	auto events = read_events();

	std::sort(events.begin(), events.end());

	auto schedules = get_guard_sleep_schedules(events);

	int max_time = std::numeric_limits<int>::min();
	int max_guard = -1;

	for (const auto &[guard, schedule] : schedules) {
		int time = std::accumulate(schedule.begin(), schedule.end(), 0,
			[](int acc, const auto &period) {
				return acc + (period.second - period.first);
			});

		if (time > max_time) {
			max_time = time;
			max_guard = guard;
		}
	}

	std::cout << max_guard * minute_most_asleep(schedules[max_guard]) << '\n';

	return 0;
}
