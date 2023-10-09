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
		
		move $a0 , $v0
		li $v0, 1
		syscall
	
    		# Exit call
    		li $v0, 10
    		syscall
	
	#void radixSort(int arr[], int n)
	radixsort: 	

	#void countSort(int arr[], int n, int exp)
	
	#int getMax(int arr[], int n)
	getMax:
		move $t0, $a0
		lw $t1, 0($t0)
		li $t2, 0
		move $t3, $a1
		subu $t3,$t3, 1
		
		loop:
			beq $t2, $t3, endloop
			
			addi $t0, $t0, 4			
			lw $t4, 0($t0)
			
			bgt $t4, $t1, greaterfound

			addi $t2,$t2,1
			j loop
		greaterfound: 
			move $t1, $t4
			addi $t2, $t2, 1
			j loop 
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
