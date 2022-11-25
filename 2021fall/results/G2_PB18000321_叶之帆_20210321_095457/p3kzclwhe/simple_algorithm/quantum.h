#ifndef PARA_SCHEDULER_QUANTUM_H
#define PARA_SCHEDULER_QUANTUM_H
#include <vector>
#include <string>
#define TOTAL_BITS 20

typedef struct{
    std::string name;

    int time_cost;
    int bit_useage;

    int repeat_times;

    //To be scheduled
    std::vector<int> start_points;
    std::vector<int> end_points;
} quantum_program;


#endif //PARA_SCHEDULER_QUANTUM_H