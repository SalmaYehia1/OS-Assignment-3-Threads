CC = gcc
CFLAGS = -Wall -pthread

all: matrix_mul merge_sort


merge_sort:
	$(CC) $(CFLAGS) -o merge_sort /home/salmay/Desktop/compu/TERMS/term7/OS/LABS/lab3/lab-4-threads-SalmaYehia1/Merge_sort.c

matrix_mul:
	$(CC) $(CFLAGS) -o matrix_mul /home/salmay/Desktop/compu/TERMS/term7/OS/LABS/lab3/lab-4-threads-SalmaYehia1/Matrix_Multiplication.c


clean:
	rm -f merge_sort matrix_mul
