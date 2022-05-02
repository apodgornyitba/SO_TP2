GLOBAL read
GLOBAL write
GLOBAL scClear
GLOBAL DateTime
GLOBAL getMem
GLOBAL getRegs
GLOBAL separateScreen
GLOBAL opCodeExc
GLOBAL writeAtPos
GLOBAL ticks

; read - lee de un fd determinado
; IN- RDI - FD
;    RSI - Buffer
;    RDX - size
read:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len en rdx
    mov rdx, rsi  ;buff en rdx
    mov rsi,rdi   ; fd en rsi
    mov rdi,0    ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi
    

    leave
    ret


; write - escribe en un fd determinado
;IN- RDI - FD
;    RSI - Buffer
;    RDX - size
write:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len en rdx
    mov rdx, rsi  ;buff en rdx
    mov rsi,rdi   ; fd en rsi
    mov rdi,1     ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret

; write - escribe en un fd determinado
;IN- RDI - FD
;    RSI - Buffer
;    RDX - size
;    RCX - x
;    R8  - y
writeAtPos:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9

    mov r9, r8  ;y en r9
    mov r8, rcx ;x en r8
    mov rcx ,rdx ;size en rdx
    mov rdx, rsi  ;buff en rdx
    mov rsi,rdi   ; fd en rsi
    mov rdi,7     ; id int
    int 80h

    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret

;scClear - Limpia la pantalla
scClear:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 2
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;DateTime - obtiene info del tiempo
;IN - RDI selector de data
DateTime:

    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 5
    int 80h

    pop rsi
    pop rdi

    leave
    ret

;getMem - recibe el valor de memoria en un vector
;IN- RDI - buffer
;    RSI - address
;    RDX - bytes
getMem:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;bytes en rdx
    mov rdx, rsi  ;address en rdx
    mov rsi, rdi   ; buff en rsi
    mov rdi, 3     ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret
    
;getRegs - obtiene el valor de los 15 registros
;IN - RDI buffer
getRegs:

    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 4
    int 80h

    pop rsi
    pop rdi
    
    leave
    ret

;separateScreen - inicializa la separacion en 4 pantallas
separateScreen:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 6
    int 80h
    
    pop rsi
    pop rdi
    leave
    
;ticks - devuelve la cantidad de ticks
ticks:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 8
    int 80h
    
    pop rsi
    pop rdi
    leave

;malloc - realiza la alocacion de memoria
malloc:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 9
    int 80h
    
    pop rsi
    pop rdi
    leave

;free - libera la memoria
free:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 10
    int 80h
    
    pop rsi
    pop rdi
    leave

;printMM - imprime el estado de la memoria
printMM:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 11
    int 80h
    
    pop rsi
    pop rdi
    leave
    
ret;opCodeExc - Programa de prueba para excepcion de operacion invalido     
opCodeExc:
    UD2
    ret