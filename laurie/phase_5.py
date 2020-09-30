#! /usr/bin/env python3
expected = "giants"
abc =     "abcdefghijklmnopqrstuvwxyz"

def phase_5(line):
    static_string = "isrveawhobpnutfg"
    r = ""
    for c in line:
        index = ord(c) & 0xf
        r += static_string[index]
    return r

print(phase_5(abc))
tab_correspondance = {phase_5(c): c for c in abc}
print("Tableau de correspondance :", tab_correspondance)

result = ""
for c in expected:
    result += tab_correspondance[c]

print(result)
