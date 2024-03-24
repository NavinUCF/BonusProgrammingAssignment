//Navin Singh COP 3502C
//Bonus Assignment
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void swap(int *xp, int *yp){
    int temp =*xp;
    *xp = *yp;
    *yp = temp;
}

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	return;
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l<r){
    int m = (l+r)/2;
    //merge sorts both halves
    mergeSort(pData, l, m);
    mergeSort(pData, m+1, r);

    int i,j,k;
    //for each size
    int n1 = m-l+1;
    int n2 = r-m;
    //allocates memory for temporary arrays
    int *L = (int*)Alloc(n1*sizeof(int));
    int *R = (int*)Alloc(n2*sizeof(int));
    //puts data to arrays
    for(i=0;i<n1;i++)
      L[i] = pData[l+i];
    for(j=0;j<n2;j++)
      R[j] = pData[m+1+j];

    i=0;
    j=0;
    k=l;
    //merge the contents of temp arrays into the original array again
    while (i<n1 && j<n2)
    {
      if(L[i] <= R[j]){
        pData[k] = L[i];
        i++;
      }
      else{
        pData[k] = R[j];
        j++;
      }
      k++;
    }
    //for extra data
    while (i<n1)
    {
      pData[k] = L[i];
      i++;
      k++;
    }

    while (j<n2)
    {
      pData[k] = R[j];
      j++;
      k++;
    }

    //deallocates memory
    DeAlloc(L);
    DeAlloc(R);
  }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	//move element to correct spot by comparing it to the element next to it, and shifting the item if it is bigger that element else it is already in the correct spot
	int i, element, j;
  	for (i = 1; i < n; i++){
    	element = pData[i];
  
    	for(j=i-1; j>=0; j--){
      		if(pData[j]>element)
        		pData[j+1] = pData[j];
      		else
        		break;
    	}
    	pData[j+1] = element;
  	}	
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	//nested for loop that swaps the item with the item next to it
  //also uses swap function to swap their positions
  	for(int i=0;i<n-1;i++){
      	for(int j=0;j<n-i-1;j++){
          	if(pData[j]>pData[j+1]){
              swap(&pData[j], &pData[j+1]);
          	}
      	}
  	}	
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i,j, min_idx, temp;
  //nested for loop that searches for the smallest value, then swap it with it with the value at i
  //increments i so the search range is smaller at every index
  	for(i=0;i<n-1;i++){
      min_idx = i;
      for(j=i+1;j<n;j++)
          if(pData[j]<pData[min_idx])
              min_idx = j;
      if(min_idx != i){
          temp = pData[i];
          pData[i] = pData[min_idx];
          pData[min_idx] = temp;
      }
  }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i,n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		if(*ppData ==NULL){
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for(i=0;i<dataSz;++i){
			fscanf(inFile, "%d ", &n);
			data = *ppData +i;
			*data = n;
		}
		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	//changed this to the way the lab did it incase of error
	int i, sz = (dataSz > 100 ? dataSz -100: 0);
  	int firstHundred = (dataSz<100 ? dataSz:100);
	printf("\tData:\n\t");
	for (i=0;i<firstHundred;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
