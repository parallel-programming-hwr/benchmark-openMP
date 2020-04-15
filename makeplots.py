#!/usr/bin/env python

import matplotlib.pylab as plt
import csv


x_values = []
y_values = []
y_err = []

with open('data.txt') as FILE:
    data = csv.reader(FILE, delimiter='\t')
    for row in data:
        x_values.append(float(row[4]))
        y_values.append(float(row[0]))
        y_err.append(float(row[1]))
    print(x_values)
    plt.errorbar(x_values,y_values,yerr=y_err)
    plt.savefig('plot.png')
