import sys
from copy import copy
import time


xLength = 2
yLength = 2

resultPuzzle = []

init_array = []


def readln():
    return sys.stdin.readline().rstrip().split()


def outln(n):
    sys.stdout.write(str(n))
    

def checkIfFits(piece, curX, curY, flag):

    if flag:
        if (curY == 0):                                                                                             # 0 1| |0 1 
            if (resultPuzzle[curY][curX - 1][1] == piece[0] and resultPuzzle[curY][curX - 1][2] == piece[3]):       # 3 2| |3 2
                return piece    

        elif (curX == 0 and curY > 0):
            if (resultPuzzle[curY - 1][curX][3] == piece[0] and resultPuzzle[curY - 1][curX][2] == piece[1]):
                return piece

        else:
            if (resultPuzzle[curY][curX - 1][1] == piece[0] and resultPuzzle[curY][curX - 1][2] == piece[3] and
                resultPuzzle[curY - 1][curX][3] == piece[0] and resultPuzzle[curY - 1][curX][2] == piece[1]):
                return piece

    else:
        local_piece = copy(piece)

        for _ in range(4):
            if (curY == 0 and resultPuzzle[curY][curX - 1][1] == local_piece[0] and resultPuzzle[curY][curX - 1][2] == local_piece[3]):     # 0 1| |0 1      
                return local_piece                                                                                                          # 3 2| |3 2

            elif (curX == 0 and curY > 0 and resultPuzzle[curY - 1][curX][3] == local_piece[0] and resultPuzzle[curY - 1][curX][2] == local_piece[1]):
                return local_piece

            elif (resultPuzzle[curY][curX - 1][1] == local_piece[0] and resultPuzzle[curY][curX - 1][2] == local_piece[3] and
                resultPuzzle[curY - 1][curX][3] == local_piece[0] and resultPuzzle[curY - 1][curX][2] == local_piece[1]):
                return local_piece

            rotate(local_piece)

    return None





def rotate(piece):
    piece[0], piece[1], piece[2], piece[3] = piece[3], piece[0], piece[1], piece[2]


def equalNumbs(piece1, piece2):
    if(intersection(piece1, piece2) < 2):
        return False

    return True

def intersection(lst1, lst2): 
    lst3 = [value for value in lst1 if value in lst2] 
    return len(lst3)


def findPossiblePieces(pieces, curX, curY):
    piecesFit = []
    
    if(curX == 0 and curY == 0):
        res = []
        s1 = copy(pieces[0])
        s1.extend([0])
        res.append(copy(s1))
        rotate(s1)
        res.append(copy(s1))
        rotate(s1)
        res.append(copy(s1))
        rotate(s1)
        res.append(copy(s1))
        

        return res


    
    for count in range(len(pieces)):

        if (curX > 0 and curY > 0 and (not equalNumbs(pieces[count], resultPuzzle[curY][curX - 1][1:3]) or not equalNumbs(pieces[count], resultPuzzle[curY - 1][curX][2:4]))):
            pass

        elif (curX > 0 and curY > 0 and (not equalNumbs(pieces[count], resultPuzzle[curY][curX - 1][1:3]) or not equalNumbs(pieces[count], resultPuzzle[curY - 1][curX][2:4]))):
            pass

        elif (curX > 0 and curY == 0 and (not equalNumbs(pieces[count], resultPuzzle[curY][curX - 1][1:3]))):
            pass

        elif (curX == 0 and curY > 0 and (not equalNumbs(pieces[count], resultPuzzle[curY - 1][curX][2:4]))):
            pass 
        
        elif(len(set(pieces[count])) <= 2):
            local_piece = copy(pieces[count])
            for _ in range(4):
                
                fit = checkIfFits(local_piece, curX, curY, True)
                fit = copy(fit)
                if(fit is not None):
                    fit.append(count)

                    piecesFit.append(fit)

                rotate(local_piece)

        else:
            local_piece = copy(pieces[count])
            fit = checkIfFits(local_piece, curX, curY, False)
            fit = copy(fit)
            if(fit is not None):
                fit.append(count)

                piecesFit.append(fit)
            
            

    piecesFit = piecesFit[::-1]
    return piecesFit


def solveImpossiblePuzzle(pieces, currentX, currentY):

    for content in findPossiblePieces(pieces, currentX, currentY):

        if(currentX == 0 and currentY == 0):
            pieces = copy(init_array)

        piece, value = content[:4], content[4]

        #print("content: ", content)
        resultPuzzle[currentY][currentX] = piece

        del pieces[value]
       
        
        nextX, nextY = currentX, currentY
        
        if (currentX == xLength - 1):
            if (currentY == yLength - 1):
                return True
            
            nextX = 0
            nextY = currentY + 1
            
        else:
            nextX = currentX + 1
            nextY = currentY

        if solveImpossiblePuzzle(copy(pieces), nextX, nextY):
            return True

        pieces.append(resultPuzzle[currentY][currentX])
        resultPuzzle[currentY][currentX] = [0,0,0,0]   


    return False
        


def initPuzzle():
    global resultPuzzle
    resultPuzzle = []
    temp = []
    for _ in range(xLength):
        temp.append([0, 0, 0, 0])
    for _ in range(yLength):        
        resultPuzzle.append(copy(temp))



def printPuzzle(array):
    txt = ""
    for i in range(yLength):
        for k in range(2):
            first = True
            
            for j in range(xLength):
                
                if k == 0:
                    if first:
                        txt += " ".join([str(array[i][j][0]), str(array[i][j][1])])
                        first = False
                    else:
                        txt += " ".join([" ", str(array[i][j][0]), str(array[i][j][1])])

                else:
                    if first:
                        txt += " ".join([str(array[i][j][3]), str(array[i][j][2])])
                        first = False
                    else:
                        txt += " ".join([" ", str(array[i][j][3]), str(array[i][j][2])])

            txt += "\n"
        if (i != yLength - 1): 
            txt += "\n"

    outln(txt)
            


def main():
    sys.setrecursionlimit(3000)

    global xLength
    global yLength
    global init_array

    number_tests = readln()
    init_time = time.time()
    for _ in range(int(number_tests[0])):

        temp = readln()
        number_pieces = int(temp[0])
        yLength = int(temp[1])
        xLength = int(temp[2])

        pieces = list()

        for _ in range(int(number_pieces)):       
            pieces.append([int(numeric_string) for numeric_string in readln()])
        

        initPuzzle()
        
        init_array = copy(pieces)
        if (solveImpossiblePuzzle(pieces, 0, 0)):
            printPuzzle(resultPuzzle)
            #outln("possible puzzle!\n")
        else:
            outln("impossible puzzle!\n")

    #print("Time: ", time.time() - init_time)



if __name__ == '__main__':
    main()

