  #define _CRT_SECURE_NO_WARNINGS
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #define MAXLINES 25
  #define MAX_BUF 256
  
  long arr[10] = { 3,6,1,2,3,8,4,1,7,2};
  char arr2[5][20] = {  "Mickey Mouse",
                      "Donald Duck",
                      "Minnie Mouse",
                      "Goofy",
                      "Ted Jensen" };
  
  int get_lines(char *lines[]);
  void sort(char *p[], int n);
  void print_strings(char *p[], int n);
  void bubble(void *p, int width, int N, int(*fptr)(const void *, const void *));
  int compare_string(const void *m, const void *n);
  int compare_long(const void *m, const void *n);

  char *lines[MAXLINES];

  int main( void )
  {
     //Sort #1
	 int number_of_lines;

     // Read in the lines from the keyboard.

     number_of_lines = get_lines(lines);

     if ( number_of_lines < 0 )
     {
         puts(" Memory allocation error");
         exit(-1);
     }

     sort(lines, number_of_lines);
     print_strings(lines, number_of_lines);
	 
     //Sort #2	 
	 int i;
     puts("\nBefore Sorting:\n");

     for (i = 0; i < 10; i++)               /* show the long ints */
     {
         printf("%ld ",arr[i]);
     }
     puts("\n");

     for (i = 0; i < 5; i++)                  /* show the strings */
     {
         printf("%s\n", arr2[i]);
     }
     bubble(arr, 4, 10, compare_long);          /* sort the longs */
     bubble(arr2, 20, 5, compare_string);     /* sort the strings */
     puts("\n\nAfter Sorting:\n");

     for (i = 0; i < 10; i++)             /* show the sorted longs */
     {
         printf("%ld ",arr[i]);
     }
     puts("\n");
 
     for (i = 0; i < 5; i++)            /* show the sorted strings */
     {
         printf("%s\n", arr2[i]);
     }
	 
     return 0;
  }

  int get_lines(char *lines[])
  {
     int n = 0;
     char buffer[80];  // Temporary storage for each line.

     puts("Enter one line at time; enter a blank when done.");

     while ((n < MAXLINES) && (gets(buffer) != 0) &&
            (buffer[0] != '\0'))
     {
         if ((lines[n] = (char *)malloc(strlen(buffer)+1)) == NULL)
             return -1;
         strcpy( lines[n++], buffer );
     }
     return n;

  } // End of get_lines()

  void sort(char *p[], int n)
  {
     int a, b;
     char *tmp;

     for (a = 1; a < n; a++)
     {
         for (b = 0; b < n-1; b++)
         {
             if (strcmp(p[b], p[b+1]) > 0)
             {
                 tmp = p[b];
                 p[b] = p[b+1];
                 p[b+1] = tmp;
             }
         }
     }
  }

  void print_strings(char *p[], int n)
  {
     int count;

     for (count = 0; count < n; count++)
         printf("%s\n", p[count]);
  }
  
  void bubble(void *p, int width, int N,
            int(*fptr)(const void *, const void *))
{
    int i, j, k;
    unsigned char buf[MAX_BUF];
    unsigned char *bp = p;

    for (i = N-1; i >= 0; i--)
    {
        for (j = 1; j <= i; j++)
        {
            k = fptr((void *)(bp + width*(j-1)), (void *)(bp + j*width));
            if (k > 0)
            {
                memcpy(buf, bp + width*(j-1), width);
                memcpy(bp + width*(j-1), bp + j*width , width);
                memcpy(bp + j*width, buf, width);
            }
        }
    }
}

int compare_string(const void *m, const void *n)
{
    char *m1 = (char *)m;
    char *n1 = (char *)n;
    return (strcmp(m1,n1));
}

int compare_long(const void *m, const void *n)
{
    long *m1, *n1;
    m1 = (long *)m;
    n1 = (long *)n;
    return (*m1 > *n1);
}
