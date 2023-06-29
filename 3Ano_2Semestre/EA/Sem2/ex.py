from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def three_sum(array):
    for i in range(len(array)-2):
        j = i+1
        k = len(array) -1 
        while(j<k):
            if array[i] +array[j] + array[k] == 0:
                return True
            elif array[i] +array[j] + array[k] > 0:
                k = k-1
            else:
                j = j+1

    return False



def main():

    num = -1
    arr = []
    first = True
    while(1):
        try:

            
            while(num != 0):
                num = int(readln()[0])
                if(num != 0):
                    arr.append(num)
                
            
            arr.sort()

            if(three_sum(arr)):
                outln("Fair")

            else:
                outln("Rigged")
               
            arr = []
            num = -1

            
        except Exception as e:
            break

    
    


if __name__ == '__main__':
    main()