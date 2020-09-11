#! /usr/bin/env python3

from turtle import *
import re
import os
import time

rx_dict = {
            'right': re.compile(r'^Tourne droite de (\d+) degrees'),
            'left': re.compile(r'^Tourne gauche de (\d+) degrees'),
            'forward': re.compile(r'^Avance (\d+) spaces'),
            'backward': re.compile(r'^Recule (\d+) spaces')
        }

figures = []
figure = []
file = open(os.path.dirname(os.path.abspath(__file__))+'/turtle', 'r')
for line in file.readlines():
    if line == "\n":
        figures.append(figure.copy())
        figure.clear()
    for key, rx in rx_dict.items():
        match = rx.search(line)
        if match:
            figure.append((key, match.group(1)))
file.close()

position = -400

for fig in figures:
    left(90)
    for ins in fig:
        if ins[0] == 'right':
            right(int(ins[1]))
        elif ins[0] == 'left':
            left(int(ins[1]))
        elif ins[0] == 'forward':
            forward(int(ins[1]))
        elif ins[0] == 'backward':
            backward(int(ins[1]))
        print(ins[0], ins[1])
    position += 200
    time.sleep(1)
    reset()
done()
