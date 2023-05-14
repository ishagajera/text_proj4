import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
f = open("random.txt", "r")
newdf = []
list = []
for x in f:
  newlist =[]
  splitter = x.split()
  list.append(splitter[0])
  for itr in splitter[1:]:
    newlist.append(float(itr))
  data = {
    splitter[0] : newlist
  }
  newdf.append(data) 
print(newdf)
newval = pd.DataFrame()
newval['bitap'] = newdf[0]['bitap']
newval['brute'] = newdf[3]['brute']
newval['rk'] = newdf[1]['rk']
newval['kmp'] = newdf[4]['kmp']
newval['bmh'] = newdf[2]['bmh']
print(newval)
