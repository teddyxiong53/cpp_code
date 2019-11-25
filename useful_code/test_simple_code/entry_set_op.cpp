#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <set>
#include <stdio.h>

typedef std::pair<int, int> Entry;
typedef std::set<Entry> TimerList;

int main()
{
    TimerList timers ;
    timers.insert({10,1});
    timers.insert({20,2});
    timers.insert({30,3});
    timers.insert({40,4});
    std::vector<Entry> expired;
    Entry sentry(30,3);
    TimerList::iterator end = timers.lower_bound(sentry);
    std::copy(timers.begin(), end, std::back_inserter(expired));
    timers.erase(timers.begin(), end);
    //
    printf("timers content:\n");
    for(const Entry& it: timers) {
        std::cout << it.first << " : " << it.second <<std::endl;
    }
    printf("expired content:\n");
    for(const Entry& it: expired) {
        std::cout << it.first << " : " << it.second << std::endl;
    }
}
