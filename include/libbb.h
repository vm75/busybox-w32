/* vi: set sw=4 ts=4: */
/*
 * Busybox main internal header file
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Based in part on code from sash, Copyright (c) 1999 by David I. Bell 
 * Permission has been granted to redistribute this code under the GPL.
 *
 */
#ifndef	__LIBBB_H__
#define	__LIBBB_H__    1

#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include <features.h>

#if __GNU_LIBRARY__ < 5
/* libc5 doesn't define socklen_t */
typedef unsigned int socklen_t;
/* libc5 doesn't implement BSD 4.4 daemon() */
extern int daemon (int nochdir, int noclose);
#endif	

/* Some useful definitions */
#define FALSE   ((int) 0)
#define TRUE    ((int) 1)
#define SKIP	((int) 2)

/* for mtab.c */
#define MTAB_GETMOUNTPT '1'
#define MTAB_GETDEVICE  '2'

#define BUF_SIZE        8192
#define EXPAND_ALLOC    1024

static inline int is_decimal(ch) { return ((ch >= '0') && (ch <= '9')); }
static inline int is_octal(ch)   { return ((ch >= '0') && (ch <= '7')); }

/* Macros for min/max.  */
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif



extern void show_usage(void) __attribute__ ((noreturn));
extern void error_msg(const char *s, ...) __attribute__ ((format (printf, 1, 2)));
extern void error_msg_and_die(const char *s, ...) __attribute__ ((noreturn, format (printf, 1, 2)));
extern void perror_msg(const char *s, ...);
extern void perror_msg_and_die(const char *s, ...) __attribute__ ((noreturn));

/* These two are used internally -- you shouldn't need to use them */
extern void verror_msg(const char *s, va_list p);
extern void vperror_msg(const char *s, va_list p);

const char *mode_string(int mode);
const char *time_string(time_t timeVal);
int is_directory(const char *name, const int followLinks, struct stat *statBuf);
int isDevice(const char *name);

typedef struct ino_dev_hash_bucket_struct {
  struct ino_dev_hash_bucket_struct *next;
  ino_t ino;
  dev_t dev;
  char name[1];
} ino_dev_hashtable_bucket_t;
int is_in_ino_dev_hashtable(const struct stat *statbuf, char **name);
void add_to_ino_dev_hashtable(const struct stat *statbuf, const char *name);
void reset_ino_dev_hashtable(void);

int copy_file(const char *src_name, const char *dst_name,
		 int set_modes, int follow_links, int force_flag, int quiet_flag);
int copy_file_chunk(FILE *src_file, FILE *dst_file, off_t chunksize);
char *buildName(const char *dirName, const char *fileName);
int makeString(int argc, const char **argv, char *buf, int bufLen);
char *getChunk(int size);
char *chunkstrdup(const char *str);
void freeChunks(void);
ssize_t safe_read(int fd, void *buf, size_t count);
int full_write(int fd, const char *buf, int len);
int full_read(int fd, char *buf, int len);
int recursive_action(const char *fileName, int recurse, int followLinks, int depthFirst,
	  int (*fileAction) (const char *fileName, struct stat* statbuf, void* userData),
	  int (*dirAction) (const char *fileName, struct stat* statbuf, void* userData),
	  void* userData);

extern int create_path (const char *name, int mode);
extern int parse_mode( const char* s, mode_t* theMode);

extern int get_kernel_revision(void);

extern int get_console_fd(char* tty_name);
extern struct mntent *find_mount_point(const char *name, const char *table);
extern void write_mtab(char* blockDevice, char* directory, 
	char* filesystemType, long flags, char* string_flags);
extern void erase_mtab(const char * name);
extern void mtab_read(void);
extern char *mtab_first(void **iter);
extern char *mtab_next(void **iter);
extern char *mtab_getinfo(const char *match, const char which);
extern int check_wildcard_match(const char* text, const char* pattern);
extern long atoi_w_units (const char *cp);
extern pid_t* find_pid_by_name( char* pidName);
extern int find_real_root_device_name(char* name);
extern char *get_line_from_file(FILE *file);
extern void print_file(FILE *file);
extern int print_file_by_name(char *filename);
extern char process_escape_sequence(const char **ptr);
extern char *get_last_path_component(char *path);
extern FILE *wfopen(const char *path, const char *mode);
extern FILE *xfopen(const char *path, const char *mode);
extern void chomp(char *s);
extern void trim(char *s);
extern struct BB_applet *find_applet_by_name(const char *name);
void run_applet_by_name(const char *name, int argc, char **argv);

