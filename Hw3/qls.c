#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if(argc < 2) {
		perror("Usage: ./qls [searched string] < [name of file to search]");
		return 1;
	}
	int size = 1;
	char* filetext = calloc(sizeof(char), size);
	int numchars = 0;


	int c;
	while((c = getc(stdin)) != EOF) {
		while(numchars >= size) {
			size *=2;
			filetext = realloc(filetext, sizeof(char) * size);
		}
		filetext[numchars] = c;
		numchars++;
		int helpful = numchars;
		while(helpful < size) {
			filetext[helpful] = 0;
			helpful++;
		}
	}

	int i = 1;
	for(i = 1; i < argc; i++) {
		char* currstring = argv[i];
		int lastchar = strlen(currstring);
		int c0 = 0;
		for(c0 = 0; c0 < numchars; c0++) {
			int c1 = 0;
			for(c1 = 0; c1 < numchars; c1++) {
				if((c0 + (lastchar - 1) * c1) > numchars) {
					break;
				}
				int c2 = 0;
				for(c2 = 0; c2 < numchars; c2++) {
					if((c0 + (lastchar - 1) * c1 + (lastchar - 1) * (lastchar - 1) * c2) > numchars) {
						break;	
					}
					int multvar = 0;
					int index = 0;
					while(multvar < lastchar) {
						if(filetext[(c0 + index * c1 + index * index * c2)] != currstring[multvar]) {
							break;
						}
						if(c1 == 0 & c2 == 0) {
							break;
						}
						if(multvar == (lastchar - 1)) {
							printf("%d %d %d %s\n", c0, c1, c2, argv[i]);
						}
						multvar++;
						index++;

					}	
				}
			}
		}
	}

	free(filetext);
	return 0;
}
