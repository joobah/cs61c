.globl classify

.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
    li t0, 5
    bne a0, t0, arg_error

    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp) #argv
    sw s1, 8(sp) # silent mode
    sw s2, 12(sp) # m0 rows ptr
    sw s3, 16(sp) # m0 cols ptr
    sw s4, 20(sp) # m1 rows ptr
    sw s5, 24(sp) # m2 cols ptr
    sw s6, 28(sp) # input rows ptr
    sw s7, 32(sp) # input cols ptr
    sw s8, 36(sp) # m0 ptr
    sw s9, 40(sp) # m1 ptr
    sw s10, 44(sp) # input ptr
    sw s11, 48(sp) # h ptr

    mv s0, a1 
    mv s1, a2

    # malloc rows + cols
    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s2, a0

    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s3, a0

    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s4, a0

    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s5, a0

    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s6, a0

    li a0, 4
    jal ra, malloc 
    beqz a0, malloc_error
    mv s7, a0

    # Read pretrained m0
    lw a0, 4(s0)
    mv a1, s2
    mv a2, s3
    jal ra, read_matrix
    mv s8, a0

    # Read pretrained m1
    lw a0, 8(s0)
    mv a1, s4
    mv a2, s5
    jal ra, read_matrix
    mv s9, a0

    # Read input matrix
    lw a0, 12(s0)
    mv a1, s6
    mv a2, s7
    jal ra, read_matrix
    mv s10, a0

    # free all row col pointers
    lw t0, 0(s2)
    mv a0, s2
    mv s2, t0
    jal ra, free

    lw t0, 0(s3)
    mv a0, s3
    mv s3, t0
    jal ra, free

    lw t0, 0(s4)
    mv a0, s4
    mv s4, t0
    jal ra, free

    lw t0, 0(s5)
    mv a0, s5
    mv s5, t0
    jal ra, free

    lw t0, 0(s6)
    mv a0, s6
    mv s6, t0
    jal ra, free

    lw t0, 0(s7)
    mv a0, s7
    mv s7, t0
    jal ra, free

    # malloc h (m0 x input)
    mul t0, s2, s7
    li t1, 4
    mul a0, t0, t1
    jal ra, malloc
    beqz a0, malloc_error
    mv s11, a0

    # Compute h = matmul(m0, input)
    mv a0, s8
    mv a1, s2
    mv a2, s3
    mv a3, s10
    mv a4, s6
    mv a5, s7
    mv a6, s11
    jal ra, matmul

    # Compute h = relu(h)
    mv a0, s11
    mul a1, s2, s7
    jal ra, relu

    # free m0 since no longer needed
    mv a0, s8
    jal ra, free

    #malloc o (m1 x h)
    mul t0, s4, s7
    li t1, 4
    mul a0, t0, t1
    jal ra, malloc
    beqz a0, malloc_error
    mv s8, a0

    # Compute o = matmul(m1, h)
    mv a0, s9
    mv a1, s4
    mv a2, s5
    mv a3, s11
    mv a4, s2
    mv a5, s7
    mv a6, s8
    jal ra, matmul

    # Write output matrix o
    lw a0, 16(s0)
    mv a1, s8
    mv a2, s4
    mv a3, s7
    jal ra, write_matrix

    # Compute and return argmax(o)
    mv a0, s8
    mul a1, s4, s7
    jal ra, argmax
    mv s0, a0 # save result since we need to free

    # If enabled, print argmax(o) and newline
    li t0, 1
    beq t0, s1, end
    jal ra, print_int
    li a0, '\n'
    jal ra, print_char

end:
    # Free everything
    mv a0, s9
    jal ra, free

    mv a0, s10
    jal ra, free

    mv a0, s11
    jal ra, free

    # Assign result back to a0
    mv a0, s0

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp) 
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp) 
    lw s10, 44(sp) 
    lw s11, 48(sp)
    addi sp, sp, 52

    jr ra

malloc_error:
    li a0, 26
    j exit

arg_error:
    li a0, 31
    j exit 