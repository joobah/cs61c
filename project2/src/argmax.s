.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue

    li t0, 1
    blt a1, t0, error 

    addi t0, a0, 4            # current array memory location  
    li t1, 1                  # current index
    lw t2, 0(a0)              # current max
    li t3, 0                  # current max index

loop_start:
    beq t1, a1 loop_end
    lw t4, 0(t0)
    ble t4, t2 loop_continue
    add t2, t4, zero
    add t3, t1 zero

loop_continue:
    addi t0, t0, 4
    addi t1, t1, 1
    j loop_start

loop_end:
    # Epilogue

    add a0, t3, zero
    jr ra

error:
    li a0, 36
    j exit
