#include<cstdio>
#include<cstdlib>
#include<cstring>

#include "err_msg.h"

void 
usage()
{
    printf("Usage: more-- [Options] aFilename\n"
           "    Options:\n"
           "        -w : ``width'' of output text.\n"
           "        -h : ``height'' of output text.\n");    
}

int main(int argc, char* argv[])
{
    int optW = 0, optH = 0;

    if ( argc < 2 )
    {
        usage();
        exit(0);
    } else {
        for ( int K = 1; K < argc; ++K )
        {
            if ( 0 == strcmp( "-w", argv[K] ))
            {
                optW = atoi( argv[K + 1] );
                DBG_MSG( "width:%d\n", optW );
                K++;
                if ( optW < 0 )
                {
                    ERR_MSG( "Invaild width, set to 80\n", NULL );
                    optW = 80;
                }
                continue;
            } else if ( 0 == strcmp( "-h", argv[K] ) )
            {
                optH = atoi( argv[K + 1] );
                DBG_MSG( "height:%d\n", optH );
                if ( optH < 0 )
                {
                    ERR_MSG( "Invaild height, set to 24\n", NULL );
                    optH = 24;
                }
                K++;
                continue;
            }

        }
    }
}
