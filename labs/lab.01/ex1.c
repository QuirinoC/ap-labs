#include <stdio.h>
//Comment
int main()
{
    FILE *fp;
    fp = fopen("ex1.c", "r+");
    FILE *fp2;
    fp2 = fopen("out.txt", "w");

    char c; 

    while ((c = getc(fp)) != EOF) {
        putc(c, fp2);
    }
    fclose(fp);
/*Multi
  line
  comment
*/
}