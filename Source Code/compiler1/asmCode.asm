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


L1:
mov esp, ebp
pop ebp
RET
_main ENDP

END
