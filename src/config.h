#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <string.h>
#include <libconfig.h>

#define DEFAULT_FILE		"ichanged.cfg"

typedef int (*ich_cfg_int_pt)(const config_t *config, const char *path, int *value);
typedef int (*ich_cfg_str_pt)(const config_t *config, const char *path, 
		const char **value);

struct ich_config_t {
	char *dir;	
	uint32_t intvl;
	char **exclude;
	int hidden;
	long threshold;
	char *filename;
	char *log_path;
	int is_import;
	int is_export;
	char *pidfile;
};

struct ich_read_int_t {
	ich_cfg_int_pt func; 	
	char *path;
	int *value;
};

struct ich_read_str_t {
	ich_cfg_str_pt func;
	char *path;
	char **value;
};

extern int ich_dirlen;
extern int ich_daemon;
extern int ich_end;
extern struct ich_config_t ich_cfg; 

extern void config_init();

#endif
