
#include "TTEncode.h"
#include <memory.h>


unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                         
void MD5::MD5Init(MD5_CTX *context)
{
     context->count[0] = 0;
     context->count[1] = 0;
     context->state[0] = 0x67452301;
     context->state[1] = 0xEFCDAB89;
     context->state[2] = 0x98BADCFE;
     context->state[3] = 0x10325476;
}
void MD5::MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)
{
    unsigned int i = 0,index = 0,partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = 64 - index;
    context->count[0] += inputlen << 3;
    if(context->count[0] < (inputlen << 3))
       context->count[1]++;
    context->count[1] += inputlen >> 29;
    
    if(inputlen >= partlen)
    {
       memcpy(&context->buffer[index],input,partlen);
       MD5Transform(context->state,context->buffer);
       for(i = partlen;i+64 <= inputlen;i+=64)
           MD5Transform(context->state,&input[i]);
       index = 0;        
    }  
    else
    {
        i = 0;
    }
    memcpy(&context->buffer[index],&input[i],inputlen-i);
}
void MD5::MD5Final(MD5_CTX *context,unsigned char digest[16])
{
    unsigned int index = 0,padlen = 0;
    unsigned char bits[8];
    index = (context->count[0] >> 3) & 0x3F;
    padlen = (index < 56)?(56-index):(120-index);
    MD5Encode(bits,context->count,8);
    MD5Update(context,PADDING,padlen);
    MD5Update(context,bits,8);
    MD5Encode(digest,context->state,16);
}
void MD5::MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)
{
    unsigned int i = 0,j = 0;
    while(j < len)
    {
         output[j] = input[i] & 0xFF;  
         output[j+1] = (input[i] >> 8) & 0xFF;
         output[j+2] = (input[i] >> 16) & 0xFF;
         output[j+3] = (input[i] >> 24) & 0xFF;
         i++;
         j+=4;
    }
}
void MD5::MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)
{
     unsigned int i = 0,j = 0;
     while(j < len)
     {
           output[i] = (input[j]) |
                       (input[j+1] << 8) |
                       (input[j+2] << 16) |
                       (input[j+3] << 24);
           i++;
           j+=4; 
     }
}
void MD5::MD5Transform(unsigned int state[4],unsigned char block[64])
{
     unsigned int a = state[0];
     unsigned int b = state[1];
     unsigned int c = state[2];
     unsigned int d = state[3];
     unsigned int x[64];
     MD5Decode(x,block,64);
     FF(a, b, c, d, x[ 0], 7, 0xd76aa478); /* 1 */
 FF(d, a, b, c, x[ 1], 12, 0xe8c7b756); /* 2 */
 FF(c, d, a, b, x[ 2], 17, 0x242070db); /* 3 */
 FF(b, c, d, a, x[ 3], 22, 0xc1bdceee); /* 4 */
 FF(a, b, c, d, x[ 4], 7, 0xf57c0faf); /* 5 */
 FF(d, a, b, c, x[ 5], 12, 0x4787c62a); /* 6 */
 FF(c, d, a, b, x[ 6], 17, 0xa8304613); /* 7 */
 FF(b, c, d, a, x[ 7], 22, 0xfd469501); /* 8 */
 FF(a, b, c, d, x[ 8], 7, 0x698098d8); /* 9 */
 FF(d, a, b, c, x[ 9], 12, 0x8b44f7af); /* 10 */
 FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
 FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
 FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
 FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
 FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
 FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
 
 /* Round 2 */
 GG(a, b, c, d, x[ 1], 5, 0xf61e2562); /* 17 */
 GG(d, a, b, c, x[ 6], 9, 0xc040b340); /* 18 */
 GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
 GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa); /* 20 */
 GG(a, b, c, d, x[ 5], 5, 0xd62f105d); /* 21 */
 GG(d, a, b, c, x[10], 9,  0x2441453); /* 22 */
 GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
 GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8); /* 24 */
 GG(a, b, c, d, x[ 9], 5, 0x21e1cde6); /* 25 */
 GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
 GG(c, d, a, b, x[ 3], 14, 0xf4d50d87); /* 27 */
 GG(b, c, d, a, x[ 8], 20, 0x455a14ed); /* 28 */
 GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
 GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8); /* 30 */
 GG(c, d, a, b, x[ 7], 14, 0x676f02d9); /* 31 */
 GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
 
 /* Round 3 */
 HH(a, b, c, d, x[ 5], 4, 0xfffa3942); /* 33 */
 HH(d, a, b, c, x[ 8], 11, 0x8771f681); /* 34 */
 HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
 HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
 HH(a, b, c, d, x[ 1], 4, 0xa4beea44); /* 37 */
 HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9); /* 38 */
 HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60); /* 39 */
 HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
 HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
 HH(d, a, b, c, x[ 0], 11, 0xeaa127fa); /* 42 */
 HH(c, d, a, b, x[ 3], 16, 0xd4ef3085); /* 43 */
 HH(b, c, d, a, x[ 6], 23,  0x4881d05); /* 44 */
 HH(a, b, c, d, x[ 9], 4, 0xd9d4d039); /* 45 */
 HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
 HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
 HH(b, c, d, a, x[ 2], 23, 0xc4ac5665); /* 48 */
 
 /* Round 4 */
 II(a, b, c, d, x[ 0], 6, 0xf4292244); /* 49 */
 II(d, a, b, c, x[ 7], 10, 0x432aff97); /* 50 */
 II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
 II(b, c, d, a, x[ 5], 21, 0xfc93a039); /* 52 */
 II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
 II(d, a, b, c, x[ 3], 10, 0x8f0ccc92); /* 54 */
 II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
 II(b, c, d, a, x[ 1], 21, 0x85845dd1); /* 56 */
 II(a, b, c, d, x[ 8], 6, 0x6fa87e4f); /* 57 */
 II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
 II(c, d, a, b, x[ 6], 15, 0xa3014314); /* 59 */
 II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
 II(a, b, c, d, x[ 4], 6, 0xf7537e82); /* 61 */
 II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
 II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb); /* 63 */
 II(b, c, d, a, x[ 9], 21, 0xeb86d391); /* 64 */
     state[0] += a;
     state[1] += b;
     state[2] += c;
     state[3] += d;
}


