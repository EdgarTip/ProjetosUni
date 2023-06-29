import random
import numpy as np


# conditions not implemented
# avoid cycles                  - done (i think)
# not allow 2 way connections   - done
# only one end point

global numberElems, maxCost, maxConnPerNode, nodesVisited
global initNodeId, endNodeId
global nodesArr

class node():
    def __init__(self, cost, children, parent, nConn):
        self.cost = cost
        self.childrenIds = children
        self.parentIds = parent
        self.nConnections = nConn



def generateNodes():
    global nodesArr
    for _ in range(numberElems):
        nodesArr.append(node(random.randint(1, maxCost), [], [], 0))

def defineStartFinish():
    global initNodeId, endNodeId
    # define init and end nodes
    initNodeId = random.randint(0, numberElems - 1)
    endNodeId = random.randint(0, numberElems - 1)
    if(initNodeId == endNodeId):
        endNodeId += 1
        endNodeId = endNodeId % numberElems


def isCyclic(currentID):
    nodesVisited[currentID] = 1

    for i in nodesArr[currentID].childrenIds:
        if (nodesVisited[i] == 0):
            return True
        else:
            if (isCyclic(i) == 0):
                return True
    
    nodesVisited[currentID] = 0
    return False

def checkForCycles():
    global nodesVisited
    nodesVisited = [0 for _ in range(numberElems)]
    print(nodesVisited)
    isCyclic(initNodeId)
    print(all(elem == 1 for elem in nodesVisited))

def addConnection(id):
    ids = list(np.arange(numberElems))
    while (len(ids) > 0):
        child = random.choice(ids)
        #print("P", initNodeId, endNodeId, "c", id, child)
        if (id == child or child == endNodeId or id == initNodeId):
            ids.remove(child)
            continue

        # avoid repeated connections
        if (child in nodesArr[id].childrenIds or id in nodesArr[child].parentIds):
            ids.remove(child)
            continue

        # avoid two way connections
        if (child in nodesArr[id].parentIds or id in nodesArr[child].childrenIds):
            ids.remove(child)
            continue

        nodesArr[id].childrenIds.append(child)
        nodesArr[child].parentIds.append(id)
        nodesArr[id].nConnections += 1
        nodesArr[child].nConnections += 1
        return

def fillLastNodes():
    for i in range(len(nodesArr)):
        if (i != endNodeId and len(nodesArr[i].childrenIds) == 0):
            addConnection(i)

def firstConnection():
    global nodesArr
    count = 0
    ids = list(np.arange(numberElems))
    while(count < numberElems):

        if (len(ids) <= 0):
            count += 1
            ids = list(np.arange(numberElems))
            continue

        child = random.choice(ids)
        print("P", initNodeId, endNodeId, "c", count, child)
        if (count == child or child == endNodeId or count == initNodeId):
            ids.remove(child)
            continue

        #print(nodesArr)
        #print(child, parent)
        # max number of connections
        if (nodesArr[count].nConnections >= maxConnPerNode or nodesArr[child].nConnections >= maxConnPerNode):
            ids.remove(child)
            continue

        # avoid repeated connections
        if (child in nodesArr[count].childrenIds or count in nodesArr[child].parentIds):
            ids.remove(child)
            continue

        # avoid two way connections
        if (child in nodesArr[count].parentIds or count in nodesArr[child].childrenIds):
            ids.remove(child)
            continue

        nodesArr[count].childrenIds.append(child)
        nodesArr[child].parentIds.append(count)
        nodesArr[count].nConnections += 1
        nodesArr[child].nConnections += 1
        count += 1
        ids = list(np.arange(numberElems))

    return count
    

def addConnections():
    global nodesArr
    #nConnections = random.randint(0, numberElems * (numberElems // 2))
    nConnections = 10
    count = firstConnection()

    fillLastNodes()
    return

    while (count < nConnections):
        parent = random.randint(0, numberElems - 1)
        child = random.randint(0, numberElems - 1)
        if (parent == child or parent == endNodeId or child == initNodeId):
            continue

        #print(nodesArr)
        #print(child, parent)
        if (nodesArr[parent].nConnections >= maxConnPerNode or nodesArr[child].nConnections >= maxConnPerNode):
            continue

        if (child in nodesArr[parent].childrenIds or parent in nodesArr[child].parentIds):
            continue

        nodesArr[parent].childrenIds.append(child)
        nodesArr[child].parentIds.append(parent)
        nodesArr[parent].nConnections += 1
        nodesArr[child].nConnections += 1

        count += 1


def printGraph():
    print(initNodeId, endNodeId)
    print(numberElems)
    for node in nodesArr:
        string = ""
        string += str(node.cost) + " " + str(len(node.parentIds))
        for id in node.parentIds:
            string += " " + str(id)
        print(string)


def main():
    global numberElems, maxCost, maxConnPerNode, initNodeId, endNodeId, nodesArr
    numberElems = 4
    maxCost = 10
    maxConnPerNode = 5
    nodesArr = []
    
    print(1, flush=True)
    defineStartFinish()

    print(1, flush=True)
    generateNodes()

    print(1, flush=True)
    addConnections()

    print(1, flush=True)
    printGraph()

    checkForCycles()

    return



    # print stuff
    print(initNodeId, endNodeId)
    for node in nodesArr:
        print(node.cost)

    


if __name__ == "__main__":
    main()