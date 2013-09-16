#include <errno.h>
#include <sys/file.h>
#undef errno
extern int errno;
#define UART0_BASE   0x20201000
#define UART0_DR     (UART0_BASE+0x00)
#define UART0_RSRECR (UART0_BASE+0x04)
#define UART0_FR     (UART0_BASE+0x18)
#define UART0_ILPR   (UART0_BASE+0x20)
#define UART0_IBRD   (UART0_BASE+0x24)
#define UART0_FBRD   (UART0_BASE+0x28)
#define UART0_LCRH   (UART0_BASE+0x2C)
#define UART0_CR     (UART0_BASE+0x30)
#define UART0_IFLS   (UART0_BASE+0x34)
#define UART0_IMSC   (UART0_BASE+0x38)
#define UART0_RIS    (UART0_BASE+0x3C)
#define UART0_MIS    (UART0_BASE+0x40)
#define UART0_ICR    (UART0_BASE+0x44)
#define UART0_DMACR  (UART0_BASE+0x48)
#define UART0_ITCR   (UART0_BASE+0x80)
#define UART0_ITIP   (UART0_BASE+0x84)
#define UART0_ITOP   (UART0_BASE+0x88)
#define UART0_TDR    (UART0_BASE+0x8C)

extern "C" char heap_low; /* Defined by the linker */
extern "C" char heap_top; /* Defined by the linker */

extern "C" {

  int _write(int, char *, int);

  /**
   * @brief Opens a file
   * @note  Minimal implementation
   */
  int _open(const char *name, int flags, int mode)
  {
    return -1;
  }

  /**
   * @brief Close a file
   * @note  Minimal implementation
   */
  int _close(int file)
  {
    return -1;
  }

  /**
   * @brief End program execution with no cleanup processing
   */
  void _exit(int status)
  {
    _write(1, "exit\n\r", 6);
    while (true) {
    }
  }

  /**
   * @brief Status of an open file
   * @note  Minimal implementation
   * @note  All files are regarded as character special devices
   */
  int _fstat(int file, struct stat *st)
  {
    st->st_mode = S_IFCHR;
    return 0;
  }

  /**
   * @brief Return process ID
   * @note  Minimal implementation
   */
  int _getpid(void)
  {
    return 1;
  }

  /**
   * @brief Query whether output stream is a terminal.
   * @note Minimal implementation
   * @note Only support output to stdout
   */
  int _isatty(int file)
  {
    return 1;
  }

  /**
   * @brief Send a signal
   * @note  Minimal implementation
   */
  int _kill(int pid, int sig)
  {
    errno = EINVAL;
    return -1;
  }

  /**
   * @brief Set position in a file
   * @note  Minimal implementation
   */
  int _lseek(int file, int ptr, int dir)
  {
    return 0;
  }

  /**
   * @brief Read from a file
   * @note  Minimal implementation
   */
int _read(int file, char *ptr, int len)
{
	int read_count = len;
	if (len)
	{
		while(((*(volatile int *)(UART0_FR)) & 0x10));
		*ptr++ = (*(volatile int *)(UART0_DR));
		for (read_count = 0; read_count < len; read_count++)
		{
			if (((*(volatile int *)(UART0_FR)) & 0x10))
			{
				break;
			}
			*ptr++ = (*(volatile int *)(UART0_DR));
		}
		read_count = len;
	}
	return read_count;
}

char *heap_end = 0;
  /**
   * @brief Increase program space
   * @note  For an stand-alone system
   */
  caddr_t _sbrk(int incr)
  {

    char *prev_heap_end;

    if (heap_end == 0)
    {
    	heap_end = &heap_low;
    }
    prev_heap_end = heap_end;

    if ((heap_end + incr) > &heap_top)
    {
      _write(1, "Heap and stack collision\n\r", 26);
      _exit(0);
    }
    heap_end += incr;
    return (caddr_t) prev_heap_end;
  }

  /**
   * @brief Write to a file
   */
  int _write(int file, char *ptr, int len)
  {
    for (int i = 0; i < len; i++)
    {
        while(((*(volatile int *)(UART0_FR)) & 0x20));
        *(volatile int *)UART0_DR = *ptr++;
    }
    return len;
  }

} // extern "C"
