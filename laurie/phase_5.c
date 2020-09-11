void phase_5(char *line) {
    /*
    0x08048d2c <+0>:	push   ebp
    0x08048d2d <+1>:	mov    ebp,esp
    0x08048d2f <+3>:	sub    esp,0x10
    0x08048d32 <+6>:	push   esi
    0x08048d33 <+7>:	push   ebx
    0x08048d34 <+8>:	mov    ebx,DWORD PTR [ebp+0x8]
    0x08048d37 <+11>:	add    esp,0xfffffff4
    0x08048d3a <+14>:	push   ebx
    0x08048d3b <+15>:	call   0x8049018 <string_length>
    */
    int len = string_length(line);
    /*
    0x08048d40 <+20>:	add    esp,0x10
    0x08048d43 <+23>:	cmp    eax,0x6
    0x08048d46 <+26>:	je     0x8048d4d <phase_5+33>
    */
    if (len != 6) {
        /*
        0x08048d48 <+28>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    }
    /*
    0x08048d4d <+33>:	xor    edx,edx
    */
    i = 0;
    /*
    0x08048d4f <+35>:	lea    ecx,[ebp-0x8]
    0x08048d52 <+38>:	mov    esi,0x804b220
    */
    char *string = "isrveawhobpnutfg"
    /*
    0x08048d66 <+58>:	cmp    edx,0x5
    0x08048d69 <+61>:	jle    0x8048d57 <phase_5+43>
    */
    while (i != 5) {
        /*
        0x08048d57 <+43>:	mov    al,BYTE PTR [edx+ebx*1] ; edx=1, ebx=param 
        0x08048d5a <+46>:	and    al,0xf
        0x08048d5c <+48>:	movsx  eax,al ; eax= line[i] & 0xff ; = len
        0x08048d5f <+51>:	mov    al,BYTE PTR [eax+esi*1] ; 
        0x08048d62 <+54>:	mov    BYTE PTR [edx+ecx*1],al
        */
        line[i] = string[line[i] & 0xf];
        /*
        0x08048d65 <+57>:	inc    edx
        */
        i++;
    }
    /*
    0x08048d6b <+63>:	mov    BYTE PTR [ebp-0x2],0x0
    0x08048d6f <+67>:	add    esp,0xfffffff8
    0x08048d72 <+70>:	push   0x804980b
    0x08048d77 <+75>:	lea    eax,[ebp-0x8]
    0x08048d7a <+78>:	push   eax
    0x08048d7b <+79>:	call   0x8049030 <strings_not_equal>
    0x08048d80 <+84>:	add    esp,0x10
    0x08048d83 <+87>:	test   eax,eax
    0x08048d85 <+89>:	je     0x8048d8c <phase_5+96>
    */
    if (strings_not_equal(line, "giants") != 0) {
        /*
        0x08048d87 <+91>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    }
    /*
    0x08048d8c <+96>:	lea    esp,[ebp-0x18]
    0x08048d8f <+99>:	pop    ebx
    0x08048d90 <+100>:	pop    esi
    0x08048d91 <+101>:	mov    esp,ebp
    0x08048d93 <+103>:	pop    ebp
    0x08048d94 <+104>:	ret  
    */
    return;
}
