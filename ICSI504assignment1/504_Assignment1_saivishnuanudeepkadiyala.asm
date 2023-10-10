.data

str: .asciiz "504 assignment 1.\n"
arr: .word 8, 9, 3, 4, 7, 2, 5, 6, 1
nextline: .asciiz " \n"
arr_end: 

.text

# Start mark for assembler.
.globl main 

	# int main() method work
	main: 
		sub $sp, $sp, 8 #for saving registers
		la $t0, arr
		sw $t0, 0($sp) #for arr saving
		
		#calc of length: 
    	la $t1, arr_end #t1 = arr_end	
    	sub $t1, $t1, $t0 #t1 = arr_end - arr
    	li $t2, 4 #t2 = 4
    	div $t1, $t1, $t2 #t1 = (arr_end - arr)/4

		sw $t1, 4($sp) #for size
		
		#parameter preparation	
    	move $a0, $t0 
		move $a1, $t1
		
		jal radixsort
		
		# parameter preparation
		lw $t0, 0($sp) #for arr
		lw $t1, 4($sp) #for size
		move $a0, $t0 
		move $a1, $zero 
		move $a2, $t1

		jal printData

		jal return_formain
	
	return_formain:
		add $sp, $sp, 8 #deallocation of stack
		
		# Exit call
    	li $v0, 10
    	syscall
		
	#void radixSort(int arr[], int n)
	radixsort: 	
		sub $sp, $sp, 24 #for saving registers
		sw $ra, 0($sp) #for address where it came from
		sw $a0, 4($sp) #for arr
		sw $a1, 8($sp) #for size
		
		#we call getMax
		lw $a0, 4($sp) #passing arr
		lw $a1, 8($sp) #passing size
		jal getMax

		#this is the max value in the array
		sw $v0, 12($sp) #max value

		# #loop variables
		li $t3, 1 # i = 1
		sw $t3, 16($sp) #loop variable
		
		radixSortloop:
			lw $t3, 16($sp) #loop variable
			lw $t2, 12($sp) #max value
			div $t2, $t3 # max/exp
			mflo $t4 # max/exp
			beq $t4, $zero, endloop_radixsort #is equal to 0 then we are done
			
			# we call callsort
			lw $t0, 4($sp) #arr
			lw $t1, 8($sp) #size
			move $a0, $t0 #passing arr
			move $a1, $t1 #passing size
			move $a2, $t3 #passing exp
			jal countSort #call countSort

			lw $t3, 16($sp) #loop variable
			#increment loop variable
			li $t4, 10 #loop increment
			mul $t3, $t3, $t4 # exp = exp * 10
			sw $t3, 16($sp) #loop variable = exp

			j radixSortloop #loop again

		endloop_radixsort:
			lw $ra, ($sp) #for address where it came from
			add $sp, $sp, 24 #deallocation of stack
			jr $ra #return to main

	#void countSort(int arr[], int n, int exp)
	countSort:
		sub $sp, $sp, 20 #for saving registers
		sw $a0, 0($sp) #arr
		sw $a1, 4($sp) #for size
		sw $a2, 8($sp) #for exp

		#create output[size]
		lw $t0, 4($sp) #size
		li $v0, 9 #sbrk
		mul $t0, $t0, 4 #size * 4
		move $a0, $t0 #size of output
		syscall
		sw $v0, 12($sp) #output

		#loop variable i 
		li $t0, 0 #i = 0
		
		#count[10]
		li $t1, 10 #count size
		li $v0, 9 #sbrk
		mul $t1, $t1, 4 #count size * 4
		move $a0, $t1 #size of count
		syscall
		sw $v0, 16($sp) #count

		#count[10] to 0
		lw $t1, 16($sp) #count
		li $t2, 0 #count = 0
		initialloop:
			li $t3, 0
			sw $t3, 0($t1) #count[i] = 0
			addi $t1, $t1, 4 #count++
			addi $t2, $t2, 1 #i++
			bne $t2, 10, initialloop #i != 10 then loop again

		#loop 1 work
		lw $t1, 0($sp) #arr
		lw $t2, 4($sp) #size
		lw $t3, 8($sp) #exp
		lw $t4, 16($sp) #count
		loop1:
			lw $t5, 0($t1) #arr[i]
			div $t5, $t3 #arr[i]/exp
			mflo $t6 #arr[i]/exp
			li $t5, 10
			div $t6, $t5 #arr[i]/exp/10
			#arr[i]/exp % 10
			mfhi $t6 #arr[i]/exp % 10
			#count[arr[i]/exp % 10]++
			sll $t5, $t6, 2 #arr[i]/exp % 10 * 4
			add $t5, $t5, $t4 #arr[i]/exp % 10 * 4 + count
			lw $t6, 0($t5) #count[arr[i]/exp % 10]
			addi $t6, $t6, 1 #count[arr[i]/exp % 10]++
			sw $t6, 0($t5) #count[arr[i]/exp % 10] = count[arr[i]/exp % 10]++
			
			addi $t1, $t1, 4 #arr++
			addi $t0, $t0, 1 #i++
			bne $t0, $t2, loop1 #i != size then loop again
		
		#loop 2
		li $t0, 1 #i = 1
		lw $t1, 16($sp) #count
		li $t2, 10 #count size
		addi $t1, $t1, 4
		loop2: 
			lw $t3, 0($t1) #count[i]
			sub $t1, $t1, 4 #count[i-1]
			lw $t4, 0($t1) #count[i-1]
			add $t4, $t4, $t3 #count[i-1] + count[i]
			addi $t1, $t1, 4 #count[i]
			sw $t4, 0($t1) #count[i] = count[i-1] + count[i]
			addi $t0, $t0, 1 #i++
			addi $t1, $t1, 4 #count++
			bne $t0, $t2, loop2 #i != 10 then loop again
		
		#loop 3 work 
		lw $t0, 4($sp) #size
		sub $t0, $t0, 1 #size--
		lw $t1, 0($sp) #arr
		lw $t2, 8($sp) #exp
		lw $t3, 12($sp) #output
		lw $t4, 16($sp) #count
		loop3:
			lw $t5, 0($t1) #arr[i]
			div $t5, $t2 #arr[i]/exp
			mflo $t6 #arr[i]/exp
			li $t5, 10
			div $t6, $t5 #arr[i]/exp/10
			#arr[i]/exp % 10
			mfhi $t6 #arr[i]/exp % 10
			
			sll $t7, $t6, 2 #arr[i]/exp % 10* 4
			add $t7, $t7, $t4 #arr[i]/exp % 10 * 4 + count
			lw $t8, 0($t7) #count[arr[i]/exp % 10--]
			sub $t8, $t8, 1 #count[arr[i]/exp % 10--]--
			sll $t8, $t8, 2 #count[arr[i]/exp % 10--]-- * 4
			add $t8, $t8, $t3 #count[arr[i]/exp % 10--]-- * 4 + output
			
			lw $t5, 0($t1) #arr[i]
			sw $t5, 0($t8) #output[count[arr[i]/exp % 10--]--] = arr[i]

			sll $t6, $t6, 2 #arr[i]/exp % 10 * 4
			add $t6, $t6, $t4 #arr[i]/exp % 10 * 4 + count
			lw $t7, 0($t6) #count[arr[i]/exp % 10]
			sub $t7, $t7, 1 #count[arr[i]/exp % 10]--
			sw $t7, 0($t6) #count[arr[i]/exp % 10] = count[arr[i]/exp % 10]--

			addi $t1, $t1, 4 #arr++
			addi $t0, $t0, -1 #size--
			bgez $t0, loop3 #size >= 0 then loop again
		
		#loop 4 work
		li $t0 , 0 #i = 0
		lw $t1, 0($sp) #arr
		lw $t2, 4($sp) #size
		lw $t3, 12($sp) #output
		loop4:
			lw $t4, 0($t3) #output[i]
			sw $t4, 0($t1) #arr[i] = output[i]
			addi $t0, $t0, 1 #i++
			addi $t1, $t1, 4 #arr++
			addi $t3, $t3, 4 #output++
			bne $t0, $t2, loop4 #i != size then loop again 

		#deallocation of stack
		add $sp, $sp, 20
		jr $ra

	#int getMax(int arr[], int n)
	getMax:
		move $t0, $a0
		lw $t1, 0($t0)
		li $t2, 1
		move $t3, $a1
		# subu $t3,$t3, 1
		
		getMaxloop:
			beq $t2, $t3, endloop
			
			addi $t0, $t0, 4			
			lw $t4, 0($t0)
			
			bgt $t4, $t1, greaterfound

			addi $t2,$t2,1
			j getMaxloop
		greaterfound: 
			move $t1, $t4
			addi $t2, $t2, 1
			j getMaxloop 
		endloop:
			move $v0, $t1
			jr $ra

	#void printData(int arr[], int start, int len)
	printData: 
		move $t0, $a0
		bge $a1, $a2 , return_fromprint
		
		#print the element here.
		lw $a0, 0($t0) 
		li $v0, 1
		syscall
		
		#for new line after number 
		la $a0, nextline
		li $v0, 4
		syscall
		
		addi $t0, $t0, 4  
		move $a0, $t0
		
		beq $a1, $zero, store_return_address
		
		addi $a1, $a1, 1
		jal printData

		store_return_address:
			sub $sp, $sp, 4
			sw $ra, 0($sp)
			addi $a1, $a1, 1
			jal printData 
		
	return_fromprint: 
		lw $ra, 0($sp)
		jr $ra