#include <string.h>

const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char * BASE64::base64Encode( const unsigned char * bindata, int binlength, char * base64 )
{
    int i, j;
    unsigned char current;

    for ( i = 0, j = 0 ; i < binlength ; i += 3 )
    {
        current = (bindata[i] >> 2) ;
        current &= (unsigned char)0x3F;
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i] << 4 ) ) & ( (unsigned char)0x30 ) ;
        if ( i + 1 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+1] >> 4) ) & ( (unsigned char) 0x0F );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)(bindata[i+1] << 2) ) & ( (unsigned char)0x3C ) ;
        if ( i + 2 >= binlength )
        {
            base64[j++] = base64char[(int)current];
            base64[j++] = '=';
            break;
        }
        current |= ( (unsigned char)(bindata[i+2] >> 6) ) & ( (unsigned char) 0x03 );
        base64[j++] = base64char[(int)current];

        current = ( (unsigned char)bindata[i+2] ) & ( (unsigned char)0x3F ) ;
        base64[j++] = base64char[(int)current];
    }
    base64[j] = '\0';
    return base64;
}

int BASE64::base64Decode( const char * base64, unsigned char * bindata )
{
    int i, j;
    unsigned char k;
    unsigned char temp[4];
    for ( i = 0, j = 0; base64[i] != '\0' ; i += 4 )
    {
        memset( temp, 0xFF, sizeof(temp) );
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i] )
                temp[0]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+1] )
                temp[1]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+2] )
                temp[2]= k;
        }
        for ( k = 0 ; k < 64 ; k ++ )
        {
            if ( base64char[k] == base64[i+3] )
                temp[3]= k;
        }

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[0] << 2))&0xFC)) |
                ((unsigned char)((unsigned char)(temp[1]>>4)&0x03));
        if ( base64[i+2] == '=' )
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[1] << 4))&0xF0)) |
                ((unsigned char)((unsigned char)(temp[2]>>2)&0x0F));
        if ( base64[i+3] == '=' )
            break;

        bindata[j++] = ((unsigned char)(((unsigned char)(temp[2] << 6))&0xF0)) |
                ((unsigned char)(temp[3]&0x3F));
    }
    return j;
}


/*********************************************************
  data type definition for Des;
**********************************************************/
#define EN0	0
#define DE1	1

#define DES_KEYBYTES	128
#define DES_KEYLONGS	32
#define DES_BLOCKLEN	8

typedef struct {
	unsigned char ek[DES_KEYBYTES];
	int	ekLen;
	unsigned char dk[DES_KEYBYTES];
	int	dkLen;
	unsigned char CbcCtx[DES_BLOCKLEN];
} DES_CTX;

typedef struct {
	unsigned char ek1[DES_KEYBYTES];
	int	ek1Len;
	unsigned char dk1[DES_KEYBYTES];
	int	dk1Len;
	unsigned char ek2[DES_KEYBYTES];
	int	ek2Len;
	unsigned char dk2[DES_KEYBYTES];
	int	dk2Len;
	unsigned char CbcCtx[DES_BLOCKLEN];
	//int	IsFirstBlock;
} DES3_CTX;


static unsigned char pc1[56] = {
	56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3 };

