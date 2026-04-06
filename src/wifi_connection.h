#ifndef wifi_connection_h
	#define wifi_connection_h

	#define WIFISTATUSDELAYMS 30000
	#define WIFIRETRYCONNECTIONDELAYMS 60000
	#define WIFIDISCONNECTEDSTRING "WiFi Disconnected"

	bool wifi_setup(void);

	void wifi_connection_begin(void);

	bool get_wifi_configured(void);

	bool get_wifi_connected(void);

	String get_wifi_connection_status(void);

	void autoprint_wifi_connection_status(void);

	void wifi_retry_connection(void);

#endif
