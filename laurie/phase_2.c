void phase_2(char *line) {
    /*
    0x08048b48 <+0>:	push   ebp
    0x08048b49 <+1>:	mov    ebp,esp
    0x08048b4b <+3>:	sub    esp,0x20
    0x08048b4e <+6>:	push   esi
    0x08048b4f <+7>:	push   ebx
    0x08048b50 <+8>:	mov    edx,DWORD PTR [ebp+0x8]
    0x08048b53 <+11>:	add    esp,0xfffffff8
    0x08048b56 <+14>:	lea    eax,[ebp-0x18]
    0x08048b59 <+17>:	push   eax
    0x08048b5a <+18>:	push   edx
    0x08048b5b <+19>:	call   0x8048fd8 <read_six_numbers>
    */
    int input_numbers[6]; /* ebp-0x18 */
    read_six_numbers(line, &input_numbers);
    /*
    0x08048b60 <+24>:	add    esp,0x10
    0x08048b63 <+27>:	cmp    DWORD PTR [ebp-0x18],0x1
    0x08048b67 <+31>:	je     0x8048b6e <phase_2+38>
    */
    if (input_numbers[0] != 1)
        /*
        0x08048b69 <+33>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    /*
    0x08048b6e <+38>:	mov    ebx,0x1
    */
    int i = 1;
    /*
    0x08048b89 <+65>:	cmp    ebx,0x5
    0x08048b8c <+68>:	jle    0x8048b76 <phase_2+46>
    */
    while (i <= 5) {
        /*
        0x08048b73 <+43>:	lea    esi,[ebp-0x18]
        0x08048b76 <+46>:	lea    eax,[ebx+0x1]
        0x08048b79 <+49>:	imul   eax,DWORD PTR [esi+ebx*4-0x4]
        0x08048b7e <+54>:	cmp    DWORD PTR [esi+ebx*4],eax
        0x08048b81 <+57>:	je     0x8048b88 <phase_2+64>
        */
        if (input_numbers[i] != (input_numbers[i - 1] * (i + 1)) {
            /*
            0x08048b83 <+59>:	call   0x80494fc <explode_bomb>
            */
            explode_bomb();
        }
        /*
        0x08048b88 <+64>:	inc    ebx
        */
        i++;
    }
    /*
    0x08048b8e <+70>:	lea    esp,[ebp-0x28]
    0x08048b91 <+73>:	pop    ebx
    0x08048b92 <+74>:	pop    esi
    0x08048b93 <+75>:	mov    esp,ebp
    0x08048b95 <+77>:	pop    ebp
    0x08048b96 <+78>:	ret 
    */
    return;
}
