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

;NUEVAS FUNCIONES

GLOBAL my_malloc
GLOBAL my_free
GLOBAL print_mm

GLOBAL my_getpid
GLOBAL my_yield
GLOBAL my_create_process
GLOBAL my_nice
GLOBAL my_kill
GLOBAL my_block
GLOBAL my_unblock
GLOBAL my_wait
GLOBAL my_print_pcs

GLOBAL my_sem_open
GLOBAL my_sem_close
GLOBAL my_sem_wait
GLOBAL my_sem_post
GLOBAL my_print_sems







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
    ret
    
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
    ret

;my_malloc - realiza la alocacion de memoria
my_malloc:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 9
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;my_free - libera la memoria
my_free:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 10
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;print_mm - imprime el estado de la memoria
print_mm:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 11
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;my_getpid - devulve el ID del porceso
my_getpid:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 12
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;my_create_process - crea un nuevo proceso
my_create_process:
   push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9

    mov r9, r8  ;fd en r9
    mov r8, rcx ;fg en r8
    mov rcx ,rdx ;argv en rcx
    mov rdx, rsi  ;argc en rdx
    mov rsi,rdi   ; name en rsi
    mov rdi, 13   ; id int
    int 80h

    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret


;my_kill - mata a un proceso
my_kill:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 14
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret



;my_block - bloquea el proceso asociado al id
my_block:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 15
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret



;my_unblock - desbloquea el proceso asociado al id
my_unblock:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 16
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret



;my_yield - 
my_yield:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 17
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret



;my_nice - 
my_nice:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi
    push rdx

    mov rdx, rsi
    mov rsi,rdi
    mov rdi, 18
    int 80h
    
    pop rdx
    pop rsi
    pop rdi
    
    leave
    ret


;my_wait - Espera por el proceso asociado al id
my_wait:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi
    
    mov rsi, rdi
    mov rdi, 19
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret
;my_print_pcs - imprime la lista de procesos
my_print_pcs:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 20
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret    

;my_sem_open - Open semaphore
my_sem_open:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi
    push rdx

    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 21
    int 80h
    
    pop rdx
    pop rsi
    pop rdi
    
    leave
    ret

;my_sem_close - Close semaphore
my_sem_close:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi, rdi
    mov rdi, 22
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;my_sem_wait - Wait semaphore
my_sem_wait:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi, rdi
    mov rdi, 23
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret        

;my_sem_post - Post semaphore
my_sem_post:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi, rdi
    mov rdi, 24
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;my_print_sems - imprime la lista de procesos
my_print_sems:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 25
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret 

ret;opCodeExc - Programa de prueba para excepcion de operacion invalido     
opCodeExc:
    UD2
    ret