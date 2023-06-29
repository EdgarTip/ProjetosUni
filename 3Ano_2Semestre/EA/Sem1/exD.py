from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")



class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.prev = None



class LinkedList:
    def __init__(self, current):
        self.current = current
        self.tail = current

    # To implement
    def insert_left(self, value):

        new_node = Node(value)

        if(self.current == self.tail):
            self.tail = new_node

        if(self.current.prev != None):
            last_prev = self.current.prev

            last_prev.next = new_node
            self.current.prev = new_node

            new_node.next = self.current
            new_node.prev = last_prev
        else:
            new_node.next = self.current
            self.current.prev = new_node

    def insert_right(self, value):
        new_node = Node(value)
        
        if(self.current.next != None):
            last_next = self.current.next

            last_next.prev = new_node
            self.current.next = new_node

          

            new_node.prev = self.current
            new_node.next = last_next
        
        else:
            self.current.next = new_node
            new_node.prev = self.current


    def move_left(self):
        self.current = self.current.prev

    def move_right(self):
        self.current = self.current.next

    def print(self):
        aux = self.tail

        while aux != None:
            outln(aux.value)
            aux = aux.next
            




def main():
    
    #Initial node
    node = Node(0)
    list = LinkedList(node)

    while 1:
        try:
            input = readln()
            if input[0] == "INSERT":

                if input[1] == "LEFT":

                    list.insert_left(input[2])

                elif input[1] == "RIGHT":

                    list.insert_right(input[2])

            elif input[0] == "MOVE":
                if input[1] == "LEFT":
                    list.move_left()

                elif input[1] == "RIGHT":
                    list.move_right()



        except Exception as e:
            break


    list.print()   












if __name__ == "__main__":
    main()