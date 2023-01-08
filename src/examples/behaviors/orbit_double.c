//
// Created by frederic turchet on 05/01/2023.
//

/**
 * 1 seul fichier
 * 2 robots stationnaires - kb0 et kb1
 * 1 robot kb2 en orbit autour de kb0 et kb1
*/

#include <kilolib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static const uint8_t TOOCLOSE_DIST = 40; //40mm
static const uint8_t DESIRED_DIST = 60; //60mm

typedef enum {
    STAR,
    PLANET
}bot_state_t;

typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;

typedef enum {
    ORBIT_TOOCLOSE,
    ORBIT_NORMAL
} orbit_state_t;

//Variables
motion_t cur_motion = STOP;
orbit_state_t orbit_state = ORBIT_NORMAL;
bot_state_t bot_state = STAR;
message_t message;
int cur_dist = 0;
uint8_t new_message = 0;
uint8_t message_sent = 0;
int uid_received, dist;
int temp_dist = 0;
int tab_dist[256][10] = {1000};


//Prototype de fonctions
//int minTabDist(int **tabDist, int id, int m);


/**
* Function to set new motion
*/
void set_motion(motion_t new_motion){
    if(cur_motion != new_motion){
        cur_motion = new_motion;
        switch (cur_motion) {
            case STOP:
                set_motors(0,0);
                break;
            case FORWARD:
                spinup_motors();
                set_motors(kilo_straight_left, kilo_straight_right);
                break;
            case LEFT:
                spinup_motors();
                set_motors(kilo_turn_left, 0);
                break;
            case RIGHT:
                spinup_motors();
                set_motors(0, kilo_turn_right);
                break;
        }
    }
}

//int minTabDist(int **tabDist, int id, int m){
//    int minDist = tab_dist[0][0];
//    for (int i = 1; i < m; ++i) {
//        if(minDist > tabDist[id][i]){
//            minDist = tabDist[id][i];
//        }
//    }
//    return minDist;
//}

/**
* Si le robot en mouvement se rapproche de trop d'une étoile alors il change d'état
 * sinon, il alternera entre des mouvements gauche droite
*/
void orbit_normal(){
    //cur_dist = minTabDist(**tab_dist,kilo_uid, 10);
    if(cur_dist < TOOCLOSE_DIST){
        printf("orbit_normal: change d'état -> orbit_tooclose\n");
        orbit_state = ORBIT_TOOCLOSE;
    } else {
        if(cur_dist < DESIRED_DIST){
            set_motion(LEFT);
            printf("orbit_normal: < DESIRED -> Left\n");
        } else {
            set_motion(RIGHT);
            printf("orbit_normal: >= DESIRED -> RIGHT\n");
        }
    }
}

/**
* Le robot s'éloignera des robots stationnaires et changera d'état lorsqu'il aura
 * atteind une limite désirée fixée
*/
void orbit_tooclose(){
    //cur_dist = minTabDist(**tab_dist, kilo_uid, 10);
    if(cur_dist >= DESIRED_DIST){
        printf("orbit_tooclose: change d'état -> orbit_normal\n");
        orbit_state = ORBIT_NORMAL;
    } else {
        set_motion(FORWARD);
        printf("orbit_tooclose: Forward\n");
    }
}

/**
 * Paramétrage du message envoyé
 * et définition des robots qui seront en mouvement
 */
void setup(){
    message.type = NORMAL;
    message.data[0] = kilo_uid;
    message.crc = message_crc(&message);

    if(message.data[0] >= 2){
        bot_state = PLANET;
    }
}

/**
 * Les robots définis comme STAR clignoteront en Magenta s'ils ont envoyés un message
 * Les robots récepteurs mettront à jour leurs distances à chaque message reçu
 */
void loop(){
    if(bot_state == STAR){
        if(message_sent == 1){
            message_sent = 0;

            set_color(RGB(1,0,1));
            delay(100);
            set_color(RGB(0,0,0));
        }
    } else {
        if(new_message){
            new_message = 0;
            for (int i = 0; i < 10; ++i) {
                if(cur_dist > tab_dist[kilo_uid][i]){
                    cur_dist = tab_dist[kilo_uid][i];
                }
            }
            //cur_dist = minTabDist(*tab_dist,kilo_uid, 10);
        } else if(cur_dist == 0){ // Quitte la state machine si la distane reçue est invalide
            return;
        }

        // Orbit State Machine
        switch (orbit_state) {
            case ORBIT_NORMAL:
                orbit_normal();
                break;
            case ORBIT_TOOCLOSE:
                orbit_tooclose();
                break;
        }
    }

}

/**
 *
 * @param m
 * @param d
 */
void message_rx(message_t *m, distance_measurement_t *d){
    new_message = 1;
    uid_received = (*m).data[0];
    tab_dist[kilo_uid][uid_received] = estimate_distance(d);
    for (int i = 0; i < 10; ++i) {
        if(cur_dist < tab_dist[kilo_uid][i]){
            cur_dist = tab_dist[kilo_uid][i];
        }
    }

    printf("kb%d a reçu un message de %d: distance nous séparant: %d\n", kilo_uid, uid_received, tab_dist[kilo_uid][uid_received]);
    printf("uid_received: %d\n", uid_received);
    printf("tabDist[%d][%d] = %d\n",kilo_uid, uid_received, tab_dist[kilo_uid][uid_received]);

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