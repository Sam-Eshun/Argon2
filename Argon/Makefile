# Compile everything
all: deps b2_test enc_test mat_test argon2 test 

# Compile dependencies
deps:
	gcc -o Blake2b.o -c Blake2b.c   
	gcc -o Argon2Encoding.o -c Argon2Encoding.c    
	gcc -o Argon2_MAT.o -c Argon2_MAT.c    
	gcc -o argon2.o -c argon2.c 
	
		
argon2: deps
	gcc Argonmain.c -lm Blake2b.o argon2.o Argon2Encoding.o Argon2_MAT.o -o Argonmain 
	
b2_test: deps
	gcc Blake2testcase.c Blake2b.o -o Blake2testcase   

enc_test: deps
	gcc Argon2Encoding_testcase.c -lm Blake2b.o Argon2Encoding.o -o Argon2Encoding_testcase  

mat_test: deps
	gcc Argon2_Mat_testcase.c -lm Blake2b.o Argon2Encoding.o Argon2_MAT.o -o Argon2_Mat_testcase 
	
test: deps
	gcc Argon2test.c -lm Blake2b.o argon2.o Argon2Encoding.o Argon2_MAT.o -o Argon2test
# Clean leftovers
clean:
	rm *.o >> /dev/null