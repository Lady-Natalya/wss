#ifndef wifi_connection_h
	#define wifi_connection_h

	#define WIFISTATUSDELAYMS 30000

	void wifi_connection_begin(void);

	String get_wifi_connection_status(void);

	void autoprint_wifi_connection_status(void);

#endif
