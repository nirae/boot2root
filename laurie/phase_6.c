void phase_6(char *line) {
    /*
    0x08048d98 <+0>:	push   ebp
    0x08048d99 <+1>:	mov    ebp,esp
    0x08048d9b <+3>:	sub    esp,0x4c
    0x08048d9e <+6>:	push   edi
    0x08048d9f <+7>:	push   esi
    0x08048da0 <+8>:	push   ebx
    0x08048da1 <+9>:	mov    edx,DWORD PTR [ebp+0x8]
    0x08048da4 <+12>:	mov    DWORD PTR [ebp-0x34],0x804b26c
    */
    int *node = &node1; /* ebp-0x34 */
    /*
    0x08048dab <+19>:	add    esp,0xfffffff8
    0x08048dae <+22>:	lea    eax,[ebp-0x18]
    0x08048db1 <+25>:	push   eax
    0x08048db2 <+26>:	push   edx
    0x08048db3 <+27>:	call   0x8048fd8 <read_six_numbers>
    */
    int numbers[6]; /* [ebp-0x18] */
    read_six_numbers(line, &numbers);
    /*
    0x08048db8 <+32>:	xor    edi,edi
    */
    int i = 0; // edi
    /*
    0x08048dba <+34>:	add    esp,0x10
    0x08048dbd <+37>:	lea    esi,[esi+0x0]
    0x08048dc0 <+40>:	lea    eax,[ebp-0x18]
    0x08048dc3 <+43>:	mov    eax,DWORD PTR [eax+edi*4]
    0x08048dc6 <+46>:	dec    eax
    0x08048dc7 <+47>:	cmp    eax,0x5
    0x08048dca <+50>:	jbe    0x8048dd1 <phase_6+57>
    */
    if (numbers[i] > 5) {
        /*
        0x08048dcc <+52>:	call   0x80494fc <explode_bomb>
        */
        explode_bomb();
    }
    /*
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
    */
    return;
}
