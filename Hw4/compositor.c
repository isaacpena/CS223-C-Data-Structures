#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE (2048)

struct display {
	int numcols;
	int numrows;
	char **a;
};

int
main(int argc, char **argv) {
	int row;
	int col;
	char filename[BUFFER_SIZE];
	struct display d;
	d.numcols = 100;
	d.numrows = 100;
	d.a = malloc(sizeof(char*) * 100);
	int senti = 0;
	int sentj = 0;
	for(senti = 0; senti < d.numrows; senti++) {
		d.a[senti] = malloc(sizeof(char) * 100);
		for(sentj = 0; sentj < d.numcols; sentj++) {
			d.a[senti][sentj] = ' ';
		}
	}

	while(scanf("%d %d %s", &row, &col, filename) == 3) {
		FILE *fp;
		fp = fopen(filename, "r");
		if(fp == 0) { 
			perror(filename);
			exit(1);
		}

		int tempcol = d.numcols;
		while(col >= d.numcols) {
			d.numcols *= 2;
			int i = 0;
			for(i = 0; i < d.numrows; i++) {
				d.a[i] = realloc(d.a[i], sizeof(char) * d.numcols);
				int j = tempcol;
				for (j = tempcol; j < d.numcols; j++) {
					d.a[i][j] = ' ';
				}
			}
		}

		int temprow = d.numrows;

		while(row >= d.numrows) { 
			d.numrows *= 2;
			d.a = realloc(d.a, sizeof(char*) * d.numrows);
			int k = temprow;
			for(k = temprow; k < d.numrows; k++) {
				d.a[k] = realloc(d.a[k], sizeof(char) * d.numcols);
				int l = 0;
				for (l = 0; l < d.numcols; l++) {
					d.a[k][l] = ' ';
				}
			}
		}


		int currrow = row;
		int currcol = col;
		int c; 
		while((c = fgetc(fp)) != EOF) {
			if(currcol >= d.numcols) {
				int tempcurrcol = d.numcols;
				d.numcols *= 2;
				int m = 0;
				for(m = 0; m < d.numrows; m++) {
					d.a[m] = realloc(d.a[m], sizeof(char) * d.numcols);
					int n = tempcurrcol;
					for (n = tempcurrcol; n < d.numcols; n++) {
						d.a[m][n] = ' ';
					}
				}
			}

			if(currrow >= d.numrows) { 	
				int tempcurrrow = d.numrows;
				d.numrows *= 2;
				d.a = realloc(d.a, sizeof(char*) * d.numrows);
				int o = tempcurrrow;
				for(o = tempcurrrow; o < d.numrows; o++) {
					d.a[o] = realloc(d.a[o], sizeof(char) * d.numcols);
					int p = 0;
					for (p = 0; p < d.numcols; p++) {
						d.a[o][p] = ' ';
					}
				}
			}

			if(c == '\n') {
				currrow++;
				currcol = col;
			} else if (c == ' ') {
				currcol++;
			} else {
				d.a[currrow][currcol] = c;
				currcol++;
			}

		}
		fclose(fp);


	}
	int flag = 0;
	int index = 0;
	int printi = 0;
	int printj = 0;
	int rowindex = 0;
	for(printi = 0; printi < d.numrows; printi++) {
		flag = 1;
		for(printj = 0; printj < d.numcols; printj++) {
			if(d.a[printi][printj] != ' ') {	
				flag = 0;
			}
		}
		if (flag == 0) {
			rowindex = printi;
		}
	}
	for(printi = 0; printi <= rowindex; printi++) {
		index = 0;
		for(printj = 0; printj < d.numcols; printj++) {
			flag = 0;
			if(d.a[printi][printj] == ' ') {
				flag = 1;
			}
			if(d.a[printi][printj] != ' ') {
				flag = 0;
				index = printj;
			}
		}
		if(flag == 0) {
			index = d.numcols;
		}
		if(index == 0) {
			putchar('\n');
			continue;
		}
		for(printj = 0; printj <= index; printj++) {
			if(printj == 0 && d.a[printi][printj] == '\n') {
				break;
			} else {
				printf("%c", d.a[printi][printj]);	
			}
		}

	putchar('\n');
	}

	for(printi = 0; printi < d.numrows; printi++) {
		free(d.a[printi]);
	}

	free(d.a);
			
	return 0;
}

