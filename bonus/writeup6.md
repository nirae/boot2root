# Dirty c0w spawn shell

We starting at the part **reverse shell** of the 1st solution

So, we have a reverse shell on the server, as the user `www-data`

We will use the Dirty Cow exploit again, with another way. With the exploit [`c0w.c`](https://gist.github.com/KrE80r/42f8629577db95782d5e4f609f437a54)

This exploit will not add a new user like the previous one but will infect the binary `/usr/bin/passwd`. After that, running the binary will open a root shell

We built a script `c0w.py` to get the reverse shell and send the exploit. The script works like this:

- Open the reverse shell
- Create the file `c0w.c`, with the local file (with the good shellcode for x86 architecture)
- Compile and run the exploit
- Run `/usr/bin/passwd`
- Root shell!


