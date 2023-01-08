//
// Created by frederic turchet on 03/01/2023.
//
#include <kilolib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

#define NEIGHBOURS_IN_RANGE 1
#define COMPARE_DESIRED_DISTANCE 2
#define ORBIT_AND_UPDATE_INDEX 3
#define FINISH 4
#define INFINITY 5

static const uint8_t TOOCLOSE_DIST = 40;
static const uint8_t DESIRED_DIST = 60;

typedef enum {
    ORBIT_TOOCLOSE,
    ORBIT_NORMAL,
} orbit_state_t;

typedef enum {
    STAR,
    PLANET,
} bot_state_t;

int neighbours[4][2] = {
        {1, 2},
        {0, 2},
        {0, 1},
        {1, 2}
};

int dist_multiplier[4][2] = {
        {1, 1},
        {1, 1.4},
        {1, 1.4},
        {1, 1}
};

int distance, uid_received, behaviour;
int current_motion = STOP;
orbit_state_t orbit_state = ORBIT_NORMAL;
uint8_t cur_distance = 0;
uint8_t new_message = 0;
bot_state_t bot_state = PLANET;

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

/**
 * Si le robot en déplacement est trop proche du robot stationnaire, alors
 * il change d'état (ORBIT_TOOCLOSE)
 * Sinon, il fera des déplacements alternés gauche / droite
 */
void orbit_normal() {
    if (cur_distance < TOOCLOSE_DIST) {
        orbit_state = ORBIT_TOOCLOSE;
    } else {
        if (cur_distance < DESIRED_DIST) {
            set_motion(LEFT);
        } else {
            set_motion(RIGHT);
        }
    }
}

/**
 * Si la distance du robot en mouvement dépasse la distance désirée,
 * il changer d'état (ORBIT_NORMAL)
 * Sinon, il se déplacera de façon rectiligne pour s'éloigner du robot
 * stationnaire
 */
void orbit_tooclose() {
    if (cur_distance >= DESIRED_DIST) {
        orbit_state = ORBIT_NORMAL;
    } else {
        set_motion(FORWARD);
    }
}

/**
 * Init le message envoyé avec l'id de l'émetteur comme donnée transmise
 *   && définit les 3 premiers robots comme étatn des robots stationnaires STAR
 */
void setup() {
    message.type = NORMAL;
    message.data[0] = kilo_uid;
    message.crc = message_crc(&message);

    if (message.data[0] < 3) {
        bot_state = STAR;
    }
}

void loop() {
    //Un robot stationnaire STAR ne fait qu'envoyer un message
    if (bot_state == STAR) {
        if (message_sent == 1) {
            message_sent = 0;

            set_color(RGB(1, 0, 1));
            delay(100);
            set_color(RGB(0, 0, 0));
        }
        printf("kb%d est une STAR\n", uid_received);
    } else {
        printf("kb%d est une PLANET\n", uid_received);
        switch (behaviour) {
            case COMPARE_DESIRED_DISTANCE:
                if ((uid_received != neighbours[kilo_uid][0] && uid_received != neighbours[kilo_uid][1]) ||
                    (uid_received == neighbours[kilo_uid][0] && uid_received != neighbours[kilo_uid][1]) ||
                    (uid_received != neighbours[kilo_uid][0] && uid_received == neighbours[kilo_uid][1])) {
                    behaviour = ORBIT_AND_UPDATE_INDEX;
                    printf("kb%d change d'état: ORBIT_AND_UPDATE_INDEX\n", kilo_uid);
                } else {
                    behaviour = FINISH;
                    printf("kb%d change d'état: FINISH\n", kilo_uid);
                }
                break;
            case ORBIT_AND_UPDATE_INDEX:
                if(orbit_state == ORBIT_NORMAL){
                    orbit_normal();
                    printf("kb%d lance la fonction orbit_normal()\n", kilo_uid);
                    break;
                } else {
                    printf("kb%d lance la fonction orbit_tooclose()\n", kilo_uid);
                    orbit_tooclose();
                    break;
                }
            case FINISH:
                set_color(RGB(0,0,1));
                bot_state = STAR;
                behaviour = INFINITY;
                break;
            default:
                break;
        }
    }
}

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
    uid_received = (*m).data[0];
    distance = estimate_distance(d);
    printf("%d a reçu un message de %d: distance nous séparant: %d\n", kilo_uid, uid_received, distance);
}

/**
 * Envoie un message
 * @return le message
 */
message_t *message_tx() {
    return &message;
}

/**
 * Fait clignoter la LED du robot émetteur en magenta et assigne la valeur '1'
 * à la variable 'message_sent
 */
void message_tx_success() {
    message_sent = 1;
    set_color(RGB(1, 0, 1));
    delay(100);
    set_color(RGB(0, 0, 0));
}

int main() {
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}