#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define START_A 0x00 
//0x31
//0x61
#define END_A 0x20
//0x7A,0x7e

/* For this program we are going to assume 26 dictionary letters
ascii from lowercase a to z
a -> 0x61
z -> 0x7A
*/


struct distribution {
    unsigned int n; //the number of letters in this dist
    unsigned int letter_count[256]; //the frequency of letters direct addr table 
    double e_frequency; //avg is 12.7%  e is 0x65
    double t_frequency; //avg is 9.1%  t is 0x74
    double a_frequency; //avg is 8.2%  a is 0x61
    double distance; //closer to 0 is better
    unsigned long key_length; //key length used
    unsigned char * key; //key used
};

typedef struct distribution distribution;

unsigned char * nextKey(unsigned char * buffer, unsigned int len) {
  
    int i;
    for (i = 0; i < len; i++) {
    
        if( buffer[i] < END_A) {
            buffer[i]++;
      
            if(i  > 0) {
                int j;
                for ( j = 0 ; j < i;j++) {
                    buffer[j] = START_A;
                }
            }
      
            break;
        }
    }
    return buffer;
}

//this function will get the distribution of a given buffer with index to test, length and key
struct distribution getDist(unsigned long b_size
                            ,unsigned char * buffer
                            ,unsigned int k_len
                            ,unsigned char * key
                            ,unsigned int index) {
    /*
      unsigned int n; //the number of letters in this dist
      unsigned int letter_count[256]; //the frequency of letters direct addr table 
      float e_frequency; //avg is 12.7%  e is 0x65
      float t_frequency; //avg is 9.1%  t is 0x74
      float a_frequency; //avg is 8.2%  a is 0x61
      float distance; //closer to 0 is better
      unsigned long key_length; //key length used
      unsigned char * key; //key used
     */
    distribution d;
    d.n = 0;
    d.key = key;
    unsigned int lc[256];
    int i,count;
    //unsigned int * list;// = (int * )malloc(b_size);
    //memset(list,0,b_size);
    unsigned char c;
    double  nE = 0;
    double  nT = 0;
    double  nA = 0;
    
    //printf("trying key %s\n",key);

    for(count = index; count < b_size; count ++) {
        
        if ((count - index) % k_len == 0) {
            
            c = buffer[count] ^ key[count % k_len];
            if( c == 0x65 ) nE++;
            if( c == 0x74 ) nT++;
            if( c == 0x61 ) nA++;
            d.n++;
			/* printf("%s\n",buffer); */
            /* printf("%02x (count %d) XOR %02x = %c  N:%d \n",buffer[count],count,key[count % k_len],c,d.n);    */
            
        }

    }
    //printf("%f,%f,%f - total letters: %d\n",nE,nT,nA,d.n);
    d.e_frequency = nE/d.n;
    d.t_frequency = nT/d.n;
    d.a_frequency = nA/d.n;
    //printf("number of letters %i, e:%f - t:%f - a:%f \n",d.n,d.e_frequency,d.t_frequency,d.a_frequency);
    d.distance = sqrt( (d.e_frequency - 0.127) *(d.e_frequency - 0.127)
                       + (d.t_frequency - 0.091) * (d.t_frequency - 0.091)
                       + (d.a_frequency - 0.082)*(d.a_frequency - 0.082) );
    //if(nE == 0 && nT == 0 && nA ==0) d.distance = 99;
    return d;
}

