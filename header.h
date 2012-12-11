
class image {
	
	public :
			IplImage *img ;
			int width , height , widthStep , nChannels ;

			image ( char * location , int color)
			{
				char loc [40] ;
				if ( location == NULL )
				{
					printf ( "Please enter the location of the Image :\t" ) ;
					scanf ( "%s" , loc ) ;
					img = cvLoadImage ( loc , color ) ; 
				}
				
				else
					img = cvLoadImage ( location , color ) ; 
				
					 
				
				if ( img == NULL )
				{
					printf ( "\n\tCant find Image\n\n" ) ;
					exit ( 0 ) ;
				}
				 
				width = img->width ;
				height = img->height ;
				widthStep = img->widthStep ;
				nChannels = img->nChannels ;				
			}
			
			~ image ( )
			{
				cvReleaseImage ( &img ) ;
			}
			
			void show ( const char * window )
			{
				cvShowImage ( window , img ) ;
				cvWaitKey ( 0 ) ;
				cvDestroyWindow ( window ) ;
			}	
			
			void rotate ( int size , int x , int y) 
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
			
			void rotateBack( int size , int x , int y ) 
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
			
			uchar * getdata ( )
			{
				return ( uchar * ) img->imageData ;
			}
			
			IplImage * getimage ( )
			{
				return ( IplImage * ) img ; 
			}
			
	} ;


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
			
		/*	void putdatafrom ( char * msg ) 
			{
				char ch;
				int i = 0 , count = 0 ;
				
				while ( 1 ) 
				{
					ch = fgetc (  fp 
					fputc
			}
		*/		
	} ;


