#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libconfig.h>

#define DEFAULT_FILE		"ichanged.cfg"

typedef int (*ich_cfg_int_pt)(const config_t *config, const char *path, int *value);
typedef int (*ich_cfg_str_pt)(const config_t *config, const char *path, 
		const char **value);

struct ich_config_t {
	const char *dir;	
	int intvl;
	const char *exclude[100];
	int hide;
	int threshold;
	const char *logfile;
	const char *pidfile;
	int import;
	int dexport;
};

struct ich_read_int_t {
	ich_cfg_int_pt func; 	
	char *path;
	int *value;
};

struct ich_read_str_t {
	ich_cfg_str_pt func;
	char *path;
	const char **value;
};

extern int ich_dirlen;
extern int ich_daemon;
extern int ich_end;
extern struct ich_config_t ich_cfg; 

extern void ich_config_init();

#endif
