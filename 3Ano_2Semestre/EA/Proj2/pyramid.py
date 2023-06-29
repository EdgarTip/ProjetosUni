from collections import defaultdict
from email.policy import default
from sys import stdin, stdout, setrecursionlimit

global cost,  graph, nodes
global best_cost

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def minCalc(src, par):
    global best_cost

    for x in graph[src]:
        if x != par:
            minCalc(x, src)
    

    for x in graph[src]:
        if x != par:
            nodes[src][0] += nodes[x][1]
            best_cost[src][0] += best_cost[x][1]

            if nodes[x][1] > nodes[x][0]:
                best_cost[src][1] +=  best_cost[x][0]
                nodes[src][1] += nodes[x][0]

            elif nodes[x][1] < nodes[x][0]:
                best_cost[src][1] +=  best_cost[x][1]
                nodes[src][1] += nodes[x][1]

            else:
                if( best_cost[x][1] > best_cost[x][0]):
                    best_cost[src][1] +=  best_cost[x][1]
                else:
                    best_cost[src][1] += best_cost[x][0]
                nodes[src][1] += nodes[x][0]



def findMin(n):
    global nodes, graph, best_cost

    
    if not graph:
        print("0 0")
        return
    
    if graph[0] is None:
        print("0 0")
        return

        
    for i in graph:
        nodes[i].append(0)
        nodes[i].append(1)

    for i in graph:
        best_cost[i].append(0)
        try:
            best_cost[i].append(cost[i])
        except:
            best_cost[i].append(0)


    
    minCalc(0,-1)
    
    if(nodes[0][0] < nodes[0][1]):
        print(nodes[0][0], best_cost[0][0])
    elif nodes[0][0] > nodes[0][1]:
        print(nodes[0][1], best_cost[0][1])
    else:
        print(nodes[0][0], max(best_cost[0][0], best_cost[0][1]))


def main():
    global graph
    global best_cost
    global nodes,cost

    graph = defaultdict(list)
    nodes = defaultdict(list)
    best_cost = defaultdict(list)
    cost = defaultdict()
    number_lines = 0
    number_conections = 0

    setrecursionlimit(180000)


    while(True):
        line = readln()

        if(line == []):
            return

        line = [int(x) for x in line]

        if(line[0] == -1):


            findMin(number_lines)
            graph = defaultdict(list)
            nodes = defaultdict(list)
            cost = defaultdict()
            best_cost = defaultdict(list)
            number_lines = 0
            number_conections = 0
                
        else:
            number_lines += 1
            for i in range(len(line)):
                if i == 0:
                    pass
                elif(i == len(line) -1 ):
                    cost[line[0]] = line[i]


                else:
                    number_conections += 1
                    graph[line[0]].append(line[i])
                    graph[line[i]].append(line[0])




if __name__ == '__main__':
    main()