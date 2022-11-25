#ifndef PRACTICAL_SCHEDULER_QUANTUM_H
#define PRACTICAL_SCHEDULER_QUANTUM_H
#include <vector>
#include <string>

#define RegionNum 2

typedef struct{
    std::string name;

    int map_count;
    std::vector<std::vector<bool>> region_usage;
    std::vector<int> time_cost;


    int repeat_times;

    //To be scheduled
    std::vector<int> start_points;
    std::vector<int> end_points;
} quantum_program;


#endif //PRACTICAL_SCHEDULER_QUANTUM_H
