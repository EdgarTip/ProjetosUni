from sys import stdin, stdout

global number_nodes, best, visited, degree, maxD, cost_matrix

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def tree(v, cost):
    global best, visited, cost_matrix, degree
    if (best is None or best > cost ) and v == number_nodes -1:
        best = cost
        return

    for i in range(number_nodes):
        if visited[i] == False:
            for j in range(i, number_nodes):
                if cost_matrix[i][j] > 0 and degree[j] < maxD:
                    degree[i] += 1
                    degree[j] += 1
                    visited[i] = True
                    tree(v+1, cost+ cost_matrix[i][j])
                    visited[i] = False
                    degree[i] -= 1
                    degree[j] -= 1

            
                


def main():
    global number_nodes
    global visited
    global maxD
    global best, cost_matrix, degree

 
    while(1):
        try:
            temp = readln()
            number_nodes = int(temp[0])
            point_to_point_links = int(temp[1])
            maxD = int(temp[2])


            degree = [0 for i in range(number_nodes)] 
            visited = [0 for i in range(number_nodes)] 
            cost_matrix = [[0 for i in range(number_nodes)]for j in range(number_nodes) ]
            best = None

            for j in range(point_to_point_links):
                line = (readln())
                line = [int(i) for i in line]
                cost_matrix[line[0]-1][line[1]-1] = line[2]
                cost_matrix[line[1]-1][line[0]-1] = line[2]
            
            tree(0,0)
            if(best is None):
                outln("NO WAY!")
            
            else:
                outln(best)



        except Exception as e:
            print(e)
            break



if __name__ == "__main__":
    main()