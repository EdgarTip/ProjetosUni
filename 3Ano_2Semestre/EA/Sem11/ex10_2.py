from sys import stdin, stdout
from collections import defaultdict
import math

from sklearn.metrics import euclidean_distances

global parent, rank, e, listNodes

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")
 

class Node:
    def _init_(self, id, x, y):
        self.parent = None
        self.id = id
        self.x = x
        self.y = y

class Graph:
 
    def __init__(self, vertices):
        self.V = vertices
        self.graph = []  

    def addEdge(self, u, v, w):
        self.graph.append([u, v, w])
 
    def find(self, parent, i):
        if parent[i] == i:
            return i
        return self.find(parent, parent[i])

    def union(self, parent, rank, x, y):
        xroot = self.find(parent, x)
        yroot = self.find(parent, y)
 
        if rank[xroot] < rank[yroot]:
            parent[xroot] = yroot
        elif rank[xroot] > rank[yroot]:
            parent[yroot] = xroot

        else:
            parent[yroot] = xroot
            rank[xroot] += 1

    def MST(self):
        global rank, parent, e
        result = []  

        i = 0


        self.graph = sorted(self.graph,
                            key=lambda item: item[2])
 
        for node in range(self.V):
            parent.append(node)
            rank.append(0)

        while e < self.V - 1:

            u, v, w = self.graph[i]
            i = i + 1
            x = self.find(parent, u)
            y = self.find(parent, v)
 
            if x != y:
                e = e + 1
                result.append([u, v, w])
                self.union(parent, rank, x, y)
 
        minimumCost = 0
        print ("Edges in the constructed MST")
        for u, v, weight in result:
            minimumCost += weight
            print("%d -- %d == %d" % (u, v, weight))
        print("Minimum Spanning Tree" , minimumCost)

def main():

    global rank, parent ,e, result
    try:
        while(True):
            nConnections = 0
            input = readln()

            parent = []
            rank = []
            result = []
            e=0

            numberNodes = int(input[0])

            graph = Graph(numberNodes)

            for i in range(1, numberNodes+1):
                line = readln()
                
                graph.addEdge(int(line[0]), int(line[1]), euclidean_distances)

            input = readln()
            numberConnections = int(input[0])

            for i in range(numberConnections):
                line = readln()
                result.append([int(line[0]), int([line[1]]), euclidean_distances])
               
                graph.union(parent, rank, int(line[0]), int(line[1]))
                


            graph.MST()




               
            


    except Exception as e:
        pass
                

if __name__ == "__main__":
    main()