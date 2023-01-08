//
// Created by frederic turchet on 02/01/2023.
//
#include <kilolib.h>

message_t message;
int message_sent = 0;

void setup(){
    message.type = NORMAL;
    message.data[0] = kilo_uid;
    message.crc = message_crc(&message);
}

void loop(){
    if(message_sent == 1){
        message_sent = 0;

        set_color(RGB(1,0,1));
        delay(100);
        set_color(RGB(0,0,0));
    }
}

message_t *message_tx(){
    return &message;
}

void message_tx_success(){
    message_sent = 1;
}

int main(){
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    kilo_start(setup, loop);

    return 0;
}