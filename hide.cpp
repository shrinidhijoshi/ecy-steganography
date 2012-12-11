#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

class SecretImage {
	
	private : 
		FILE * fp ;
		int length ;
	public :
			
		SecretImage ( char * location , const char * mode )
		{
			char loc [40] ;
				
			if ( location == NULL )
			{
				printf ( "Enter the location of file :\t" ) ;
				scanf ( "%s" , loc ) ;
				fp = fopen ( loc , mode ) ;
			}
				
			else
				fp = fopen ( location , mode ) ;
					
			if ( fp == NULL ) 
			{
				printf ( "\n\n\tCouldn't find the file\n\n " );
				exit ( 0 ) ;
			}
				
				if ( *loc != 'w' || *location != 'w' ) 
				{
					fseek ( fp , 0L , SEEK_END ) ;
					length = ftell ( fp ) ; 
					rewind ( fp ) ;
				}
			}
			
			~ SecretImage ( )
			{
				fclose ( fp ) ;
			}
			
			int getlength ( )
			{
				return length ;
			} 
			
		void putdatain ( char * msg )
		{
				char ch ;
				int i = 2 ;				// 24bits to store the size
					
				while ( i++ < length + 3 )
				{
					ch = fgetc ( fp ) ;
					msg[i] = ch;
				}
				
				i = length ;
				msg[2] = ( 1 << 8 ) - 1 & i ;
				msg[1] = ( 1 << 8 ) - 1 & ( i >> 8 ) ;
				msg[0] = ( 1 << 8 ) - 1 & ( i >> 16 ) ;

		}	 
			
		FILE * getpointer ( ) 
		{
			return fp ;
		}
			
} ;


void rotate ( IplImage * img ,int size , int x , int y) 
			{
	    			int N = size;
    				uchar* data = ( uchar* ) img->imageData;
	    			
	    			for( int i = 0; i < N/2; ++i ) {
	        
	        			for( int j = 0; j < ( N + 1 ) / 2; ++j ) {
	        
	       				 	for(int k = 0; k < ( img->nChannels ); k++ ) {
	     				       		int temp = data[ ( i + x ) * ( img->widthStep ) + ( j + y ) * ( img->nChannels ) + k ] ;
							data[ ( i + x ) * img->widthStep + ( j + y ) * img->nChannels + k] = data[ ( N - 1 - ( j ) + x ) * img->widthStep + ( i + y ) * img->nChannels+k];
							data[ ( N - 1 - ( j ) + x ) * img->widthStep + ( i + y ) * img->nChannels + k ] = data[ ( N - 1 - ( i ) + x ) * img->widthStep + ( N - 1 - ( j ) + y ) * img->nChannels + k ];
							data[ ( N - 1 - ( i ) + x ) * img->widthStep + ( N - 1 - ( j ) + y ) * img->nChannels + k ] = data[( j + x ) * img->widthStep + ( N - 1 - ( i ) + y ) * img->nChannels + k ];
							data[ ( j + x ) * img->widthStep + ( N - 1 - ( i ) + y ) * img->nChannels + k ] = temp;
	     					   }
	    				}
				}
			}
			
			void rotateBack(IplImage * img , int size , int x , int y ) 
			{
	    			int N = size;
    				uchar* data = (uchar*) img->imageData;
	    
	    			for( int i = 0; i < N/2; ++i ) {
	    			
	    			    for( int j = 0; j < ( N + 1 ) / 2 ; ++j ) {
	    			    
	    				    	for( int k = 0 ; k < ( img->nChannels ) ; k++ ) {
	    				    	
	    			        		int temp = data[( i + x ) * ( img->widthStep ) + ( j + y ) * ( img->nChannels ) + k ] ;
							data[ ( i + x ) * img->widthStep + ( j + y ) * img->nChannels + k ] = data[ ( j + x ) * img->widthStep + ( N - 1 - ( i ) + y ) * img->nChannels + k ];
							data[ ( j + x ) * img->widthStep + ( N - 1 - ( i ) + y ) * img->nChannels + k ] = data[( N - 1 - ( i ) + x ) * img->widthStep + ( N - 1 - ( j ) + y) * img->nChannels + k ];
							data[(N-1-(i)+x)*img->widthStep+(N-1-(j)+y)*img->nChannels+k]= data[(N-1-(j)+x)*img->widthStep+(i+y)*img->nChannels+k];
							data[(N-1-(j)+x)*img->widthStep+(i+y)*img->nChannels+k] = temp;
	   		      		    	}
	    			    }
				}
			}
			

