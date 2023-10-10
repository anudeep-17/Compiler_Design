.data

str: .asciiz "504 assignment 1.\n"
arr: .word 8, 9, 3, 4, 7, 2, 5, 6, 1
nextline: .asciiz " \n"
arr_end: 

.text

# Start mark for assembler.
.globl main 

	# int main() method work
	# parameter: arr with values to sort
	# return: void
	main: 
		sub $sp, $sp, 8 #for saving arr address and its length
		la $t0, arr
		sw $t0, 0($sp) #for arr saving
		
		#calc of length: 

    	la $t1, arr_end #t1 = arr_end	
    	sub $t1, $t1, $t0 #t1 = arr_end - arr -> (size of array)
    	li $t2, 4 #t2 = 4
    	div $t1, $t1, $t2 #t1 = (arr_end - arr)/4 -> (size of array)/(size of array[0])
		sw $t1, 4($sp) #saving size
		
		#parameter preparation	
    	move $a0, $t0 #passing arr 
		move $a1, $t1 #passing size
		
		jal radixsort
		
		# parameter preparation
		lw $t0, 0($sp) #for arr
		lw $t1, 4($sp) #for size
		move $a0, $t0 #passing arr 
		move $a1, $zero  #passing start
		move $a2, $t1 #passing size

		jal printData 

		jal return_formain 	
	
	# return (0) from main
	return_formain:
		add $sp, $sp, 8 #deallocation of stack
		# Exit call
    	li $v0, 10
    	syscall
		
	#void radixSort(int arr[], int n)
	#parameter: arr with values to sort, size of array, exp
	#return: void
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

		#loop variables
		li $t3, 1 # exp = 1
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
	#parameter: arr with values to sort, size of array, exp
	#return: void
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
		
		# creation of count[10]
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

		mul $t5, $t0, 4 #size * 4
		add $t1, $t1, $t5 #arr + size * 4

		loop3:
			lw $t5, 0($t1) #arr[i]
			div $t5, $t2 #arr[i]/exp
			mflo $t6 #arr[i]/exp
			li $t5, 10
			div $t6, $t5 #arr[i]/exp/10
			mfhi $t6 #arr[i]/exp % 10
			sll $t5, $t6, 2 #arr[i]/exp % 10 * 4
			add $t5, $t5, $t4 #arr[i]/exp % 10 * 4 + count
			lw $t6, 0($t5) #count[arr[i]/exp % 10]
			sub $t6, $t6, 1 #count[arr[i]/exp % 10]--
			sw $t6, 0($t5) #count[arr[i]/exp % 10] = count[arr[i]/exp % 10]--
			sll $t6, $t6, 2 #count[arr[i]/exp % 10] * 4
			add $t6, $t6, $t3 #count[arr[i]/exp % 10] * 4 + output
			
			lw $t5, 0($t1) #arr[i]
			sw $t5, 0($t6) #output[count[arr[i]/exp % 10]] = arr[i]
			
			sub $t1, $t1, 4 #arr--
			sub $t0, $t0, 1 #size--
			bge $t0, $zero, loop3 #size >= 0 then loop again

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

		#deallocation of stack values 
		add $sp, $sp, 20 
		jr $ra #return to radixsort

	#int getMax(int arr[], int n)
	#parameter: arr with values to sort, size of array
	#return: max value in the array
	getMax:
		move $t0, $a0 # stores arr address
		lw $t1, 0($t0) # stores first element of arr
		li $t2, 1 # stores loop variable
		move $t3, $a1 # stores size of arr

		getMaxloop: 
			beq $t2, $t3, endloop # if loop variable == size then end loop
			
			addi $t0, $t0, 4 # increment arr address		 
			lw $t4, 0($t0) # stores arr[i]
			
 			bgt $t4, $t1, greaterfound # if arr[i] > max then go to greaterfound

			addi $t2,$t2,1 # increment loop variable
			j getMaxloop # loop again
		greaterfound: 
			move $t1, $t4 # max = arr[i]
			addi $t2, $t2, 1 # increment loop variable
			j getMaxloop  
		endloop:
			move $v0, $t1 # return max
			jr $ra # return to radixsort

	#void printData(int arr[], int start, int len)
	#parameter: arr with values to sort, start of array, length of array
	#return: void

	printData: 
		move $t0, $a0 # passed arr address 
		bge $a1, $a2 , return_fromprint # if start >= len then return to radixsort
		
		#print the element here.
		lw $a0, 0($t0) 
		li $v0, 1 #
		syscall
		
		#for new line after number 
		la $a0, nextline
		li $v0, 4
		syscall
		
		addi $t0, $t0, 4  # increment arr address
		move $a0, $t0 # pass arr address
		
		beq $a1, $zero, store_return_address # if start == 0 then store return address
		
		addi $a1, $a1, 1
		jal printData

		store_return_address:
		# if the start is zero then we store where did we come from to make sure when we return we jump back right
			sub $sp, $sp, 4 #for saving return address
			sw $ra, 0($sp) #for return address
			addi $a1, $a1, 1 # increment start
			jal printData  # call printData
		
	return_fromprint: 
		lw $ra, 0($sp) #for return address
		jr $ra 	# return
