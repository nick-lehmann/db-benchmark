
def oneBits(number):
    bits = []
    for i in range (0, 8, 1):
        if (number % 2) == 1:
            #bit position needs to be inverted, since AVX counts from the other side or someting (whatever bit order needs to be reversed for correct results)
            bits.append(7-i)
        number = number // 2
    #bit list needs to be reversed to have them in ascending order
    bits.reverse()
    return bits


def bitsRuns(bits):
    if(bits == []):
        return []
    runs = []
    run = 1
    begin = bits[0]
    last = bits[0]
    for i in range (1, len(bits), 1):
        if bits[i] == last + 1:
            run += 1
            last += 1
        else:
            runs.append((begin,run))
            run = 1
            begin = bits[i]
            last = bits[i]
    runs.append((begin, run))
    return runs
    
    
def sortKey(number):
    return len(oneBits(number))
    
    
def getMaskList():
    a = [i for i in range(0, 256, 1)]
    a.sort(key=sortKey)
    return a
    

def copyFileContent(f, fileName):
    f2 = open(fileName, "r")
    for line in f2:
        f.write(line)
    
    
    
########################################################################### Main
f = open("copyMaskedTuple.h", "w")

copyFileContent(f, "head.txt")

f.write("template<typename T>\n")
f.write("uint8_t _copyTuple(T * destAddr, T * sourceAddr,\n")
f.write("                   size_t tupleSize, uint8_t mask) {\n")

f.write("   switch(mask) {\n")
for mask in getMaskList():
    print("Mask " + str(mask))

    f.write("   case " + str(mask) + ":\n")
    
    bitList = oneBits(mask)
    runList = bitsRuns(bitList)
    print("List of set bit: " + str(bitList))
    print("List of runs: " + str(runList))
    count = 0;
    for run in runList:
        f.write("       std::memcpy(destAddr + (tupleSize * " + str(count) + "), sourceAddr + (tupleSize * " + str(run[0]) + "), tupleSize * sizeof(T) * " + str(run[1]) + ");\n")
        count += run[1]
    f.write("       return " + str(count) + ";\n")
    
f.write("   }\n}\n");

copyFileContent(f, "foot.txt")

f.close()
