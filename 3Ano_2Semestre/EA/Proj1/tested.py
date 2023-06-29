from sys import stdin, stdout
import numpy as np
import copy



testPuzzle =  np.array([[1, 2, 3, 4], 
                        [2, 5, 1, 3], 
                        [3, 1, 4, 2], 
                        [4, 3, 2, 5]])



xLength = 2
yLength = 2

resultPuzzle = []


def readln():
    return stdin.readline().rstrip().split()


def outln(n):
    stdout.write(str(n))
    stdout.write("\n")
    

def checkIfFits(piece, curX, curY):

    if (curX > 0):
        if(not equalNumbs(piece, resultPuzzle[curY, curX - 1])):
            return None

    elif (curX == 0 and curY > 0):
        if(not equalNumbs(piece, resultPuzzle[curY - 1, curX])):
            return None

    
    for i in range(4):
        if (curY == 0):                                                                                             # 0 1| |0 1 
            if (resultPuzzle[curY, curX - 1, 1] == piece[0] and resultPuzzle[curY, curX - 1, 2] == piece[3]):       # 3 2| |3 2
                return piece    

        elif (curX == 0 and curY > 0):
            if (resultPuzzle[curY - 1, curX, 3] == piece[0] and resultPuzzle[curY - 1, curX, 2] == piece[1]):
                return piece

        else:
            if (resultPuzzle[curY, curX - 1, 1] == piece[0] and resultPuzzle[curY, curX - 1, 2] == piece[3] and
                resultPuzzle[curY - 1, curX, 3] == piece[0] and resultPuzzle[curY - 1, curX, 2] == piece[1]):
                return piece

        piece = rotate(piece, 1)

    return None


def rotate(piece, count):
    pieceCopy = copy.deepcopy(piece)    
    for _ in range(count):
        pieceCopy[0], pieceCopy[1], pieceCopy[2], pieceCopy[3] = piece[3], piece[0], piece[1], piece[2]
    return pieceCopy

def equalNumbs(piece1, piece2):
    if(len(np.intersect1d(piece1, piece2)) < 2):
        return False

    return True


def findPossiblePieces(pieces, currentX, currentY):

    piecesFit = []
    
    if(currentX == 0 and currentY == 0):
        res = []
        s1 = np.append(pieces[0], 0).tolist()
        s2 = np.append(rotate(pieces[0], 1), 0).tolist()
        s3 = np.append(rotate(pieces[0], 2), 0).tolist()
        s4 = np.append(rotate(pieces[0], 2), 0).tolist()

        res.append(s1)
        res.append(s2)
        res.append(s3)
        res.append(s4)

        return res

    for count in range(len(pieces)):
        

        fit = checkIfFits(pieces[count], currentX, currentY)
        if(fit is not None):
            fit = np.append(fit, count)
            piecesFit = np.append(piecesFit, fit)

   
    piecesFit = [int(x) for x in piecesFit]

    return [piecesFit]



def solveImpossiblePuzzle(pieces, currentX, currentY):

    for content in findPossiblePieces(pieces, currentX, currentY):
       
        if not content: 
            return False

        piece = content
        piece = piece[:4]
        value = content[4]

        resultPuzzle[currentY, currentX] = piece
        
        pieces = pieces.tolist()
        pieces.pop(value)
        pieces = np.array(pieces)
        
        nextX, nextY = currentX, currentY
        
        if (currentX == xLength - 1):
            if (currentY == yLength - 1):
                return True
            
            nextX = 0
            nextY = currentY + 1
            
        else:
            nextX = currentX + 1
            nextY = currentY

        if solveImpossiblePuzzle(pieces, nextX, nextY):
            return True

        pieces = pieces.tolist()
        pieces.append(resultPuzzle[currentX, currentY])
        pieces = np.array(pieces)

        resultPuzzle[currentX, currentY] = np.array([0,0,0,0], dtype = int)
        

    return False
        

def initPuzzle():
    global resultPuzzle
    resultPuzzle = np.zeros([yLength, xLength, 4], dtype = int)


def printPuzzle(array):
    for i in range(yLength):
        for k in range(2):
            txt = ""
            first = True
            for j in range(xLength):
                if k == 0:
                    if first:
                        txt += str(array[i][j][0]) + " " + str(array[i][j][1])
                        first = False
                    else:
                        txt += "  " + str(array[i][j][0]) + " " + str(array[i][j][1])
                else:
                    if first:
                        txt += str(array[i][j][3]) + " " + str(array[i][j][2])
                        first = False
                    else:
                        txt += "  " + str(array[i][j][3]) + " " + str(array[i][j][2])

            outln(txt)
        outln("")
            


def main():
    
    global xLength
    global yLength

    
    number_tests = readln()
    
    
    for _ in range(int(number_tests[0])):

        temp = readln()
        number_pieces = int(temp[0])
        yLength = int(temp[1])
        xLength = int(temp[2])

        pieces = list()

        for _ in range(int(number_pieces)):       
            pieces.append([int(numeric_string) for numeric_string in readln()])
        
        pieces = np.array(pieces)

        initPuzzle()

    
        if (solveImpossiblePuzzle(pieces, 0, 0)):
            printPuzzle(resultPuzzle)
        else:
            outln("impossible puzzle!")



if __name__ == '__main__':
    main()

