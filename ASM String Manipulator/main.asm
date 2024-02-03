;Olivia Patterson
;ID: DK92177
;CMSC 313 Project 4
;This programs utilized an array of pointers with dynamic memory allocation and allows the user to manipulate strings in various ways.
;This program is the main file that calls the subroutines in the other files and itself.
        extern display
        extern readIn
        extern printStats
        extern freeUp
        extern weave
        extern reverse
        extern scanf
        extern printf
        section .data
menu:                   db "Encryption menu options: ", 10, 0
menulen:                equ $-menu
show:                   db "s - show current messages", 10, 0
showlen:                equ $-show
read:                   db "r - read new message", 10, 0
readlen:                equ $-read
encrypt:                db "e - encrypt", 10, 0
encryptlen:             equ $-encrypt
print:                  db "p - print", 10, 0
printlen:               equ $-print
quit:                   db "q - quit", 10, 0
quitlen:                equ $-quit
invalid:                db "Invalid option! Try again!", 10, 0
invalidlen:             equ $-invalid
prompt:                 db "enter option letter -> ", 0
promptlen:              equ $-prompt
originalmessage:        db "This is the original message.", 0
originalmessagelen:     equ $-originalmessage
bye:                    db "Goodbye!", 10, 0
byelen:                 equ $-bye
newline:                db 10, 0
newlinelen:             equ $-newline
choosestring:           db "What string do you want to see? ", 0
choosestringlen:        equ $-choosestring
replace:                db "What string do you want to replace? ", 0
replacelen:             equ $-replace
newmsg:                 db "Enter new message: ", 0
newmsglen:              equ $-newmsg
counter:                db 0
easteregg:              db 0
eggmsg:                 db "What!? How did you get in here!?", 10, 0
eggmsglen:              equ $-eggmsg
currmsg:                db "Current message: ", 0
currmsglen:             equ $-currmsg
fmtD:                   db "%d", 0
stringfmt:              db "%s", 0

        section .bss

stringArray:            resq 10 ;array of 10 pointers to messages
input:                  resb 2  ;holds the user menu choice
index:                  resb 2  ;holds index of string to work with



        section .text
        global main
main:
        xor r9, r9

init:
        ;initialize the array of pointers to messages
        mov rax, originalmessage
        mov [stringArray+r9], rax
        add r9, 8
        cmp r9, 80
        jl init

mainloop:
        call printmenu
        call printuserprompt

        ;check user input and either call subroutines or print invalid
        cmp byte [input], 's'
        je s
        cmp byte [input], 'S'
        je s
        cmp byte [input], 'r'
        je r
        cmp byte [input], 'R'
        je r
        cmp byte [input], 'e'
        je e
        cmp byte [input], 'E'
        je e
        cmp byte [input], 'p'
        je p
        cmp byte [input], 'P'
        je p
        cmp byte [input], 'q'
        je q
        cmp byte [input], 'Q'
        je q
        call invalidinput
        jmp mainloop

s:
        mov [easteregg], byte 0
        call displayASM
        jmp mainloop
r:
        mov [easteregg], byte 0
        call readinASM
        jmp mainloop
e:
        mov [easteregg], byte 0
        call encryptASM
        jmp mainloop
p:
        mov [easteregg], byte 0
        call printASM
        jmp mainloop
q:
        mov [easteregg], byte 0
        call quitASM
        jmp mainloop

;subroutines below
convertASCII:
        ;converts user input to integer
        movzx rax, byte[index]
        sub rax, '0'

        cmp rax, 9
        jg invalidindex
        ret
invalidindex:
        ;error check for invalid index chosen
        mov rax, -1
        ret

printmenu:
        ;print menu
        mov rax, 1
        mov rdi, 1
        mov rsi, menu
        mov rdx, menulen
        syscall

        ;print show
        mov rax, 1
        mov rdi, 1
        mov rsi, show
        mov rdx, showlen
        syscall

        ;print read
        mov rax, 1
        mov rdi, 1
        mov rsi, read
        mov rdx, readlen
        syscall

        ;print encrypt
        mov rax, 1
        mov rdi, 1
        mov rsi, encrypt
        mov rdx, encryptlen
        syscall

        ;print print
        mov rax, 1
        mov rdi, 1
        mov rsi, print
        mov rdx, printlen
        syscall

        ;print quit
        mov rax, 1
        mov rdi, 1
        mov rsi, quit
        mov rdx, quitlen
        syscall

        ret

