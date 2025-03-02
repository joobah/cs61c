.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

    # Prologue

    li t0, 1
    blt a2, t0, error_36
    blt a3, t0, error_37
    blt a4, t0, error_37

    li t0, 0                  # t0 = number of elements used so far
    li t1, 4                  # t1 = word size
    li t2, 0                  # running dot product

loop_start:
    beq t0, a2 loop_end

    mul t3, t0, t1
    mul t3, t3, a3
    add t3, t3, a0
    lw t3, 0(t3)

    mul t4, t0, t1
    mul t4, t4, a4
    add t4, t4, a1
    lw t4, 0(t4)

    mul t3, t3, t4
    add t2, t2, t3

    addi t0, t0, 1
    j loop_start

loop_end:

    # Epilogue

    add a0, t2, zero
    jr ra

error_36:
    li a0, 36
    j exit

error_37:
    li a0, 37
    j exit