static unsigned char pc2[48] = {
	13, 16, 10, 23,  0,  4,		 2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 	15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54,		29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 	45, 41, 49, 35, 28, 31 };

static unsigned short bytebit[8] = {0200,0100,040,020,010,04,02,01 };
static unsigned char totrot[16] = {1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28};
static unsigned long bigbyte[24] = {
	0x800000L,	0x400000L,	0x200000L,	0x100000L,
	0x80000L,	0x40000L,	0x20000L,	0x10000L,
	0x8000L,	0x4000L,	0x2000L,	0x1000L,
	0x800L,		0x400L,		0x200L,		0x100L,
	0x80L,		0x40L,		0x20L,		0x10L,
	0x8L,		0x4L,		0x2L,		0x1L	};

//insert digits
static unsigned long SP1[64] ={
       0x01010400l,0x00000000l,0x00010000l,0x01010404l,
       0x01010004l,0x00010404l,0x00000004l,0x00010000l,
       0x00000400l,0x01010400l,0x01010404l,0x00000400l,
       0x01000404l,0x01010004l,0x01000000l,0x00000004l,
       0x00000404l,0x01000400l,0x01000400l,0x00010400l,
       0x00010400l,0x01010000l,0x01010000l,0x01000404l,
       0x00010004l,0x01000004l,0x01000004l,0x00010004l,
       0x00000000l,0x00000404l,0x00010404l,0x01000000l,
       0x00010000l,0x01010404l,0x00000004l,0x01010000l,
       0x01010400l,0x01000000l,0x01000000l,0x00000400l,
       0x01010004l,0x00010000l,0x00010400l,0x01000004l,
       0x00000400l,0x00000004l,0x01000404l,0x00010404l,
       0x01010404l,0x00010004l,0x01010000l,0x01000404l,
       0x01000004l,0x00000404l,0x00010404l,0x01010400l,
       0x00000404l,0x01000400l,0x01000400l,0x00000000l,
       0x00010004l,0x00010400l,0x00000000l,0x01010004l };
       
       
static unsigned long SP2[64]={
       0x80108020l,0x80008000l,0x00008000l,0x00108020l,
       0x00100000l,0x00000020l,0x80100020l,0x80008020l,
       0x80000020l,0x80108020l,0x80108000l,0x80000000l,
       0x80008000l,0x00100000l,0x00000020l,0x80100020l,
       0x00108000l,0x00100020l,0x80008020l,0x00000000l,
       0x80000000l,0x00008000l,0x00108020l,0x80100000l,
       0x00100020l,0x80000020l,0x00000000l,0x00108000l,
       0x00008020l,0x80108000l,0x80100000l,0x00008020l,
       0x00000000l,0x00108020l,0x80100020l,0x00100000l,
       0x80008020l,0x80100000l,0x80108000l,0x00008000l,
       0x80100000l,0x80008000l,0x00000020l,0x80108020l,
       0x00108020l,0x00000020l,0x00008000l,0x80000000l,
       0x00008020l,0x80108000l,0x00100000l,0x80000020l,
       0x00100020l,0x80008020l,0x80000020l,0x00100020l,
       0x00108000l,0x00000000l,0x80008000l,0x00008020l,
       0x80000000l,0x80100020l,0x80108020l,0x00108000l };
       
       
static unsigned long SP3[64]={ 
       0x00000208l,0x08020200l,0x00000000l,0x08020008l,
       0x08000200l,0x00000000l,0x00020208l,0x08000200l,
       0x00020008l,0x08000008l,0x08000008l,0x00020000l,
       0x08020208l,0x00020008l,0x08020000l,0x00000208l,
       0x08000000l,0x00000008l,0x08020200l,0x00000200l,
       0x00020200l,0x08020000l,0x08020008l,0x00020208l,
       0x08000208l,0x00020200l,0x00020000l,0x08000208l,
       0x00000008l,0x08020208l,0x00000200l,0x08000000l,
       0x08020200l,0x08000000l,0x00020008l,0x00000208l,
       0x00020000l,0x08020200l,0x08000200l,0x00000000l,
       0x00000200l,0x00020008l,0x08020208l,0x08000200l,
       0x08000008l,0x00000200l,0x00000000l,0x08020008l,
       0x08000208l,0x00020000l,0x08000000l,0x08020208l,
       0x00000008l,0x00020208l,0x00020200l,0x08000008l,
       0x08020000l,0x08000208l,0x00000208l,0x08020000l,
       0x00020208l,0x00000008l,0x08020008l,0x00020200l };
       
       
