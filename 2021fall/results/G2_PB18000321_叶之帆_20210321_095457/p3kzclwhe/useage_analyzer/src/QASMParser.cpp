#include <sstream>
#include <cassert>
#include <cstring>
#include "QASMParser.h"

using std::string;

/**
 * read and parse `qname [ index ]` from line_stream
 */
void getQubit(std::istringstream &line_stream, string &qname, int &index) {
    char c;
    // skip blanks
    do {
        c = line_stream.get();
    } while(isblank(c));
    // read qubit name
    while(true) {
        if(
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '1' && !qname.empty()) ||
            c == '_'
        ) {
            qname.push_back(c);
        } else {
            break;
        }
        c = line_stream.get();
    }
    if(qname.empty())
        throw ParserException("except qubit name");
    // skip blanks
    while(isblank(c)) {
        c = line_stream.get();
    }
    // read qubit array size
    if(c != '[')
        throw ParserException("except '['");
    if(!(line_stream >> index))
        throw ParserException("except index or array size");
    // skip blanks
    do {
        c = line_stream.get();
    } while(isblank(c));
    // read qubit array size
    if(c != ']')
        throw ParserException("except ']'");
}

void getGate(std::istringstream &line_stream, string &gate_name) {
    int bra = 0;
    
    char c;
    // skip blanks
    do {
        c = line_stream.get();
    } while(isblank(c));
    // read qubit name
    while(true) {
        if(
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9' && !gate_name.empty()) ||
            c == '_'
        ) {
            gate_name.push_back(c);
        } else if(c == '(') {
            gate_name.push_back(c);
            bra++;
        } else if(c == ')') {
            gate_name.push_back(c);
            bra--;
            assert(bra >= 0);
        } else if(bra > 0) {
            if(c == EOF) {
                throw ParserException("except ')'");
            }
            gate_name.push_back(c);
        } else {
            if(isblank(c) || c == EOF)
                break;
            if(c == '/') {
                c = line_stream.get();
                if(c == '/') {
                    break;
                }
            }
            throw ParserException("invalid charactor in gate name");
        }
        c = line_stream.get();
    }
    // if(gate_name.empty())
    //    throw ParserException("except gate name");
}

int QASMParser::getQubit(std::istringstream &line_stream) {
    string qname;
    int index;
    ::getQubit(line_stream, qname, index);
    if(qubit_map.count(qname) == 0) {
        throw ParserException("qubit not defined");
    }
    auto &q_arr = qubit_map[qname];
    if(index >= q_arr.second)
        throw ParserException("index out of range");
    return q_arr.first + index;
}

int QASMParser::getQubitCount() {
    while(true) {
        if(!readline())
            return qcount;
        std::istringstream line_stream(line);
        string qreg_s;
        line_stream >> qreg_s;
        if(qreg_s == "")
            continue;
        if(qreg_s == "OPENQASM")
            continue;
        if(qreg_s == "include")
            continue;
        if(qreg_s == "creg")
            continue;
        if(qreg_s == "gate") {
            while(qreg_s != "}") {
                if(!readline())
                    throw ParserException("except '}'");
                std::istringstream line_stream2(line);
                line_stream2 >> qreg_s;
            }
            continue;
        }
        if(qreg_s == "qreg") {
            string qname;
            int qsize;
            ::getQubit(line_stream, qname, qsize);
            addQreg(qname, qsize);
            continue;
        }
        unreadline();
        used = new bool[qcount];
        for(int i = 0; i < qcount; i++) {
            used[i] = false;
        }
        return qcount;
    } 
}

void QASMParser::addQreg(string &name, int size) {
    if(qubit_map.count(name))
        throw ParserException("qreg mult-defined");
    qubit_map[name] = std::pair<int, int>(qcount, size);
    qcount += size;        
}

const char *QASMParser::getInstruction(int &q1, int &q2) {
    while(true) {
        if(!readline())
            return nullptr;
        std::istringstream line_stream(line);
        string gate;
        getGate(line_stream, gate);
        if(gate == "")
            continue;
        if(gate == "measure") {
            continue;    
        }
        if(gate == "barrier") {
            continue;
        }
        if(gate == "reset") {
            continue;
        }
        q1 = getQubit(line_stream);
        used[q1] = true;
        char c;
        line_stream >> c;
        if(c == ',') {
            q2 = getQubit(line_stream);
            used[q2] = true;
        } else if(c == ';') {
            q2 = -1;
        } else {
            throw ParserException("except ',' or ';'");
        }
        auto &gate_str = *(gate_set.insert(gate).first);
#ifdef TESTING
        gate_count++;
        if(gate_str[0] == 'c' || gate_str[0] == 'C')
            cnot_count++;
        if(gate_str == "swap" || gate_str == "intrinsic_swap__") {
            swap_count++;
        }
        if( (gate_str.length() >= 2 && (
            (gate_str[0] == 'u' && gate_str[1] == '1') || (tolower(gate_str[0]) == 'r' && tolower(gate_str[1]) == 'z')
            )) ||
            gate_str == "t" || gate_str == "T" || gate_str == "z" || gate_str == "Z" || 
            gate_str == "tdg" || gate_str == "Tdg" || gate_str == "S" || gate_str == "s" || 
            gate_str == "Sdg" || gate_str == "Sdg"
        ) {
           u1_count++; 
        }
#endif
        return gate_str.c_str();
    }
}

const char *ParserException::what() const noexcept {
    return msg;
}