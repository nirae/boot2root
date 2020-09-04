#! /usr/bin/env python3
import os
import re
import sys

results = {}

for file in os.listdir("ft_fun"):
    f = open("ft_fun/%s" % file, 'r')
    content = f.read()
    f.close()
    file_line = re.search(r'//file([0-9]*)', content)
    file_number = int(file_line.group(1))
    results[file_number] = content

original_stdout = sys.stdout
with open("main.c", 'w+') as file:
    sys.stdout = file
    for _, value in sorted(results.items()):
        print(value)
    file.close()