static unsigned long SP4[64]={             
       0x00802001l,0x00002081l,0x00002081l,0x00000080l,
       0x00802080l,0x00800081l,0x00800001l,0x00002001l,
       0x00000000l,0x00802000l,0x00802000l,0x00802081l,
       0x00000081l,0x00000000l,0x00800080l,0x00800001l,
       0x00000001l,0x00002000l,0x00800000l,0x00802001l,
       0x00000080l,0x00800000l,0x00002001l,0x00002080l,
       0x00800081l,0x00000001l,0x00002080l,0x00800080l,
       0x00002000l,0x00802080l,0x00802081l,0x00000081l,
       0x00800080l,0x00800001l,0x00802000l,0x00802081l,
       0x00000081l,0x00000000l,0x00000000l,0x00802000l,
       0x00002080l,0x00800080l,0x00800081l,0x00000001l,
       0x00802001l,0x00002081l,0x00002081l,0x00000080l,
       0x00802081l,0x00000081l,0x00000001l,0x00002000l,
       0x00800001l,0x00002001l,0x00802080l,0x00800081l,
       0x00002001l,0x00002080l,0x00800000l,0x00802001l,
       0x00000080l,0x00800000l,0x00002000l,0x00802080l };
       
       
static unsigned long SP5[64]={   
       0x00000100l,0x02080100l,0x02080000l,0x42000100l,
       0x00080000l,0x00000100l,0x40000000l,0x02080000l,
       0x40080100l,0x00080000l,0x02000100l,0x40080100l,
       0x42000100l,0x42080000l,0x00080100l,0x40000000l,
       0x02000000l,0x40080000l,0x40080000l,0x00000000l,
       0x40000100l,0x42080100l,0x42080100l,0x02000100l,
       0x42080000l,0x40000100l,0x00000000l,0x42000000l,
       0x02080100l,0x02000000l,0x42000000l,0x00080100l,
       0x00080000l,0x42000100l,0x00000100l,0x02000000l,
       0x40000000l,0x02080000l,0x42000100l,0x40080100l,
       0x02000100l,0x40000000l,0x42080000l,0x02080100l,
       0x40080100l,0x00000100l,0x20000000l,0x42080000l,
       0x42080100l,0x00080100l,0x42000000l,0x42080100l,
       0x02080000l,0x02000100l,0x40000100l,0x00080000l,
       0x00080100l,0x02000100l,0x40000100l,0x00080000l,
       0x00000000l,0x40080000l,0x02080100l,0x40000100l };
       
       
static unsigned long SP6[64]={ 
       0x20000010l,0x20400000l,0x00004000l,0x20404010l,
       0x20400000l,0x00000010l,0x20404010l,0x00400000l,
       0x20004000l,0x00404010l,0x00400000l,0x20000010l,
       0x00400010l,0x20004000l,0x20000000l,0x00004010l,
       0x00000000l,0x00400010l,0x20004010l,0x00004000l,
       0x00404000l,0x20004010l,0x00000010l,0x20400010l,
       0x20400010l,0x00000000l,0x00404010l,0x20404000l,
       0x00004010l,0x00404000l,0x20404000l,0x20000000l,
       0x20004000l,0x00000010l,0x20400010l,0x00404000l,
       0x20404010l,0x00400000l,0x00004010l,0x20000010l,
       0x00400000l,0x20004000l,0x20000000l,0x00004010l,
       0x20000010l,0x20404010l,0x00404000l,0x20400000l,
       0x00404010l,0x20404000l,0x00000000l,0x20400010l,
       0x00000010l,0x00004000l,0x20400000l,0x00404010l,
       0x00004000l,0x00400010l,0x20004010l,0x00000000l,
       0x20404000l,0x20000000l,0x00400010l,0x20004010l };  
            
static unsigned long SP7[64] = {
	0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
	0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
	0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
	0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
	0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
	0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
	0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
	0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
	0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
	0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
	0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
	0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
	0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
	0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
	0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
	0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };
	
