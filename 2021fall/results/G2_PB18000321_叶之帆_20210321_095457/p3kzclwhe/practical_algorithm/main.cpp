#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include "quantum.h"
#include "scheduler.h"

void printhelp(char* arg0){
    using namespace std;
    cerr << "usage: " << arg0 << "[inputs] [args]" << endl;
    cerr << "args:" << endl;
    cerr << "inputs:" << endl;
    cerr << "\t(<input filename> <repeat_times>) appears in pair"
         << "\tset input qasm files and their repeat times" << endl;
}


int main(int argc, char** argv){
    if(argc < 2){
        printhelp(argv[0]);
    }
    else if(argc %2 ==0){
        std::cerr << "Illegal input number." << std::endl;
        exit(1);
    }

    //Load useful information
    std::vector <quantum_program> input_programs;
    for(int i=0; i<argc/2; i++){
        quantum_program program;
        std::ifstream input(argv[i*2+1]);
        input >> program.name;

        input >> program.map_count;

        program.region_usage.resize(program.map_count);
        program.time_cost.resize(program.map_count);
        for(int j=0; j< program.map_count; j++){
            program.region_usage[j].resize(RegionNum);
            for(int k=0; k<RegionNum; k++){
                int temp;
                input >> temp;
                program.region_usage[j][k] = (temp==1);
            }
            input >> program.time_cost[j];
        }
        program.repeat_times = atoi(argv[i*2+2]);
        input_programs.push_back(program);
        input.close();
    }

    //Schedule
    std::cout << input_programs.size() << std::endl;
    int total_time = greedy_scheduler(input_programs);

    //Output
    std::cout << "Total time cost: " << total_time << std::endl;

#ifdef DEBUG
    for(auto p:input_programs){
        std::cout << "Program: " << p.name << std::endl;
        for(int i=0; i<p.start_points.size(); i++){
            std::cout << "\t" << i << ">\tstart point: " << p.start_points[i] << "\t\tend point: " << p.end_points[i]
                      << std::endl;
        }
    }
#endif
}
