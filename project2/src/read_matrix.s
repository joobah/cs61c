.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
#   - If malloc returns an error,
#     this function terminates the program with error code 26
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fread error or eof,
#     this function terminates the program with error code 29
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -24
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)

    mv s0, a1 # row num pointer
    mv s1, a2 # col num pointer 

    # open file
    li a1, 0
    jal ra, fopen
    bltz a0 fopen_error

    mv s2, a0 # file descriptor

    # read rows
    mv a1, s0
    li a2, 4
    jal ra, fread
    li t0, 4
    bne a0, t0 fread_error

    # read cols
    mv a0, s2
    mv a1, s1
    li a2, 4
    jal ra, fread
    li t0, 4
    bne a0, t0 fread_error

    lw a0, 0(s0)
    lw t0, 0(s1)
    mul a0, a0, t0
    li t0, 4
    mul a0, a0, t0
    mv s3, a0 # matrix length in bytes

    # malloc
    jal ra, malloc
    beqz a0, malloc_error

    mv s4, a0 # matrix pointer

    # read matrix
    mv a0, s2
    mv a1, s4
    mv a2, s3
    jal ra, fread
    bne a0, s3 fread_error

    # close file
    mv a0, s2
    jal ra, fclose
    bltz a0 fclose_error

    # populate return values
    mv a0, s4

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    addi sp, sp, 24

    jr ra

malloc_error:
    li a0, 26
    j exit

fopen_error:
    li a0, 27
    j exit

fclose_error:
    li a0, 28
    j exit

fread_error:
    li a0, 29
    j exit