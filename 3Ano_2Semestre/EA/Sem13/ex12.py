from sys import stdin, stdout
from collections import defaultdict
import math

global cubeIntersection

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def intersection(cube):
    global cubeIntersection
    
    

    x = max(cubeIntersection[0], cube[0])
    y = max(cubeIntersection[1], cube[1])
    z = max(cubeIntersection[2], cube[2])

    x2 = min(cubeIntersection[3], cube[3])
    y2 = min(cubeIntersection[4], cube[4])
    z2 = min(cubeIntersection[5], cube[5])

    if y > cubeIntersection[3] or x > cubeIntersection[4] or z >cubeIntersection[5]:
        cubeIntersection = [0,0,0,0,0,0]
        return

    if cube[3] < cubeIntersection[0] or cube[4] < cubeIntersection[1] or cubeIntersection[2] > cube[5]:
        cubeIntersection = [0,0,0,0,0,0]
        return

    cubeIntersection = [x,y,z,x2,y2,z2]

def calcArea():
    global cubeIntersection
    dx = cubeIntersection[3] - cubeIntersection[0]
    dy = cubeIntersection[4] - cubeIntersection[1]
    dz = cubeIntersection[5] - cubeIntersection[2]

    return dx * dy * dz


def main():

    global cubeIntersection

    first = True
    cubeIntersection = [0 for i in range(6)]

    input = int(readln()[0])

    for i in range(input):
        line = readln()
        line = [int(i) for i in line]

        cube = [line[0], line[1], line[2], line[0]+line[3], line[1] + line[3], line[2] +line[3]]

        if first:
            cubeIntersection = cube
            first = False
        else:
            intersection(cube)                
            
    
    outln(calcArea())        

                


if __name__ == "__main__":
    main()