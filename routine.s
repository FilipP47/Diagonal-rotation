global diagonal_swap
diagonal_swap:
    push ebp
    mov ebp, esp
    sub esp, 8

    push esi
    push edi
    push ebx

    mov esi, [ebp + 8]        ; array pointer
    mov edi, [ebp + 12]       ; row count (assuming edi used as a row counter)
    mov ah, [esi]
    mov ecx, [ebp + 12]       ; width
    add ecx, 31
    and ecx, 0xFFFFFFE0
    mov [ebp - 4], ecx        ; rowsize
    mov al, 1
ROW_LOOP:
    mov ebx, [ebp + 12]       ; col counter
COL_LOOP:
    mov ecx, [ebp + 12]
    sub ecx, ebx
    mov edx, [ebp + 12]
    sub edx, edi
    ; x = eax, y = edx
    ; Calculate the original bit position
    ror al, 1
    ; Check if y <= x, if so jump to NEXT_COL
    ;if y > x swap with diag pixel
    cmp edx, ecx
    jbe NEXT_COL

    ; Calculate diagonal bit position
    imul ecx, [ebp - 4]
    add ecx, edx
    mov [ebp - 8], ecx

    ; Get the diagonal byte
    shr ecx, 3
    add ecx, [ebp + 8]  ; Load the byte containing the diagonal bit
    mov dl, [ecx]
    ; calculate diagonal bit position
    mov cl, 7
    and cl, [ebp - 8]
    inc cl
    mov ch, 1
    ror ch, cl
load_diag_bit_pos:
    ; Test original bit
    test ah, al
    jnz original_bit_white

    ; Original bit black
    test dl, ch
    jz same
    jmp bits_different

original_bit_white:
    test dl, ch
    jnz same

bits_different:
    xor dl, ch
    xor ah, al

same:
    ;load diagonal byte
    mov ecx, [ebp - 8]
    shr ecx, 3
    add ecx, [ebp + 8]
    mov [ecx], dl

NEXT_COL:
    test al, 1
    je JUMP_COL_LOOP
    ;  get next_byte
    mov [esi], ah
    inc esi
    mov ah, [esi]

JUMP_COL_LOOP:
    dec ebx
    mov ecx, [ebp + 12]
    sub ecx, ebx
    cmp ecx, [ebp - 4]
    jne COL_LOOP
NEXT_ROW:
    dec edi
    jnz ROW_LOOP

    ;END
    pop ebx
    pop edi
    pop esi
    mov esp, ebp
    pop ebp
    ret
