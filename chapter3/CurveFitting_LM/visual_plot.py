import numpy as np
import matplotlib.pyplot as plt

print("fuck you")
stat=[]
with open("data.txt","r") as data:
    for line in data:
        str = line.split()
        for ele in str:
            try:
                num = float(ele)
                stat.append(num)
                print(num)
            except Exception as e:
                print(e)

stat=np.array(stat).reshape((-1,3))

print(stat)

#plt.plot(stat[:,0],stat[:,1])
plt.plot(stat[:,0],stat[:,2])
plt.xlabel("#iter")
plt.ylabel("value of u(lambda)")
plt.show()                
