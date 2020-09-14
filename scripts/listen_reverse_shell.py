#! /usr/bin/env python3
from pwn import *

l = listen(1234)
l.wait_for_connection()
l.sendline("whoami")
l.interactive()
