#ifndef QASM_PARSER_H
#define QASM_PARSER_H

#include <iostream>
#include <set>
#include <string>
#include <exception>
#include <map>
#include "basis.h"

// OpenQASM Parser
// `module` and `include` is not supported
// must call `getQubitCount` first, then `getInstruction` until you get a nullptr as return value

class QASMParser {
public:
    /**
     * is: input stream, the parser will read circuit from it.
     */
    inline QASMParser(std::istream &is)
        : is(is) {
        }

    /**
     * read and parse the qubit declaration statements
     * return count of qubits
     * uniform the index if there are more than one qubit array
     * e.g. for `qreg a[2], b[3]`, we set the uniformed index as
     * 0: a[0], 1: a[1], 2: b[0], 3: b[1], 4: b[2]
     */
    int getQubitCount();

    /**
     * read and parse an instruction (usually it is a quantum gate)
     * return the type of the gate (or instruction), e.g. "cnot"
     * return nullptr if failed
     * q1: output, index of the first parament qubit
     * q2: output, index of the second parament qubit
     * the index is set to -1 if no such parament
     * e.g. q2 is set to -1 for single-qubit gate 
     * QASMParser will save the instruction type and free it when destructing
     * return pointer to the same object if the types are equal,
     * that is, the QASMParser would have only one copy of the intruction-type string
     */
    const char *getInstruction(int &q1, int &q2);

    inline const char *getCurrentLine() const {
        return line;
    }

    inline int getUsedQubitCount() {
        int c = 0;
        for(int i = 0; i < qcount; i++) {
            if(used[i]) {
                c++;
            }
        }
        return c;
    }


private:

    /**
     * read a line ends with ';' and save it to `line`
     */
    inline bool readline() {
        if(read_new_line) {
            bool ret = (bool)(is.getline(line, sizeof(line)));
            for(int i = 0; i < 511; i++) {
                if(line[i] == 0)
                    break;
                if(line[i] == '/' && line[i + 1] == '/') {
                    line[i] = 0;
                    break;
                }
            }
            return ret;
        } else {
            read_new_line = true;
            return true;
        }
    }

    inline void unreadline() {
        read_new_line = false;
    }

    /**
     * add a qubit array (qreg)
     */
    void addQreg(std::string &name, int size);
    
    /**
     * read and parse a qubit (`q[2]` for example) and return its index
     */
    int getQubit(std::istringstream &line_stream);

private:
    std::istream &is;
    std::set<std::string> inst_types;
    char line[512] = ""; // input line buffer
    // the map table of qubits, <name, <start index, array size>>
    std::map<std::string, std::pair<int, int>> qubit_map;
    std::set<std::string> gate_set;
    int qcount = 0; // count of qubits
    bool read_new_line = true;  // whether read a new line on next readline() or use the last line
    bool *used;
    
public:
#ifdef TESTING
    int swap_count = 0;
    int gate_count = 0;
    int cnot_count = 0;
    int u1_count = 0; // count of u1 or rz (including z, s, t, sdg, tdg)
#endif
}; // QASMParser

class ParserException : public std::exception {
public:
    inline ParserException(const char *msg) {
        this->msg = msg;
    }

    const char *what() const noexcept;
private:
    const char *msg;
};

#endif // QASM_PARSER_H