static unsigned long SP8[64] = {
	0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
	0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
	0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
	0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
	0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
	0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
	0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
	0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
	0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
	0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
	0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
	0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
	0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
	0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
	0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
	0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

void deskey(unsigned char *key,short edf, unsigned long *kn);
void cookey(register unsigned long *raw1, unsigned long *dough);
//void cpkey(register unsigned long *into);
//void usekey(register unsigned long *from);
//void des(unsigned char *inblock,unsigned char *outblock);
void scrunch(register unsigned char *outof, register unsigned long *into);
void unscrun(register unsigned long *outof, register unsigned char *into);
void desfunc(register unsigned long *block,register unsigned long *keys);

/*****************  DES Function  *****************/
unsigned long OPENCOMM_DesExpandEncKey(
		unsigned char *pbDesKey,
		unsigned long  ulDesKeyLen,
		unsigned char *pbDesEncKey,
		unsigned long *ulDesEncKeyLen);

unsigned long OPENCOMM_DesExpandDecKey(
		unsigned char *pbDesKey,
		unsigned long  ulDesKeyLen,
		unsigned char *pbDesDecKey,
		unsigned long *ulDesDecKeyLen);

unsigned long OPENCOMM_DesEncRaw(
		unsigned char *pbDesEncKey,
		unsigned long  ulDesEncKeyLen,
		unsigned char *pbInData,
		unsigned long  ulInDataLen,
		unsigned char *pbOutData,
		unsigned long *ulOutDataLen);

unsigned long OPENCOMM_DesDecRaw(
		unsigned char *pbDesDecKey,
		unsigned long  ulDesDecKeyLen,
		unsigned char *pbInData,
		unsigned long  ulInDataLen,
		unsigned char *pbOutData,
		unsigned long *ulOutDataLen);


int myic_DESDecrypt(
		unsigned char *pDesKey,
		int            nDesKeyLen,
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen);

int myic_DESEncrypt(
		unsigned char *pDesKey,
		int            nDesKeyLen,
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen);


void deskey(unsigned char *key,short edf, unsigned long *kn)
{
	register int i, j, l, m, n;
	unsigned long pc1m[56],pcr[56];
	
	
	for ( j = 0; j < 56; j++ ) 
	{
		l = pc1[j];
		m = l & 07;
		pc1m[j] = (((unsigned long) key[l >> 3] & (unsigned long)bytebit[m] ) ? 1:0);
	}
	for ( i = 0;i < 16; i++)
	{
		if ( edf == DE1 )	m = (15 - i) << 1;
		else	m = i << 1;
		n = m + 1;
		kn[m] = kn[n] = 0L;
		for ( j = 0; j < 28; j++ )
		{
			l = j + totrot[i];
			if ( l < 28 )	pcr[j] = pc1m[l];
			else	pcr[j] = pc1m[l-28];
		}
		for (j = 28; j < 56; j++ ) 
		{
			l = j + totrot[i];
			if ( l < 56 )	pcr[j] = pc1m[l];
			else	pcr[j] = pc1m[l-28];
		} 
		for ( j = 0; j < 24; j++ ) 
		{
			if ( pcr[pc2[j]] )	kn[m] |= bigbyte[j];
			if ( pcr[pc2[j+24]] )	kn[n] |= bigbyte[j];
		}
	}
	return;
}

void cookey(register unsigned long *raw1, unsigned long *dough)
{
	register unsigned long *cook,*raw0;
	register int i;
	
	cook = dough;
	for ( i = 0; i < 16; i++, raw1++ ) {
		raw0 = raw1++;
		*cook	 = (*raw0 & 0x00fc0000L) << 6;
		*cook	|= (*raw0 & 0x00000fc0L) << 10;
		*cook	|= (*raw1 & 0x00fc0000L) >> 10;
		*cook++	|= (*raw1 & 0x00000fc0L) >> 6;
		*cook	 = (*raw0 & 0x0003f000L) << 12;
		*cook	|= (*raw0 & 0x0000003fL) << 16;
		*cook	|= (*raw1 & 0x0003f000L) >> 4;
		*cook++	|= (*raw1 & 0x0000003fL);
	}
	return;
}

void scrunch(register unsigned char *outof, register unsigned long *into)
{
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into++	|= (*outof++ & 0xffL);
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into++	|= (*outof   & 0xffL);
	return;
}

void unscrun(register unsigned long *outof, register unsigned char *into)
{
	*into++	 = (unsigned char)((*outof >> 24) & 0xffL);
	*into++	 = (unsigned char)((*outof >> 16) & 0xffL);
	*into++	 = (unsigned char)((*outof >>  8) & 0xffL);
	*into++	 = (unsigned char)( *outof++	  & 0xffL);
	*into++	 = (unsigned char)((*outof >> 24) & 0xffL);
	*into++	 = (unsigned char)((*outof >> 16) & 0xffL);
	*into++	 = (unsigned char)((*outof >>  8) & 0xffL);
	*into	 = (unsigned char)( *outof		  & 0xffL);
	return;
}

void desfunc(register unsigned long *block,register unsigned long *keys)
{
	register unsigned long fval, work, right, leftt;
	register int round;
	
	leftt = block[0];
	right = block[1];
	work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
	
	right ^= work;
	leftt ^= (work << 4);
	work = ((leftt >> 16) ^ right) & 0x0000ffffL;
	
	right ^= work;
	leftt ^= (work << 16);
	work = ((right >> 2) ^ leftt) & 0x33333333L;
	
	leftt ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
	
	leftt ^= work;
	right ^= (work << 8);
	right = ((right << 1) | ((right >>31) & 1L)) & 0xffffffffL;
	work = (leftt ^ right) & 0xaaaaaaaaL;
	
	leftt ^= work;
	right ^= work;
	leftt = ((leftt << 1) | ((leftt >> 31)&1L)) & 0xffffffffL;
	
	for (round = 0; round < 8; round++) {
		work  = (right << 28) | (right >> 4);
		work ^= *keys++;
		fval  = SP7[ work	& 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = right ^ *keys++;
		fval |= SP8[ work 	& 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		leftt ^= fval;
		work  = (leftt << 28) | (leftt >> 4);
		work ^= *keys++;
		fval  = SP7[ work 	& 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = leftt ^ *keys++;
		fval |= SP8[ work 	& 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		right ^= fval;
	}
	
	right = (right << 31) | (right >> 1);
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = (leftt << 31) | (leftt >> 1);
	work = ((leftt >>  8) ^ right) & 0x00ff00ffL;
	right ^= work;
	leftt ^= (work << 8);
	work = ((leftt >>  2) ^ right) & 0x33333333L;
	right ^= work;
	leftt ^= (work << 2);
	work = ((right >> 16) ^ leftt) & 0x0000ffffL;
	leftt ^= work;
	right ^= (work << 16);
	work = ((right >>  4) ^ leftt) & 0x0f0f0f0fL;
	leftt ^= work;
	right ^= (work << 4);
	*block++ = right;
	*block = leftt;
	return;
}

/*****************************************************************
	OPENCOMM_DesExpandEncKey	: Expand Des Enc Key ??des????
	Return value:
		0         : Success
		other     : failed
	Parameters:
		pbDesKey        : ????DES??(8??)       input
		ulDesKeyLen     : ????DES????          input
		pbDesEncKey     : ????DES????(128??)  output
		*ulDesEncKeyLen : ????DES??????       output
*****************************************************************/
unsigned long OPENCOMM_DesExpandEncKey(
		unsigned char *pbDesKey,
		unsigned long  ulDesKeyLen,
		unsigned char *pbDesEncKey,
		unsigned long *ulDesEncKeyLen)
{
	unsigned long kn[32], dough[32];
	
	if (ulDesKeyLen != 8)
		return 0xEE20;

	deskey(pbDesKey, EN0, kn);
	cookey(kn, dough);
	*ulDesEncKeyLen = DES_KEYBYTES;  //32 long = 128 bytes
	memcpy(pbDesEncKey, dough, *ulDesEncKeyLen);
	
	return 0;
}

/*****************************************************************
	OPENCOMM_DesExpandDecKey	: Expand Des Dec Key ??des????
	Return value:
		0       : Success
		other   : failed
	Parameters:
		pbDesKey        : ????DES??(8??)      input
		ulDesKeyLen     : ????DES????         input
		pbDesDecKey     : ????DES????(128??) output
		*ulDesDecKeyLen : ????DES??????      output
*****************************************************************/
unsigned long OPENCOMM_DesExpandDecKey(
		unsigned char *pbDesKey,
		unsigned long  ulDesKeyLen,
		unsigned char *pbDesDecKey,
		unsigned long *ulDesDecKeyLen)
{
	unsigned long kn[32], dough[32];
	
	if (ulDesKeyLen != 8)
		return 0xEE20;

	deskey(pbDesKey, DE1, kn);
	cookey(kn, dough);
	*ulDesDecKeyLen = DES_KEYBYTES;  //32 long = 128 bytes
	memcpy(pbDesDecKey, dough, *ulDesDecKeyLen);
	
	return 0;
}

/****************************************************************
	OPENCOMM_DesEncRaw		: Des?????????8?? 
	Return value:
		0       : Success
		other   : failed
	Parameters:
		pbDesEncKey    : DES????    input
		ulDesEncKeyLen : DES?????? input
		pbInData       : ??????    input
		ulInDataLen    : ???????? input
		pbOutData      : ??????    output
		*ulOutDataLen  : ???????? output
*****************************************************************/
unsigned long OPENCOMM_DesEncRaw(
		unsigned char *pbDesEncKey,
		unsigned long  ulDesEncKeyLen,
		unsigned char *pbInData,
		unsigned long  ulInDataLen,
		unsigned char *pbOutData,
		unsigned long *ulOutDataLen)
{
	unsigned long work[2], ek[DES_KEYLONGS];
	unsigned char cp[DES_BLOCKLEN];

	if (ulInDataLen != DES_BLOCKLEN)
		return 0xEE20;
	
	if (ulDesEncKeyLen != DES_KEYBYTES)
		return 0xEE20;

	memcpy(cp, pbInData, DES_BLOCKLEN);
	scrunch(cp,work);  // 8 bytes -> 2 long
	memcpy(ek, pbDesEncKey, ulDesEncKeyLen);
	desfunc(work,ek);
	unscrun(work,cp); // 2 long -> 8 bytes
	memcpy(pbOutData, cp, DES_BLOCKLEN);
	*ulOutDataLen = DES_BLOCKLEN;

	return 0;
}

/*****************************************************************
	OPENCOMM_DesDecRaw : Des?????????8?? 
	Return value:
		0     : Success
		other : failed
	Parameters:
		pbDesDecKey    : DES????     input
		ulDesDecKeyLen : DES??????  input
		pbInData       : ??????     input
		ulInDataLen    : ????????  input
		pbOutData      : ??????     output
		*ulOutDataLen  : ????????  output
*****************************************************************/
unsigned long OPENCOMM_DesDecRaw(
		unsigned char *pbDesDecKey,
		unsigned long  ulDesDecKeyLen,
		unsigned char *pbInData,
		unsigned long  ulInDataLen,
		unsigned char *pbOutData,
		unsigned long *ulOutDataLen)
{
	unsigned long work[2], dk[DES_KEYLONGS];
	unsigned char cp[DES_BLOCKLEN];

	if (ulInDataLen != DES_BLOCKLEN)
		return 0xEE20;
	
	if (ulDesDecKeyLen != DES_KEYBYTES)
		return 0xEE20;

	memcpy(cp, pbInData, DES_BLOCKLEN);
	scrunch(cp,work);  // 8 bytes -> 2 long
	memcpy(dk, pbDesDecKey, ulDesDecKeyLen);
	desfunc(work,dk);
	unscrun(work,cp); // 2 long -> 8 bytes
	memcpy(pbOutData, cp, DES_BLOCKLEN);
//	des_enc(pbDesEncKey, pbInData, pbOutData);
	*ulOutDataLen = DES_BLOCKLEN;

	return 0;
}

/*********************   DES    *********************/

int myic_DESEncrypt(
		unsigned char *pDesKey,
		int            nDesKeyLen,
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen)
{
	unsigned char DesKeyBuf[32];
	unsigned char DesEncKeyBuf[128];
	int EncKeyLen, KeyLen = 0;
	int retval = 0, loops, i;
	
	if(nInDataLen%8 != 0)
		return 0xEE20;
	
	if(nDesKeyLen != 8)
		return 0xEE20;
	KeyLen = nDesKeyLen;
	memcpy(DesKeyBuf, pDesKey, nDesKeyLen);

	
	retval = OPENCOMM_DesExpandEncKey(DesKeyBuf, KeyLen,
		DesEncKeyBuf, (unsigned long *)&EncKeyLen);
	if(retval != 0)
		return retval;

	loops = nInDataLen/8;
	for(i = 0; i < loops; i++)
	{
		retval = OPENCOMM_DesEncRaw(DesEncKeyBuf, EncKeyLen, pInData + i*8,
			8, pOutData + i*8, (unsigned long *)pOutDataLen);
		if(retval != 0)
			return retval;
	}
	*pOutDataLen = nInDataLen;
	return retval;
}


int myic_DESDecrypt(
		unsigned char *pDesKey,
		int            nDesKeyLen,
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen)
{
	unsigned char DesKeyBuf[32];
	unsigned char DesDecKeyBuf[128];
	int DecKeyLen, KeyLen = 0;
	int retval = 0, loops, i;
	
	if(nInDataLen%8 != 0)
		return 0xEE20;
	
	if(nDesKeyLen != 8)
		return 0xEE20;
	KeyLen = nDesKeyLen;
	memcpy(DesKeyBuf, pDesKey, nDesKeyLen);

	retval = OPENCOMM_DesExpandDecKey(DesKeyBuf, KeyLen,
		DesDecKeyBuf, (unsigned long *)&DecKeyLen);
	if(retval != 0)
		return retval;
	
	loops = nInDataLen/8;
	for(i = 0; i < loops; i++)
	{
		retval = OPENCOMM_DesDecRaw(DesDecKeyBuf, DecKeyLen, pInData + i*8,
			8, pOutData + i*8, (unsigned long *)pOutDataLen);
		if(retval != 0)
			return retval;
	}
	*pOutDataLen = nInDataLen;
	return retval;
}


//???????pading
void  CW_dataPadAdd(int tag, unsigned char *date, unsigned int dateLen, 
					unsigned char **padDate, unsigned int *padDateLen)
{
	int           i, padLen;
	unsigned char *pTmp   = NULL;
	
	pTmp = (unsigned char *)malloc(dateLen+24);
	if (pTmp == NULL)
	{
		*padDate = NULL;
		return ;
	}
	memset(pTmp, 0, dateLen+24);
	memcpy(pTmp, date, dateLen);
	
	if (tag == 0)
	{
		padLen = 8 - dateLen % 8;
		for (i=0; i<padLen; i++)
		{
			pTmp[dateLen+i] = (char)padLen;
		}
		*padDateLen = dateLen + padLen;
	}
	else
	{
		padLen = 16 - dateLen % 16;
		for (i=0; i<padLen; i++)
		{
			pTmp[dateLen+i] = (char)padLen;
		}		
	}
	
	*padDateLen = dateLen + padLen;	
	*padDate = pTmp;
}

#define  USER_PASSWORD_KEY "abcd1234"


//????
int DesEncode::DesEnc(
		 unsigned char *pInData,
		 int            nInDataLen,
		 unsigned char *pOutData,
		 int           *pOutDataLen)
{
	int				rv;
	unsigned char	*padDate = NULL;
	unsigned int	padDateLen = 0;

	CW_dataPadAdd(0, pInData, (unsigned int )nInDataLen, &padDate, &padDateLen);

	rv = myic_DESEncrypt((unsigned char *)USER_PASSWORD_KEY, strlen(USER_PASSWORD_KEY),
		padDate, (int)padDateLen, pOutData, pOutDataLen);
	if (rv != 0)
	{
		if (padDate != NULL)
		{
			free(padDate);
		}
		return rv;	
	}

	if (padDate != NULL)
	{
		free(padDate);
	}
	return 0;
}


//????
int DesEncode::DesEnc_raw(
	unsigned char *pInData,
	int            nInDataLen,
	unsigned char *pOutData,
	int           *pOutDataLen)
{
	int				rv;
//	unsigned char	*padDate = NULL;
//	unsigned int	padDateLen = 0;

	rv = myic_DESEncrypt((unsigned char *)USER_PASSWORD_KEY, strlen(USER_PASSWORD_KEY),
		pInData, (int)nInDataLen, pOutData, pOutDataLen);
	if (rv != 0)
	{
		return rv;	
	}
	return 0;
}

//????????
#define  ERR_MALLOC 20
//??????8????, ???
#define  ERR_FILECONT 20


//??????des??
int DesEncode::DesDec(
		   unsigned char *pInData,
		   int            nInDataLen,
		   unsigned char *pOutData,
		   int           *pOutDataLen)
{
	int				rv;
	char			padChar;
	unsigned char 	*tmpPlain = NULL;
	
	tmpPlain =		(unsigned char *)malloc(nInDataLen+24);
	if (tmpPlain == NULL)
	{
		return ERR_MALLOC;
	}
	memset(tmpPlain, 0, nInDataLen+24);

	//??
	rv = myic_DESDecrypt((unsigned char *)USER_PASSWORD_KEY, strlen(USER_PASSWORD_KEY),
		pInData, nInDataLen, tmpPlain, pOutDataLen);
	if (rv != 0)
	{
		if (tmpPlain != NULL) free(tmpPlain);
		return rv;
	}

	//?pading
	padChar = tmpPlain[*pOutDataLen - 1];
	if ( (int)padChar<=0 || (int)padChar>8) //????
	{
		if (tmpPlain) free(tmpPlain);
		return ERR_FILECONT;
	}

	*pOutDataLen = *pOutDataLen - (int)padChar;	
	//memset(tmpPlain + *pOutDataLen, 0, (int)padChar);	
	memcpy(pOutData, tmpPlain, *pOutDataLen);
	if (tmpPlain) free(tmpPlain);	
	return 0;
}


//??????des??
int DesEncode::DesDec_raw(
	unsigned char *pInData,
	int            nInDataLen,
	unsigned char *pOutData,
	int           *pOutDataLen)
{
	int				rv;
	//char			padChar;
	//unsigned char 	*tmpPlain = NULL;

	/*
	tmpPlain =		(unsigned char *)malloc(nInDataLen+24);
	if (tmpPlain == NULL)
	{
		return ERR_MALLOC;
	}
	memset(tmpPlain, 0, nInDataLen+24);
	*/

	//??
	rv = myic_DESDecrypt((unsigned char *)USER_PASSWORD_KEY, strlen(USER_PASSWORD_KEY),
		pInData, nInDataLen, pOutData, pOutDataLen);
	if (rv != 0)
	{
		//if (tmpPlain != NULL) free(tmpPlain);
		return rv;
	}
	/*
	//?pading
	padChar = tmpPlain[*pOutDataLen - 1];
	if ( (int)padChar<=0 || (int)padChar>8) //????
	{
		if (tmpPlain) free(tmpPlain);
		return ERR_FILECONT;
	}

	*pOutDataLen = *pOutDataLen - (int)padChar;	
	//memset(tmpPlain + *pOutDataLen, 0, (int)padChar);	
	memcpy(pOutData, tmpPlain, *pOutDataLen);
	if (tmpPlain) free(tmpPlain);	
	*/
	return 0;
}






