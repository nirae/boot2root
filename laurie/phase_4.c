int func4(int nb) {
    /*
    0x08048ca0 <+0>:	push   ebp
    0x08048ca1 <+1>:	mov    ebp,esp
    0x08048ca3 <+3>:	sub    esp,0x10
    0x08048ca6 <+6>:	push   esi
    0x08048ca7 <+7>:	push   ebx
    0x08048ca8 <+8>:	mov    ebx,DWORD PTR [ebp+0x8]
    0x08048cab <+11>:	cmp    ebx,0x1
    0x08048cae <+14>:	jle    0x8048cd0 <func4+48>
    */
    if (nb <= 1)
        /*
        0x08048cd0 <+48>:	mov    eax,0x1
        0x08048cd5 <+53>:	lea    esp,[ebp-0x18]
        0x08048cd8 <+56>:	pop    ebx
        0x08048cd9 <+57>:	pop    esi
        0x08048cda <+58>:	mov    esp,ebp
        0x08048cdc <+60>:	pop    ebp
        0x08048cdd <+61>:	ret    
        */
        return 1;
    /*
    0x08048cb0 <+16>:	add    esp,0xfffffff4
    0x08048cb3 <+19>:	lea    eax,[ebx-0x1]
    0x08048cb6 <+22>:	push   eax
    0x08048cb7 <+23>:	call   0x8048ca0 <func4>
    0x08048cbc <+28>:	mov    esi,eax
    */
    int n1 = func4(nb - 1);
    /*
    0x08048cbe <+30>:	add    esp,0xfffffff4
    0x08048cc1 <+33>:	lea    eax,[ebx-0x2]
    0x08048cc4 <+36>:	push   eax
    0x08048cc5 <+37>:	call   0x8048ca0 <func4>
    */
    int n2 = func4(nb - 2);
    /*
    0x08048cca <+42>:	add    eax,esi
    0x08048ccc <+44>:	jmp    0x8048cd5 <func4+53>
    0x08048cce <+46>:	mov    esi,esi
    0x08048cd5 <+53>:	lea    esp,[ebp-0x18]
    0x08048cd8 <+56>:	pop    ebx
    0x08048cd9 <+57>:	pop    esi
    0x08048cda <+58>:	mov    esp,ebp
    0x08048cdc <+60>:	pop    ebp
    0x08048cdd <+61>:	ret    
    */
    return n1 + n2
}

void phase_4(char *line) {
    /*
    0x08048ce0 <+0>:	push   ebp
    0x08048ce1 <+1>:	mov    ebp,esp
    0x08048ce3 <+3>:	sub    esp,0x18
    0x08048ce6 <+6>:	mov    edx,DWORD PTR [ebp+0x8]
    0x08048ce9 <+9>:	add    esp,0xfffffffc
    0x08048cec <+12>:	lea    eax,[ebp-0x4]
    */
    int nb; /* [ebp-0x4] */
    /*
    0x08048cef <+15>:	push   eax
    0x08048cf0 <+16>:	push   0x8049808
    0x08048cf5 <+21>:	push   edx
    0x08048cf6 <+22>:	call   0x8048860 <sscanf@plt>
    */
    len = sscanf(line, "%d", nb)
    /*
    0x08048cfb <+27>:	add    esp,0x10
    0x08048cfe <+30>:	cmp    eax,0x1
    0x08048d01 <+33>:	jne    0x8048d09 <phase_4+41>
    */
    if (len != 1) {
        /*
        0x08048d09 <+41>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    }
    /*
    0x08048d03 <+35>:	cmp    DWORD PTR [ebp-0x4],0x0
    0x08048d07 <+39>:	jg     0x8048d0e <phase_4+46>
    */
    if (nb > 0) {
        /*
        0x08048d0e <+46>:	add    esp,0xfffffff4
        0x08048d11 <+49>:	mov    eax,DWORD PTR [ebp-0x4]
        0x08048d14 <+52>:	push   eax
        0x08048d15 <+53>:	call   0x8048ca0 <func4>
        0x08048d1a <+58>:	add    esp,0x10
        0x08048d1d <+61>:	cmp    eax,0x37
        0x08048d20 <+64>:	je     0x8048d27 <phase_4+71>
        */
        if (func4(nb) != 55)
            /*
            0x08048d22 <+66>:	call   0x80494fc <explode_bomb>
            */
            explode_bomb();
    }
    /*
    0x08048d27 <+71>:	mov    esp,ebp
    0x08048d29 <+73>:	pop    ebp
    0x08048d2a <+74>:	ret    
    */
    return;
}
