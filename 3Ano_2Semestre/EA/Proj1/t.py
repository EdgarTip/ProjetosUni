import sys

def recursive(num):
    print(num)
    recursive(num + 1)

def main():
    sys.setrecursionlimit(3000)
    recursive(1)

if __name__ == '__main__':
    main()