from sys import stdin, stdout

def readln():
    return stdin.readline().rstrip().split()

def outln(n):
    stdout.write(str(n))
    stdout.write("\n")

def main():

    n = (readln())
    out = [int(numeric_string) for numeric_string in n]
    outln(out[0] * out[1])


if __name__ == '__main__':
    main()