# Pthreads – Merge Sort & Matrix Multiplication

## Part 1: Merge Sort
- **Algorithm:** Divide-and-conquer, O(n log n).  
- **Threads:** Each half of the array is sorted by **two separate threads** recursively.  The main thread is responsible for merging the sorted sub-arrays.
- **Input:** Read from a file named `input`.  
- **Output:** Sorted array.

**Run test cases:**

![Merge Sort Step 2](images/image.png)  
![Merge Sort Step 1](images/image1.png)  
![Merge Sort Step 2](images/image4.png)  

---

## Part 2: Matrix Multiplication
- **Variations:**
  1. **One thread per element** – each thread calculates a single element of the result matrix.  
  2. **One thread per row** – each thread calculates a full row of the result matrix.  
- **Input:** Read two matrices from a file named `matrices_input.txt`.  
- **Output:** Result matrix and total time taken for each method.

**Run test cases:**

![Matrix Multiplication Threads](images/image2.png)  
![Matrix Multiplication Result](images/image.png)  

---

## Compile & Run
```bash
make
./merge_sort
./matrix_mul
