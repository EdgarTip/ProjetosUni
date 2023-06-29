from collections import defaultdict
from email.policy import default
from sys import stdin, stdout, setrecursionlimit

global graph
def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


class Node:
    def __init__(self):

        self.nodes = list()
        self.cost = -1
        self.num = -1
        self.inc = -1
        self.exc = -1
        self.cost_exc = -1
        self.cost_inc = -1

    def add_node(self, node):
        self.nodes.append(node)


def calc(src, par):

    for x in src.nodes:
        if graph[x] != par:
            calc(graph[x], src)
    
    
    for x in src.nodes:
        if graph[x] != par:
            src.exc += graph[x].inc
            src.cost_exc += graph[x].cost_inc

            if graph[x].inc > graph[x].exc:
                src.cost_inc += graph[x].cost_exc
                src.inc += graph[x].exc

            elif graph[x].inc < graph[x].exc:
                src.cost_inc += graph[x].cost_inc
                src.inc += graph[x].inc

            else:
                if( graph[x].cost_inc > graph[x].cost_exc):
                    src.cost_inc += graph[x].cost_inc
                else:
                    src.cost_inc += graph[x].cost_exc
                src.inc += graph[x].exc
    
# def printTree(root, dots):
#     for i in root.nodes:
#         printTree(graph[i], dots+".")
    
#     print(dots ,root.num, root.nodes, root.cost_inc, root.cost_exc)

def main():
    global graph

    root = None
    new_node = None


    graph = defaultdict(list)
    setrecursionlimit(180000)
    while(True):
            line = readln()

            if(line == []):
                return

            line = [int(x) for x in line]

            if(line[0] == -1):

                if root is None:
                    print("0 0")
                    continue

                calc(root, None)


                if root.inc < root.exc:
                    print(str(root.inc) + " " + str(root.cost_inc))

                elif root.inc > root.exc:
                    print(str(root.exc) + " " + str(root.cost_exc))

                else:
                    if(root.cost_inc > root.cost_exc):
                        print(str(root.inc) + " " + str(root.cost_inc))
                    else:
                        print(str(root.exc) + " " + str(root.cost_exc))

                root = None
                    
            else:
                new_node = Node()
                
                for i in range(len(line)):
                    if i == 0:
                        new_node.num = line[0]
                        
                    elif(i == len(line) -1 ):
                        new_node.cost_inc = line[i]

                    else:
                        new_node.nodes.append(line[i])
                        
                
                new_node.cost_exc = 0
                new_node.inc = 1
                new_node.exc = 0
                graph[new_node.num] = new_node
                if line[0] == 0:
                    root = new_node
                    
if __name__ == '__main__':
    main()