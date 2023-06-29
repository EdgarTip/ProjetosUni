from re import T
from sys import stdin, stdout
from collections import defaultdict
import math

global graph, low, dfs, t, parents, count, iterations, isAdded

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def ap(parent):
    global graph, low, dfs, t, parents, count, iterations, isAdded
    iterations += 1
    low[parent] = t
    dfs[parent] = t
    t += 1
    
    for i in graph[parent]:

        if dfs[i] == 0:
            parents[i] = parent
            ap(i)

            low[parent] = min(low[parent], low[i])

            if dfs[parent] == 1 and dfs[i] != 2:
                if(isAdded[parent] == 0):
                    isAdded[parent] = 1
                    count +=1
            elif dfs[parent] <= low[i]:
                if(isAdded[parent] == 0):
                    isAdded[parent] = 1
                    count +=1
            
        elif parents[parent] != i:
            low[parent] = min(low[parent], low[i])








def main():

    global graph, iterations, dfs, low, t, parents, count, isAdded

    try:
        while(True):

                input = readln()
                numbers = int(input[0])
                count = 0


                graph = defaultdict(list)
                low = [0 for i in range(numbers+1)]
                dfs = [0 for i in range(numbers+1)]
                isAdded = [0 for i in range(numbers+1)]
                parents = [0 for i in range(numbers + 1)]
                t = 1

                input = readln()  
                input = [int(i) for i in input] 

                
                while(input[0] != 0):
                    for i in range(1,len(input)):
                        graph[input[0]].append(input[i])
                        graph[input[i]].append(input[0])
                    
                    
                    input = readln()
                    input = [int(i) for i in input] 
                
                iterations = 0
                ap(1)
                    

                print(count -1)
            
    except Exception:
        pass

if __name__ == "__main__":
    main()