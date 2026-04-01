#ifndef preferences_manager_h
	#define preferences_manager_h

	void preferences_do_setup(void);

	bool preferences_get_bool(char *keyName);

	String preferences_get_string(char *keyName);

	uint32_t preferences_get_uint(char *keyName);

	void preferences_update_server_pref(char *keyName, String keyVal, String dataType);

	bool preferences_is_wifi_configured(void);

	bool preferences_check_discord_configured(void);

#endif
