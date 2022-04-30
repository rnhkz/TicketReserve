//Albert Baker
//alb190010

#ifndef PROJECT4_AUDITORIUM_H
#define PROJECT4_AUDITORIUM_H

#include <fstream>
#include "Node.h"
#include "Seat.h"

class Auditorium {
private:
    Node<Seat*>* head;
    Node<Seat*>* tail;
public:
    Auditorium();
    Auditorium(const std::string& fileName, int groupNum);
    Node<Seat*>* getHead(){ return head; }
    void setHead(Node<Seat*>* head){ this->head = head; }
    Node<Seat*>* getTail(){ return tail; }
    void setTail(Node<Seat*>* tail){ this->tail = tail; }
    ~Auditorium();
};


#endif //PROJECT4_AUDITORIUM_H
