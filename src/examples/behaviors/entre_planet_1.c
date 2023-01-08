#include <kilolib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STOP_DISTANCE 88

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

int distance, star_index;
int current_motion = STOP;
uint8_t new_message = 0;
int range_distance[2] = {0,0};

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

void setup() {

}

void loop(){
    if(new_message == 1){
        new_message = 0;

        if(range_distance[0] >= STOP_DISTANCE && range_distance[1] >= STOP_DISTANCE){
            set_motion(STOP);
            printf("STOP\n");
        } else {
            set_motion(FORWARD);
            printf("FORWARD\n");
        }
    }
}

void message_rx(message_t *m, distance_measurement_t *d)
{
    new_message = 1;
    star_index = (*m).data[0];
    distance = estimate_distance(d);
    range_distance[star_index] = distance;
    printf("distance reçue de %d : %d\n", star_index, range_distance[star_index]);
}

int main()
{
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}

