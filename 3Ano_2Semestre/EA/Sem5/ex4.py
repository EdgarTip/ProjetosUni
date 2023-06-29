from sys import stdin, stdout

global Mp, Dp

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def dp(n):
    global Dp, Mp
    Dp[0][0] = Mp [0][0]

    i = 1

    for i in range(n):
        for j in range(i+1):
            if j == 0:
                Dp[i][j] = Dp[i-1][j] + Mp[i][j]
            else:   
                Dp[i][j] = max(Dp[i-1][j], Dp[i-1][ j-1]) + Mp[i][j]

            
                


def main():
    global Mp, Dp

 
    while(1):
        try:
            temp = readln()
            executions = int(temp[0])

            for i in range(executions):
                n = int(readln()[0])

                Mp = [[0 for t in range(n)]for s in range(n) ]
                Dp =  [[0 for t in range(n)]for s in range(n) ]

                amount = 1
                for j in range(n):
                    temp = readln()
                    for k in range(amount):
                        Mp[j][k] = int(temp[k])

                    amount += 1

                dp(n)

            
                best = 0
                print(Dp[n-1][:])
                for j in range(n):
                    
                    if(best < Dp[n-1][j]):
                        best = Dp[n-1][j]

                print(best)


        
            
        
        except Exception: 
            break


            



        



if __name__ == "__main__":
    main()