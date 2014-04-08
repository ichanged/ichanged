#include "config.h"
#include "logger.h"
#include "options.h"

static struct ich_read_int_t ich_cfg_int[] = {
	{config_lookup_int, "interval", &ich_cfg->intvl},
	{config_lookup_int, "watch_hidden", &ich_cfg->hide},
	{config_lookup_int, "threshold", &ich_cfg->threshold},
	{config_lookup_int, "is_import", &ich_cfg->import},
	{config_lookup_int, "is_export", &ich_cfg->export},
	{config_lookup_int, "is_daemon", &ich_cfg->daemon}
};

static struct ich_read_str_t ich_cfg_str[] = {
	{config_lookup_string, "directory", &ich_cfg->dir},
	{config_lookup_string, "log_path", &ich_cfg->logfile},
	{config_lookup_string, "pidfile", &ich_cfg->pidfile}
};

void
config_init()
{
	int i, count;
	config_t cfg;	
	config_setting_t *set;
	struct ich_read_int_t *t;

	config_init(&);
	if (!config_read_file(&cfg, DEFAULT_FILE)) {
		SWS_log_fatal("%s", config_error_line(&cfg));
	}

	t = ich_cfg_int;
	for (i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
		if (t[i]->func(&cfg, t[i]->path, t[i]->value) < 0)	{
			SWS_log_error("[%s:%d], read %s of configuration error",
					__FILE__, __LINE__, config_error_line(&cfg));	
		}
	}

	t = ich_cfg_str;
	for (i = 0; i < sizeof(t) / sizeof(t[0]); i++) {
		if (t[i]->func(&cfg, t[i]->path, t[i]->value) < 0)	{
			SWS_log_error("[%s:%d], read %s of configuration error",
					__FILE__, __LINE__, config_error_line(&cfg));	
		}
	}
	
	set = config_lookup(&cfg, "exclude");  	
	count = config_setting_length;
	for (i = 0; i < count; i++) {
		ich_cfg->exclude[i] = config_setting_lookup_string(set, i);		
	}

}

//std::string config::_filename = "ichanged.cfg";
//
//void
//config::init()
//{
//	Config cfg;
//
//	try {
//		cfg.readFile(config::_filename.c_str());
//		config::get_config();
//	} catch (const FileIOException &x) {	
//		config::write_config(cfg);	
//	}
//
//	return;
//}
//
//void
//config::write_config(Config &cfg)
//{
//	std::vector<std::string>::iterator iter;
//
//	Setting &root = cfg.getRoot();	
//	root.add("log_path", Setting::TypeString) = "change.log";  		
//	root.add("interval", Setting::TypeInt) = 5;
//	root.add("directory", Setting::TypeString) = "/tmp";
//
//	Setting &exclude = root.add("exclude", Setting::TypeArray);
//
//	for (iter = options::exclude.begin(); iter != options::exclude.end();
//			iter++) {
//		exclude.add(*iter, Setting::TypeString);	
//	}
//
//	root.add("threshold", Setting::TypeInt) = 0;
//	root.add("watch_hidden", Setting::TypeBoolean) = false;
//	root.add("is_import", Setting::TypeBoolean) = false;
//	root.add("is_export", Setting::TypeBoolean) = false;
//
//	cfg.writeFile(config::_filename.c_str());
//}
//
//void
//config::get_config()
//{
//	Config cfg;
//	int interval;
//	int threshold;
//	bool watch_hidden, is_import, is_export;
//	std::string log_path, directory, pidfile;
//
//	try {
//		cfg.readFile(config::_filename.c_str());	
//	} catch (const FileIOException &x) {
//		logger::fatal("[%s %d]I/O error while reading file.", __FILE__,
//				__LINE__);
//	} catch (const ParseException &x) {
//		logger::fatal("[%s %d] Parse error at %s:%d - %s", __FILE__,
//				__LINE__, x.getFile(), x.getLine(), x.getError());
//	}
//
//	if (cfg.lookupValue("log_path", log_path)) {
//		if (log_path.size() >= 0) {
//			options::log_path = log_path;  	
//		}
//	}
//	if (cfg.lookupValue("interval", interval)) {
//		if (interval >= 1) {
//			options::interval = interval;	
//		}
//	}
//	if (cfg.lookupValue("threshold", threshold)) {
//		if (threshold >= 0) {
//			options::threshold = threshold;
//		}	
//	}
//	if (cfg.lookupValue("watch_hidden", watch_hidden)) {
//		options::watch_hidden = watch_hidden;		
//	}
//	if (cfg.lookupValue("directory", directory)) {
//		if (log_path.size() > 0) {
//			options::directory = directory;
//		}
//	}
//	if (cfg.lookupValue("is_import", is_import)) {
//		options::is_import = is_import;	
//	}
//	if (cfg.lookupValue("is_export", is_export)) {
//		options::is_export = is_export;
//	}
//	if (cfg.lookupValue("pidfile", pidfile)) {
//		options::pidfile = pidfile;
//	}
//
//   	try {
//		Setting &ex_set = cfg.lookup("exclude");
//		
//		for(int i = 0; i < ex_set.getLength(); i++) {
//			const char *s = ex_set[i];
//			if (strlen(s) > 0) {
//				options::exclude.push_back(std::string(s));
//			}
//		}
//	} catch (const SettingNotFoundException &x) {
//		// ignore	
//	}
//}

