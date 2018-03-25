#include<stdio.h>
#include<stdlib.h>

int main()
{
       int i,j;
       int count;
       int number1 = 0;
       for(i=3;i<10000;i++)
       {
              count = 0;
              for(j=2;j<i;j++)
              {
                     if(i%j == 0)
                     {
                           count++;
                     }
              }
              if(count == 0)
              {
                    if(i == number1 + 2){
                        printf("%d %d\n", number1, i);
                    }
                    number1 = i;
              }
       }
}