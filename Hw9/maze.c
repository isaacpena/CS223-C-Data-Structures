#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>


/* return values : 
0 - do nothing
1 - jump out of recursive stack 
2 - jump out of recursive stack while whitening cells */

char
cycleFind(char **input, int width, int height, int xcoord, int ycoord, int xpar, int ypar, char **table) {
     

    if(table[ycoord][xcoord] == 1) {
        /* cycle detected, you've seen this point before.  */
        /* removing indicates that we're gonna step backwards and overwrite for a while. */
        input[ycoord][xcoord] = 2;
        return 2;
    }

    table[ycoord][xcoord] = 1;

    printf("%d %d\n", xcoord, ycoord);
    char ne = 1;
    char n = 1;
    char nw = 1;
    char e = 1;
    char w = 1;
    char sw = 1;
    char s = 1;
    char se = 1; 
    char top = 1;
    char bottom = 1;
    char left = 1;
    char right = 1;


    if(ycoord == 0) {
        ne = 0;
        n = 0;
        nw = 0;
        top = 0;
    } else if (ycoord == height - 1) {
        sw = 0;
        s = 0;
        se = 0;
        bottom = 0;
    }

    if(xcoord == 0) {
        nw = 0;
        w = 0;
        sw = 0;
        left = 0;
    } else if (xcoord == width - 1) {
        ne = 0;
        e = 0;
        se = 0;
        right = 0;
    }

    if(xpar == xcoord - 1 && ypar == ycoord) {
        w = 0;
    } else if (xpar == xcoord - 1 && ypar == ycoord - 1) {
        nw = 0;
    } else if (xpar == xcoord && ypar == ycoord - 1) {
        n = 0;
    } else if (xpar == xcoord + 1 && ypar == ycoord - 1) {
        ne = 0;
    } else if (xpar == xcoord + 1 && ypar == ycoord) {
        e = 0;
    } else if (xpar == xcoord + 1 && ypar == ycoord + 1) {
        se = 0;
    } else if (xpar == xcoord && ypar == ycoord + 1) {
        s = 0;
    } else if (xpar == xcoord - 1 && ypar == ycoord + 1) {
        sw = 0;
    }

    if(left == 1 && input[ycoord][xcoord - 1] == 0) {
        w = 0;
    }
    if(left == 1 && top == 1 && input[ycoord - 1][xcoord - 1] == 0) {
        nw = 0;
    }
    if(top == 1 && input[ycoord - 1][xcoord] == 0) {
        n = 0;
    }
    if(right == 1 && top == 1 && input[ycoord - 1][xcoord + 1] == 0) {
        ne = 0;
    }
    if(right == 1 && input[ycoord][xcoord + 1] == 0) {
        e = 0;
    }
    if(bottom == 1 && right == 1 && input[ycoord + 1][xcoord + 1] == 0) {
        se = 0;
    }
    if(bottom == 1 && input[ycoord + 1][xcoord] == 0) {
        s = 0;
    }
    if(bottom == 1 && left == 1 && input[ycoord + 1][xcoord - 1] == 0) {
        sw = 0;
    }

    char retval;

    if(w == 1) {
        retval = cycleFind(input, width, height, xcoord - 1, ycoord, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    if(nw == 1) {
        retval = cycleFind(input, width, height, xcoord - 1, ycoord - 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    } 
    if(n == 1) {
        retval = cycleFind(input, width, height, xcoord, ycoord - 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    if(ne == 1) {
        retval = cycleFind(input, width, height, xcoord + 1, ycoord - 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    } 
    if(e == 1) {
        retval = cycleFind(input, width, height, xcoord + 1, ycoord, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    if(se == 1) {
        retval = cycleFind(input, width, height, xcoord + 1, ycoord + 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    if(s == 1) {
        retval = cycleFind(input, width, height, xcoord, ycoord + 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    if(sw == 1) {
        retval = cycleFind(input, width, height, xcoord - 1, ycoord + 1, xcoord, ycoord, table);
        if(retval == 2) {
            if(input[ycoord][xcoord] == 2) {
                return 1;
            } else {
                input[ycoord][xcoord] = 2;
                return 2;
            }
        } else if (retval == 1) {
            return 1;
        }
    }
    return 0;
}

int
main(int argc, char **argv)
{
    int height;
    int width;

    scanf("P5 %d %d 255\n", &width, &height);

    char **input = malloc(sizeof(char*) * height);
    char **table = malloc(sizeof(char*) * height);
    int i = 0;
    for(i = 0; i < height; i++) {
        input[i] = malloc(sizeof(char) * width);
        table[i] = malloc(sizeof(char) * width);
    } 

    char c;
    int currh = 0;
    int currw = 0;
    int starth = 0;
    int startw = 0;
    int flag = 1;
    while((c = getc(stdin)) != EOF) {
        if(c == 1 && flag == 1) {
            starth = currh;
            startw = currw;
            flag = 0;
        }
        input[currh][currw] = c;
        currw++;
        if(currw == width) {
            currw = 0;
            currh++;
        }
    }


    int j = 0;
    int n = 0;
    for(j = 0; j < height; j++) {
        for(n = 0; n < width; n++) {
            table[j][n] = -1;
        }
    }

    cycleFind(input, width, height, starth, startw, 0, 0, table);

    printf("P5 %d %d 255\n", width, height);

    int k = 0;
    int l = 0;
    for(k = 0; k < height; k++) {
        for(l = 0; l < width; l++) {
            printf("%d ", input[k][l]);
        }
        printf("\n");
    }

    int m;
    for(m = height - 1; m >= 0; m--) {
        free(input[m]);
        free(table[m]);
    }
    free(input);
    free(table);
    return 0;
    
}