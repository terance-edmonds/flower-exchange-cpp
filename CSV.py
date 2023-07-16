import csv
import random

name= ["Rose","Lavender","Lotus","Tulip","Orchid"]

test_str = 'aa'

     
with open('lg.csv', 'w' ,newline='') as csvfile:    
    fieldnames = ['Cl.Ord.ID', 'Instrument','Side','Quantity','Price']    
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)    
     
    writer.writeheader()
    
    for i in range(500000):
        res = test_str + str(i).zfill(2)
        Q = random.randint(10, 1000)
        p = random.randint(0, 600)
        r3 = random.randint(0, 4)
        s = random.randint(1, 2)
        writer.writerow({'Price': p, 'Quantity': Q, 'Side':s, 'Instrument': name[r3] ,'Cl.Ord.ID':str(res)})    
        continue
     
print("Writing complete")  
