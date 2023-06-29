from sys import stdin, stdout
from collections import defaultdict

global number, graph, end, dic, cost

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def dijkstra(root):
    global number, graph, end, dic, cost
    
    cost[root] = 0

    while dic:
        best_id = -1
        best_cost = None

        for i in dic:
            if best_cost is None or (cost[i] is not None and cost[i] < best_cost):
                best_id = i
                best_cost = cost[i]

        
        if best_id ==end:
            break

        for i in dic[best_id]:
            if cost[i] is None or cost[i] > cost[best_id] + graph[best_id][i]:
                cost[i] = cost[best_id] + graph[best_id][i]
        dic.pop(best_id)


def main():
    global number, graph, best, end, dic, cost

    best = 0
    try:
        while(True):
            dic = defaultdict(list)
            best = 0
            input = readln()
            number = int(input[0])
            end = int(input[1])

            graph = [[0 for i in range(number+1)]for j in range(number+1) ]
            cost = [None for i in range(number+1)]
            for _ in range(number):
                
                line = (readln())
                line = [ int(x) for x in line ]
                for i in range(1, len(line)):
                    if(line[i] == -1):
                        continue
                    graph[line[0]][i] = line[i]


                    dic[line[0]].append(i)
                    

            dijkstra(1)
            print(cost[end])
            


    except Exception as e:
        pass
                

if __name__ == "__main__":
    main()