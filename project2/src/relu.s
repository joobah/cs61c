.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue

    li t0, 1
    blt a1, t0, error 

    add t0, a0, zero          # array memory location  
    li t1, 0                  # array index

loop_start:
    beq t1, a1 loop_end
    lw t2, 0(t0)
    bge t2, zero loop_continue
    sw zero, 0(t0)

loop_continue:
    addi t0, t0, 4
    addi t1, t1, 1
    j loop_start

loop_end:


    # Epilogue


    jr ra

error:
    li a0, 36
    j exit

    
