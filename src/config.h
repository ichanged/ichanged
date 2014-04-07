#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <string.h>
#include <libconfig.h>

#define DEFAULT_FILE		"ichanged.cfg"

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
	int is_daemon;
	char *pidfile;
};

extern int ich_dirlen;
extern struct ich_config_t ich_cfg; 

extern void config_init();

#endif
