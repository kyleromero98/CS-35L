
lines = [10, 50, 100, 250, 500, 1000]

for val in lines:
    filename = 'big%d.txt' % val
    fileoutput = open(filename, 'w')
    for i in range(0, val):
        fileoutput.write('Hi there, my name is Kyle\n')

fileoutput.close()
