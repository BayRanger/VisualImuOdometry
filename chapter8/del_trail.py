import numpy as np
from decimal import Decimal


filename ='/home/chahe/project/VisualImuOdometry/chapter8/VINS-Course/bin/pose_output.txt' 
my_data =[]
file1 = open("esti_data.txt","w")
with open(filename) as f:
    for line in f:
        #line_str =(line[:-1])
        new_str=""
        for j,ele in enumerate(line.strip().split()):
            if(j==0):
                ele = Decimal(float(ele))
                ele = str(ele)[:17]
            new_str+= str(ele)
            if(j!=7):
                new_str+=' '
            else:    
                new_str +="\n"
        print(new_str)
        file1.writelines(new_str)

file1.close()

        
'''

for i,line in enumerate(f):
        if(i==0):
            continue
        str_ele=""
            if(j==0):
                ele = Decimal(float(ele)/1000000000)
                ele = str(ele)[:17]
            str_ele= str_ele +str(ele)
            if(j!=7):
                str_ele+=" "
            if (j==7):
                str_ele+="\n"
                file1.writelines(str_ele)
                print(str_ele)
                break

    '''
#file1.close()
