#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include "quantum.h"
#include "scheduler.h"

//最小优先队列的比较方法

namespace{
struct cmp{
    bool operator() (std::pair<int,std::vector<bool>> l, std::pair<int,std::vector<bool>> r){
        return l.first < r.first;
    }
};

int region_usage_count(std::vector<bool> region){
    int count=0;
    for(int i=0; i<RegionNum; i++){
        count += region[i]?1:0;
    }
    return count;
}

bool region_converge(std::vector<bool> region, std::vector<bool> occupied){
    for(int i=0; i<RegionNum; i++){
        if(region[i] && occupied[i])
            return true;
    }
    return false;
}

void region_occupy(std::vector<bool> region, std::vector<bool>& occupied){
    for(int i=0; i<RegionNum; i++){
        if(region[i])
            occupied[i] = true;
    }
}

void region_release(std::vector<bool> region, std::vector<bool>& occupied){
    for(int i=0; i<RegionNum; i++){
        if(region[i])
            occupied[i] = false;
    }
}

}


int greedy_scheduler(std::vector<quantum_program>& programs){
    std::sort(programs.begin(),programs.end(),
                [](quantum_program& l, quantum_program& r){
                    return region_usage_count(l.region_usage[0]) > region_usage_count(r.region_usage[0]);
                });

    //Greedily Schedule
    int time=0;
    std::vector<bool> occupied(RegionNum, false);

    std::priority_queue<std::pair<int, std::vector<bool>>, std::vector<std::pair<int, std::vector<bool>>>, cmp>
            timeline;
    //timeline represents in progress programs, 最小优先队列
    while(true){
        //Release occupied bits
        if(!timeline.empty()){
            auto next = timeline.top();
            timeline.pop();
            time = next.first;
            region_release(next.second, occupied);
        }
        //Release bits that should be released at the same time
        while(!timeline.empty() && timeline.top().first == time){
            auto next = timeline.top();
            timeline.pop();
            region_release(next.second, occupied);
        }

        for(auto& p: programs){
            if(p.repeat_times > 0){
                for(int i=0; i < p.map_count; i++){
                    if(!region_converge(p.region_usage[i], occupied)){
                        p.start_points.push_back(time);
                        p.end_points.push_back(time+p.time_cost[i]);
                        p.repeat_times--;

                        region_occupy(p.region_usage[i], occupied);
                        timeline.push(std::make_pair(p.end_points.back(), p.region_usage[i]));
                    }
                }
            }
        }

        if(timeline.empty()){
            break;
        }
    }

    return time;
}