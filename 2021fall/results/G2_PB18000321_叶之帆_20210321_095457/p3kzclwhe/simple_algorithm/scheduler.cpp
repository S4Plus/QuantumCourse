//
// Created by yzf on 2021/2/17.
//
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include "quantum.h"
#include "scheduler.h"

//最小优先队列的比较方法
struct cmp{
    bool operator() (std::pair<int,int> l, std::pair<int,int> r){
        return l.first > r.first;
    }
};


int greedy_scheduler(std::vector<quantum_program>& programs){
    std::sort(programs.begin(),programs.end(),
              [](quantum_program& l, quantum_program& r){return l.bit_useage > r.bit_useage;});

    //Check
    for(auto p : programs){
        if(p.bit_useage > TOTAL_BITS){
            std::cerr << "Program named " << p.name << " requires " << p.bit_useage
                << ", which is larger than TOTAL_BITS=" << TOTAL_BITS << std::endl;
            exit(2);
        }
        else{
            break;
        }
    }

    //Greedily Schedule
    int time=0;
    int occupied=0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, cmp>
        timeline;
        //timeline represents in progress programs, 最小优先队列
    while(true){
        //Release occupied bits
        if(!timeline.empty()){
            auto next = timeline.top();
            timeline.pop();
            time = next.first;
            occupied -= next.second;
        }
        while(!timeline.empty() && timeline.top().first == time){
            auto next = timeline.top();
            timeline.pop();
            occupied -= next.second;
        }

        for(auto& p: programs){
            while(p.bit_useage <= TOTAL_BITS-occupied && p.repeat_times > 0){
                p.start_points.push_back(time);
                p.end_points.push_back(time+p.time_cost);
                p.repeat_times--;

                occupied += p.bit_useage;
                timeline.push(std::make_pair(p.end_points.back(), p.bit_useage));
            }
        }

        if(timeline.empty()){
            break;
        }
    }

    return time;
}