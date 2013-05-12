.386P
.MODEL FLAT
INCLUDELIB LIBCMT
EXTRN _printf : NEAR
.code


;--------------- Function mofunc Declaration ---------------
PUBLIC _mofunc
_mofunc PROC
push ebp
mov ebp, esp

push [ebp+8]
push 1d

;--------------- sub ---------------
pop ecx
pop eax
sub eax, ecx
push eax

;--------------- Return ---------------
pop eax
JMP L1

L1:
mov esp, ebp
pop ebp
RET
_mofunc ENDP

;--------------- Function main Declaration ---------------
PUBLIC _main
_main PROC
push ebp
mov ebp, esp

sub esp, 4
push 1d

;--------------- Assignment ---------------
pop eax
mov [ebp-4], eax
push eax
pop eax
.data
S0 BYTE "%d mod", 0Ah, "", 00h
.code
push OFFSET S0
push [ebp-4]
push [ebp-4]

;--------------- Multiply ---------------
pop ecx
pop eax
imul ecx
push eax
push 1d

;--------------- add ---------------
pop ecx
pop eax
add eax, ecx
push eax

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
push 0d

;--------------- Return ---------------
pop eax
JMP L2

L2:
mov esp, ebp
pop ebp
RET
_main ENDP

END
