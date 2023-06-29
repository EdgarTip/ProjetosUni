from sys import stdin, stdout
from copy import copy

def readln():
    return stdin.readline().rstrip().split()


def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

board_chess = []
board_size = 400

def calculate(x, y, moves):
    v = 0
    if board_chess[x][y] == 0:
        board_chess[x][y] = 1
        v = 1

    if moves == 0:
        return v

    
    return v + calculate(x - 1, y + 2, moves - 1) + calculate(x + 1, y + 2, moves - 1) + calculate(x - 1, y - 2, moves - 1) + calculate(x + 1, y - 2, moves - 1) \
                + calculate(x - 2, y + 1, moves - 1) + calculate(x + 2, y + 1, moves - 1) + calculate(x - 2, y - 1, moves - 1) + calculate(x + 2, y - 1, moves - 1)


def init():
    temp = []
    for _ in range(board_size):
        temp.append(0)
    for _ in range(board_size):
        board_chess.append(copy(temp))



    

def main():

    num = int(readln()[0])
    pieces = []
    for _ in range(num):
        pieces.append([int(value) for value in readln()])
        
    init()
    total = 0
    for p in pieces:
        total += calculate(p[0] + board_size//2, p[1] + board_size//2, p[2])

    outln(total)



if __name__ == "__main__":
    main()