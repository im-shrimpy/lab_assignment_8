#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int extraMemoryAllocated;

void swap(int *a, int *b)
{
	int temp =*a;
	*a=*b;
	*b=temp;
}



struct node
{
	int data;
	struct node *left;
	struct node *right;
};

void heapify(struct node *head)
{
    if(head == NULL)
	{ 
		return;
	}
    struct node *smallest=head;
    struct node *l=head->left;
    struct node *r=head->right;
    
    if((l!=NULL)&&(l->data<smallest->data)) 
	{
        smallest=l;
    }
    if ((r!=NULL)&&(r->data<smallest->data)) 
	{
        smallest=r;
    }
    if (smallest != head) {
        int temp=head->data;
        head->data=smallest->data;
        smallest->data=temp;
        heapify(smallest);
    }
}

void buildTree(struct node *head, int *arr, int i, int n)
{
	if (i>=n)
	{
		head->left=NULL;
		head->right=NULL;
		return;
	}
    
    head->data = arr[i];
    
    struct node *left = malloc(sizeof(struct node));
    struct node *right = malloc(sizeof(struct node));
    extraMemoryAllocated+=sizeof(struct node)*2;

    head->left = left;
    head->right = right;
    
    buildTree(left, arr, 2*i+1, n);
    buildTree(right, arr, 2*i+2, n);
}

void heapSort(int arr[], int n)
{
	struct node *root = malloc(sizeof(struct node));
	extraMemoryAllocated+=sizeof(struct node);
    buildTree(root, arr, 0, n);
    
    for (int i=n-1;i>=0;i--) 
	{
        int temp=root->data;
        root->data=arr[i];
        arr[i]=temp;
        
        heapify(root);
    }
}

void mergeList(int *pData, int l, int m, int r) {
    int i, j, k;
    int n1=m-l+1;
    int n2=r-m;

    int *L=malloc(n1*sizeof(int));
    extraMemoryAllocated+=n1*sizeof(int);
    int *R=malloc(n2*sizeof(int));
    extraMemoryAllocated += n2*sizeof(int);

    for (i = 0; i < n1; i++)
	{
		L[i] = pData[l + i];
	}
    for (j = 0; j < n2; j++)
	{
        R[j] = pData[m + 1 + j];
	}
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i];
            i++;
        }
        else {
            pData[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        pData[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        pData[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int *pData, int l, int r){
    if (l<r){
        int m=l+(r-l)/2;
        mergeSort(pData,l,m);
        mergeSort(pData,m+1,r);
        mergeList(pData,l,m,r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
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
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
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
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}