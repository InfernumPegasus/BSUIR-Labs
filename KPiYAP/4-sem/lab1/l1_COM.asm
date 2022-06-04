.model  tiny
.code

org 100h              	  			; начальное значение IP = 100h
start:
    mov ah,9              			; номер функции DOS - в АН
    mov dx,offset first_message 			; адрес строки - в DX
    int 21h               			; вызов системной функции DOS
    mov dx,offset second_message
    int 21h
    ret                   			; завершение СОМ-программы

first_message db "Hello World!",0Dh,0Ah,'$'
second_message db "Bye World!",0Dh,0Ah,'$'
end start
