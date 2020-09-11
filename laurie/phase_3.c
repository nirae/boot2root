void phase_3(char *line) {
    /*
    0x08048b98 <+0>:	push   ebp
    0x08048b99 <+1>:	mov    ebp,esp
    0x08048b9b <+3>:	sub    esp,0x14
    0x08048b9e <+6>:	push   ebx
    0x08048b9f <+7>:	mov    edx,DWORD PTR [ebp+0x8]
    0x08048ba2 <+10>:	add    esp,0xfffffff4
    0x08048ba5 <+13>:	lea    eax,[ebp-0x4]
    */
    int third; /* [ebp-0x4] */
    /*
    0x08048ba8 <+16>:	push   eax
    0x08048ba9 <+17>:	lea    eax,[ebp-0x5]
    */
    char second; /* [ebp-0x5] */
    /*
    0x08048bac <+20>:	push   eax
    0x08048bad <+21>:	lea    eax,[ebp-0xc]
    */
    int first; /* [ebp-0xc] */
    /*
    0x08048bb0 <+24>:	push   eax
    0x08048bb1 <+25>:	push   0x80497de
    0x08048bb6 <+30>:	push   edx
    0x08048bb7 <+31>:	call   0x8048860 <sscanf@plt>
    */
    len = sscanf(line, "%d %c %d", first, second, third)
    /*
    0x08048bbc <+36>:	add    esp,0x20
    0x08048bbf <+39>:	cmp    eax,0x2
    0x08048bc2 <+42>:	jg     0x8048bc9 <phase_3+49>
    */
    if (len <= 2) {
        /*
        0x08048bc4 <+44>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    }
    /*
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
    */
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
    /*
    0x08048c99 <+257>:	mov    ebx,DWORD PTR [ebp-0x18]
    0x08048c9c <+260>:	mov    esp,ebp
    0x08048c9e <+262>:	pop    ebp
    0x08048c9f <+263>:	ret    
    */
    return;
}
