from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    

def main():
    #We do not need first input, but we still need to read it.
    readln()

    #Get array 
    temp = (readln())

    #Transform array of strings to array of integers 
    array = [int(numeric_string) for numeric_string in temp]

    array = array[::-1]

    for i in range(len(array)): 
        
        outln(array[i])
        if(i != len(array) - 1):
            outln(" ")



    stdout.write("\n")
    
if __name__ == '__main__':
    main()