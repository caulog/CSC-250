# @author Olivia Caulfield {@literal caulog20@wfu.edu}
# @date Dec 1, 2021
# @assignment Lab 8
# @file print_sum.s
# @course CSC 250
#
# This program reads a sequence of integers until a negative
# value is entered and displays the current maximum
#
# Compile and run (Linux)
#   gcc -no-pie print_sum.s && ./a.out


.text
   .global main               # use main if using C library


main:
   push %rbp                  # save the old frame
   mov  %rsp, %rbp            # create a new frame  

   sub  $16, %rsp             # make some space on the stack (stack alignment)

   # prompt the user
   mov  $prompt_format, %rdi  # first printf argument, format string  
   xor  %rax, %rax            # zero out rax  
   call printf                # printf

   # read the value
   mov  $read_format, %rdi    # first scanf argument, format string 
   lea  -8(%rbp), %rsi        # second scanf argument, memory address
   xor  %rax, %rax            # zero out rax
   call scanf                 # scanf

   jmp .L2                    # jumps to middle of "while loop"

   .L1:
   # prompt the user
   mov  $prompt_format, %rdi  # first printf argument, format string  
   xor  %rax, %rax            # zero out rax  
   call printf                # printf

   # read the value
   mov  $read_format, %rdi    # first scanf argument, format string 
   lea  -8(%rbp), %rsi        # second scanf argument, memory address
   xor  %rax, %rax            # zero out rax
   call scanf                 # scanf
   jmp .L2

.L2:
   cmp $0, -8(%rbp)           # checks if integer is negative
   jl .L3                     # if negative jump to end
   add -8(%rbp), %rbx         # else adds integer to total sum
   
   # print to the screen
   mov  $write_format, %rdi   # first printf argument, format string  
   mov %rbx, %rsi             # second printf argument, the integer  
   xor  %rax, %rax            # zero out rax  
   call printf                # printf
   jmp .L1                    # loop back to ask for another integer

.L3:
   add  $16, %rsp             # release stack space
   pop  %rbp                  # restore old frame
   ret                        # return to C library to end

.data

read_format:
   .asciz  "%d"

prompt_format:
   .asciz  "Enter an integer (negative to quit) -> "

write_format:
   .asciz  "Current sum is: %d \n"



