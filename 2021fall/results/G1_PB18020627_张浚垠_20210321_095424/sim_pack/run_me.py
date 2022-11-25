from simplify_pack import simplify_with_rule
from qiskit import QuantumCircuit, ClassicalRegister, QuantumRegister

# import circuits and simplification rule
fp_circuit = open('example/original_circuit.qasm')
list = fp_circuit.readlines()
fp_circuit.close()

fp_rule = open('rule/rule1.txt')
list_rule = fp_rule.readlines()
fp_rule.close()

list_new = simplify_with_rule(list, list_rule)# get the simplified result


#Save the result
fnew = open('example/new_circuit.qasm','w')
fnew.writelines(list_new)
fnew.close()

# print out the result(if possible)
circuit = QuantumCircuit.from_qasm_file("example/original_circuit.qasm")
circuit.draw(output='mpl')
circuit = QuantumCircuit.from_qasm_file("example/new_circuit.qasm")
circuit.draw(output='mpl')
