#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "posix_ringbuf.h"

// Tuning
static const int read_usleep = 1;
static const int write_usleep = 1;
static const int verbose = 2;
// #define USE_POSIX

class Dummy
{
public:
    int serialNumber;
    double doubleMember;
    char stringMember[32];
};

#ifdef USE_POSIX
static Posix_Ringbuf<Dummy> rbuf (11);
#else
static Ringbuf<Dummy> rbuf (11);
#endif

static void* Reader (void* arg);
static void* Writer (void* arg);

int main ()
{
    pthread_t hReader, hWriter;

    if (pthread_create (&hReader, NULL, Reader, NULL) != 0)
    {
        std::cerr << "pthread_create failed (1)" << std::endl;
	return 1;
    }
    if (pthread_create (&hWriter, NULL, Writer, NULL) != 0)
    {
        std::cerr << "pthread_create failed (2)" << std::endl;
	return 1;
    }
    
    sleep (30);
    if (pthread_cancel (hWriter) != 0)
    {
        std::cerr << "pthread_cancel failed (1)" << std::endl;
	return 1;
    }
    if (pthread_cancel (hReader) != 0)
    {
        std::cerr << "pthread_cancel failed (2)" << std::endl;
	return 1;
    }

    return 0;
}

static void* Writer (void* arg)
{
    static __thread int serial = 0;

    while (true)
    {
	usleep (write_usleep);
        Dummy dummy;
        dummy.serialNumber = ++serial;
        try
        {
            rbuf << dummy;
        }
        catch (RingbufFullException)
        {
	    if (verbose >= 2)
		std::cout << "Write failure " << serial << std::endl;
            --serial;
            continue;
        }
	if (verbose >= 3)
	    std::cout << "Put " << serial << std::endl;
    }
    
    return NULL;
}


static void* Reader (void* arg)
{
    static __thread int serial = 0;

    while (true)
    {
        usleep (read_usleep);
	++serial;
        Dummy dummy;
        try
        {
            rbuf >> dummy;
        }
        catch (RingbufEmptyException)
        {
	    if (verbose >= 2)
		std::cout << "Read failure " << serial << std::endl;
            --serial;
            continue;
        }
	if (dummy.serialNumber != serial)
	{
	    if (verbose >= 1)
	    {
		std::cout << "*** ERROR *** ";
		std::cout << "Pop " << serial << std::endl;
		exit (1);
	    }
	}
	else
	{
	    if (verbose >= 3)
		std::cout << "Pop " << serial << std::endl;
	}
    }

    return NULL;
}
