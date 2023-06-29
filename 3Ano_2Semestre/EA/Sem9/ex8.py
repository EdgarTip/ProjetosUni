from sys import stdin, stdout
from collections import defaultdict

global color, dic

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def bfs(v):
    global color, dic

    q = list()
    color[v-1] = 1
    q.append(v)
    while q:
        t = q.pop()
        for u in dic[t]:
            if color[u-1] == -1:
                color[u-1] = 1 - color[t-1]
                q.append(u)
            elif color[u-1] == color[t-1]:
                return False

    return True 


def main():
    global color, q, dic


    try:
        while(True):
            dic = defaultdict(list)
            input = readln()
            n = int(input[0])
            m = int(input[1])

            color =  [-1 for i in range(n)]
            for i in range(m):
                line = readln()
                line = [ int(x) for x in line ]
                dic[line[0]].append(line[1])
                dic[line[1]].append(line[0])
            
            if(bfs(1)):
                print("NOT SURE")
            else:
                print("NO")

    except Exception:
        pass
                

if __name__ == "__main__":
    main()