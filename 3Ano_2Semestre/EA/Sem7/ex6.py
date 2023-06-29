from sys import stdin, stdout

global guards, objectsP, m 

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def calculateGuards():
    i = 0
    guards[i] = objectsP[0] + m

    for j in range(1,len(objectsP)):
        if objectsP[j] > guards[i] + m:
            guards[i+1] = objectsP[j] +m
            i += 1
    
    return i


def main():
    global guards, objectsP, m

    n_tests = int(readln()[0])
    for _ in range(n_tests):
        
        out = (readln())
        number_objects = int(out[0])
        m = int(out[1])

        objectsP = [0 for t in range(number_objects)]
        
        for i in range(number_objects):
            objectsP[i] = int(readln()[0])

        objectsP = sorted(objectsP, reverse = False)

        guards = [0 for t in range(len(objectsP))]

    
        print(calculateGuards() +1 )
           

if __name__ == "__main__":
    main()