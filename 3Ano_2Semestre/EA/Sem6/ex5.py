from sys import stdin, stdout

global t, Dp, number_pizzas, amount

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def pizzas():
    global Dp
    for i in range(number_pizzas+1):
        Dp[i][0] = True
    
    j = 1
    for j in range(1,amount +1):
        Dp[0][j] = False

    i = 1
    j = 1
    for i in range(1,number_pizzas+1):
        for j in range(1,amount+1):
            if t[i-1] > j:
                Dp[i][j] = Dp[i-1][j]
            else:
                Dp[i][j] = (Dp[i-1][j] or Dp[i-1][j-t[i-1]])


def main():
    global t, Dp, number_pizzas, amount

 
    while(1):
        try:
            number_pizzas = int(readln()[0])

            sum = 0
            t = [0 for t in range(number_pizzas)]

            for i in range(number_pizzas):
                n = int(readln()[0])
                t[i] = n
                sum +=n
            amount = int(sum / 2)
    
            Dp =  [[0 for t in range(amount + 1)]for s in range(number_pizzas +1) ]

            pizzas()
     
            i = amount
            while i >=0:
                if(Dp[number_pizzas][i]):
                    break
                i -=1
           
            
            outln(abs(sum - 2*i))
        except Exception:
            break

        
            
        
  


if __name__ == "__main__":
    main()