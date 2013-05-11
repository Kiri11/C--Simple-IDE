.386P
.MODEL FLAT
INCLUDELIB LIBCMT
EXTRN _printf : NEAR
.code


;--------------- Function main Declaration ---------------
PUBLIC _main
_main PROC
push ebp
mov ebp, esp

sub esp, 4
push 2d

;--------------- Assignment ---------------
pop eax
mov [ebp-4], eax
push eax
pop eax
.data
S0 BYTE "%d", 00h
.code
push OFFSET S0
push [ebp-4]

;--------------- Reversing last 2 stack items ---------------
.data
stackReveseVar0 DWORD ?
stackReveseVar1 DWORD ?
.code
pop stackReveseVar0
pop stackReveseVar1
push stackReveseVar0
push stackReveseVar1

;--------------- Call printf Function ---------------
CALL _printf
add esp, 8d
push eax
pop eax
sub esp, 4
.data
S1 BYTE "%d", 00h
.code
push OFFSET S1
push [ebp-4]

;--------------- Reversing last 2 stack items ---------------
.data
stackReveseVar2 DWORD ?
stackReveseVar3 DWORD ?
.code
pop stackReveseVar2
pop stackReveseVar3
push stackReveseVar2
push stackReveseVar3

;--------------- Call printf Function ---------------
CALL _printf
add esp, 8d
push eax
pop eax

L1:
mov esp, ebp
pop ebp
RET
_main ENDP

END
