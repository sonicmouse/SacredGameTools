// SacredTestRes.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#pragma pack(push,1)
typedef struct _tag_node{
	unsigned long id;
	unsigned long addr;
	unsigned long unknown;
	unsigned long size;
} NODE, *LPNODE;
#pragma pack(pop)


char* RemoveNs(char* szTxt){
	char* n;
	while( (n=strstr(szTxt,"<n>")) != NULL ){
		n[0] = ' ';
		strcpy(&n[1],&n[3]);
	}
	return szTxt;
}

char* TrimSpaces(char* szTxt){
	for(int i = 0; i < (int)strlen(szTxt); i++){
		if(szTxt[i]==' '){
			strcpy(szTxt,&szTxt[i+1]);
			--i;
		}else{
			for(int e = (int)strlen(szTxt)-1; e >= 0; e--){
				if(szTxt[e] == ' '){
					szTxt[e]=0;
				}else{
					break;
				}
			}
			break;
		}
	}
	return szTxt;
}

int main(int argc, char* argv[])
{

	FILE* f = fopen("E:\\SacredGame\\scripts\\us\\global.res","rb");

	if(!f){
		printf("Unable to open res file!\n");
		return 0;
	}

	unsigned long totalItems;
	fread(&totalItems,sizeof(unsigned long),1,f);

	FILE* log = fopen("c:\\log.txt","wt");
	
	int nCountGraves = 0;

	for(unsigned long i = 0; i < totalItems; i++){

		NODE node;
		fread(&node,sizeof(node),1,f);

		unsigned long lastAddr = ftell(f);

		fseek(f,node.addr+sizeof(totalItems),SEEK_SET);

		unsigned char* pBuf = new unsigned char[node.size];
		fread(pBuf,node.size,1,f);

		int nNameSize = node.size / sizeof(short);

		char* pName = new char[nNameSize + 1];
		pName[nNameSize] = 0;

		int rc = WideCharToMultiByte(CP_ACP,0,(WCHAR*)pBuf,-1,pName,nNameSize,NULL,NULL);
		delete pBuf;

		fprintf(log,"%u] (ID: %08X) (Addr: 0x%08X) \"%s\"\n",i,node.id,node.addr,pName);

		//if(strnicmp(pName,"Grave<n><n>",11)==0){
		//	++nCountGraves;
		//	fprintf(log,"\"%s\"\n",TrimSpaces(RemoveNs(&pName[11])));
		//}

		delete pName;
		
		fseek(f,lastAddr,SEEK_SET);
	}

	printf("Count: %u\n", nCountGraves);

	fclose(log);
	fclose(f);

	return 0;
}

