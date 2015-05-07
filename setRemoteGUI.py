import os
os.system("sudo xauth -f /var/run/lightdm/root/:0 list > output")
with open("output") as f:
    for line in f:
        hexNum = line.strip().split(' ')[4]
os.system("xauth add :0 MIT-MAGIC-COOKIE-1 " + hexNum) 
