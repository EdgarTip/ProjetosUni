from sys import stdin, stdout

global number, mE, best, vis 

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def ub(n):
    global number, mE, best, vis
    c = 0
    for i in range(n,number):
        if(vis[i] == 0):
            c+=1
    return c

def f(n,a):
    global number, mE, best, vis
    b = a + ub(n)

    if b <= best:
        return
    
    if a > best:
        best = a

    if n == number:
        return
    
    if vis[n] == 0:
        vis[n] = 1
        for j in range(number):
            if mE[n][j] == 1:
                vis[j] += 1

        f(n+1, a+1)

        for j in range(number):
            if mE[n][j] == 1:
                vis[j] -= 1
        vis[n] = 0
    
    f(n+1,a)



def main():
    global number, mE, best, vis 

    best = 0
    try:
        while(True):
            best = 0
            input = readln()
            number = int(input[0])

            vis = [0 for i in range(number)]

            mE = [[0 for i in range(number)]for j in range(number) ]
            for _ in range(int(input[1])):
                
                line = (readln())
                aux1 = int(line[0])
                aux2 = int(line[1])
                mE[aux1][aux2] = 1
                mE[aux2][aux1] = 1
            
            f(0,0)
            print(best)


    except Exception as e:
        pass
                

if __name__ == "__main__":
    main()