#!/usr/bin/python

import sys, string, locale
from optparse import OptionParser

class compareinput:
    def __init__(self, input1, input2):
        #figuring out inputs for given arguments
        if input1 == "-" and input2 == "-":
            print("Both files cannot read from standard input")
            exit()
        elif input1 == "-":
            try:
                file1 = sys.stdin
                file2 = open(input2, 'r')
            except IOError as e:
                print("I/O error(%s): %s" % (e.errno, e.strerror))
                exit()                
        elif input2 == "-":
            try:
                file1 = open(input1, 'r')
                file2 = sys.stdin
            except IOError as e:
                print("I/O error(%s): %s" % (e.errno, e.strerror))
                exit()
        else:
            try:
                file1 = open(input1, 'r')
                file2 = open(input2, 'r')
            except IOError as e:
                print("I/O error(%s): %s" % (e.errno, e.strerror))
                exit()
                
        #getting lines for each file  
        self.file1lines = file1.read().split('\n');
        self.file2lines = file2.read().split('\n');

        self.file1lines = [x for x in self.file1lines if x]
        self.file2lines = [x for x in self.file2lines if x]

        #lists for output
        self.list1 = []
        self.list2 = []
        self.list3 = []

        file1.close()
        file2.close()

    # to check if the file is sorted (not required but just to do it)
    def isSorted(self, lines, filenum):
        for i in range(len(lines) - 1):
            if lines[i] > lines[i+1]:
                print ("File %d is not sorted" % filenum)
                exit()
        return True
        
    def sorted_comp (self):
        if self.isSorted(self.file1lines, 1) and self.isSorted(self.file2lines, 2):
            i = j = 0
            # removing from line lists if we find matches or nonmatches
            while  len(self.file1lines) > 0 and j < len(self.file2lines):
                #in both, remove from both
                if self.file1lines[0] == self.file2lines[0]:
                    self.list3.append(self.file1lines[0])
                    self.list1.append("\t")
                    self.list2.append("\t")
                    del self.file1lines[0]
                    del self.file2lines[0]
                #in list 2, remove from list 2
                elif self.file1lines[0] > self.file2lines[0]:
                    self.list2.append(self.file2lines[0])
                    self.list1.append("\t")
                    self.list3.append("")
                    del self.file2lines[0]
                #in list 1, remove from list 1
                elif self.file1lines[0] < self.file2lines[0]:
                    self.list1.append(self.file1lines[0])
                    self.list2.append("")
                    self.list3.append("")
                    del self.file1lines[0]
            # list 1 was longer
            if len(self.file1lines) > 0:
                self.list1 += self.file1lines
                self.list2 += [""] * len(self.file1lines)
                self.list3 += [""] * len(self.file1lines)
            # list 2 was longer
            elif len(self.file2lines) > 0:
                self.list1 += ["\t"] * len(self.file2lines)
                self.list2 += self.file2lines
                self.list3 += [""] * len(self.file2lines)

    def unsort_comp (self):
        while len(self.file1lines) > 0:
            for j in range(len(self.file2lines)):
                #in both, so we add to column 3
                if self.file1lines[0] == self.file2lines[j]:
                    self.list1 += ["\t"]
                    self.list2 += ["\t"]
                    self.list3.append(self.file1lines[0])
                    del self.file1lines[0]
                    del self.file2lines[j]
                    inBoth = True
                    break
                else:
                    inBoth = False
            # only in first file
            if inBoth == False:
                self.list1.append(self.file1lines[0])
                self.list2.append("")
                self.list3.append("")
                del self.file1lines[0]
        #adding in extras from second file
        self.list1 += ["\t"] * len(self.file2lines)
        self.list2 += self.file2lines
        self.list3 += [""] * len(self.file2lines)
        
    def print_lines (self, hide1, hide2, hide3):
        # checking if hiding certain cols
        if hide1:
            self.list1 = [""] * len(self.list1)
        if hide2:
            self.list2 = [""] * len(self.list2)
        if hide3:
            self.list3 = [""] * len(self.list3)
        # only print our the lines that aren't hidden
        for i in range(len(self.list1)):
            output = (self.list1[i] + self.list2[i] + self.list3[i])
            if output != "":
                print(output)
            

# a lot of this main was based heavily on randline.py 
def main():
    version_mes="%prog 1.0"
    usage_mes="""%prog [OPTION] ... FILE1 FILE2

Implements the comm Linux command in Python.
Compares FILE1 and FILE2 line by line.
- denotes reading from standard input (cannot be used for both)"""

    opt_parser= OptionParser(version=version_mes, usage=usage_mes)
    opt_parser.add_option("-1", action="store_true", dest="hide_col1", default = False, help="hides the output of col 1 (unique to FILE1)")
    opt_parser.add_option("-2", action="store_true", dest="hide_col2", default = False, help="hides the output of col 2 (unique to FILE2)")
    opt_parser.add_option("-3", action="store_true", dest="hide_col3", default = False, help="hides the output of col 3 (common to both files)")
    opt_parser.add_option("-u", "--unsorted", action="store_true", dest="allow_unsort", default = False, help="allows comparision of unsorted files" )
    
    options, args = opt_parser.parse_args(sys.argv[1:])

    try:
        hide_col1 = bool(options.hide_col1)
    except:
        opt_parser.error("invalid arg for -1: {0}".format(options.hide_col1))

    try:
        hide_col2 = bool(options.hide_col2)
    except:
        opt_parser.error("invalid arg for -2: {0}".format(options.hide_col2))

    try:
        hide_col3 = bool(options.hide_col3)
    except:
        opt_parser.error("invalid arg for -3: {0}".format(options.hide_col3))

    try:
        allow_unsort = bool(options.allow_unsort)
    except:
        opt_parser.error("invalid arg for -u --unsorted: {0}".format(options.allow_unsort))

    if len(args) != 2:
        opt_parser.error("missing arguments or too many arguments")

    input1 = args[0]
    input2 = args[1]

    try:
    #change this to whatever your class is
        comparator = compareinput(input1, input2)
        if allow_unsort:
            comparator.unsort_comp()
        else:
            comparator.sorted_comp()
        comparator.print_lines(hide_col1, hide_col2, hide_col3)
    except IOError as err:
        opt_parser.error("I/O error({0}): {1}".format(err.errno, err.strerror))
        
if __name__ == "__main__":
	main()
