.LC0:
  .string "%d %d\n" ; STRING INITIASATION
main:
  lea ecx, [esp+4] ; computes the effetive address of the second operand (source operand)
  ; and stores in first
  and esp, -16 ; bitwise operation, zeros 4 least significant bits
  push DWORD PTR [ecx-4] ;decrement stack pointer pushes into the stack
  push ebp ;decrement stack pointer and pushes ebp into the stack
  mov ebp, esp ;ebp <- esp
  push ecx ;decrement stack pointer and pushes ebp into the stack
  sub esp, 20 ; esp = esp - 20
  mov DWORD PTR [ebp-24], 0 ; ebp-24 = 0
  mov DWORD PTR [ebp-12], 3 ; ebp-12 = 3
.L8:
  cmp DWORD PTR [ebp-12], 9999 ;compares ebp-12 to 9999
  jg .L2 ;jump if grater, checks EFLAGS(ZF = 0 (zero flag) and SF = 0F (sign flag)) //for exit 
  mov DWORD PTR [ebp-20], 0
  mov DWORD PTR [ebp-16], 2
.L5:
  mov eax, DWORD PTR [ebp-16]
  cmp eax, DWORD PTR [ebp-12]
  jge .L3
  mov eax, DWORD PTR [ebp-12]
  cdq
  idiv DWORD PTR [ebp-16]
  mov eax, edx
  test eax, eax
  jne .L4
  add DWORD PTR [ebp-20], 1
.L4:
  add DWORD PTR [ebp-16], 1
  jmp .L5
.L3:
  cmp DWORD PTR [ebp-20], 0
  jne .L6
  mov eax, DWORD PTR [ebp-24]
  add eax, 2
  cmp DWORD PTR [ebp-12], eax
  jne .L7
  sub esp, 4
  push DWORD PTR [ebp-12]
  push DWORD PTR [ebp-24]
  push OFFSET FLAT:.LC0
  call printf
  add esp, 16
.L7:
  mov eax, DWORD PTR [ebp-12]
  mov DWORD PTR [ebp-24], eax
.L6:
  add DWORD PTR [ebp-12], 1
  jmp .L8
.L2:
  mov eax, 0
  mov ecx, DWORD PTR [ebp-4]
  leave
  lea esp, [ecx-4]
  ret