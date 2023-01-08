//
// Created by frederic turchet on 28/11/2022.
//
#include <kilolib.h>

#define MESSAGE_CHECK 0
#define DISTANCE_CHECK 1
#define TURN_LEFT 2
#define TURN_RIGHT 3
#define LEFT 100
#define RIGHT 150

#define THRESHOLD 50

#define MOTOR_ON_DURATION 500

int state = MESSAGE_CHECK, distance, message_rx_status = 0;

void message_rx(message_t *m, distance_measurement_t *d){
    message_rx_status = 1;
    distance = estimate_distance(d);
}

void move(int direction){
    switch (direction) {
        case LEFT:
            spinup_motors();
            set_motors(kilo_straight_left, 0);
            delay(MOTOR_ON_DURATION);
            set_motors(0,0);
            break;
        case RIGHT:
            spinup_motors();
            set_motors(0, kilo_straight_right);
            delay(MOTOR_ON_DURATION);
            set_motors(0,0);
            break;

        default:
            break;
    }
}

void setup(){

}

void loop(){
    switch (state) {
        case MESSAGE_CHECK:
            if (message_rx_status == 1){
                message_rx_status = 0;
                state = DISTANCE_CHECK;
            }
            break;
        case DISTANCE_CHECK:
            if(distance > THRESHOLD){
                state = TURN_RIGHT;
            } else {
                state = TURN_LEFT;
            }
            break;
        case TURN_LEFT:
            move(LEFT);
            state = MESSAGE_CHECK;
            break;
        case TURN_RIGHT:
            move(RIGHT);
            state = MESSAGE_CHECK;
            break;

        default:
            break;
    }
}

int main(){
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}