int main(int argc, char* argv[])
{
	
	IplImage * img = cvLoadImage ( argv[1], 1 ) ;
	IplImage * mod4 = cvLoadImage ( argv[1], 1 ) ;
	IplImage * imgy = cvCreateImage ( cvSize ( img->width , img->height ) , IPL_DEPTH_8U , 3 ) ;
	IplImage * out = cvCreateImage ( cvSize ( img->width , img->height ) , IPL_DEPTH_8U , 3 ) ;
	
	
	cvCvtColor ( img , imgy , CV_BGR2YCrCb ) ;
	cvCvtColor ( img , out , CV_BGR2YCrCb ) ;

	//cvCvtColor ( img , mod4 , CV_BGR2YCrCb ) ;
	//cvCvtColor ( mod4 , mod4 ,CV_YCrCb2RGB);


		
	uchar	* data ,						// to store ori	
			* sdata ,
			* mod4data;
	char 	msg[500000] ,						// Secret message string
		bit_num    ;						// counter ( 0 - 3 ) for extracting 2 bits / count from message
		
	
	int 	prev[3]   ,						// to store the r,g,b values of previous pixel
		threshold , 						// Threshold
		count = 0 , 						// to count the interested pixels
		Z ;							// number of pixel to rotate
	
	unsigned char pix_data ,					
		      edges    ,					// flag to show or dont show edges 
		      subs 	,
		      * img_prev ,					// pointer to previous pixel value
		      * img_curr ;					// pointer to current pixel value
	     					
		 
	int 	size , 							// secret message/image size
		capacity   , 						// to store the total embedding capacity of image
		i , j , k  , 						// loop variables
		char_num   , 						// index variable of secret message ( current embedding byte )
		mi , mi_1  , 						// previous and current pixel values for comparison ( LSMBR )
		flag = 0   ;						// flag to end the edge detection after data hiding finished
	
	
	SecretImage sec_msg ( argv[2] , "rb" ) ;//, store ( "store.txt" , "w" );					// file of secret message
			
	printf ( "Do you want to see the Edges of image (Y or N ?) :	" ) ;
	scanf ( "%c" , &edges ) ;
	
		

	if ( edges == 'y' || edges == 'Y' )
		cvNamedWindow("Edges",1);
	 
	///////////////////Rotate///////////////////////////////////////////////////////////////////////////////////////////////////
	
	printf("Enter the Number of pixels to rotate:	");
	scanf ( "%d" , &Z ) ;
			
	for(i=0;i<Z*(out->height/Z);i+=Z)
	{
		for(j=0;j<Z*(out->width/Z);j+=Z)
		{	
			rotate(imgy,Z,i,j);	
			rotate(out,Z,i,j);
		}
	}
	
	
	////////////////////Detect edges//////////////////////////////////////////////////////////////////////////////////////////////////	
	
	data = (uchar*) out->imageData ;
	sdata = ( uchar* ) imgy->imageData ;
	mod4data= ( uchar*) mod4->imageData;
	printf ( "Enter the Threshold value: 	") ;
	scanf ( "%d" , &threshold ) ;
	
	
	/*
	printf("Enter the secret Message:	");
	scanf("%s",msg);
	size = strlen(msg);
	*/
	
	sec_msg.putdatain ( msg ) ;
	size = sec_msg.getlength ( ) ;
	size += 3 ;				
		
	char_num = 0 ;
	bit_num = 3 ;


////////////////////////////////////////Hide the data/////////////////////////////////////////////////////////////////////////////////////////
	int mod4_value,to_add;

	int dum,c255=0;
	int mod4_loops=10;
	for(mod4_loops=0;mod4_loops<10;mod4_loops++){
		for( i = 0; i < mod4->height ; i++ )
		{
			for( j = 0; j < mod4->width; j ++)
			{
				for( k = 0; k < mod4->nChannels; k++ )
				{

					///mod4 the image
					
					mod4_value = mod4data[i * mod4->widthStep + j * mod4->nChannels + k] % 4; 
					if(mod4data[i * mod4->widthStep + j * mod4->nChannels + k] ==255){

						//cin>>dum;
					}
					switch(mod4_value){

						case 0:
							to_add=0;
							break;
						case 1:
							to_add=-1;
							break;
						case 2:
							if(mod4data[i * mod4->widthStep + j * mod4->nChannels + k]  == 254)
								to_add=-2;
							else
								to_add=2;
							break;
						case 3:
							if(mod4data[i * mod4->widthStep + j * mod4->nChannels + k] == 255)
								to_add=-3;
							else
								to_add=1;
							break;
						default:
						;
					}

					mod4data[i * mod4->widthStep + j * mod4->nChannels + k] += to_add; 
					//cout<<(int)mod4data[i * mod4->widthStep + j * mod4->nChannels + k]<<endl;
					if(mod4data[i * mod4->widthStep + j * mod4->nChannels + k] == 255){
						c255++;
					}
				}
			}
		}
		//cout<<"255 count = "<<c255<<endl;
	}

   	for( i = 0; i < out->height ; i++ )
	{
		for( j = 1; j < out->width; j += 2 )
		{
			for( k = 0; k < out->nChannels; k++ )
			{

			
				if ( k != 0 ) {
				
					pix_data = data[i * out->widthStep + j * out->nChannels + k]; 
					prev[k] = data[i * out->widthStep + ( j - 1 ) * out->nChannels + k];	


					if( ( threshold <= ( pix_data - prev[k]) || -threshold >= (pix_data - prev[k] ) ) )
					{
						count++;
						
						if( flag == 0 )
						{
							if ( bit_num == -1 )
							{
								bit_num = 3;
								char_num++;
							}		
							mi_1 = msg[char_num] >> bit_num * 2 & 1;		
							mi = msg[char_num] >> ( bit_num * 2 + 1 ) & 1;
							bit_num--;
							
							img_prev = ( unsigned char* ) &sdata[i * out->widthStep + (j-1) * out->nChannels + k];
							img_curr = ( unsigned char* ) &sdata[i * out->widthStep + j * out->nChannels + k];
						
							if ( (prev[k] &	 1) == mi && (1 & ( (prev[k] >> 1) +  pix_data )) != mi_1 )
							{
									sdata[i * out->widthStep + j * out->nChannels + k] += 1;	
							}
						
							else if ( (prev[k] & 1) != mi && (1 & ( (( prev[k] - 1 ) >> 1) +  pix_data )) == mi_1 )
							{
								sdata[i * out->widthStep + (j-1) * out->nChannels + k ] -= 1;
							}
						
							else if ( ( prev[k] & 1 ) != mi && (1 & ( (( prev[k] - 1 ) >> 1) +  pix_data )) != mi_1 )
							{
									sdata[i * out->widthStep + (j-1) * out->nChannels + k ] += 1;
							}
						
							if ( abs( *img_curr - *img_prev ) < threshold )
							{	
								if ( *img_curr > *img_prev )
								{
						
									if ( *img_curr + 2 < 255 )			
										*img_curr += 2;
									else
										*img_prev -= 4;
								}
							
								else
								{
									if( *img_curr - 2 > 0 )
										*img_curr -= 2;
									else
										*img_prev += 4;
								} 
							}
					}
					
				
/////////////////////////////////  Processing for edge detection  //////////////////////////////////////////////////////////////////////
					
					data[i * out->widthStep + j * out->nChannels + k] = 255;	
				}
				else
					data[i * out->widthStep + j * out->nChannels + k] = data[i * out->widthStep + ( j - 1 ) * out->nChannels + k] = 0;		
							
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					


				if ( bit_num == -1 && char_num == size && flag == 0  )		// break all 3 loops
				{
					if ( edges == 'y' || edges == 'Y' )
					{
						flag = 1;
					}
					else
					{
						i = out->height + 1;
						j = out->width + 1;
						k = out->nChannels + 1;
					}
				}
				}//////////////
			}
		}
	}
	

//////////////////////////////////////////hiding ends///////////////////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////Rotate image to its original form/////////////////////////////////////////////////////////////////	

	for( i = 0; i < Z * ( out->height / Z ); i += Z )
	{
		for( j = 0; j < Z * ( out->width / Z ); j += Z)
		{	
			rotateBack (imgy, Z , i , j );
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	printf (" \nMaximum encoding capacity for this image on given threshold T = %d :    %d bytes" , threshold , count / 4 );
	printf (" \nLength of Secret Message :	%d\t\n\n" , size , msg[0]*256*256+msg[1]*256+msg[2]);
	
	if ( edges == 'y' || edges == 'Y' )
	{
		cvShowImage ( "Edges",out ) ;
		cvWaitKey ( 0 ) ;
		cvDestroyWindow ( "Edges" ) ;	
	}
	
	printf("Enter the Stego Image name to save:	");
	scanf( "%s" , msg );
//	strcat( msg , ".png" );

	cvSaveImage ( msg , imgy );					//save stego image



	cvNamedWindow ( "Main Image" , 1 );
	cvShowImage ( "Main Image",img ) ;

	cvNamedWindow ( "Mod4 Image" , 1 );
	cvShowImage ( "Mod4 Image",mod4 ) ;

	cvNamedWindow ( "ycrcb window" , 1 ) ;
	cvShowImage ( "ycrcb window" , imgy ) ;


	cvWaitKey ( 0 ) ;

	cvDestroyWindow ( "ycrcb window" ) ;
	cvDestroyWindow ( "Mod4 Image" ) ;
	cvDestroyWindow ( "Main Image" ) ;

	return 0;
}