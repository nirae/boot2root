#! /usr/bin/env python3

with open('output', 'r') as f:
    lines = f.readlines()
    for i, line in enumerate(lines):
        if 'adduser zaz' in line:
            print("user: zaz\npassword: %s" % lines[i + 1])
            break
