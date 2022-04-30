//Albert Baker
//alb190010

#include "Seat.h"

Seat::Seat(){
    groupNum = 0;
    row = 0;
    seat = 0;
    ticketType = 0;
}

Seat::Seat(int row, char seat, char ticketType, int groupNum){
    this->groupNum = groupNum;
    this->row = row;
    this->seat = seat;
    this->ticketType = ticketType;
}