printuserprompt:
        ;clear input
        mov word[input], 0

        ;print prompt
        mov rax, 1
        mov rdi, 1
        mov rsi, prompt
        mov rdx, promptlen
        syscall

        ;read user input
        mov rax, 0
        mov rdi, 0
        mov rsi, input
        mov rdx, 2
        syscall

        ret

invalidinput:
        ;easter egg check
        add [easteregg], byte 1
        cmp [easteregg], byte 4
        je egg

        ;print invalid message
        mov rax, 1
        mov rdi, 1
        mov rsi, invalid
        mov rdx, invalidlen
        syscall

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        ret

egg:
        ;print easter egg message
        mov rax, 1
        mov rdi, 1
        mov rsi, eggmsg
        mov rdx, eggmsglen
        syscall

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        ret

displayASM:
        ;calls the c function to display messages in the array
        xor rdx, rdx            ;initialize the registers
        xor rcx, rcx
        xor r8, r8
        xor r9, r9
        push rbp
        mov rdi, stringArray    ;setting parameters for function call
        mov rsi, 10
        call display
        pop rbp

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

         ret

readinASM:
        ;calls c function to read in and validate a new message
        mov word[index], 0      ;clear index

        ;ask which message to replace
        mov rax, 1
        mov rdi, 1
        mov rsi, replace
        mov rdx, replacelen
        syscall

        ;save selected message index
        mov rax, 0
        mov rdi, 0
        mov rsi, index
        mov rdx, 2
        syscall

        ;integer conversion and bounds checking
        call convertASCII
        cmp rax, -1
        je invalidinput
        cmp rax, 9
        jg invalidinput

        push rax

        ;ask user to enter the new message
        mov rax, 1
        mov rdi, 1
        mov rsi, newmsg
        mov rdx, newmsglen
        syscall

        ;prep for function call
        pop rsi
        mov rdi, stringArray
        call readIn

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        ret

encryptASM:
        ;calls an assembly function to reverse or c function to weave a message
        rdtsc                   ;use the clock to randomize reverse or weave selection
        and eax, 1
        cmp eax, 0
        je callrev              ;if 0, reverse, otherwise, weave
        jmp callweave
callrev:
        ;initialize registers
        xor r9,	r9
        xor r10, r10

       ;call to c function to reverse the message
        mov rdi, stringArray
        movzx rsi, byte[counter]
        call reverse

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        jmp cleanup
callweave:
        ;call to c function to weave a message
        mov rdi, stringArray        ;prep to send stringArray and counter to weave function
        movzx rsi, byte[counter]
        call weave

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        jmp cleanup
cleanup:
        add [counter], byte 1       ;inc counter
        cmp byte[counter], 9        ;if counter hits 9, needs to be reset to 0
        jg reset
        ret
reset:
        mov [counter], byte 0       ;reset counter to 0
        ret

strlenASM:
        xor rcx, rcx
strloop:
        cmp byte[rdi + rcx], 0      ;compare first char in string to 0, if equal, done counting
        je strdone
        inc rcx                     ;else, inc and loop again
        jmp strloop
strdone:
        mov rax, rcx                ;return string length
        ret

printASM:
        ;calls a c routine to print stats of message
        mov word[index], 0          ;clear index

        ;Asks user to choose a string
        mov rax, 1
        mov rdi, 1
        mov rsi, choosestring
        mov rdx, choosestringlen
        syscall

        ;save user chosen index in variable
        mov rax, 0
        mov rdi, 0
        mov rsi, index
        mov rdx, 2
        syscall

        call convertASCII

        ;bounds check the index
        cmp rax, -1
        je invalidinput
        cmp rax, 9
        jg invalidinput

        ;prep and call printStats
        push rbp
        mov rdi, stringArray
        mov rsi, rax
        call printStats
        pop rbp

        mov rax, 1
        mov rdi, 1
        mov rsi, newline
        mov rdx, newlinelen
        syscall

        ret

quitASM:
        xor r9, r9
loopfree:
        mov rdi, [stringArray + r9*8]   ;move array at curr location into rdi
        cmp rdi, originalmessage        ;if it's the original message, I don't need to free
        je skipfree
        call freeUp                     ;otherwise, free the memory
skipfree:
        add r9, 1                       ;inc r9
        cmp r9, 10                      ;if r9 != 10, check next array index
        jl loopfree

        ;print goobye message
        mov rax, 1
        mov rdi, 1
        mov rsi, bye
        mov rdx, byelen
        syscall

        ;exit call
        mov rax, 60
        xor rdi, rdi
        syscall