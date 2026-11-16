#define LEDRED			(*((int*)0xC02200A0))
#define LEDBLUE			(*((int*)0xC0220000))
#define LEDON			0x46
#define LEDOFF			0x44

#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2
#define O_CREAT         0x200
#define O_TRUNC         0x400

#define SEEK_SET		0
#define SEEK_CUR		1
#define SEEK_END		2

#define STD_IN			0
#define STD_OUT			1
#define STD_ERR			2

// The firmware's own C library routines, at the addresses in funclist.txt.
extern int printf(const char* fmt, ...);
extern int sprintf(char* buf, const char* fmt, ...);
extern char* strcpy(char* dst, const char* src);
extern int strcmp(const char* a, const char* b);
extern unsigned int strlen(const char* s);

extern int FIO_OpenFile(const char* name, int flags, int mode);
extern int FIO_CreateFile(const char* name, int flags);
extern int FIO_RemoveFile(const char* name);
extern int FIO_ReadFile(int f, char* buffer, long size);
extern int FIO_WriteFile(int f, void* buf, long size);
extern int FIO_SeekFile(int f, long offset, int whence);
extern int FIO_CloseFile(int f);

extern void tickAnnounce(void);
extern void tickSet(unsigned long ticks);
extern unsigned long tickGet(void);

extern int CreateTask(const char* name, int prio, int stack_size /*?*/, void* entry, long parm /*?*/);
extern void SleepTask(long msec);
extern void ExitTask(void);

extern long eventproc_EdLedOff(void);
extern long eventproc_EdLedOn(void);
extern long eventproc_EdLedBlink(void);

extern void eventproc_RiseEvent(const char* name, int param);

extern int ReceiveMessageQueue(void* hMessageQueue, void* pMessage, int FOREVER);
extern int PostMessageQueue(void* hMessageQueue, void* pMessage, int FOREVER);

struct tm {
	int tm_sec;		// sec, 0-59
	int tm_min;		// min, 0-59
	int tm_hour;	// hour, 0-23
	int tm_mday;	// day of month, 1-31
	int tm_mon;		// month, 0-11
	int tm_year;	// year, year - 1900
	int tm_wday;	// day of week, 0-6
	int tm_yday;	// day of year, 0-365
	int tm_isdst;
};
typedef long time_t;
struct tm* localtime(const time_t* timer);

// 0 - AF beep
// 1 - Timer beep
extern void Beep(int mode);

/*typedef long FILE;

extern FILE* fopen(const char* file, const char* mode);
extern int fclose(FILE* fp);
extern int fwrite(const void* buf, long size, long count, FILE* fp);
extern int fread(void* buf, long size, long count, FILE* fp);

extern int open(const char* name, int flags, int mode);
extern int close(int fd);
extern int write(int fd, void* buffer, long nbytes);
extern int read(int fd, void* buffer, long nbytes);

extern void createHook (void *pNewTcb);
extern void deleteHook (void *pTcb);

extern int taskCreateHookAdd (void *createHook);
extern int taskDeleteHookAdd (void *deleteHook);

extern void* OpenLogFile(char* name);
extern void  CloseLogFile(void*);

extern void sprintf(char*, char*, ...);
*/
