#!/usr/bin/env python

import matplotlib.pylab as plt
import csv
import sys
if len(sys.argv) == 2:
    infile = sys.argv[1]
else:
    infile = 'data.txt'
if len(sys.argv) == 3:
    outfile = sys.argv[2]
else:
    outfile = 'plot.png'


x_values = []
y_values = []
y_err = []

with open(infile) as FILE:
    data = csv.reader(FILE, delimiter='\t')
    for row in data:
        x_values.append(float(row[4]))
        y_values.append(float(row[0]))
        y_err.append(float(row[1]))
    print(x_values)
    plt.errorbar(x_values,y_values,yerr=y_err, fmt='.')
    plt.savefig(outfile)
