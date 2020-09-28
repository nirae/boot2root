#! /usr/bin/env python3
import sys
import pycdlib

if len(sys.argv) != 3:
    print('Usage: %s <path> <iso>' % (sys.argv[0]))
    sys.exit(1)

iso = pycdlib.PyCdlib()
iso.open(sys.argv[2])

for child in iso.list_children(iso_path=sys.argv[1]):
    print(child.file_identifier().decode())

iso.close()
