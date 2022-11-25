OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
creg c[5];
cx q[3],q[1];
cx q[2],q[4];
h q[2];
x q[1];
cx q[4],q[1];
h q[1];
cx q[1],q[2];
x q[4];
measure q -> c;


