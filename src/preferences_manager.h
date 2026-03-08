#ifndef preferences_manager_h
	#define preferences_manager_h

	void preferences_do_setup(void);

	String preferences_get_string(char *keyName);

	void preferences_update_server_pref(char *keyName, String keyVal);

#endif
