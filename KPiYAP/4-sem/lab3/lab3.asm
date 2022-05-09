; 12. гистограмма

.model small
.stack 100h
JUMPS

.data

dog 	 db "@", '$'
new_line db 10, 13, '$'

incorrect_input db 10,13,"Error: incorrect input$"
overflow_error  db 10,13,"Error: overflow$"
input_request   db       "Enter array of 30 numbers$"

buf db 10,10 dup(?)
out_buf db 6 dup(?), '$'

array dw 30 dup(?)
; т.к. массив на 30 элементов, далее используется "магическое" число 60
; т.к. мы используем word, а не byte для каждой ячейки нашего массива

.code

endl macro
	push ax
	push dx

    mov ah, 09h
    mov dx, offset new_line
    int 21h
    
    pop dx
    pop ax
endm

print_str macro out_str
	push dx
	push ax

    mov dx,offset out_str
    mov ah,9h
    int 21h
    
    pop ax
    pop dx
endm

get_str macro buff
    mov ah,0Ah
	mov dx,offset buff
	int 21h
endm


start:

    mov ax,@data
    mov ds,ax 
    
    print_str input_request
    print_str new_line

	; ВВОД МАССИВА
    call get_array
     
    main:  
        
    ; ВЫВОД МАССИВА
    call_print_array:
    call print_array
    
    ; РИСОВАНИЕ ГИСТОГРАММЫ
    
    call abs_all
    
    mov di, offset array
    mov cx, 30
    
    endl
    
    start_loop_process:
    call print_dogs
    	inc di
    	inc di
    loop start_loop_process
    
    exit_main:
    	mov ah, 4ch
    	int 21h


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; ВЫВОД ПЕСИКОВ ^-^

print_dogs proc
	push cx
	
	mov cx, [di]
	
	jcxz if_cx_zero
	
	output_dogs:
	
	print_str dog
	
	loop output_dogs
	endl
	
	jmp end
	
	if_cx_zero:
	endl
	jmp end
	
	end:
	pop cx
	ret
endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; МОДУЛЬ МАССИВА

abs proc uses ax
    mov ax,array[di]
    cmp ax,32768
    je abs_error
    and ax,32768
    cmp ax,32768
    jne abs_end  ;if positive

    mov ax,array[di]
    not ax
    add ax,1
    mov array[di],ax
    jmp abs_end

    abs_error:
    print_str overflow_error

    abs_end:

    ret
abs endp 


abs_all proc uses di
    mov di,0
    aa:
        cmp di,60
        je aa_end
        call abs
        add di,2
        jmp aa

    aa_end:
    ret
abs_all endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

print_num proc uses ax,bx,cx,dx,si
    
    xor si,si
    mov si,5 
    
    mov cx, 60
    
    check_sign:
        mov ax, array[di]
        and ax,32768
        cmp ax,32768
        je num_negative
        jmp positive
        
    num_negative:
        mov ax,array[di]
        not ax
        add ax,1
        mov bl,'-'
        mov out_buf,bl
        jmp cycle1
        
    
    positive:
        mov ax,array[di]
        mov bl,'0'
        mov out_buf,bl
        jmp cycle1
    
    cycle1: 
        xor bx,bx
        mov bx, offset out_buf
        
        xor dx,dx
        div cx
        add dx,'0'
        mov bx[si], dl
        dec si
        cmp si,0
        je end_print_num
        jmp cycle1
    
    end_print_num:
        print_str new_line
        print_str out_buf
        
    ret
print_num endp
   
print_array proc uses di
    mov di, 0
    print:
        cmp di, 60
        je print_end
        call print_num
        add di, 2
        jmp print
    
    print_end:
    
    ret
print_array endp

get_num proc uses ax,bx,cx,dx,si
    xor cx,cx
    mov cl, buf[1]
    cmp cx,0
    je error_input ;no input (pressed enter)
    mov si,2
    xor bx,bx
    mov bl,buf[si]
    cmp bl,'-'
    jne _atoi
    cmp cl,1
    je error_input
    inc si
    dec cx
    
    _atoi:
        xor ax, ax ; zero a "result so far"
        top:
            cmp cx,0
            je end_atoi
            xor bx,bx 
            mov dx, 10
            mov bl, buf[si] ; get a character
            inc si ; ready for next one
            cmp bl, '0' ; valid?
            jb error_input
            cmp bl, '9'
            ja error_input
            sub bl, '0' ; "convert" character to number
            imul dx ; multiply "result so far" by ten
            jc error_overflow
            add ax, bx ; add in current digit
            dec cx
            jmp top ; until done
            
        end_atoi:
            xor bx,bx
            mov bl,buf[2]
            cmp bl,'-'
            je _negative
            
            cmp ax,32767
            ja error_overflow
            
            jmp put_in_array
    
    _negative:
        cmp ax, 32768
        ja error_overflow
        not ax
        add ax,1
        
    put_in_array:
        mov array[di],ax
        jmp exit
        
    error_input: 
        sub di,2
        print_str incorrect_input
        jmp exit
    error_overflow:
        sub di,2
        print_str overflow_error
    
    exit:
    
    ret
get_num endp


get_array proc uses di
    xor di,di
    get_array_loop:
        get_str buf
        call get_num
        add di,2
        print_str new_line
        cmp di, 60
        jne get_array_loop
    ret
get_array endp

end start
