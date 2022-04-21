#include <pthread.h>

struct station {

    int emptySeats;//no. of empty (available) seats in the train
    int passWait;//no. of waiting passengers
    int passBoard;//no. of passengers on board
    
    pthread_mutex_t mutex;
    pthread_cond_t loadPassengers;//condition variable "Passenger can move to the train board".
    pthread_cond_t trainDepart;//condition variable "Train is ready to depart".
	
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
