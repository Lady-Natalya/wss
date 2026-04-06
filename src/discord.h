#ifndef discord_h
	#define discord_h

	#define DISCORDSTATUSDELAYMS 30000

	bool discord_setup(void);

	void discord_begin(void);

	void discord_send_status(bool force);

	bool discord_get_last_status_failed(void);

#endif
