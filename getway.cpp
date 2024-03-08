#include <iostream>
#include <stdlib.h>
#include <mosquitto.h>

/* Hàm callback được gọi khi kết nối tới broker MQTT được thiết lập.*/
void on_connect(struct mosquitto *mosq, void *obj, int rc) {
    std :: cout <<"ID:" << *(int *)obj;
    if (rc) {
        std :: cout << "Error with result code:" << rc;
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "raspberry", 0);
}

/*Hàm callback được gọi khi nhận được một message từ broker MQTT.*/
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    std :: cout << "New message with topic" <<  msg->topic << ":" <<(char *)msg->payload;
    int rc = mosquitto_connect(mosq, "172.16.0.121", 1883, 10);
	if (rc) {
        std :: cout << "Could not connect to Broker with return code" << rc;
        return -1;
    }
    mosquitto_publish(mosq, NULL, "led", 2, (char *)msg->payload, 0, false);
	mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
}

int main() {
    int rc, id = 12;
    mosquitto_lib_init();
    struct mosquitto *mosq;

    mosq = mosquitto_new("subscribe-test", true, &id);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);

    rc = mosquitto_connect(mosq, "172.16.0.227", 1883, 10);
    if (rc) {
        std :: cout << "Could not connect to Broker with return code" << rc;
        return -1;
    }

    mosquitto_loop_start(mosq);
    std :: cout << "Press Enter to quit...";
    getchar();
    mosquitto_loop_stop(mosq, true);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}