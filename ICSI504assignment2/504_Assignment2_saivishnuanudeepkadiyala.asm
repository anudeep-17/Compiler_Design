.data
prompt: .asciiz "Enter a string to post fix it: "
input_buffer:   .space 64 # 64 bytes for input buffer
output_buffer:  .space 64 # 64 bytes for output buffer
outputvalue:   .space 4  # 4 bytes for output value
leftparenthesis: .ascii "("
rightparenthesis: .ascii ")"
plus: .ascii "+"
minus: .ascii "-"
newline: .asciiz "\n"

.text
.globl main

    main:
        # print prompt
        li $v0, 4
        la $a0, prompt
        syscall
        
        # read input
        li $v0, 8
        la $a0, input_buffer
        li $a1, 64
        syscall

        # # print input
        # li $v0, 4
        # la $a0, input_buffer
        # syscall

        la $a0, input_buffer # move input_buffer to $a0
        jal convertopostfix # jump to convertopostfix
        
        # exit
        li $v0, 10
        syscall

    convertopostfix:
        sub $sp, $sp, 68 # allocate 68 bytes on stack
        move $t0, $a0 # move input_buffer to $t0
        sw $ra, 0($sp) # save return address
        sub $sp, $sp, 4 # allocate 4 bytes on stack
        la $t8, output_buffer # load output_buffer to $t8

        read_each_char:

            lb $t1, 0($t0) # load first char of input_buffer to $t1
            
            li $t2, 10 # load 10 to $t2
            beq $t1, $t2, endofstring # if $t1 == 10 => newline in ascii, exit loop

            sb $t1, 0($sp) # store $t1 to output_buffer

            la $a0, 0($sp) # load first char of output_buffer to $a0
            jal isdigit
            
            beq $v0, -1, checkforparenthesis # if $v0 == -1 => not a digit, check for parenthesis
            
            sb $v0, 0($t8) # store $v0 to output_buffer

            li $v0, 1
            lb $a0, 0($t8)
            syscall

            addi $t8, $t8, 1 # increment output_buffer pointer
            addi $t0, $t0, 1 # increment input_buffer pointer
            j read_each_char # jump to convertopostfix

            checkforparenthesis:
                la $a0, 0($sp)
                jal isparenthesis

                beq $v0, -1, checkoperator # if $v0 == -1 => not a parenthesis, check for operator
                
                move $t1, $v0 # move $v0 to $t1
                li $v0, 1
                move $a0, $t1
                syscall

                addi $t0, $t0, 1 # increment input_buffer pointer
                j read_each_char # jump to read_each_char
            
            checkoperator:
                la $a0, 0($sp)
                move $a1, $t8
                jal isoperator

                beq $v0, -1, endofstring # if $v0 == -1 => not a parenthesis, check for operator

                beq $v0, 0, isplus # if $v0 == 0 => plus, jump to isplus
                beq $v0, 1, isminus # if $v0 == 1 => minus, jump to isminus


                isplus:
                    lb $v0, plus # load plus to $v0
                    sb $v0, 0($t8) # store plus to output_buffer

                    li $v0, 11,
                    lb $a0, 0($t8)
                    syscall

                    addi $t8, $t8, 1 # increment output_buffer pointer
                    addi $t0, $t0, 1 # increment input_buffer pointer
                    j read_each_char # jump to read_each_char

                isminus:
                    lb $v0, minus # load plus to $v0
                    sb $v0, 0($t8) # store plus to output_buffer

                    li $v0, 11,
                    lb $a0, 0($t8)
                    syscall
                    
                    addi $t8, $t8, 1 # increment output_buffer pointer
                    addi $t0, $t0, 1 # increment input_buffer pointer
                    j read_each_char # jump to read_each_char

        endofstring:
            #return 
            addi $sp, $sp, 4 # deallocate 4 bytes on stack
            lw $ra, 0($sp) # load return address
            add $sp, $sp, 68 # deallocate 68 bytes on stack
            jr $ra # jump to return address


    isdigit:
        lb $t1, 0($a0) # load first char of output_buffer to $t1
        li $t2, 48 # load 48 => ascii(0) to $t1
        blt $t1, $t2, return_notdigit # if $t1 < 0 => not a digit, jump to isoperator
        li $t2, 57 # load 57 => ascii(9) to $t1
        bgt $t1, $t2, return_notdigit # if $t1 > 9 => not a digit, jump to isoperator

        # if $t1 is a digit, store it to output_buffer
        li $t2, 48 # load 48 => ascii(0) to $t1
        sub $t1, $t1, $t2 # convert ascii to digit
        move $v0, $t1 
        jr $ra # jump to return address

        return_notdigit:
            # if $t1 is not a digit, jump to isoperator
            li $v0, -1
            jr $ra # jump to return address
    
    isparenthesis:
        lb $t1, 0($a0) # load first char of output_buffer to $t1
        lb $t2, leftparenthesis # load leftparenthesis to $t2
        bne $t1, $t2, checkforrightparenthesis # if $t1 != $t2 => not leftparenthesis, jump to checkforrightparenthesis
        
        li $v0, 0 # if $t1 == $t2 => leftparenthesis
        jr $ra # jump to return address

        checkforrightparenthesis:
            lb $t2, rightparenthesis # load rightparenthesis to $t2
            bne $t1, $t2, return_notparenthesis # if $t1 != $t2 => not rightparenthesis 

            li $v0, 1 # if $t1 == $t2 => rightparenthesis
            jr $ra # jump to return address

        return_notparenthesis:
            li $v0, -1
            jr $ra # jump to return address
    
    isoperator:
        lb $t1, 0($a0) # load first char of output_buffer to $t1
        lb, $t2, plus # load plus to $t2
        bne $t1, $t2, checkforminus # if $t1 != $t2 => not plus, jump to checkforminus

        li $v0, 0 # if $t1 == $t2 => plus
        jr $ra # jump to return address
        
        checkforminus:
            lb $t2, minus # load minus to $t2
            bne $t1, $t2, return_notoperator # if $t1 != $t2 => not minus
            li $v0, 1   # if $t1 == $t2 => minus
            jr $ra # jump to return address

        return_notoperator:
            li $v0, -1
            jr $ra # jump to return address