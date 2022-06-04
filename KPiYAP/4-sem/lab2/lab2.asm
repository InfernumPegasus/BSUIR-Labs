.model small
.stack 100h
.code
JUMPS

start:

mov ax, @data
mov ds, ax

mov dx, offset enter_message
call output_string

mov    cx, size_string - 1
mov    bx, offset string
mov    dx, offset current_size
mov    dl, current_size

call   input_string

mov    current_size , dl


mov    dx, offset enter_symbol
call   output_string
cmp    current_size, 0
je     empty_string_error


mov    dx, offset enter_find_word
call   output_string

mov    cx, size_string - 1
mov    bx, offset find_world
mov    dx, offset find_size
mov    dl, find_size

call   input_string

mov    find_size, dl


mov    dx, offset enter_symbol
call   output_string
cmp    find_size, 0
je     empty_string_error

mov    dx, offset enter_insert_word
call   output_string

mov     cx, size_string - 1
mov     bx, offset insert_world
mov     dx, offset insert_size
mov     dl, insert_size

call   input_string

mov    insert_size , dl


mov    dx, offset enter_symbol
call   output_string
cmp    insert_size, 0
je     empty_string_error

call   string_operation

mov    dx, offset entered_message
call   output_string
lea    dx, string
call   output_string
call   end_proc

empty_string_error proc
    mov    dx, offset empty_string
    mov    ah, 09h
    int    21h
    call   end_proc
    ret
empty_string_error endp

compareb MACRO second
    push   bx

    mov    bx, [si]
    cmp    bl, second

    pop    bx
ENDM

end_proc proc
    mov    ax, 4C00h
    int    21h
end_proc endp

insert_word_macro Macro
    push    ax
    push    bx
    push    cx
    push    dx
    push    di

    mov     di, bx
    mov     si, bx

    mov     cl, insert_size
    inc     cl

    mov dx, bx

end_string_loop:
    inc     si
    inc     di
    compareb    24h
    jne     end_string_loop
    dec     si

move_right:
    mov     al, [si]
    mov     [di], al
    dec     di
    dec     si
    cmp     bx, di
    jne     move_right

put_last_space:
    mov     al, ' '
    mov     [di], al
    mov     si, di
    inc     bx
    loop    end_string_loop

    mov     cl, insert_size

    lea     di, insert_world
    mov     si, dx

write_word:
    mov     al, [di]
    mov     [si], al
    inc     di
    inc     si
    loop    write_word

    inc     si
loop_find_next_word:
    compareb    20h
    je      end_insert

    inc     si

    jmp     loop_find_next_word

end_insert:

    pop     di
    pop     dx
    pop     cx
    pop     bx
    pop     ax

ENDM

string_operation proc
    push    ax
    push    bx
    push    cx
    push    si
    push    di

    mov     cx, offset current_size
    mov     cl, current_size

    mov     si, offset string
    jcxz    end_loop

start_loop:

skip_spaces:

    compareb    20h ; сравнение с пробелом

    jne     check_is_required_word

    inc     si
    loop    skip_spaces

    jmp     end_loop

check_is_required_word:
    mov     dx, cx
    mov     bx, si
    mov     di, offset find_world

loop_if_required_word:

    mov     al, [di]
    compareb al
    jne     word_is_not_required
    inc     si
    inc     di
    mov     al, [di]
    cmp     al, "$"
    je      word_is_required
    loop    loop_if_required_word

    jmp     word_is_required

check_space:
    compareb    20h
    jne     word_is_not_required

word_is_required:
   insert_word_macro
   jmp      end_loop

word_is_not_required:
    mov     si, bx
    mov     cx, dx

loop_word_is_not_required:
    compareb    20h

    je      skip_spaces

    inc     si

    loop    loop_word_is_not_required

    jmp     end_loop

end_loop:

    pop     di
    pop     si
    pop     cx
    pop     bx
    pop     ax

    ret
string_operation endp

input_string proc

enter_char:
    mov     ah, 01h
    int     21h
    cmp     al, 36 ; код $
    je      end_enter_symbol
    cmp     al, 13 				; сравнение введенного символа с enter
    je      end_enter_symbol
    mov     [bx],al
    inc     bx
    inc     dl
    loop    enter_char
end_enter_symbol:
; mov [bx], ' '
    ret
input_string endp

output_string proc

    mov     ah, 09h
    int     21h
    ret

output_string endp

.data
size_string equ 200
current_size db 0
string db 950 dup ("$")

find_world db 200 dup ("$")
find_size db 0
insert_world db 200 dup ("$")
insert_size db 0

enter_symbol db 0Dh, 0Ah, '$'
enter_message db "Enter your string: $"
entered_message db "Your string after processing: $"
empty_string db "your string is empty", 0Dh, 0Ah, '$'
enter_find_word db "Enter find word", 0Dh, 0Ah, '$'
enter_insert_word db "Enter insert word", 0Dh, 0Ah, '$'

end start

