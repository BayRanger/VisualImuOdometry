import numpy as np
from decimal import Decimal


filename ="euroc_mh05_groundtruth.csv"
my_data =[]
file1 = open("gt_pose.txt","w")

with open(filename) as f:
    for i,line in enumerate(f):
        if(i==0):
            continue
        str_ele=""
        for j,ele in enumerate(line.strip().split(",")):
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
#print(my_data[1])      

file1.close()
