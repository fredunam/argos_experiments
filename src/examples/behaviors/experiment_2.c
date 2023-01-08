//
// Created by frederic turchet on 02/01/2023.
//
/*
 * 3 robots dont 2 initialisés pour rester stationnaires. Le 3eme passera en ligne droite entre les 2 robots
 * stationnaires et s'immobilisera à une distane >= 88. Il changera alors d'état pour devenir stationnaire à
 * son tour
 */

#include <kilolib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STOP_DISTANCE 88

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

typedef enum {
    STAR,
    PLANET,
}bot_state_t;

int distance, star_index;
int current_motion = STOP;
uint8_t new_message = 0;
bot_state_t bot_state = STAR;
int range_distance[4] = {0,0};

message_t message;
int message_sent = 0;

void set_motion(int new_motion) {
    // Only take an action if the motion is being changed.
    if (current_motion != new_motion) {
        current_motion = new_motion;

        if (current_motion == STOP) {
            set_motors(0, 0);
        } else if (current_motion == FORWARD) {
            spinup_motors();
            set_motors(kilo_straight_left, kilo_straight_right);
        } else if (current_motion == LEFT) {
            spinup_motors();
            set_motors(kilo_turn_left, 0);
        } else if (current_motion == RIGHT) {
            spinup_motors();
            set_motors(0, kilo_turn_right);
        }
    }
}

void setup(){
    message.type = NORMAL;
    message.data[0] = kilo_uid;
    message.crc = message_crc(&message);

    if(message.data[0] >=3){
        bot_state = PLANET;
    }
}

void loop(){
    if(bot_state == STAR){
        if(message_sent == 1){
            message_sent = 0;

            set_color(RGB(1,0,1));
            delay(100);
            set_color(RGB(0,0,0));
        }
    } else {
        if(new_message == 1){
            new_message = 0;

            if((range_distance[0] >= STOP_DISTANCE && range_distance[1] >= STOP_DISTANCE) || (range_distance[2] >= STOP_DISTANCE && range_distance[3] >= STOP_DISTANCE)){
                set_motion(STOP);
                printf("STOP\n");
                bot_state = STAR;
                printf("je suis maintenant un STAR\n");
            } else {
                set_motion(FORWARD);
                printf("FORWARD\n");
            }
        }
    }
}

void message_rx(message_t *m, distance_measurement_t *d){
    new_message = 1;
    star_index = (*m).data[0];
    distance = estimate_distance(d);
    range_distance[star_index] = distance;
    printf("%d a reçu un message\n", kilo_uid);
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
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}
