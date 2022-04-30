//Albert Baker
//alb190010

#ifndef PROJECT4_SEAT_H
#define PROJECT4_SEAT_H

class Seat {
private:
    int groupNum;
    int row;
    char seat;
    char ticketType;
public:
    Seat();
    Seat(int row, char seat, char ticketType, int groupNum);
    int getGroupNum(){ return groupNum; }
    int getRow(){ return row; }
    char getSeat(){ return seat; }
    char getTicketType(){ return ticketType; }
    void setGroupNum(int groupNum){ this->groupNum = groupNum; }
    void setRow(int row){ this->row = row; }
    void setSeat(char seat){ this->seat = seat; }
    void setTicketType(char ticketType){ this->ticketType = ticketType; }
};

#endif //PROJECT4_SEAT_H
