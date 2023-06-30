# @author Olivia Caulfield {@literal caulog20@wfu.edu}
# @date Dec 1, 2021
# @assignment Lab 8
# @file max_two.s
# @course CSC 250
#
# This program reads two integers and displays the larger
#
# Compile and run (Linux)
#   gcc -no-pie max_two.s && ./a.out


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
   lea  -16(%rbp), %rdx       # third scanf argument, second memory address
   xor  %rax, %rax            # zero out rax
   call scanf                 # scanf

   # print to the screen integers
   mov  $write_format, %rdi   # first printf argument, format string  
   movslq -8(%rbp), %rsi      # second printf argument, the first integer  
   movslq -16(%rbp), %rdx     # third printf argument, the second integer

   call find_max              # calls function to find larger integer
   mov %rax, %rcx             # copies rax to rcx

   xor  %rax, %rax            # zero out rax 
   call printf                # printf

   add  $16, %rsp             # release stack space
   pop  %rbp                  # restore old frame
   ret                        # return to C library to end

.data

read_format:
   .asciz  "%d %d"

prompt_format:
   .asciz  "Enter two integers -> "

write_format:
   .asciz  "Entered %d and %d, the larger integer is %d\n"


find_max:
   push %rbp                  # saves old frame
   mov %rsp, %rbp             # copy the first integer to the %eax register
   xor %rax, %rax             # zero out rax
   mov %rsi, %rax             # copies rsi to rax 
   cmp %rdx, %rsi             # compare the two integers 
   jg .which                  # jumps if below
   mov %rdx, %rax             # copies rdx to rax

.which:
   pop %rbp                   # # restore old frame
   ret                        # return to C library to end




