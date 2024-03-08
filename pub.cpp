#include <iostream>
#include <string.h>
#include <mosquitto.h>

int main(){
	int rc;
	struct mosquitto * mosq;

	mosquitto_lib_init();
	mosq = mosquitto_new("publisher-test", true, NULL);
	rc = mosquitto_connect(mosq, "172.16.0.227", 1883, 60);
	if(rc != 0){
		std :: cout << "Client could not connect to broker! Error Code: %d\n" << rc;
		mosquitto_destroy(mosq);
		return -1;
	}
	std :: cout << "We are now connected to the broker!";
	std :: cout << std :: endl;
	char temp;
	std :: cout <<"enter: ";
	std :: cin >> temp;
	mosquitto_publish(mosq, NULL, "raspberry", strlen(&temp), &temp, 0, false);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	return 0;
}
