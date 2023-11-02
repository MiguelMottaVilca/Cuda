#include <cuda.h>

// Kernel para realizar la suma de vectores en la GPU
__global__ void vectorAddKernel(float* d_A, float* d_B, float* d_C, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        d_C[idx] = d_A[idx] + d_B[idx];
    }
}

void vecAdd(float* A, float* B, float* C, int n) {
    int size = n * sizeof(float);
    float *d_A, *d_B, *d_C;

    // Alojar memoria en el dispositivo para d_A, d_B y d_C
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copiar los datos de A y B desde la CPU al dispositivo
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Definir el tamaño de bloque y la cantidad de bloques
    int threadsPerBlock = 256; // Puedes ajustar este valor según tus necesidades
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;

    // Lanzar el kernel en la GPU
    vectorAddKernel<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, n);

    // Copiar el resultado desde el dispositivo de vuelta a la CPU
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Liberar memoria en el dispositivo
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

int main() {
    int n = 1000; // Tamaño de los vectores
    float A[n], B[n], C[n];

    // Inicializar A y B con datos

    vecAdd(A, B, C, n);

    // C contiene el resultado de la suma en la CPU
    // Puedes imprimir C o realizar otras operaciones con él

    return 0;
}
