//
// Created by frederic turchet on 26/11/2022.
//
#include <kilolib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define THRESHOLD 5
#define STOP_DISTANCE 55

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

// declare constants
static const uint8_t TOOCLOSE_DISTANCE = 35; // 35 mm
static const uint8_t DESIRED_DISTANCE = 70; // 70 mm

typedef enum {
    ORBIT_TOOCLOSE,
    ORBIT_NORMAL,
    ORBIT_MULTI_BOTS,
    ORBIT_STOP,
} orbit_state_t;


int distance, speaker_index, nbBots = 0, state, rand_move;
int init_ticks = 0;
int current_motion = STOP;
orbit_state_t orbit_state = ORBIT_NORMAL;
uint8_t cur_distance = 0;
uint8_t new_message = 0;
int temp_distance[2];
int inComm = 0;

int range_dist[3] = {0, 0, 0};
int neighbour_id[3] = {0, 0, 0};

int neighbours[7][2] = {
        {0, 0},
        {0, 0},
        {0, 0},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5}
};

float neighbour_distance[7][2] = {
        {0,  0},
        {0,  0},
        {0,  0},
        {45, 45},
        {45, 64},
        {45, 45},
        {64, 45}
};

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

//TODO Ajouter une condition d'arrêt et modifier temp_distance[] par cur_distance
void orbit_normal() {
    printf("----------------------------------------------\n");
    printf("Orbit_normal(): \n");
    printf("temp_distance[%d] = %d\n", speaker_index - 1, temp_distance[speaker_index - 1]);
    printf("cur_distance = %d\n", cur_distance);
    printf("range_dist[%d] = %d\n", (speaker_index - 1) % 3, range_dist[(speaker_index - 1) % 3]);
    printf("----------------------------------------------\n");

    if ((speaker_index - 1) % 3 != -1) {
        printf("range_dist[0] = %d\n", range_dist[0]);
        printf("range_dist[1] = %d\n", range_dist[1]);
        printf("range_dist[2] = %d\n", range_dist[2]);
        if ((range_dist[(speaker_index - 1) % 3] <= range_dist[(speaker_index) % 3]) &&
            ((speaker_index - 1) % 3 != 0 || (speaker_index) % 3 != 0)) {
            if (range_dist[(speaker_index - 1) % 3] < TOOCLOSE_DISTANCE) {
                orbit_state = ORBIT_TOOCLOSE;
            } else {
                if (range_dist[(speaker_index - 1) % 3] < DESIRED_DISTANCE) {
                    set_motion(LEFT);
                    printf("cur_dist < DESIRED - Left\n");
                } else {
                    set_motion(RIGHT);
                    printf("cur_dist >= DESIRED - Right\n");
                }
            }
        }
        if (((range_dist[1] >= neighbour_distance[kilo_uid][0] - THRESHOLD &&
              range_dist[1] <= neighbour_distance[kilo_uid][0] + THRESHOLD) &&
             (range_dist[2] >= neighbour_distance[kilo_uid][1] - THRESHOLD &&
              range_dist[2] <= neighbour_distance[kilo_uid][1] + THRESHOLD)) &&
            (range_dist[0] > range_dist[1] && range_dist[0] > range_dist[2])) {
            //set_motion(STOP);
            orbit_state = ORBIT_STOP;
            printf("change etat - stop_orbit\n");
        }
    }
}

void orbit_multi_bots() {
    printf("------------------------------------\n");
    printf("Orbit_multi_bots\n");
    printf("------------------------------------\n");
    if (neighbour_id[1] != neighbours[kilo_uid][0] && neighbour_id[2] != neighbours[kilo_uid][1]) {
        orbit_state = ORBIT_NORMAL;
    } else {
        if ((range_dist[1] >= TOOCLOSE_DISTANCE + THRESHOLD || range_dist[1] <= DESIRED_DISTANCE + THRESHOLD) &&
            (range_dist[2] >= TOOCLOSE_DISTANCE + THRESHOLD || range_dist[2] <= DESIRED_DISTANCE + THRESHOLD)) {
            set_motion(STOP);
            printf("orbit_multi_bots - Stop\n");
        }
    }

}

void orbit_tooclose() {
    if (range_dist[(speaker_index - 1) % 3] >= DESIRED_DISTANCE)
        orbit_state = ORBIT_NORMAL;
    else {
        set_motion(FORWARD);
        printf("----------------------------------------------\n");
        printf("orbit_tooClose\n");
        printf("Forward\n");
        printf("----------------------------------------------\n");
    }
}

void orbit_stop() {
    set_motion(STOP);
    printf("Orbit_stop\n");
}

void setup() {

}

void loop() {

    printf("----------------------------------------------\n");
    printf("loop()\n");
    printf("ticks: %d\n", kilo_ticks);
    printf("kilo_uid: %d\n", kilo_uid);
    printf("neightbour[3][0]: %d\n", neighbours[kilo_uid][0]);
    printf("neightbour[3][1]: %d\n", neighbours[kilo_uid][1]);
    printf("\n");
    printf("neighbour_distance[%d][0]: %f\n",kilo_uid-1, neighbour_distance[kilo_uid -1][0]);
    printf("neighbour_distance[%d][1]: %f\n",kilo_uid-1, neighbour_distance[kilo_uid -1][1]);
    printf("----------------------------------------------\n");

    if (new_message == 1) {
        new_message = 0;
        if (range_dist[0] <= range_dist[1] && range_dist[0] <= range_dist[2]) {
            cur_distance = range_dist[0];
        } else if (range_dist[1] <= range_dist[0] && range_dist[1] <= range_dist[2]) {
            cur_distance = range_dist[1];
        } else {
            cur_distance = range_dist[3];
        }
    }

    switch (orbit_state) {
        case ORBIT_NORMAL:
            orbit_normal();
            break;
        case ORBIT_TOOCLOSE:
            orbit_tooclose();
            break;
        case ORBIT_STOP:
            orbit_stop();
            break;
    }
}

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
    speaker_index = (*m).data[0];
    temp_distance[speaker_index - 1] = estimate_distance(d);
    printf("------------------------------------------------\n");
    printf("message_rx()\n");
    printf("(message_rx) kb%d a envoyé un nouveau message à kb%d lors du %dème ticks\n", speaker_index, kilo_uid,
           kilo_ticks);
    printf("%dmm sépare kb%d de kb%d\n", temp_distance[speaker_index - 1], kilo_uid, speaker_index);
    if (nbBots < speaker_index) {
        nbBots = speaker_index;
    }
    //On enregistre les distances reçues
    range_dist[(speaker_index - 1) % 3] = estimate_distance(d);
    printf("range_dist[%d] = %d\n", (speaker_index - 1) % 3, range_dist[(speaker_index - 1) % 3]);

    //On enregistre les index des speakers
    neighbour_id[(speaker_index - 1) % 3] = speaker_index;
    printf("neighbour_id[%d] = %d\n", (speaker_index - 1) % 3, speaker_index);
    printf("------------------------------------------------\n");

}

int main() {
    kilo_init();

    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}


