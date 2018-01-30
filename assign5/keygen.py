
fileoutput = open('keygen.txt', 'w')
for i in range(0, 256):
    fileoutput.write('\\');
    fileoutput.write(str(oct(i ^ 42)).lstrip('0'))
    if i % 10 == 0:
        fileoutput.write('\\')
        fileoutput.write('\n')
fileoutput.close()
    
