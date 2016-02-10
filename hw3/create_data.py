import random

def writeFile(f):
    f.write("site,timestamp,util,dmo,resp\n")

    for site in range(2):
        for day in range(20*7):
            r0 = 0.0
            r1 = 0.0
            r2 = 0.0
            if site == 0:
                f.write("local,")
                r0 = random.normalvariate(.4, .2)
                r1 = random.normalvariate(.1, .1)
                r2 = random.normalvariate(.3, .3)
            else:
                f.write("remote,")
                r0 = random.normalvariate(.7, .2)
                r1 = random.normalvariate(.5, .2)
                r2 = random.normalvariate(.6, .2)

            f.write(str(day)+",")
            f.write(str(abs(r0)) +","+  str(abs(r1)) +","+ str(abs(r2))  + "\n") 

f = open("FCFS.csv", "w")
writeFile(f)
f.close()

f = open("Best-Fit.csv", "w")
writeFile(f)
f.close()

f = open("Greedy.csv", "w")
writeFile(f)
f.close()

f = open("Proxy-Aware.csv", "w")
writeFile(f)
f.close()
