from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def main():

    array = list()
    while 1:
        try:
            array.append(int(readln()))

        except:
            break
    
    array.sort()

    for i in array:
        outln(i)
        



if __name__ == "__main__":
    main()