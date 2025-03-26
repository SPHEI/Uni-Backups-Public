; Bilinear interpolation project. - Asm implementation. 
; Winter Semester 2024/2025
; Authors: Jakub (https://github.com/SPHEI), Alicja (https://github.com/Smoka24)
; Version 1.0
.data  

.code



floorDouble proc
	
    ;Function for rounding (floor function)
    ;Called with int a = floor(double);
	;Input is in register xmm0 (double)
	;Output is in register eax (int)

    cvttsd2si rax, xmm0          ; Converting double from xmm0 to int in rax by truncating towards 0
    cvtsi2sd xmm1, rax           ; We make it double again in xmm1 so we are able to compare
    ucomisd xmm0, xmm1           ; Comparing original double in xmm0 with new one in xmm1
    jae done                     ; If original is greater or equal, jump to "done" part of the code 
    sub rax, 1                   ; If original is less, decrement rax to floor
done:
    mov eax, eax                 ; Moving result to eax 
    ret                          ; Return 

floorDouble endp



calcP proc
	
    ;Function for calculating new position of pixels after scaling
    ;float Py = ((i * (height-1)) / (targetHeight-1)); The same formula is used for Px.
	;Inputs: x/y in ecx; width/height in edx; targetWidth/targetHeight in r8
	;Output in xmm0 (double)

	dec edx                      ; Decrementing height by 1 like in formula
	dec r8                       ; Decrementing targetHeight by 1 like in formula
	imul ecx, edx                ; Multiplying i with decremented height
                                    
	
	cvtsi2sd xmm1, ecx           ; Converting multiplied value from ecx to double
	movapd xmm0, xmm1            ; Moving xmm1 to xmm0
	cvtsi2sd xmm1, r8            ; Converting value from r8 to double

	divpd xmm0, xmm1             ; Dividing xmm0 by xmm1 (Division from formula)
	ret                          ; Return

calcP endp



linearWeights proc

    ;Function for calculating linear weights
    ;int y1 = floor(Py); The same formula is used for Px
	;int y2 = floor(Py) + 1;
    ;float w1 = (y2 - Py);
	;float w2 = (Py - y1);

	;Inputs: P in xmm0
	;Outputs: w1 in xmm4, w2 in xmm5, y2 in rcx

    cvttsd2si rax, xmm0          ; Converting double in xmm0 to integer in rax
    cvtsi2sd xmm1, rax           ; Converting it back to double so it's now rounded (floored)
                                 ; Now we have y1 in xmm1
    mov rcx, rax                 ; Moving data to rcx
    add rcx, 1                   ; Adding 1 to rcx like in formula
    cvtsi2sd xmm2, rcx           ; Converting to double
                                 ; Now we have y2 in xmm2
    movapd xmm4, xmm2            ; Copying y2 from xmm2 into xmm4
    subsd xmm4, xmm0             ; Substracting like in formula
                                 ; Now we have w1 in xmm4
    movapd xmm5, xmm0            ; Copying Py from xmm0 into xmm5
    subsd xmm5, xmm1             ; Substracting like in formula
								 ; Now we have w2 in xmm5
    ret                          ; Return

linearWeights endp



bilinearWeights proc

	; Function for calculating bilinear weights
    ; int x1 = floor(Px);
    ; int x2 = floor(Px) + 1;
    ; int y1 = floor(Py);
    ; int y2 = floor(Py) + 1;
    ; float w11 = (x2 - Px) * (y2 - Py)
    ; float w12 = (x2 - Px) * (Py - y1)
    ; float w21 = (Px - x1) * (y2 - Py)
    ; float w22 = (Px - x1) * (Py - y1)

    ; Inputs: Px in xmm0, Py in xmm1

    cvttsd2si rax, xmm0          ; Converting Px to integer
    cvttsd2si rcx, xmm1          ; Converting Py to integer
    cvtsi2sd xmm2, rax           ; Converting Px back to double so it is now rounded
    cvtsi2sd xmm3, rcx           ; Converting Py back to double so it is now rounded
                                 ; Now we have x1 in xmm2 and y1 in xmm3
    add rax, 1                   ; Adding 1 to x1 like in formula
    mov r8, rcx                  ; Copying y1 to r8
    add r8, 1                    ; Adding 1 to y1 like in formula
                                 ; Now we have x2 in rdx and y2 in r8
    cvtsi2sd xmm4, rax           ; Converting x2 to double
    cvtsi2sd xmm5, r8            ; Converting y2 to double
                                 ; Now we have x2 in xmm4 and y2 in xmm5

    ; Calculating w11
    movapd xmm6, xmm4            ; Moving x2 to xmm6
    subsd xmm6, xmm0             ; Subtracting like in formula (x2 - Px)
                                 ; Now we have (x2 - Px) in xmm6
    movapd xmm7, xmm5            ; Moving y2 to xmm7
    subsd xmm7, xmm1             ; Subtracting like in formula (y2 - Py)
                                 ; Now we have (y2 - Py) in xmm7
    mulsd xmm6, xmm7             ; Computing w11 = (x2 - Px) * (y2 - Py)
                                 ; Now we have w11 in xmm6

    ; Calculating w12
    movapd xmm8, xmm4            ; Copying x2 to xmm8
    subsd xmm8, xmm0             ; Subtracting like in formula (x2 - Px)
    movapd xmm9, xmm1            ; Copying Py to xmm9
    subsd xmm9, xmm3             ; Subtracting like in formula (Py - y1)
    mulsd xmm8, xmm9             ; Calculating w12

    ; Calculating w21
    movapd xmm10, xmm0           ; Copying Px to xmm10
    subsd xmm10, xmm2            ; Subtracting like in formula (Px - x1)
    movapd xmm11, xmm5           ; Copying y2 to xmm11
    subsd xmm11, xmm1            ; Subtracting like in formula (y2 - Py)
    mulsd xmm10, xmm11           ; Calculating w21


    ; Calculating w22
    movapd xmm12, xmm0           ; Copying Px to xmm12
    subsd xmm12, xmm2            ; Subtracting like in formula (Px - x1)
    movapd xmm13, xmm1           ; Copying Py to xmm13
    subsd xmm13, xmm3            ; Subtracting like in formula (Py - y1)
    mulsd xmm12, xmm13           ; Calculating w22

    movapd xmm0, xmm6            ; Moving w11 into xmm0
    movapd xmm1, xmm8            ; Moving w12 to xmm1
    movapd xmm2, xmm10           ; Moving w21 to xmm2
    movapd xmm3, xmm12           ; Moving w22 to xmm3

    ret                          ; Return

bilinearWeights endp

interpolate proc    
    ;The main bilinear interpolation function.
    ;Inputs:
    ;rcx - inArray pointer
    ;rdx - outArray pointer
    ;width - r8
    ;height - r9
    ;targetWidth/targetHeight - on the stack
    ;Output - outArray

    ;Moving Inputs to safer registers (not used by above functions).
    mov r11, r8             ;width
    mov r12, r9             ;height
    mov r13, [rsp + 28h]    ;targetWidth
    mov r14, [rsp + 30h]    ;targetHeight
    mov r9, rcx             ;inArray ptr
    mov r10, rdx            ;outArray ptr
    mov rax, 0              ;reset 
    mov rbx, r14            ;i = targetHeight

loopHeight:         ;for(int i = targetHeight; i >= 0; i--)
    dec rbx                 ;i--
    mov rcx, r13            ;j = targetWidth
    loopWidth:      ;for(int j = targetWidth; j >=0; j--)
        dec rcx             ;j--
        push rcx            ;push j to the stack so we dont lose it
        
        ;P point calculations
        mov rdx, r11        ;mov width to edx
        mov r8, r13         ;mov targetWidth to r8
        call calcP          ;Px in xmm0
        movapd xmm2, xmm0   ;Px to xmm2

        mov rdx, r12        ;height to rdx
        mov r8, r14         ;targetHeight to r8
        mov rcx, rbx        ;i to rcx
        call calcP          ;Py in xmm0
        movapd xmm3, xmm0   ;Py in xmm3
        
        ;Choosing which mode to use
        mov rcx, 0

        movapd xmm0, xmm2   ;Px in xmm0
        cvttsd2si eax, xmm0 ;Round Px to int
        cvtsi2sd xmm1, eax  ;Back to double
        ucomisd xmm0, xmm1  ;Check if Px is an integer
        jnz PxNotInt
        mov cl, 1
    PxNotInt:
        movapd xmm0, xmm3   ;Py in xmm0
        cvttsd2si eax, xmm0 ;Round Py to int
        cvtsi2sd xmm1, eax  ;Back to double
        ucomisd xmm0, xmm1  ;Check if Py is an integer
        jnz PyNotInt
        mov ch, 1
    PyNotInt:    
        mov al, cl
        add al, ch
        cmp al, 2       ;If both are 1 then we get 2
        je DirectMode
        cmp cl, 1       ;Else if cl is 1 then we linearInterp on y
        je LinearY
        cmp ch, 1       ;Else if ch is 1 then we linearInterp on x
        je LinearX
        jmp Bilinear    ;Else we do bilinear

        ;Modes:
    DirectMode:         
        movapd xmm0, xmm2
        call floorDouble    
        mov rdx, rax        ;floor(Px) in rdx
        movapd xmm0, xmm3
        call floorDouble    ;floor(Py) in rax
        imul rax, r11      
        add rax, rdx        ;Py * width + Px in rax
        imul rax, 3         ;Py * width * 3 + Px * 3

        pop rcx             ;get j
        push rcx            ;store j
        mov rdx, rbx        ;i to rdx
        imul rdx, r13       ;i * targetWidth
        add rcx, rdx        ;i * targetWidth + j in rcx
        imul rcx, 3         ;i * targetWidth * 3 + j * 3

        mov r8, r10         ;Get address of output array
        add r8, rcx         ;rcx is now free
        mov rcx, 0
        mov cl, [r9 + rax]                 ;Copy BLUE
        mov BYTE PTR [r8], cl             
        mov cl, [r9 + rax + 1]             ;Copy GREEN
        mov BYTE PTR [r8 + 1], cl          
        mov cl, [r9 + rax + 2]             ;Copy RED
        mov BYTE PTR [r8 + 2], cl        

        jmp EndLoop
    LinearX:           
        movapd xmm0, xmm2   ;Px in xmm0
        call linearWeights
        ;Outputs: w1 in xmm4, w2 in xmm5, x2 in rcx
        ;w1 * table[(int)floor(Py)][x1].red +
        ;w2 * table[(int)floor(Py)][x2].red

        movapd xmm0, xmm3       ;Py -> xmm0
        call floorDouble        ;floor(Py) in rax
        imul rax, r11           ;floor(Py) * width
        add rax, rcx            ;floor(Py) * width + x2
        imul rax, 3             ;floor(Py) * width * 3 + x2 * 3

        mov rdx, 0              ;Reset rdx to avoid glitches.
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm0, edx      ;BLUE in xmm0
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm1, edx      ;GREEN in xmm1
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm2, edx      ;RED in xmm2

        mulpd xmm0, xmm5        ;blue * w2
        mulpd xmm1, xmm5        ;green * w2
        mulpd xmm2, xmm5        ;red * w2

        sub rax, 3              ;floor(Py) * width * 3 + x1 * 3
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm5, edx      ;BLUE in xmm5
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm6, edx      ;GREEN in xmm6
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm7, edx      ;RED in xmm7

        mulpd xmm5, xmm4        ;blue * w1
        mulpd xmm6, xmm4        ;green * w1
        mulpd xmm7, xmm4        ;red * w1

        addpd xmm0, xmm5
        addpd xmm1, xmm6
        addpd xmm2, xmm7        ;Colors interpolated


        pop rcx             ;get j
        push rcx            ;store j
        mov rdx, rbx        ;i to rdx
        imul rdx, r13       ;i * targetWidth
        add rcx, rdx        ;i * targetWidth + j in rcx
        imul rcx, 3         ;i * targetWidth * 3 + j * 3

        mov r8, r10         ;Get address of output array
        add r8, rcx         ;rcx is now free

        mov rax, 0
        cvttsd2si rax, xmm0
        mov BYTE PTR [r8], al ;Copy BLUE
        cvttsd2si rax, xmm1
        mov BYTE PTR [r8 + 1], al ;Copy GREEN
        cvttsd2si rax, xmm2
        mov BYTE PTR [r8 + 2], al ;Copy RED
        jmp EndLoop
    LinearY:
        movapd xmm0, xmm3   ;Py in xmm0
        movapd xmm6, xmm2   ;Px in xmm6
        call linearWeights
        ;Outputs: w1 in xmm4, w2 in xmm5, y2 in rcx
        ;w1 * table[y1][(int)floor(Px)].red + 
        ;w2 * table[y2][(int)floor(Px)].red

        movapd xmm0, xmm6       ;Px -> xmm0
        call floorDouble        ;floor(Px) in rax

        imul rcx, r11           ;y2 * width
        add rax, rcx            ;y2 * width + floor(Px)
        imul rax, 3             ;y2 * width * 3 + floor(Px) * 3

        mov rdx, 0              ;Reset rdx to avoid glitches.
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm0, edx      ;BLUE in xmm0
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm1, edx      ;GREEN in xmm1
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm2, edx      ;RED in xmm2

        mulpd xmm0, xmm5        ;blue * w2
        mulpd xmm1, xmm5        ;green * w2
        mulpd xmm2, xmm5        ;red * w2

        mov rcx, r11            ;width
        imul rcx, 3             ;width * 3
        sub rax, rcx            ;y1 * width * 3 + floor(Px) * 3
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm5, edx      ;BLUE in xmm5
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm6, edx      ;GREEN in xmm6
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm7, edx      ;RED in xmm7

        mulpd xmm5, xmm4        ;blue * w1
        mulpd xmm6, xmm4        ;green * w1
        mulpd xmm7, xmm4        ;red * w1

        addpd xmm0, xmm5
        addpd xmm1, xmm6
        addpd xmm2, xmm7        ;Colors interpolated

        pop rcx             ;get j
        push rcx            ;store j
        mov rdx, rbx        ;i to rdx
        imul rdx, r13       ;i * targetWidth
        add rcx, rdx        ;i * targetWidth + j in rcx
        imul rcx, 3         ;i * targetWidth * 3 + j * 3

        mov r8, r10         ;Get address of output array
        add r8, rcx         ;rcx is now free

        mov rax, 0
        cvttsd2si rax, xmm0
        mov BYTE PTR [r8], al ;Copy BLUE
        cvttsd2si rax, xmm1
        mov BYTE PTR [r8 + 1], al ;Copy GREEN
        cvttsd2si rax, xmm2
        mov BYTE PTR [r8 + 2], al ;Copy RED

        jmp EndLoop
    Bilinear:
        ; Inputs: Px in xmm0, Py in xmm1
        movapd xmm0, xmm2
        movapd xmm1, xmm3
        call bilinearWeights
        ; Moving w11 - xmm0 Moving w12 - xmm1 Moving w21 - xmm2 w22 - xmm3
        ; x2 - rax y2 - r8
        ;w11 * table[y1][x1].red + 
        ;w12 * table[y2][x1].red + 
        ;w21 * table[y1][x2].red + 
        ;w22 * table[y2][x2].red;
        imul r8, r11    ;y2 * width
        add rax, r8     ;y2 * width + x2
        imul rax, 3     ;y2 * width * 3 + x2*3
        mov rdx, 0              ;Reset rdx to avoid glitches.
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm5, edx      ;BLUE in xmm5
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm6, edx      ;GREEN in xmm6
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm7, edx      ;RED in xmm7

        mulpd xmm5, xmm3        ;blue * w22
        mulpd xmm6, xmm3        ;green * w22
        mulpd xmm7, xmm3        ;red * w22

        sub eax, 3              ;y2 * width * 3 + x1 * 3
        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm8, edx      ;BLUE in xmm8
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm9, edx      ;GREEN in xmm9
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm10, edx      ;RED in xmm10

        mulpd xmm8, xmm1        ;blue * w12
        mulpd xmm9, xmm1        ;green * w12
        mulpd xmm10, xmm1       ;red * w12

        addpd xmm5, xmm8
        addpd xmm6, xmm9
        addpd xmm7, xmm10

        add rax, 3              ;y2 * width * 3 + x2*3
        mov r8, r11             ;width
        imul r8, 3              ;width * 3
        sub rax, r8             ;y1 * width * 3 + x2*3

        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm8, edx      ;BLUE in xmm8
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm9, edx      ;GREEN in xmm9
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm10, edx     ;RED in xmm10

        mulpd xmm8, xmm2        ;blue * w21
        mulpd xmm9, xmm2        ;green * w21
        mulpd xmm10, xmm2       ;red * w21

        addpd xmm5, xmm8
        addpd xmm6, xmm9
        addpd xmm7, xmm10       

        sub eax, 3              ;y1 * width * 3 + x1*3

        mov dl, [r9 + rax]      ;Copy BLUE
        cvtsi2sd xmm8, edx      ;BLUE in xmm8
        mov dl, [r9 + rax + 1]  ;Copy GREEN
        cvtsi2sd xmm9, edx      ;GREEN in xmm9
        mov dl, [r9 + rax + 2]  ;Copy RED
        cvtsi2sd xmm10, edx     ;RED in xmm10

        mulpd xmm8, xmm0        ;blue * w11
        mulpd xmm9, xmm0        ;green * w11
        mulpd xmm10, xmm0       ;red * w11

        addpd xmm5, xmm8
        addpd xmm6, xmm9    
        addpd xmm7, xmm10       ;colors interpolated


        pop rcx             ;get j
        push rcx            ;store j
        mov rdx, rbx        ;i to rdx
        imul rdx, r13       ;i * targetWidth
        add rcx, rdx        ;i * targetWidth + j in rcx
        imul rcx, 3         ;i * targetWidth * 3 + j * 3

        mov r8, r10         ;Get address of output array
        add r8, rcx         ;rcx is now free

        mov rax, 0
        cvttsd2si rax, xmm5
        mov BYTE PTR [r8], al ;Copy BLUE
        cvttsd2si rax, xmm6
        mov BYTE PTR [r8 + 1], al ;Copy GREEN
        cvttsd2si rax, xmm7
        mov BYTE PTR [r8 + 2], al ;Copy RED


        jmp EndLoop
    EndLoop:
        pop rcx             ;recover j
        cmp rcx, 0
        jne loopWidth
    cmp rbx, 0
    jne loopHeight
    ret
interpolate endp

end