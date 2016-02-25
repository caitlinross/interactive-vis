def combine(paramType, start, end, num_pe):
    out = open("alldata-"+paramType+".csv", "w")
    header_written = False
    for batch in range(start,end+1):
        for pe in range(num_pe):
            f = open("dragonfly-"+paramType+"/"+paramType+str(pow(2,batch))+"-"+str(pe)+".txt", "r")
            for line in f:
                if line.startswith("Forced") and not header_written:
                    header_written = True
                    out.write(paramType+",")
                    tokens = line.split(",")
                    del tokens[0]
                    del tokens[4]
                    del tokens[5]
                    del tokens[6]
                    del tokens[6]
                    out.write(",".join(tokens))
                elif not line.startswith("Forced"):
                    tokens = line.split(",")
                    del tokens[0]
                    del tokens[4]
                    del tokens[5]
                    del tokens[6]
                    del tokens[6]
                    out.write(str(pow(2,batch))+",")
                    out.write(",".join(tokens))
            f.close()

    out.close()

combine("batch", 1, 7, 8)
combine("gvt", 4, 11, 8)
