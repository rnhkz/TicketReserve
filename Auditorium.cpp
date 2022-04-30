//Albert Baker
//alb190010

#include "Auditorium.h"

Auditorium::Auditorium(){
    head = nullptr;
    tail = nullptr;
}

Auditorium::Auditorium(const std::string& fileName, int groupNum){
    std::ifstream inFile;
    std::string str;
    int row = 1; //Starting row
    char col = 'A'; //Starting column
    Node<Seat*>* prevNode; //Used for linking nodes horizontally.
    Node<Seat*>* nodeAbove = nullptr; //Used for linking nodes vertically.
    head = nullptr;
    inFile.open(fileName);
    while(getline(inFile, str)) {
        col = 'A';
        for (char &c : str) {
            Seat* data = new Seat(row, col, c, groupNum);
            Node<Seat*>* n = new Node<Seat*>(data);
            //If auditorium is empty, allocates head
            if(head == nullptr){
                head = n; //Sets head to row 1, col 1
                prevNode = head;
            }
            else{
                //Links the current node (n) to the previous node.
                n->setLeft(prevNode);
                prevNode->setRight(n);
                prevNode = n;
            }
            //When the row count exceeds one, introduce pointer to node above
            if(row > 1){
                //Initializes the node above to head.
                if(nodeAbove == nullptr){
                    nodeAbove = head;
                }
                //Links the current node (n) to the node above.
                n->setUp(nodeAbove);
                nodeAbove->setDown(n);
                nodeAbove = nodeAbove->getRight();
            }
            col++;
            tail = n;
        }
        row++;
    }
    inFile.close();
}

Auditorium::~Auditorium(){
    Node<Seat*>* current = head;
    if(current != nullptr) {
        while (current->getRight() != nullptr) {
            Node<Seat *> *next = current->getRight();
            delete current;
            current = next;
        }
    }
}