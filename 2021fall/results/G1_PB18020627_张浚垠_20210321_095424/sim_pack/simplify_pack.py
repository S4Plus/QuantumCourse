import random
import re
phase_gate_error = 1e-3

def find_first_gate(list,start_index,str_gate_name):
    if start_index < len(list):
        for line_index in range(start_index,len(list)):
            line = list[line_index]
            gate_sign = line.find(str_gate_name) #find the CX gate
            if gate_sign != -1:# find the first gate
                number_list = [int(s) for s in re.findall(r'\b\d+\b', line)]
                return [line_index,number_list]
        else:
            return None
    else:
        return None

def simplify_with_rule(list,list_rule):
    #######################################################################
    for line_index in range(len(list_rule)):
        line = list_rule[line_index]
        from_sign = line.find('FROM')
        if from_sign != -1:
            from_index = line_index

        to_sign = line.find('TO')
        if to_sign != -1:
            to_index = line_index

    #print(list_rule[to_index])

    list_from = list_rule[from_index+1:to_index-1]
    list_to = list_rule[to_index+1:len(list)]


    #####################################################################
    # Find the max_index, and then create the related register for it
    max_index = 0
    for line_index in range(len(list_from)):
        line = list_from[line_index]
        line_split = line.split(' ')
        if line_split[0] in ['rx','ry','rz']:
            number_list = [0]
            number_list.append([int(s) for s in re.findall(r'\b\d+\b', line_split[3])][0])
        else:
            number_list = [int(s) for s in re.findall(r'\b\d+\b', line)]
        if max(number_list[1:]) > max_index:#The first one is the pin_index
            max_index = max(number_list[1:])



    #####################################################################
    simplify_index = 0
    while(simplify_index < len(list)):#Check the circuit iff line_index > simplify_index
        print("Checking line [%d]\n"%simplify_index)
        s_good = 1
        for line_index in range(simplify_index,len(list)):#Check for simplification before simplify_index
            s_good = 1
            line_from_index = 0
            corr_list = []
            for i in range(max_index+1):
                corr_list.append(-1)
            line_list = []
            line_list_count = 0
            for i in range(len(list_from)):
                line_list.append(-1)

            line_run = line_index
            while(line_from_index < len(list_from)):# Check the from_list one by one for simplification rules
            # the continue here means looks for the simplification from next circuit line
                line_from_ori = list_from[line_from_index]
                line_from = line_from_ori.split(' ')
                pin_num = int(line_from[1])#这个门是一个多少量子比特的门
                find_ans = find_first_gate(list,line_run,line_from[0])# The First Gate
                if find_ans == None:
                    s_good = 0
                    break
                else:
                    line_run = find_ans[0]+1
                    number_list = find_ans[1]
                    line_read = list[find_ans[0]]
                    line_read_split = line_read.split(' ')
                    if line_read_split[0][0:2] in ['rx','ry','rz'] and line_read_split[0][0:2] == line_from[0]:
                        number_list = [int(s) for s in re.findall(r'\b\d+\b', line_read_split[1])]
                        phase_from = float(re.findall("\d+\.\d+", line_from[2])[0])
                        phase_read = float(re.findall("\d+\.\d+", line_read_split[0])[0])
                        if abs(phase_from - phase_read) < phase_gate_error: # A special concerns for the phase gates
                            if corr_list[int(line_from[3])] == -1:
                                corr_list[int(line_from[3])] = number_list[0]
                            else:
                                if corr_list[int(line_from[3])] != number_list[0]:
                                    continue
                            line_list[line_list_count] = find_ans[0]
                            line_list_count = line_list_count + 1
                            line_from_index = line_from_index + 1
                        else:
                            continue
                    else:
                        if pin_num == 1:
                            if corr_list[int(line_from[2])] == -1:
                                corr_list[int(line_from[2])] = number_list[0]
                            else:
                                if corr_list[int(line_from[2])] != number_list[0]:
                                    continue
                            line_list[line_list_count] = find_ans[0]
                            line_list_count = line_list_count + 1
                            line_from_index = line_from_index + 1
                        if pin_num == 2:
                            if corr_list[int(line_from[2])] == -1:
                                corr_list[int(line_from[2])] = number_list[0]
                            else:
                                if corr_list[int(line_from[2])] != number_list[0]:
                                    continue
                            if corr_list[int(line_from[3])] == -1:
                                corr_list[int(line_from[3])] = number_list[1]
                            else:
                                if corr_list[int(line_from[3])] != number_list[1]:
                                    corr_list[int(line_from[2])] = -1
                                    continue
                            line_list[line_list_count] = find_ans[0]
                            line_list_count = line_list_count + 1
                            line_from_index = line_from_index + 1
            else:
                break
            if s_good == 1:
                break

        ## Notes
        # If the line on check is fitted to the from_list, we update the corr_list + add one elements to the line_list
        # If not, we need to undo the changes


        ##############################################################
        #Check for the diturbing terms
        #Extract the Protection List
        #The phase gate is not included yet
        if s_good == 1:
            print("Sub-Graph detected   ")
            protection_list = []
            for index in range(len(list_from)-1):
                temp_list = []
                for temp_index in range(index+1,len(list_from)):
                    line_read_split = list_from[temp_index].split(' ')
                    if line_read_split[0] in ['rx','ry','rz']:
                        number_list = [-1]
                        number_list.append(line_read_split[3])
                    else:
                        number_list = [int(s) for s in re.findall(r'\b\d+\b', list_from[temp_index])]
                    for num in number_list[1:]:
                        temp_list.append(corr_list[int(num)])
                protection_list.append(temp_list)

            # Check the protective
            check_index = 0
            for index in range(min(line_list)+1,max(line_list)):
                if index >= line_list[check_index+1]:
                    check_index = check_index + 1
                if not (index in line_list):
                    line = list[index]
                    line_split = line.split(' ')
                    if line_split[0][0:2] in ['rx','ry','rz']:
                        s = []
                        s.append(line_split[1])
                    else:
                        s = [int(s) for s in re.findall(r'\b\d+\b', line)]
                    for num in s:
                        if num in protection_list[check_index]:
                            s_good = 0
                            break

        ###############################################################
        # 实现替换
        if s_good == 1:
            print("Simplified Successfully\n")
            for i in range(len(line_list)):
                del list[line_list[len(line_list)-1-i]]
                #print(line_list[len(line_list)-1-i])
            index_insert = line_list[0]
            for to_index in range(len(list_to)):
                line_to_ori = list_to[to_index]
                line_to = line_to_ori.split(' ')
                pin_num = int(line_to[1])
                # if pin_num == 1:
                #     list.insert(index_insert,line_to[0]+' '+'q[%d];\n'%corr_list[int(line_to[2])])
                # if pin_num == 2:
                #     list.insert(index_insert,line_to[0]+' '+'q[%d],q[%d];\n'%(corr_list[int(line_to[2])],corr_list[int(line_to[3])]))
                if line_to[0] in ['rx','ry','rz']:
                    phase = float(re.findall("\d+\.\d+", line_to[2])[0])
                    list.insert(index_insert,line_to[0]+'(%f) q[%d];\n'%(phase,corr_list[int(line_to[3])]))
                else:
                    q_str = ''
                    for pin_index in range(pin_num):#0-4(pin_num = 5)
                        q_str = q_str + 'q[%d]'%corr_list[int(line_to[pin_index+2])]
                        if pin_index < pin_num - 1:
                            q_str = q_str + ','
                        else:
                            q_str = q_str + ';\n'
                    list.insert(index_insert,line_to[0]+' '+q_str)
                index_insert = index_insert + 1

        simplify_index = simplify_index + 1
    return list




####################################################################





