call Lib\vcvars32.bat
call asm32m asmCode
echo Output:
asmCode.exe
pause
del asmCode.ilk
del asmCode.obj
del asmCode.pdb
del asmCode.exe