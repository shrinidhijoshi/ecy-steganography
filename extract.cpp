#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "header.h"								// class's SecretImage and image

int main(int argc, char* argv[])
{
	image 	img ( argv[1] , 1 ) ;						// stego image
	
	uchar 	* sdata ;							// stego image data
	
	IplImage * imgac = cvCreateImage ( cvSize ( img.width , img.height ) , IPL_DEPTH_8U , 3 ) ;
	cvCvtColor ( img.getimage() , imgac , CV_YCrCb2BGR ) ;
	
	
	char 	ext[150000] , 
		sec_file [40] ,							// to store the data saving file name
		bit_num , 
		*img_prev , 
		*img_curr ;
	
	unsigned char pix_data , sum ;
	
	SecretImage sec_data ( argv[2] , "wb" ) ;
	
	int 	counter = -3 ,
		size = 0 ,
	    	i , j , k ,							// loop counters 
	    	char_num , 							// index to store message in ext
	    	mi , mi_1 , 							// previous and current extracted bits
	    	prev[3] , 							// to store the previous channel data
	    	threshold ;							// Threshold

	char_num=0;
	bit_num=3;
	
	printf("Enter the Threshold:	");
	scanf( "%d" , &threshold );
	cvNamedWindow( "Stego Image" , 1 );
	
	///////////////////Rotate///////////////////////////////////////////////////////////////////////////////////////////////////
	int Z ;
	printf("Enter the Number of pixels to rotate:	");
	scanf("%d",&Z);
	
	cvShowImage ( "Stego Image" , imgac ) ;
	cvWaitKey( 0 ) ;
	cvDestroyWindow ( "Stego Image " ) ;
	
	for( i = 0; i < Z * ( img.height / Z ); i += Z )
	{
		for( j = 0; j < Z * (img.width / Z ); j += Z)
		{	
			img.rotate ( Z , i , j );
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////Extract Data//////////////////////////////////////////////////////////////////////////////////////////
	
	sdata = ( uchar * ) img.getdata ( ) ;

	for ( i = 0; i < img.height ; i++)
	{
		for ( j = 1; j < img.width; j += 2)
		{	
			for ( k = 0 ;  k < img.nChannels ; k++ ) 
			{
				if ( k != 0 ) {
					
				pix_data = sdata[i * img.widthStep + j * img.nChannels + k]; 
				prev[k] = sdata[i * img.widthStep + ( j - 1 ) * img.nChannels + k];
				
				if( ( threshold <= ( pix_data - prev[k]) || -threshold >= (pix_data - prev[k] ) ) )
				{
					sum += ( ( ( prev[k] & 1 ) << ( bit_num * 2 + 1 ) ) ) + ( ( ( ( prev[k] >> 1 ) + pix_data ) & 1 ) << ( bit_num * 2 ) );
					bit_num--;
	
					if ( bit_num == -1 )
					{
						//ext[char_num] = sum;
						counter ++ ;
						
						if ( char_num < 3 ) 
						{
							size <<= 8 ;
							size += sum ;
						}
						else
							fputc ( sum , sec_data.getpointer ( ) ) ;
						
						if ( counter == size )
						{
							i = img.height + 1 ;
							j = img.width + 1 ;
							k = img.nChannels + 1 ;
						}
						
						sum = 0 ;
						bit_num = 3 ;
						char_num++ ;
					}
				}
				}
			}
		}	
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	printf ( "\n\nTotal length of message :\t%d\n\n", size ) ;
	
	return 0;
}
	
