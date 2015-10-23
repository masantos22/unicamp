Minimize
  50 C0 + 50 C1 + 50 C2 + 50 C3 + 50 C4 + 50 C5 + 50 C6 + 50 C7 + 50 C8
   + 50 C9 + 50 C10 + 50 C11 + 0 C12 + 0 C15 + 0 C17 + 0 C18
Subject To
 R0: 50 C0 + 50 C1 + 50 C2 + 50 C3 + 50 C4 + 50 C5 <= 200
 R1: 50 C6 + 50 C7 + 50 C8 + 50 C9 + 50 C10 + 50 C11 <= 200
 R2: C0 + C6 <= 10
 R3: C1 + C7 <= 10
 R4: C2 + C8 <= 10
 R5: C3 + C9 <= 10
 R6: C4 + C10 <= 10
 R7: C5 + C11 <= 10
 R8: C0 + C1 + C5 = 1
 R9: C0 + C2 + C4 - 2 C13 = 0
 R10: C1 + C2 + C3 - 2 C14 = 0
 R11: C3 + C4 + C5 = 1
 R12: C6 + C7 + C11 - 2 C16 = 0
 R13: C6 + C8 + C10 = 1
 R14: C7 + C8 + C9 = 1
 R15: C9 + C10 + C11 - 2 C19 = 0
Bounds
Binaries
 C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 C10 C11 C12 C13 C14 C15 C16 C17 C18 C19
End
