bits 32
global sys_call_isr

extern sys_call
sys_call_isr:
    ; CPU already pushes EFLAGS, CS, and EIP automatically.

    ; Push general purpose registers in reverse order
    ; struct order: eax, ebx, ecx, edx, esi, edi, ebp, esp
    ; So push: ebp, edi, esi, edx, ecx, ebx, eax
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    ; Push segment registers in reverse order of context struct
    ; struct order: cs, ds, es, fs, gs, ss
    ; But CS is already pushed by CPU, so push: ss, gs, fs, es, ds
    push ss
    push gs
    push fs
    push es
    push ds

    ; Push ESP as parameter - it points to the context struct we just built
    push esp
    
    ; Call C function
    call sys_call

    ; EAX now contains pointer to the new context
    ; Set ESP to point to new context
    mov esp, eax
    
    ; Pop registers from NEW context in order of struct
    pop ds
    pop es
    pop fs
    pop gs
    pop ss

    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp

    ; CPU will pop EIP, CS, EFLAGS
    iret