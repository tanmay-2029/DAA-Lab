// Taking Red as '1',Blue as '2' and Yellow as '3';

#include <stdio.h>

void sort(int n, int arr[], int brr[]){
    int k=0;
    for (int i=0;i<n;i++) { if(arr[i]==1) {brr[k]=arr[i]; k++;} }
    for (int i=0;i<n;i++) { if(arr[i]==2) {brr[k]=arr[i]; k++;} }
    for (int i=0;i<n;i++) { if(arr[i]==3) {brr[k]=arr[i]; k++;} }
}

void display(int n,int arr[]){
    for (int i=0;i<n;i++) printf("%d ",arr[i]);
    printf("\n");
}

int main(){
    int n;
    printf("Enter Number of Ball : ");
    scanf("%d",&n);

    int arr[n];

    printf("\nTaking Red as '1',Blue as '2' and Yellow as '3'\n Enter the %d pair of sequence :\n",n);
    for (int i=0;i<n;i++){
        scanf("%d",&arr[i]);
    }

    int brr[n];

    sort(n,arr,brr);
    display(n,brr);

    return 0;
}
