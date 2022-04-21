#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station){

    //initializing.

    station->emptySeats=0;
    station->passWait=0;
    station->passBoard=0;

    pthread_mutex_init( &station->mutex, NULL);
    pthread_cond_init( &station->loadPassengers, NULL);
    pthread_cond_init( &station->trainDepart, NULL);

}



void
station_load_train(struct station *station, int count){

    pthread_mutex_lock(&station->mutex);
   

    if(count>0 && station->passWait >0){//if there are at least 1 seat available and 1 passenger waiting,
                                        // broadcast the condition variable to load passengers.
                                        
        station->emptySeats=count;
        
        pthread_cond_broadcast(&station->loadPassengers);//wake up all the waiting passengers.
        pthread_cond_wait(&station->trainDepart,&station->mutex);//Equivalent to: pthread_mutex_unlock(&station->mutex);
                                                                 // Wait for signal on trainDepart                        
                                                                 //pthread_mutex_lock(&station->mutex);
    }
          

    //release the lock and depart the station.
    pthread_mutex_unlock(&station->mutex);

}



void
station_wait_for_train(struct station *station){

    pthread_mutex_lock(&station->mutex);
    station->passWait++;

    while(station->emptySeats==0){//while the train is full, wait for empty seats to load the passenger.

        pthread_cond_wait(&station->loadPassengers,&station->mutex);//Equivalent to: pthread_mutex_unlock(&station->mutex);
                                                                                // Wait for signal on loadPassengers
                                                                                // pthread_mutex_lock(&station->mutex);

    }

    station->emptySeats--;//decrementing the available seats' no.
    station->passWait--;//decrementing the waiting passengers' no.
    station->passBoard++;//incrementing the passengers on board's no.

    //release the lock.
    pthread_mutex_unlock(&station->mutex);

}



void
station_on_board(struct station *station){

    pthread_mutex_lock(&station->mutex);
    station->passBoard--;//passenger is in his seat.

    if((station->emptySeats==0 || station->passWait==0) && station->passBoard==0){//all passengers are in their seats, and either the train is full or all waiting passengers have boarded.

        pthread_cond_signal(&station->trainDepart);

    }

    //release the lock.
    pthread_mutex_unlock(&station->mutex);

}
