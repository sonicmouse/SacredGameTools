#include <stdio.h>
#include <conio.h>
#include "SHLib.h"

#define SECTION_HEROATAGLANCE 0xC3

//
// pep
//
int main(int argc, const char** argv){

	printf("*******************************************\n");
	printf("*     Sacred Hero Level Unlocker v1.0     *\n");
	printf("*   Authors: Liquid Klone and SonicMouse  *\n");
	printf("*       Web: www.mouseindustries.com      *\n");
	printf("*          Release Date: 06/09/05         *\n");
	printf("*******************************************\n\n");

	if(argc > 1){
		CSHLib shLib;
		if(shLib.IsFullyLoaded()){
			for(int i = 1; i < argc; i++){

				char* cur = strdup(argv[i]);
				if(strrchr(cur,'\\')){
					printf("Patching: %s... ",strrchr(cur,'\\')+1);
					
					// create a hero instance
					HHERO hHero = shLib.CreateHero();
					int rc = shLib.DecompileHero(hHero,cur);
					switch(rc){
						case ERROR_SHLIB_SUCCESS:
							{
								// get the section size
								int nSize = shLib.GetHeroSection(hHero,SECTION_HEROATAGLANCE,NULL);
								if(nSize != -1){
									unsigned char* pSection = new unsigned char[nSize];
									if(pSection){
										
										// get the section data
										shLib.GetHeroSection(hHero,SECTION_HEROATAGLANCE,&pSection);
										
										unsigned char* levelSection = &pSection[0xEE];

										*(unsigned long*)levelSection = 0x00000000; // brnz [Unlocks Bronze, Silver]
										*(unsigned long*)levelSection = 0x00000001; // silv [Unlocks Bronze, Silver, Gold]
										*(unsigned long*)levelSection = 0x00000100; // gold [Unlocks Bronze, Silver, Gold, Platinum]
										*(unsigned long*)levelSection = 0x00010000; // plat [Unlocks Bronze, Silver, Gold, Platinum, Niobium]
										*(unsigned long*)levelSection = 0x01000000; // niob [Unlocks Bronze, Silver, Gold, Platinum, Niobium]

										// set the section back
										shLib.SetHeroSection(hHero,SECTION_HEROATAGLANCE,pSection,nSize);

										// recompile hero
										if(shLib.CompileHero(hHero,cur)){
											printf("Success: All levels have been unlocked!\n\n");
										}else{
											printf("Error: Unable to re-compile hero\n\n");
										}

										// free memory
										delete pSection;
									}else{
										printf("Error: Insufficient memory!\n\n");
									}
								}else{
									printf("Error: Unable to seek to correct section!\n\n");
								}
							}
							break;
						case ERROR_SHLIB_WRONGVERSION:
							printf("Error: Wrong Hero Version!\n\n");
							break;
						case ERROR_SHLIB_FILE:
							printf("Error: Unable to open target .PAX/.PTX file!\n\n");
							break;
						default:
							printf("Error: Unknown Error!\n\n");
							break;
					}
					shLib.DestroyHero(hHero);
				}
				free(cur);
			}
		}else{
			printf("Unable to load the SHLIB.DLL\nMake sure it exists on this computer.\n\n");
		}
	}else{
		printf("In order to use this program, drag and\ndrop (multiple) Sacred .PAX/.PTX files on top of my icon.\n\n");
	}


	printf("Press any key to continue...");
	getch();
	return 0;
}
