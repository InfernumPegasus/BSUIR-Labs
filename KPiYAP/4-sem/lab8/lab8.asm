.model tiny
.code 
.386
org 80h
length_cmd      db ?
line_cmd        db ?
org 100h

start:          
    jmp start_not_res   
s:     
OLD_INT9 dd ?
OLD_INT8 dd ?


OPEN_FALG db 0
STOP db 0
CLOSE db 0  
END_F db 1      
PREW_SCAN_CODE db 0
FILE_POINTER dw 160
FILE_DESKRIPTOR dw 0
COUNTER dw 0
cx_less db 0
ctrl_flag db 0

FILE_NAME db 50 dup('$')     

BUFFER db 100 dup('$')                    
BUFFER_SIZE db 100
BYTES_WAS_READ db 0
OPEN_FILE_ERROR db 'ERROR WITH OPENING FILE',0Dh,0Ah,'$'
       
label_tsr dw 4376h
NEW_INT9 proc far
    pushf
    call cs:OLD_INT9
    pusha
    
    push es
    in al, 60h 

    cmp al, 1
    je SET_EXIT_FLAG  

    cmp al, 0AEh
    je EOI
    
    cmp al, 9DH
    je EOI

    cmp al, 2Eh
    jne RESET

    cmp PREW_SCAN_CODE, 1Dh
    jne RESET
    mov [stop], 1   
    jmp EOI
         

SET_EXIT_FLAG:
    call CLOSE_FILE 
    mov [close], 1
    int 08h
    push cs
    pop es
    mov ah, 49h        
    int 21h
    call INSTALL_OLD_HANDLER
    jmp exit

RESET:
    mov [STOP], 0 

EOI:
    mov PREW_SCAN_CODE, al
exit:
	; сообщаем контроллеру прерываний что обработка прерывания завершена
    MOV AL, 20h
    OUT 20h, AL
    
    pop es
    popa
    iret
NEW_INT9 endp  

NEW_INT8 proc far
    cmp [close], 1
    jne ok8
    push cs
    pop es
    mov ah, 49h        
    int 21h
    iret
ok8:
    pushf
    call cs:OLD_INT8
    pusha 

    push cs
    pop ds

    cmp [OPEN_FALG], 1
    je open
    call OPEN_FILE
    mov [open_falg], 1
open:
    cmp [END_F], 1
    jne outp

    mov [END_F], 0 

    call READ_FROM_FILE
    call PRE_OUT
outp:
    call OUTPUT_BUFFER
    MOV AL, 20h
    OUT 20h, AL
    popa
    iret
NEW_INT8 endp

READ_FROM_FILE proc
    mov ah, 3fh
    mov bx, [file_deskriptor]

    xor cx, cx
    mov cl, [buffer_size]     
    lea dx, BUFFER
    int 21h    

    cmp al, 0 
    je set_exit_flag
    mov [BYTES_WAS_READ], al
    ret
READ_FROM_FILE endp


PRE_OUT proc
    xor cx, cx
    mov cl, [BYTES_WAS_READ]
    mov di, offset BUFFER
    mov [cx_less], cl
    mov [counter], di
    ret
endp

OUTPUT_BUFFER proc 
    mov cl, [cx_less]
    mov di, [counter]
    cmp cl, 0
    je end_of_out
  
    cmp [STOP], 1
    je next       


    mov dl, [di]
    mov ah, 02h
    int 21h
    inc di
    dec cl
    jmp next

end_of_out:

    mov [END_F], 1
next:
    mov [cx_less], cl
    mov [counter], di
    ret     
OUTPUT_BUFFER endp   

INSTALL_OLD_HANDLER proc
    mov dx, word ptr cs:OLD_INT9
    mov ds, word ptr cs:OLD_INT9 + 2
    mov ax, 2509h
    int 21h

    mov dx, word ptr cs:OLD_INT8
    mov ds, word ptr cs:OLD_INT8 + 2
    mov ax, 2508h
    int 21h
    
    ret
INSTALL_OLD_HANDLER endp

OPEN_FILE proc 
    mov ah, 3dh
    mov al, 0
    mov dx, offset FILE_NAME
    int 21h
    mov [FILE_DESKRIPTOR], ax
    jc ERROR
    ret
ERROR:
    lea dx, OPEN_FILE_ERROR
    mov ah, 9
    int 21h
    jmp SET_EXIT_FLAG 

OPEN_FILE endp

CLOSE_FILE proc
    mov ah, 3Eh
    mov bx, [FILE_DESKRIPTOR]
    int 21h
    ret
CLOSE_FILE endp
endr:
BEGIN:

INSTALL_NEW_HANDLER proc
	; получаем адрес обработчика прерывания 09h
    mov ax, 3509h
    int 21h
    
    ; проверка на наличие в памяти
    mov ax, es:[bx-2]
    cmp ax, cs:label_tsr
    je exit_start

	; запоминаем смещение и сегментный адрес старого обработчика
    mov word ptr OLD_INT9 + 2, es
    mov word ptr OLD_INT9, bx

	получаем адрес обработчика прерывания 08h
    mov ax, 3508h
    int 21h
    
    mov word ptr OLD_INT8 + 2, es	; сегмент обработчика
    mov word ptr OLD_INT8, bx		; смещение обработчика

    push cs
    pop ds

	; устанавливаем адрес нового обработчика прерывания 09h
    mov ax, 2509h
    lea dx, NEW_INT9
    int 21h

	; устанавливаем адрес нового обработчика прерывания 08h
    mov ax, 2508h
    lea dx, NEW_INT8
    int 21h

    ret
INSTALL_NEW_HANDLER endp

GET_FILE_NAME proc       
    mov di, offset line_cmd
    mov si, offset FILE_NAME
    
    mov al, ' '
    repe scasb     
    dec di         
    
    WRITE_CYCLE:
        mov ax, [di]
        mov [si], ax
        inc di
        inc si
        cmp [di], 0Dh
    jne WRITE_CYCLE    
    mov [si], 0          
    
    lea dx, FILE_NAME
    mov ah, 9
    int 21h
    
    ret
GET_FILE_NAME endp

alredy_exec db 'Programm running now' ,0Dh,0Ah,'$'
PARAMETR_ERROR db 'No file provided' ,0Dh,0Ah,'$' 

start_not_res:
    mov cl, [length_cmd]
    cmp cl, 1
    jle WRONG_PARAMETR

    call GET_FILE_NAME
    
    ; установка нового обработчика
    call INSTALL_NEW_HANDLER
    
    mov ax,3100h					; делаем программу резидентной
    mov dx, (endr - S + 10Fh)/16	; размер резидентной части
    int 21h

exit_start:
	; вызов 9h функции прерывания 21h для проверки наличия в памяти
    mov ah, 9h
    mov dx, offset alredy_exec
    int 21h

    mov ax, 4C00h
    int 21h

WRONG_PARAMETR:
    lea dx, PARAMETR_ERROR
    mov ah, 9
    int 21h
    mov ah,  4ch
    int 21h   
end start
