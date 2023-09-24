#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Node {
	int freq;
	char ch;
	struct Node*left;
	struct Node* right;
} Nodes;

Nodes* root;
Nodes *nodes[256];
char codes[255][20];


int cnt = 0;
void treetraverse(Nodes *root,char str1[], int len1){
    if(root->left==NULL && root->right==NULL){
        str1[len1] = '\0';
        strcat(codes[(unsigned char)root->ch], str1);
        //printf("%c %s\n",root->ch,codes[(unsigned char)root->ch]);
    }
    str1[len1] = '0';
    if(root->right)treetraverse(root->right,str1, len1+1);
    str1[len1] = '1';
    if(root->left)treetraverse(root->left,str1, len1+1);
    return;
}



int main() {
	char* str = (char*)malloc(101);
	FILE *fp1;
	//FILE *fp1=fopen( "huffman.txt", "rb");
	fp1=fopen( "huffman.txt", "rb");

//initialion of nodes array
	for (int i = 0; i<256; i++) {
		nodes[i] = (Nodes*)malloc(sizeof(Nodes));
		nodes[i]->ch = i;
		nodes[i]->freq = 0;
		nodes[i]->left=NULL;
		nodes[i]->right=NULL;
	}

	int i=0;
   /* while(i<256){

	printf("%d>>%d %c %p %p\n",i,nodes[i]->freq,nodes[i]->ch,nodes[i]->left,nodes[i]->right);
    i++;
    }
    */
    // frequency counting
	while (fread(str, 1, 1, fp1)) {
        if(str[0]==EOF)break;
		str[1] = '\0';
		for (int i = 0; i < 1; i++) {
			nodes[(unsigned char)str[i]]->freq++;
		}
	}

    //sorting
	for (int i = 0; i<255; i++) {
		for (int j = i; j<256; j++) {
			if (nodes[j]->freq<nodes[i]->freq) {
				Nodes *temp = nodes[j];
				nodes[j] = nodes[i];
				nodes[i] = temp;
			}
		}
	}
    fclose(fp1);

    // avoiding 0 count
	int front = 0;
	while (nodes[front]->freq == 0) {
		front++;
	}
    int k=front;

    //printf("Front: %d", front);
    FILE*fp2=fopen("a.zzz","wb");

    fputc(256-k,fp2);  // no of distinct char
    for(i=0;i<(256-k);i++){
        fwrite(nodes[k+i],sizeof(struct Node),1,fp2);
    }
    while(front!=255){
     int min1 = nodes[front]->freq;
        int min2 = nodes[front+1]->freq;
        Nodes* temp = (Nodes*)malloc(sizeof(Nodes));
        temp->freq = min1 + min2;
        temp->left = nodes[front];
        temp->right = nodes[front + 1];
        front+=2;
        int i;
        for(i = front; i<256 ;i++){
            if(temp->freq<nodes[i]->freq){
                break;
            }
        }
        int j;
        for(j = front; j<i; j++){
            nodes[j-1] = nodes[j];
        }
        i--;
        nodes[i] = temp;
        front--;
    }
    char str1[20] = {};
	root = nodes[front];
	treetraverse(root, str1, 0);
    //printf("Front: %d", front);
	fp1=fopen("huffman.txt","rb");

	char sstr[100000];
	sstr[0]=0;
	int coun=0;
    while (1) {
        char ch2=fgetc(fp1);
        if(ch2==EOF)break;
        strcat(sstr,codes[(unsigned char)ch2]);
	}
	k=0;
	unsigned char cch;
    for(i=0;i<strlen(sstr);i+=8){
        int j;
        for(j=0;j<8;j++){
            cch=cch<<1;
            if(sstr[k++]=='0'){}
            else{cch=cch | 1;}
        }
        fputc(cch,fp2);
    }
    fclose(fp1);
    fclose(fp2);
    fp1=fopen("a.zzz","rb");
    struct Node** nodes2;
    unsigned char cn=fgetc(fp2);
    nodes2=(struct Node**)malloc(sizeof(struct Node*)*cn);
    for(i=0;i<cn;i++){
        nodes2[i]=(struct Node*)malloc(sizeof(struct Node));
        fread(nodes2[i],sizeof(struct Node),1,fp1);
    }
    front=0;
    while(front!=cn-1){
     int min1 = nodes2[front]->freq;
        int min2 = nodes2[front+1]->freq;
        Nodes* temp = (Nodes*)malloc(sizeof(Nodes));
        temp->freq = min1 + min2;
        temp->left = nodes2[front];
        temp->right = nodes2[front + 1];
        front+=2;
        int i;
        for(i = front; i<cn ;i++){
            if(temp->freq<nodes2[i]->freq){
                break;
            }
        }
        int j;
        for(j = front; j<i; j++){
            nodes2[j-1] = nodes2[j];
        }
        i--;
        nodes2[i] = temp;
        front--;
    }
    fp2=fopen("b.txt","wb");
    Node *temp=nodes2[front];
    str1[0]=0;
    treetraverse(nodes2[front], str1, 0);
    while (1) {
        char ch2=fgetc(fp1);
        if(ch2==EOF)break;
		for (int i = 0; i < 8; i++) {
            if(ch2 & (1<<(7-i))){temp=temp->left;}
            else{temp=temp->right;}
            if(temp->left==NULL && temp->right==NULL){
                //printf("%c",temp->ch);
                fputc(temp->ch,fp2);
                temp=nodes2[front];
            }
		}
	}
    fclose(fp1);
    fclose(fp2);

    //FILE *fp1,*fp2,*fp3;

    fp1=fopen("huffman.txt","rb");
    fp2=fopen("a.zzz","rb");
    FILE *fp3=fopen("b.txt","rb");



   fseek(fp1, 0, SEEK_END);
   long size1 = ftell(fp1); // tell the current bytes
   printf("Size of the main file: %ld\n", size1);


   fseek(fp2, 0, SEEK_END);
   long size2 = ftell(fp2); // tell the current bytes
   printf("Size of the compressesd file: %ld\n", size2);


   fseek(fp3, 0, SEEK_END);
   long size3 = ftell(fp3); // tell the current bytes
   printf("Size of the decompressd file: %ld\n", size3);

   printf("memory saved %lf%%" ,100-(((double)(size1-size2)*100)/size1));

	//_getche();
	return 0;
}

