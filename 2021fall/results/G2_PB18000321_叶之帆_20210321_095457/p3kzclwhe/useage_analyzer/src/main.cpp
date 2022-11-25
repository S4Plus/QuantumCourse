#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "qmachine.h"
#include "QASMParser.h"

using std::cin;
using std::endl;
using std::cerr;
using std::cout;
using std::string;

// analyze a qasm circuit and output its gate count, max time, etc.

int main(int argc, char **argv) {
    int m, n;
    if(argc != 2) {
        cerr << "usage: " << argv[0] << " filename" << endl;
        return 0;
    }
    auto is = std::ifstream(argv[1]);


    std::ostringstream output;

    QASMParser parser(is);

    // cerr << "DEBUG: initialized" << endl;

    try {
        int qcount = parser.getQubitCount();
        int *qtime = new int[qcount];
        for(int i = 0; i < qcount; i++)
            qtime[i] = 0;

        while(true) {
            int q1, q2;
            const char *gate = parser.getInstruction(q1, q2);
            if(gate == nullptr)
                break;
            if(q2 == -1) {
                qtime[q1] += RX_TIME;
            } else {
                int time = CNOT_TIME;
                if(
                        strcmp(gate, "swap") == 0 ||
                        strcmp(gate, "intrinsic_swap__") == 0
                        )
                    time = SWAP_TIME;
                qtime[q1] = qtime[q2] = std::max(qtime[q1], qtime[q2]) + time;
            }
        }

        int max_time = 0;
        for(int i = 0; i < qcount; i++) {
            max_time = std::max(max_time, qtime[i]);
        }
        delete[] qtime;

        // -------- OUTPUT --------
        const char *filename = argv[1];
        const char *p = filename;
        while(*p) {
            if(*(p++) == '/') {
                filename = p;
            }
        }
        cout << filename
             // << ", " << parser.getUsedQubitCount()
             << ", " << parser.gate_count
             << ", " << parser.cnot_count
             << ", " << parser.swap_count
             << ", " << parser.u1_count
             << ", " << parser.gate_count - parser.u1_count + parser.cnot_count * 9 + parser.swap_count * 29 // fidelity (defined by ibm's competition)
             << ", " << max_time
             << endl;
        cout << output.str();
    } catch (const std::exception &e) {
        cerr << "error: " << e.what() << endl;
        cerr << "\tat: " << parser.getCurrentLine() << endl;
        return -1;
    }
    cerr << endl;
    return 0;
}