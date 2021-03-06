#include "config.h"
#include "logger.h"
#include "options.h"

std::string config::_filename = "ichanged.cfg";

void
config::init()
{
	Config cfg;

	try {
		cfg.readFile(config::_filename.c_str());
		config::get_config();
	} catch (const FileIOException &x) {	
		config::write_config(cfg);	
	}

	return;
}

void
config::write_config(Config &cfg)
{
	std::vector<std::string>::iterator iter;

	Setting &root = cfg.getRoot();	
	root.add("log_path", Setting::TypeString) = "change.log";  		
	root.add("interval", Setting::TypeInt) = 5;
	root.add("directory", Setting::TypeString) = "/tmp";

	Setting &exclude = root.add("exclude", Setting::TypeArray);

	for (iter = options::exclude.begin(); iter != options::exclude.end();
			iter++) {
		exclude.add(*iter, Setting::TypeString);	
	}

	root.add("threshold", Setting::TypeInt) = 0;
	root.add("watch_hidden", Setting::TypeBoolean) = false;
	root.add("is_import", Setting::TypeBoolean) = false;
	root.add("is_export", Setting::TypeBoolean) = false;

	cfg.writeFile(config::_filename.c_str());
}

void
config::get_config()
{
	Config cfg;
	int interval;
	int threshold;
	bool watch_hidden, is_import, is_export;
	std::string log_path;
	std::string directory;

	try {
		cfg.readFile(config::_filename.c_str());	
	} catch (const FileIOException &x) {
		logger::fatal("[%s %d]I/O error while reading file.", __FILE__,
				__LINE__);
	} catch (const ParseException &x) {
		logger::fatal("[%s %d] Parse error at %s:%d - %s", __FILE__,
				__LINE__, x.getFile(), x.getLine(), x.getError());
	}

	if (cfg.lookupValue("log_path", log_path)) {
		if (log_path.size() >= 0) {
			options::log_path = log_path;  	
		}
	}
	if (cfg.lookupValue("interval", interval)) {
		if (interval >= 1) {
			options::interval = interval;	
		}
	}
	if (cfg.lookupValue("threshold", threshold)) {
		if (threshold >= 0) {
			options::threshold = threshold;
		}	
	}
	if (cfg.lookupValue("watch_hidden", watch_hidden)) {
		options::watch_hidden = watch_hidden;		
	}
	if (cfg.lookupValue("directory", directory)) {
		if (log_path.size() > 0) {
			options::directory = directory;
		}
	}
	if (cfg.lookupValue("is_import", is_import)) {
		options::is_import = is_import;	
	}
	if (cfg.lookupValue("is_export", is_export)) {
		options::is_export = is_export;
	}

   	try {
		Setting &ex_set = cfg.lookup("exclude");
		
		for(int i = 0; i < ex_set.getLength(); i++) {
			const char *s = ex_set[i];
			if (strlen(s) > 0) {
				options::exclude.push_back(std::string(s));
			}
		}
	} catch (const SettingNotFoundException &x) {
		// ignore	
	}
}

