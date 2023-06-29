from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def main():
    
    d = dict()

    while 1:
        try:
            input = readln()

            if input[0] == "ADD":
                if input[2] in d:
                    d[input[2]] += int(input[1])
                else:
                    d[input[2]] = int(input[1])
                    
                
            elif input[0] == "REM":
                if input[2] not in d:
                    outln("removal refused")
                    continue
                
                if d[input[2]] < int(input[1]):
                    outln("removal refused")
                    continue

                d[input[2]] -= int(input[1])
            elif input[0] == "QUERY":
                if input[1] in d:
                    outln(d[input[1]])
                else:
                    outln(0)
        except:
            break

            


    

if __name__ == "__main__":
    main()