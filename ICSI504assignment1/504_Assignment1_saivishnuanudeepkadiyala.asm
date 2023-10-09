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
		la $t0, arr

		#calc of length: 
    	la $t1, arr_end
    	subu $t1, $t1, $t0
    	li $t2, 4
    	divu $t1, $t1, $t2

		#parameter preparation	
    	move $a0, $t0
		move $a1, $t1
    		
		jal getMax
		move $a0, $v0
		li $v0, 1
		syscall
    	
		# Exit call
    	li $v0, 10
    	syscall
	
	#void radixSort(int arr[], int n)
	radixsort: 	
		move $t0, $a0
		move $t1, $a1
		jal getMax

		#this is the max value in the array
		move $t2, $v0
		
		#loop variables
		li $t3, 1

		radixSortloop:
			divu $t4, $t2, $t3
			beq $t4, 0, endloop_radixsort
			
			# we call callsort
			move $a0, $t0
			move $a1, $t1
			move $a2, $t3
			jal countSort
			
			#increment loop variable
			mul $t3, $t3, 10
			j radixSortloop

		endloop_radixsort:
			jr $ra

	#void countSort(int arr[], int n, int exp)
	countSort:
		move $t0, $a0
		move $t1, $a1
		move $t2, $a2

		#we create a new array output[n]
		li $v0, 9
		mul $a0, $t1, 4 #passing size in bytes  
		syscall
		#address of new memory is in $v0
		move $t3, $v0

		li $t4, 0 #loop variable
		#count[10]
		li $v0, 9
		li $t5, 4
		li $t6, 10
		mul $a0, $t5, $t6 #passing size in bytes
		syscall

		#address of new memory is in $v0
		move $t5, $v0

		#temp to fill count[n]
		initialloop:
			sw $zero, 0($t5)
			addi $t5, $t5, 4
			addi $t4, $t4, -1
			bnez $t6, initialloop
		
		loop1:
			lw $t6, 0($t0) #load element from arr
			divu $t6, $t6, $t2 #div it with exp
			div $t6, $t6, 10 #mod it with 10
			mfhi $t6 #store the result in t6
			
			add $t7, $t6, $t5 #add t6 with t5 to get the address of count
			
			lw $t8, 0($t7) #load the value from count
			addi $t8, $t8, 1 #increment the value
			sw $t8, 0($t7) #store the value in count

			addi $t4, $t4, 1 #increment loop variable
			addi $t0, $t0, 4 #increment arr pointer

			bne $t4, $t1, loop1 #loop1
		
		li $t4, 1 #loop variable

		
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
		
		beq $a1, $a2, return_fromprint
		
		#print the element here.
		lw $a0, 0($t0) 
		li $v0, 1
		syscall
		
		#for new line after number 
		la $a0, nextline
		li $v0, 4
		syscall
		
		addi $a1, $a1, 1
		addi $t0, $t0, 4  
		
		move $a0, $t0
		jal printData
		
	return_fromprint: 
		jr $ra
