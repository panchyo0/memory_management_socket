#include <stdio.h>

#define N 200000//40000000//20000
void process()
{
    int  i, j, c, root, temp;
    
    //printf("\n Enter no of elements :");
    
    for (i = 0; i < N; i++) put(i,lrand48());
    
    for (i = 1; i < N; i++)
    {
        c = i;
        do
        {
            root = (c - 1) / 2;
       
            if (get(root) < get(c))  
            {
                temp = get(root);
                put(root,get(c));
           
                put(c,temp);
               
            }
            c = root;
        } while (c != 0);
    }
    
 
   
    for (j = N - 1; j >= 0; j--)
    {
        temp = get(0);
        put(0,get(j));
       
        put(j,temp);
 
        root = 0;
        do
        {
            c = 2 * root + 1;   
            if (c>j) break;
            if ((get(c) < get(c + 1)) && c < j-1)
                c++;
            if (get(root)<get(c) && c<j)   
            {
                    temp = get(root);
                    put(root,get(c));
           
                    put(c,temp);
             
            }
            root = c;
            
        } while (c < j);
    }

    
    done();
}