#ifndef DMALLOC
extern void *xmalloc (size_t size);
extern void *xrealloc(void *old, size_t size);
extern void *xcalloc(size_t nmemb, size_t size);
extern char *xstrdup (const char *s);
#endif
extern char *xstrndup (const char *s, int n);
extern char * safe_strncpy(char *dst, const char *src, size_t size);

struct suffix_mult {
	const char *suffix;
	int mult;
};

extern unsigned long parse_number(const char *numstr,
		const struct suffix_mult *suffixes);


/* These parse entries in /etc/passwd and /etc/group.  This is desirable
 * for BusyBox since we want to avoid using the glibc NSS stuff, which
 * increases target size and is often not needed embedded systems.  */
extern long my_getpwnam(const char *name);
extern long my_getgrnam(const char *name);
extern void my_getpwuid(char *name, long uid);
extern void my_getgrgid(char *group, long gid);
extern long my_getpwnamegid(const char *name);

extern int device_open(char *device, int mode);

extern int del_loop(const char *device);
extern int set_loop(const char *device, const char *file, int offset, int *loopro);
extern char *find_unused_loop_device (void);


#if (__GLIBC__ < 2)
extern int vdprintf(int d, const char *format, va_list ap);
#endif

int nfsmount(const char *spec, const char *node, int *flags,
	     char **extra_opts, char **mount_opts, int running_bg);

void syslog_msg_with_name(const char *name, int facility, int pri, const char *msg);
void syslog_msg(int facility, int pri, const char *msg);

/* Include our own copy of struct sysinfo to avoid binary compatability
 * problems with Linux 2.4, which changed things.  Grumble, grumble. */
struct sysinfo {
	long uptime;			/* Seconds since boot */
	unsigned long loads[3];		/* 1, 5, and 15 minute load averages */
	unsigned long totalram;		/* Total usable main memory size */
	unsigned long freeram;		/* Available memory size */
	unsigned long sharedram;	/* Amount of shared memory */
	unsigned long bufferram;	/* Memory used by buffers */
	unsigned long totalswap;	/* Total swap space size */
	unsigned long freeswap;		/* swap space still available */
	unsigned short procs;		/* Number of current processes */
	unsigned long totalhigh;	/* Total high memory size */
	unsigned long freehigh;		/* Available high memory size */
	unsigned int mem_unit;		/* Memory unit size in bytes */
	char _f[20-2*sizeof(long)-sizeof(int)];	/* Padding: libc5 uses this.. */
};
extern int sysinfo (struct sysinfo* info);

const char *make_human_readable_str(unsigned long val, unsigned long not_hr);
enum {
	KILOBYTE = 1024,
	MEGABYTE = (KILOBYTE*1024),
	GIGABYTE = (MEGABYTE*1024)
};

int ask_confirmation(void);
int klogctl(int type, char * b, int len);

char *xgetcwd(char *cwd);
char *concat_path_file(const char *path, const char *filename);

typedef struct ar_headers_s {
	char *name;
	off_t size;
	uid_t uid;
	gid_t gid;
	mode_t mode;
	time_t mtime;
	off_t offset;
	struct ar_headers_s *next;
} ar_headers_t;
extern ar_headers_t *get_ar_headers(FILE *in_file);
extern int seek_ared_file(FILE *in_file, ar_headers_t *headers, const char *tar_gz_file);
extern int deb_extract(const char *package_filename, const int function, char *target_dir);
extern int untar(FILE *src_tar_file, int untar_function, char *base_path);
extern int unzip(FILE *l_in_file, FILE *l_out_file);
extern void gz_close(int gunzip_pid);
extern int gz_open(FILE *compressed_file, int *pid);

#define CT_AUTO	0
#define CT_UNIX2DOS	1
#define CT_DOS2UNIX	2
extern int convert(char *fn, int ConvType);

#endif /* __LIBBB_H__ */
