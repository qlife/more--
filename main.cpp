#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cassert>
#include<vector>
#include<list>
#include<string>
#include<ncurses.h>
#include"err_msg.h"


static const char sym_end = '\\';
 
void 
readFile(const char* fileName,
         std::list<std::string>& lines,
         std::vector< std::list<std::string>::const_iterator >& pageHdrs,
         const int maxWidth,
         const int maxHeight,
         int* nlines)
{
    FILE *fp = NULL;
    char *buf = new char[ 4096 ];
    int nbytes = 0, cnrt_lines = 0;

    lines.clear();
    pageHdrs.clear();

    if ( NULL == (fp = fopen( fileName, "r" )) )
    {
        ERR_MSG( "Open File %s failed.\n", fileName );
        exit(-1);
    }

    while( 0 < (nbytes = fread( (void*) buf, sizeof(char), 4096, fp )))
    {
        int K, ptr = 0;
        
        for ( K = 0; K < nbytes; ++K )
        {
            if ( '\n' == buf[K] || maxWidth == ( K - ptr + 1 ) )
            {
                std::string stemp( &(buf[ptr]), (K - ptr + 1) );
                lines.push_back( stemp );

                if ( 0 == ( cnrt_lines % maxHeight ) )
                {
                    std::list<std::string>::const_iterator citer = lines.end();
                    --citer;
                    pageHdrs.push_back( citer );
                }
                cnrt_lines ++;
                ptr = K + 1;
            }
        }
    }
    
    if ( NULL != nlines )
        *nlines = cnrt_lines;

    delete [] buf;
}


void 
usage()
{
    printf("Usage: more-- [Options] aFilename\n"
           "    Options:\n"
           "        -w : ``width'' of output text.\n"
           "        -h : ``height'' of output text.\n");    
}

void
updatePageNo(WINDOW *bar, int pageNo)
{
    werase( bar );
    mvwprintw( bar, 1, 1, "Page %d", pageNo );
    box( bar, 0, 0 );
    wrefresh( bar );
}

int 
main(int argc, char* argv[])
{
    int optW = 80, optH = 20, nlines = 0;
    FILE *fp = NULL;
    char fname[256];
    std::list<std::string> lines;
    std::vector< std::list<std::string>::const_iterator > hdrs;

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
                K++;
                if ( optW < 0 )
                {
                    ERR_MSG( "Invalid width, set to 80\n", NULL );
                    optW = 80;
                }
                continue;
            } else if ( 0 == strcmp( "-h", argv[K] ) )
            {
                optH = atoi( argv[K + 1] );
                if ( optH < 0 )
                {
                    ERR_MSG( "Invalid height, set to 24\n", NULL );
                    optH = 24;
                }
                K++;
                continue;
            } else {
                strncpy( fname, argv[K], sizeof(fname) );
            }

        }
    }

    WINDOW* win = NULL, *top_bar = NULL;
    int maxW, maxH, crnt_page = 0;
    int actualWidth, actualHeight;
    char ch = '\0';

    // Init ncurses library
    initscr();
    cbreak();

    keypad( stdscr, TRUE );
    noecho();

    // Detect max screen width and screen height
    getmaxyx( stdscr, maxH, maxW );
    if ( maxH < optH )
        optH = maxH;

    if ( maxW < optW )
        optW = maxW;

    actualWidth = optW - 2;
    actualHeight = optH - 2;

    readFile( fname, lines, hdrs, actualWidth, actualHeight , &nlines );

    refresh(); // This call of refresh seems required.

    // Create top bar
    top_bar = newwin(3, optW, 0, 0 );
    box( top_bar, 0, 0 );
    mvwprintw( top_bar, 1, 1, "Page 0" );
    wrefresh( top_bar );

    // Create window
    win = newwin( optH, optW, 3, 0 );
    box( win, 0, 0 );

    // Paint a string
    wattron( win, A_BOLD );
    mvwprintw( win, 1, 1, "Press n to next page, p to previous page\n" );
    wattroff( win, A_BOLD );
    wrefresh( win );

    std::vector< std::list<std::string>::const_iterator >::const_iterator iter = hdrs.begin();

    while ( 'q' != ( ch = getch() ) )
    {
        switch ( ch )
        {
        case 'n':
            if ( iter != hdrs.end() )
            {
                ++crnt_page;
		updatePageNo( top_bar, crnt_page );
		werase( win );
		wrefresh( win );

                std::list< std::string >::const_iterator list_iter = (*iter);
                for ( int K = 0 ; K < actualHeight && list_iter != lines.end() ; ++ K, ++list_iter )
                {
                    mvwprintw( win, 1 + K, 1, (*list_iter).c_str(), NULL );
                }
                ++iter;
		box( win, 0, 0 );
                wrefresh( win );
            }
            break;
        case 'p':
            if ( iter > hdrs.begin() )
            {
                --crnt_page;
		updatePageNo( top_bar, crnt_page );

		werase( win );
                wrefresh( win );

                --iter;
                std::list< std::string >::const_iterator list_iter = (*iter);
                for ( int K = 0; K < actualHeight && list_iter != lines.end() ; ++K, ++list_iter )
                {
                    mvwprintw( win, 1 + K, 1, (*list_iter).c_str(), NULL );
                }
		box( win, 0, 0 );
                wrefresh( win );
            }
            break;
        default:
            NULL;
            break;
        }
    }

    // cleanup
    delwin( top_bar );
    delwin( win );
    endwin();
    return 0;
}
