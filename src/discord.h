#ifndef discord_h
	#define discord_h

	#define DISCORDSTATUSDELAYMS 30000

	void discord_set_configuration_status(bool configured);

	void discord_check_if_enabled(void);

	void discord_begin(void);

	void discord_send_status(void);

#endif
