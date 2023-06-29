from sys import stdin, stdout
from collections import defaultdict
import math

global listNodes, costMatrix , nConnections, total_cost

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


class Node:
    def __init__(self, id, x, y):
        self.parent = None
        self.id = id
        self.x = x
        self.y = y
        self.height = 0

    
def euclidianDistance(a,b):
    return math.sqrt( abs(a.x - b.x) * abs(a.x - b.x) + abs(a.y - b.y) * abs(a.y - b.y))


def calculateCostMatrix():
    global costMatrix, listNodes

    for i in range(1, len(listNodes)):
        for j in range(i+1, len(listNodes)):

            costMatrix.append([i,j,euclidianDistance(listNodes[i], listNodes[j])])

def findParent(a):
    aux = a
    while aux.parent != None:
        aux = aux.parent
    
    return aux



def minimumSpanningTree():
    global costMatrix, listNodes, nConnections, total_cost

    costMatrix = sorted(costMatrix ,key=lambda i: i[2])
    curr = 0


    while(nConnections < len(listNodes)-2):
        node1, node2, cost = costMatrix[curr]
        
        parent1 = findParent(listNodes[node1])
        parent2 = findParent(listNodes[node2])

        if(parent1 != parent2):
            nConnections +=1
            total_cost += cost

            if parent1.height > parent2.height:
                parent2.parent = parent1
                parent2.height += parent1.height +1

            elif parent2.height > parent1.height:
                parent1.parent = parent2
                parent1.height += parent2.height +1
            
            else:
                if(parent1.id > parent2.id):
                    parent1.parent = parent2
                    parent1.height += parent2.height +1
                else:
                    parent2.parent = parent1
                    parent2.height += parent1.height +1

            

        
        curr +=1




def main():
    global listNodes,costMatrix, nConnections, total_cost

    best = 0
    try: 
        while(True):
                nConnections = 0
                total_cost = 0
                input = readln()
                numberNodes = int(input[0])


                listNodes = [None for i in range(numberNodes+1)]
                costMatrix = []

                for i in range(1, numberNodes+1):
                    line = readln()
                    listNodes[i] = Node(i+1, int(line[0]), int(line[1]))

                input = readln()    
                numberConnections = int(input[0])

                for i in range(numberConnections):
                    nConnections += 1
                    line = readln()
                    
                    aux1 = int(line[0])
                    aux2 = int(line[1])

                    if(aux1 > aux2):
                        listNodes[aux1].parent = listNodes[aux2]
                        listNodes[aux2].height += listNodes[aux1].height + 1
                    else:
                        listNodes[aux2].parent = listNodes[aux1]
                        listNodes[aux1].height += listNodes[aux2].height + 1

                calculateCostMatrix()
                minimumSpanningTree()

                print("{:.2f}".format(total_cost))
    except Exception:
        pass

       

if __name__ == "__main__":
    main()