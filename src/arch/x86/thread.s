[GLOBAL switch_threads]

extern thread_stack_offset  ; offset of the stack member of struct thread
                            ; it is required so we can set esp to the 
                            ; stack pointer in the new thread.
                            ; defined in thread.c

switch_threads:
    push ebx    ; push registers onto the stack where they are expected
    push ebp
    push esi
    push edi

    mov edx, thread_stack_offset
    mov eax, [esp + 20]     ; move the cur member (first parameter to 
                            ; function switch_threads) into eax
    mov [eax + edx], esp    ; move esp (the stack pointer) into the 
                            ; current thread's stack member
    mov ecx, [esp + 24]     ; move the next member (second parameter to
                            ; function switch_threads) into ecx
    mov esp, [ecx + edx]    ; move the next thread's stack member into
                            ; esp (the stack pointer)

    pop edi     ; pop the new thread's registers off the stack
    pop esi
    pop ebp
    pop ebx

    ret

[GLOBAL switch_entry]

switch_entry:

    ret
