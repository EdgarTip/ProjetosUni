file = open("test.txt", "w")

i = 80000

for k in range(0,i):

    line = str(k) + " " + str(k+1) + " 2"
    file.write(line)
    file.write("\n")
file.write("-1")

file.close()
