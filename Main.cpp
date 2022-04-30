//Albert Baker
//alb190010

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Auditorium.h"

using namespace std;

const string seatString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct User{
    string username;
    string password;
    vector<vector<Seat*>> orders; //Stores orders (Vectors of seats)

    User(){
        username = "";
        password = "";
    }

    User(string u, string p){
        username = u;
        password = p;
    }
};
/**
 *
 * @param order
 * @return cost of order (float)
 *
 * Prints a individual order.
 */
float PrintOrder(vector<Seat*> order){
    int adultSold = 0, childSold = 0, seniorSold = 0;
    cout << "Auditorium " << order.at(0)->getGroupNum();
    for(Seat* curSeat : order) { //Checks every seat in order, keeps count of ticket type
        cout << ", " << curSeat->getRow() << curSeat->getSeat();
        switch(curSeat->getTicketType()){
            case 'A':
                adultSold++;
                break;
            case 'C':
                childSold++;
                break;
            case 'S':
                seniorSold++;
                break;
            default:
                break;
        }
    }
    cout << endl;
    cout << adultSold << " adult, " << childSold << " child, " << seniorSold << " senior" << endl;
    return (float)((adultSold*10.0)+(childSold*5.0)+(seniorSold*7.50)); //Cost of order
}
/**
 *
 * @param orders
 * @return none
 *
 * Prints all orders for user with price. Also prints total price of every order.
 */
void PrintReceipt(const vector<vector<Seat*>>& orders){
    float total = 0;
    if(!orders.empty()) { //If there are orders
        for (const vector<Seat *> &order : orders) { //GO through every order
            float orderAmt = PrintOrder(order); //Order is printed here. Cost is stored as well.
            cout << fixed << setprecision(2) << "Order Total: $" << orderAmt << endl;
            cout << endl;
            total += orderAmt; //Add cost of individual order to total cost to user.
        }
    }
    else{ //If there are no orders
        cout << "No orders" << endl;
    }
    cout << fixed << setprecision(2) << "Customer Total: $" << total << endl;
    cout << endl;
}
/**
 * @param i
 * @return |i|
 *
 * Turns i positive, if applicable.
 */
int makeReal(int i){
    //If i is negative, turn positive
    if(i < 0){
        i*=-1;
    }
    return i;
}
/**
 * @param auditorium
 * @param totalQuantity
 * @param theaterDim
 * @return pointer to best seat, null if no best seat found
 *
 * Finds the best available sequence of empty seats if user selection contains at least one reserved seat.
 * Should only be used if CheckAvailability() returns false.
 * Best available is shortest distance b/t center of selection and center of row.
 */
Node<Seat*>* BestAvailable(Auditorium* a, int totalQuantity){
    int seatBound = (int)seatString.find(a->getTail()->getPayload()->getSeat()) + 1;
    pair<int, Node<Seat*>*> bestSeat = make_pair(seatBound*a->getTail()->getPayload()->getRow(), nullptr); //<Smallest distance, Node to best seat>
    Node<Seat *> *subHead = a->getHead(); //Starting position for each row searched
    int distanceY = 0; //Distance between rows
    //Set subhead to seat 'A', middle row
    while(subHead->getPayload()->getRow() < a->getTail()->getPayload()->getRow()/2)
        subHead = subHead->getDown();
    if(a->getTail()->getPayload()->getRow()%2 == 1)
        subHead = subHead->getDown();
    int middleY = subHead->getPayload()->getRow();
    //Finding best seat
    for(int i = 1; i <= a->getTail()->getPayload()->getRow(); i++) {
        Node<Seat *> *curNode = subHead;
        for (int j = 1; j <= seatBound - totalQuantity + 1; j++) {
            Node<Seat *> *toIterate = curNode;
            bool allEmpty = true;
            for (int k = 0; k < totalQuantity; k++) {
                //If the current seat is not located in this row
                if(toIterate->getPayload()->getRow()!=subHead->getPayload()->getRow()){
                    allEmpty = false;
                    break;
                }
                //If the current seat is not empty
                if (toIterate->getPayload()->getTicketType() != '.') {
                    allEmpty = false;
                    break;
                }
                toIterate = toIterate->getRight();
            }
            if (allEmpty) {
                int middleX = (seatBound + 1) / 2;
                int distanceX = makeReal((j + (totalQuantity) / 2) - middleX);
                int distanceTotal = distanceX + distanceY;
                if (distanceTotal < bestSeat.first) {
                    //Declare curNode as the closest available node to center
                    bestSeat = make_pair(distanceTotal, curNode);
                }
                else if(distanceTotal == bestSeat.first){
                    //If distance is equal, compares distance by longest lateral.
                    //Subtracts distance away on y-axis to distance away on x-axis. Shortest number is closest to center.
                    int x1 = makeReal(middleX - ((int)seatString.find(curNode->getPayload()->getSeat()) + 1));
                    int y1 = makeReal(middleY - curNode->getPayload()->getRow());
                    int x2 = makeReal(middleX - ((int)seatString.find(bestSeat.second->getPayload()->getSeat()) + 1));
                    int y2 = makeReal(middleY - bestSeat.second->getPayload()->getRow());
                    int distAcross1 = makeReal(x1 - y1);
                    int distAcross2 = makeReal(x2 - y2);
                    if(distAcross1 < distAcross2){
                        //Declare curNode as the closest available node to center
                        bestSeat = make_pair(distanceTotal, curNode);
                    }
                    else if(distAcross1 == distAcross2){
                        //If distance is still a tie, compare rows. Smallest number takes priority.
                        if(y1 < y2){
                            //Declare curNode as the closest available node to center
                            bestSeat = make_pair(distanceTotal, curNode);
                        }
                    }
                }
            }
            curNode = curNode->getRight();
        }
        //Go to next row
        if(a->getTail()->getPayload()->getRow()%2 == 1) { //Odd amt of rows
            if (i % 2 == 1) {
                for (int l = 0; l < i; l++)
                    subHead = subHead->getUp();
                distanceY++;
            } else
                for (int l = 0; l < i; l++)
                    subHead = subHead->getDown();
        }
        else{ //Even amt of rows
            if (i % 2 == 1) {
                for (int l = 0; l < i; l++)
                    subHead = subHead->getDown();
                distanceY++;
            } else
                for (int l = 0; l < i; l++)
                    subHead = subHead->getUp();
        }
    }
    return bestSeat.second;
}

/**
 * @param auditorium
 * @param theaterDim
 * @return none
 *
 * Displays the Auditorium, with a column header for seat letters, and each row numbered on the left.
 * Empty seats represented with '.'.
 * Reserved seats represented with '#'.
 */
void DisplayAuditorium(Auditorium* a){
    Node<Seat*>* curNode = a->getHead();
    size_t to = seatString.find(a->getTail()->getPayload()->getSeat());
    cout << " " << seatString.substr(0,to+1); // Displays column header
    while(curNode){
        if(curNode->getPayload()->getSeat() == 'A'){
            cout << endl;
            cout << curNode->getPayload()->getRow();
        }
        if(curNode->getPayload()->getTicketType() != '.')
            cout << "#";
        else {
            cout << curNode->getPayload()->getTicketType();
        }
        curNode = curNode->getRight();
    }
    cout << endl;
}
/**
 *
 * @param order
 * @param toAdd
 * @return none
 *
 * Adds a new seat to the current order, ordered. Helper method for ReserveSeats.
 * Order is: Smallest  row, smallest seat -> Largest row, largest seat
 * If seat matches, row takes priority.
 */
void insertForOrder(vector<Seat*> &order, Seat* toAdd){
    size_t indexOfSeat = order.size()-1;  //Start at last seat in order.
    if(order.empty()){ //If order is empty, add seat
        order.push_back(toAdd);
    }
    else {

        while (indexOfSeat >= 0) { //From last seat to first seat:
            if (toAdd->getRow() > order.at(indexOfSeat)->getRow()) { //Firsts checks for a higher row
                order.insert(order.begin() + (int) indexOfSeat+1, toAdd); //Inserts & terminates if true
                break;
            }
            else if (toAdd->getRow() == order.at(indexOfSeat)->getRow()) { //If rows are the same
                if (toAdd->getSeat() > order.at(indexOfSeat)->getSeat()) { //Then checks for larger seat
                    order.insert(order.begin() + (int) indexOfSeat+1, toAdd); //Inserts & terminates if true
                    break;
                }
            }
            if(indexOfSeat == 0){ //If the seat compared to wat the first in the order
                order.insert(order.begin(), toAdd); //Adds new seat to beginning of order
                break;
            }
            else {
                indexOfSeat--;
            }
        }
    }
}
/**
 * @param auditorium
 * @param row
 * @param seat
 * @param adultQuantity
 * @param seniorQuantity
 * @param childQuantity
 * @return none
 *
 * Adds seats to the row, one after the other, with adults first, children second, and seniors last.
 * Validity of selected seats should be checked beforehand with CheckAvailability().
 * When updating a order, a index has to be supplied. If not, then the order is created.
 */
void ReserveSeats(Auditorium* a,  User *curUser, int row, char seat, int adultQuantity,  int seniorQuantity, int childQuantity, int updateIndex = -1){
    vector<Seat*> order; //Create new order
    if(updateIndex != -1){ //Providing an index means a pre-existing order is being updated
        order = curUser->orders.at(updateIndex); // Sets the new order to a pre-existing order.
    }
    Node<Seat*>* curNode = a->getHead();
    while(curNode->getPayload()->getRow() != row){
        curNode = curNode->getDown();
    }
    while(curNode->getPayload()->getSeat() != seat){
        curNode = curNode->getRight();
    }
    for(int x = 0; x < adultQuantity; x++){
        curNode->getPayload()->setTicketType('A'); //seat - 'A' = seatString.at(seat)
        insertForOrder(order, curNode->getPayload());
        curNode = curNode->getRight();
    }
    for(int x = 0; x < childQuantity; x++){
        curNode->getPayload()->setTicketType('C'); //seat - 'C' = seatString.at(seat)
        insertForOrder(order, curNode->getPayload());
        curNode = curNode->getRight();
    }
    for(int x = 0; x < seniorQuantity; x++){
        curNode->getPayload()->setTicketType('S'); //seat - 'S' = seatString.at(seat)
        insertForOrder(order, curNode->getPayload());
        curNode = curNode->getRight();
    }
    if(updateIndex != -1){ //If order is being updated, deletes pre-existing order, pushes new order to the back.
        curUser->orders.erase(curUser->orders.begin() + updateIndex);
    }
    curUser->orders.push_back(order); //Push new order to the back.
}
/**
 * @param a
 * @param curOrder
 * @param row
 * @param seat
 * @return none
 *
 * Removes a individual seat.
 */
void RemoveSeat(Auditorium* a, vector<Seat*> *curOrder, int row, char seat){
    Node<Seat*>* curNode = a->getHead();
    while(curNode->getPayload()->getRow() != row){
        curNode = curNode->getDown();
    }
    while(curNode->getPayload()->getSeat() != seat){
        curNode = curNode->getRight();
    }
    curNode->getPayload()->setTicketType('.'); //seat - '.'
    size_t x = 0;
    while(curOrder->at(x)->getSeat() != seat){ //Finds seat to remove
        x++;
    }
    curOrder->erase(curOrder->begin() + (int)x); //Removes seat
}
/**
 * @param auditorium
 * @param row
 * @param seat
 * @param totalQuantity
 * @return True if all empty, false if at least one is reserved.
 *
 * Checks if all seats selected by user are empty.
 * Should be used in conjunction with other methods (ReserveSeats() and BestAvailable()).
 */
bool CheckAvailability(Auditorium* a, int row, char seat, int totalQuantity){
    Node<Seat*>* curNode = a->getHead();
    while(curNode->getPayload()->getRow() != row){
        curNode = curNode->getDown();
    }
    while(curNode->getPayload()->getSeat() != seat){
        curNode = curNode->getRight();
    }
    for(int i = 0; i < totalQuantity; i++){
        if (curNode->getPayload()->getSeat() >= seat) {
            if (curNode->getPayload()->getTicketType() != '.') {
                return false;
            }
            if(curNode->getPayload()->getRow() == a->getTail()->getPayload()->getRow()
               && curNode->getPayload()->getSeat() == a->getTail()->getPayload()->getSeat()){
                return true;
            }
        }
        else {
            return false;
        }
        if(curNode->getPayload()->getRow() == a->getTail()->getPayload()->getRow()
        && curNode->getPayload()->getSeat() == a->getTail()->getPayload()->getSeat()){
            return false;
        }
        curNode = curNode->getRight();
    }
    return true;
}

/**
 * @param auditorium
 * @return none
 *
 * Displays information about the auditorium and ticket sales.
 * Is called when user exits the menu & when exiting the program.
 */
void DisplayReport(Auditorium* a, int auditoriumNum){
    int adultSold = 0, childSold = 0, seniorSold = 0;
    Node<Seat*>* curNode = a->getHead();
    size_t to = seatString.find(a->getTail()->getPayload()->getSeat());
    while(curNode){
        switch(curNode->getPayload()->getTicketType()){
            case 'A':
                adultSold++;
                break;
            case 'C':
                childSold++;
                break;
            case 'S':
                seniorSold++;
                break;
            default:
                break;
        }
        curNode = curNode->getRight();
    }
    cout << left << setw(11) << "Auditorium " << auditoriumNum; //Auditorium
    cout << right << setw(8) << a->getTail()->getPayload()->getRow()*(to+1) - (adultSold+childSold+seniorSold); //Open Seats
    cout << setw(8) << adultSold+childSold+seniorSold; //Reserved Seats
    cout << setw(8) << adultSold; //Adult Tickets
    cout << setw(8) << childSold; //Child Tickets
    cout << setw(8) << seniorSold; //Senior Tickets
    cout << setw(8) << fixed << setprecision(2) << "$" << left << (adultSold*10.0)+(childSold*5.0)+(seniorSold*7.50) << endl;
}
/**
 * @param auditorium
 * @return none
 *
 * Displays information about every auditorium.
 * Is called when exiting out of the program.
 */
void DisplayFinalReport(Auditorium** auditoriums){
    int adultSold = 0, childSold = 0, seniorSold = 0, total = 0;
    for(int x = 0; x < 3; x++) {
        Node<Seat *> *curNode = auditoriums[x]->getHead();
        while (curNode) {
            switch (curNode->getPayload()->getTicketType()) {
                case 'A':
                    adultSold++;
                    break;
                case 'C':
                    childSold++;
                    break;
                case 'S':
                    seniorSold++;
                    break;
                default:
                    break;
            }
            total++;
            curNode = curNode->getRight();
        }
    }
    cout << left << setw(12) << "Total"; //Total
    cout << right << setw(8) << total - (adultSold+childSold+seniorSold); //Open Seats
    cout << setw(8) << adultSold+childSold+seniorSold; //Reserved Seats
    cout << setw(8) << adultSold; //Adult Tickets
    cout << setw(8) << childSold; //Child Tickets
    cout << setw(8) << seniorSold; //Senior Tickets
    cout << setw(8) << fixed << setprecision(2) << "$" << left << (adultSold*10.0)+(childSold*5.0)+(seniorSold*7.50) << endl;
}
/**
 * @param s
 * @return true if s is int, false if otherwise
 *
 * Checks if user input is an int or not.
 */
bool isInt(const string& s){
    for(char c : s){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}

/**TEST CASES**/
int test01(){
    Auditorium* a = new Auditorium("sample_theater.txt", 1);
    /**DisplayAuditorium()
     * Should Display:
     * ABCDEFGHIJKLMNOPQRST
     *1...##..#####........
     *2########....####..##
     *3.........##.........
     *4#.#.#.#.#.#.#.#.#.#.
     *5########.#####.#####
     **/
    DisplayAuditorium(a);
    /**CheckAvailability()**/
    bool check1 = CheckAvailability(a, 2, 'I', 5);
    ///Test: Empty start, empty finish
    if(check1){
        cout << "ERROR 1 : Check Availability" << endl;
    }
    bool check2 = CheckAvailability(a, 3, 'L', 9);
    ///Test: Empty start, reserved finish
    if(!check2){
        cout << "ERROR 2 : Check Availability" << endl;
    }
    bool check3 = CheckAvailability(a, 3, 'K', 3);
    ///Test: Resrved start, empty finish
    if(check3){
        cout << "ERROR 3 : Check Availability" << endl;
    }
    /**ReserveSeats()
     *3######...##.........
     **/
    ReserveSeats(a, new User(), 3, 'A', 2, 2, 2);
    DisplayAuditorium(a);
    /**BestAvailable**/
    Node<Seat*>* test = BestAvailable(a, 2);
    if(test->getPayload()->getSeat() != 'H')
        cout << "ERROR 1 : Best Available" << endl;
    if(test->getPayload()->getRow() != 3)
        cout << "ERROR 2 : Best Available" << endl;
    /**DisplayReport()
     * Should Display:
     * Total Seats:    100
     * Total Tickets:  57
     * Adult Tickets:  25
     * Child Tickets:  16
     * Senior Tickets: 16
     * Total Sales:    $450.00
     **/
    DisplayReport(a, 1);
    return 0;
}

int main() {
    string fileName, str; //Used in reading file into auditorium
    string username, password;
    ifstream inFile;
    Auditorium* Auditoriums[3];
    unordered_map<string, User*> userDB;
    //Create Auditorium
    for(int x = 0; x < 3; x++){
        stringstream audNum;
        audNum << (x+1);
        string auditoriumFile = "A" + audNum.str() + ".txt";
        Auditoriums[x] = new Auditorium(auditoriumFile, x+1);
    }
    //Create User Database
    inFile.open("userdb.dat");
    while(getline(inFile,str)){
        //if(str.find('\r'))
        username = str.substr(0, str.find(' '));
        password = str.substr(str.find(' ')+1);
        User* temp = new User(username, password);
        pair<string,User*> p(username, temp);
        userDB.insert(p);
    }
    inFile.close();
    //Menu Start
    int menuInput;
    string mInput;
    bool exit = false;
    while(!exit) {
        bool isLoggedIn = false;
        User* curUser;
        while(!isLoggedIn) { //Begin Log-in Process
            cout << "Enter Username: ";
            cin >> username;
            unordered_map<string,User*>::const_iterator got = userDB.find(username);
            if(got != userDB.end()) {
                for(int attempt = 1; attempt <= 3; attempt++) {
                    cout << "Enter Password: ";
                    cin >> password;
                    if(got->second->password == password) {
                        curUser = got->second;
                        isLoggedIn = true;
                        break;
                    }
                    else{
                        cout << "Invalid password" << endl;
                    }
                }
            }
        }
        if(curUser->username == "admin"){ //Admin Menu
            while(isLoggedIn){
                cout << "1. Print Report" << endl;
                cout << "2. Logout" << endl;
                cout << "3. Exit" << endl;
                cin >> mInput;
                if (isInt(mInput)) {
                    stringstream int1(mInput);
                    int1 >> menuInput;
                }
                if(menuInput == 1){
                    cout << "Auditorium #   Empty Reserve       A       C       S       Total $" << endl;
                    for(int x = 0; x < 3; x++){
                        DisplayReport(Auditoriums[x], x+1);
                    }
                    DisplayFinalReport(Auditoriums);
                }
                if(menuInput == 2){
                    isLoggedIn = false;
                }
                if(menuInput == 3){
                    isLoggedIn = false;
                    exit = true;
                }
            }
        }
        while (isLoggedIn) { //Menu
            menuInput = -1;
            cout << "1. Reserve Seats" << endl;
            cout << "2. View Orders" << endl;
            cout << "3. Update Order" << endl;
            cout << "4. Display Receipt" << endl;
            cout << "5. Log Out" << endl;
            cin >> mInput;
            if (isInt(mInput)) {
                stringstream int1(mInput);
                int1 >> menuInput;
            }
            //cin >> menuInput; //Invalid # = loop to beginning
            if (menuInput == 1) { //1. Reserve Seats
                Auditorium *a = nullptr;
                while (!a) { //Pick Auditorium
                    menuInput = -1;
                    cout << "1. Auditorium 1" << endl;
                    cout << "2. Auditorium 2" << endl;
                    cout << "3. Auditorium 3" << endl;
                    cin >> mInput;
                    if (isInt(mInput)) {
                        stringstream int1(mInput);
                        int1 >> menuInput;
                    }
                    if (menuInput <= 3 && menuInput >= 1) {
                        a = Auditoriums[menuInput-1];
                    }
                }
                DisplayAuditorium(a);
                //Five inputs total
                int row = -1, adultTickets = -1, childTickets = -1, seniorTickets = -1;
                char seatLetter = -1;
                string ticketInput;
                //1. row input
                while (row < 1 || row > a->getTail()->getPayload()->getRow()) {
                    cout << "Row Number: ";
                    cin >> ticketInput;
                    if (isInt(ticketInput)) {
                        stringstream int1(ticketInput);
                        int1 >> row;
                    }
                }
                //2. seatLetter input
                size_t to = seatString.find(a->getTail()->getPayload()->getSeat());
                while (seatString.substr(0, to + 1).find(seatLetter) == string::npos) {
                    cout << "Starting seat letter:  ";
                    cin >> seatLetter;
                }
                //3. adultTickets input
                while (adultTickets < 0) {
                    cout << "Number of adult tickets:  ";
                    cin >> ticketInput;
                    if (isInt(ticketInput)) {
                        stringstream int1(ticketInput);
                        int1 >> adultTickets;
                    }
                }
                //4. childTickets input
                while (childTickets < 0) {
                    cout << "Number of child tickets:  ";
                    cin >> ticketInput;
                    if (isInt(ticketInput)) {
                        stringstream int1(ticketInput);
                        int1 >> childTickets;
                    }
                }
                //5. seniorTickets input
                while (seniorTickets < 0) {
                    cout << "Number of senior tickets: ";
                    cin >> ticketInput;
                    if (isInt(ticketInput)) {
                        stringstream int1(ticketInput);
                        int1 >> seniorTickets;
                    }
                }
                //After all inputs are collected...
                if (CheckAvailability(a, row, seatLetter, adultTickets + childTickets + seniorTickets)) {
                    ReserveSeats(a, curUser, row, seatLetter, adultTickets, seniorTickets, childTickets);
                }
                //If selected seats are unavailable, tries to find the best alternative.
                else {
                    Node<Seat *> *bestSeat = nullptr;
                    bestSeat = BestAvailable(a, adultTickets + childTickets + seniorTickets);
                    //if bestSeat = -1, a better seat was not found.
                    if (bestSeat != nullptr) {
                        char rhs = (char)(bestSeat->getPayload()->getSeat() + adultTickets + childTickets + seniorTickets - 1);
                        cout << bestSeat->getPayload()->getRow() << bestSeat->getPayload()->getSeat() << " - "
                             << bestSeat->getPayload()->getRow() << rhs << endl;
                        char bestSeatInput = -1;
                        //Asks if user would like to reserve the new seats.
                        while (bestSeatInput != 'Y' && bestSeatInput != 'N') {
                            cout << "Reserve Seat(s)? Y/N:  ";
                            cin >> bestSeatInput;
                        }
                        if (bestSeatInput == 'Y') {
                            ReserveSeats(a, curUser, bestSeat->getPayload()->getRow(), bestSeat->getPayload()->getSeat(),
                                         adultTickets, seniorTickets, childTickets);
                        }
                    } else {
                        cout << "no seats available" << endl;
                    }
                }
            }
            else if (menuInput == 2) { //2. View Orders
                if(!curUser->orders.empty()) { //If user has orders assiciated with account
                    for (const vector<Seat *> &order : curUser->orders) { //Prints every order
                        PrintOrder(order);
                        cout << endl;
                    }
                }
                else{
                    cout << "No orders available." << endl;
                }
            }
            else if (menuInput == 3) { //3. Update Order
                bool inUpdateLoop = true; //Does not return to menu until order is modified in some way, or if no orders are available
                if (!curUser->orders.empty()) {
                    size_t orderNum;
                    vector<Seat *> *curOrder = nullptr;
                    int curOrderNum = 1; //Counts orders when printing
                    for (const vector<Seat *> &order : curUser->orders) {
                        stringstream ss;
                        ss >> curOrderNum;
                        cout << "Order #" << ss.str() << endl;
                        PrintOrder(order);
                        curOrderNum++;
                        cout << endl;
                    }
                    //Gets input
                    while (curOrder == nullptr) {
                        cout << "Enter order to edit: ";
                        cin >> mInput;
                        if (isInt(mInput)) {
                            stringstream int1(mInput);
                            int1 >> orderNum;
                        }
                        if (orderNum - 1 < curUser->orders.size()) {
                            curOrder = &curUser->orders.at(orderNum - 1);
                        }
                    }
                    //Begin update menu
                    while(inUpdateLoop) {
                        cout << "1. Add tickets to order" << endl;
                        cout << "2. Delete tickets from order" << endl;
                        cout << "3. Cancel order" << endl;
                        cin >> mInput;
                        if (isInt(mInput)) {
                            stringstream int1(mInput);
                            int1 >> menuInput;
                        }
                        if (menuInput == 1) {
                            int auditoriumNum = curOrder->at(0)->getGroupNum();
                            Auditorium *a = Auditoriums[auditoriumNum - 1];
                            DisplayAuditorium(a);
                            //Five inputs total
                            int row = -1, adultTickets = -1, childTickets = -1, seniorTickets = -1;
                            char seatLetter = -1;
                            string ticketInput;
                            //1. row input
                            while (row < 1 || row > a->getTail()->getPayload()->getRow()) {
                                cout << "Row Number: ";
                                cin >> ticketInput;
                                if (isInt(ticketInput)) {
                                    stringstream int1(ticketInput);
                                    int1 >> row;
                                }
                            }
                            //2. seatLetter input
                            size_t to = seatString.find(a->getTail()->getPayload()->getSeat());
                            while (seatString.substr(0, to + 1).find(seatLetter) == string::npos) {
                                cout << "Starting seat letter:  ";
                                cin >> seatLetter;
                            }
                            //3. adultTickets input
                            while (adultTickets < 0) {
                                cout << "Number of adult tickets:  ";
                                cin >> ticketInput;
                                if (isInt(ticketInput)) {
                                    stringstream int1(ticketInput);
                                    int1 >> adultTickets;
                                }
                            }
                            //4. childTickets input
                            while (childTickets < 0) {
                                cout << "Number of child tickets:  ";
                                cin >> ticketInput;
                                if (isInt(ticketInput)) {
                                    stringstream int1(ticketInput);
                                    int1 >> childTickets;
                                }
                            }
                            //5. seniorTickets input
                            while (seniorTickets < 0) {
                                cout << "Number of senior tickets: ";
                                cin >> ticketInput;
                                if (isInt(ticketInput)) {
                                    stringstream int1(ticketInput);
                                    int1 >> seniorTickets;
                                }
                            }
                            //After all inputs are collected...
                            if (CheckAvailability(a, row, seatLetter, adultTickets + childTickets + seniorTickets)) {
                                ReserveSeats(a, curUser, row, seatLetter, adultTickets, seniorTickets, childTickets,
                                             (int) (orderNum - 1));
                                inUpdateLoop = false;
                            }
                            else {
                                cout << "Seats unavailable" << endl;
                            }
                        }
                        else if (menuInput == 2) {
                            int row;
                            char seat;
                            cout << "Row Number: ";
                            cin >> row;
                            cout << "Starting seat letter:  ";
                            cin >> seat;
                            int auditoriumNum = curOrder->at(0)->getGroupNum();
                            Auditorium *a = Auditoriums[auditoriumNum - 1];
                            for (size_t x = 0; x < curOrder->size(); x++) {
                                if (seat == curOrder->at(x)->getSeat() || row == curOrder->at(x)->getRow()) { //If seat and row supplied are within the order
                                    RemoveSeat(a, curOrder, row, seat); //Removes seat
                                    if (curOrder->empty()) { //Deletes if order has no seats
                                        curUser->orders.erase(curUser->orders.begin() + (int) orderNum);
                                    }
                                    inUpdateLoop = false;
                                    break;
                                }
                                else if ((x + 1) == curOrder->size()) {
                                    cout << "Invalid selection." << endl;
                                }
                            }
                        }
                        else if (menuInput == 3) {
                            Auditorium *a = Auditoriums[curOrder->at(0)->getGroupNum() - 1];
                            while (!curOrder->empty()) { //Removes every seat in order
                                RemoveSeat(a, curOrder, curOrder->at(0)->getRow(), curOrder->at(0)->getSeat());
                            }
                            curUser->orders.erase(curUser->orders.begin() + (int) (orderNum - 1)); //Deletes order
                            inUpdateLoop = false;
                        }
                    }
                }
                else {
                    cout << "No orders" << endl;
                    inUpdateLoop = false;
                }
            }
            else if (menuInput == 4) { //4. Display Receipt
                PrintReceipt(curUser->orders);
            }
            else if (menuInput == 5) { //5. Log-Out
                isLoggedIn = false;
            }
        }
    }
    cout << "Auditorium #   Empty Reserve       A       C       S       Total $" << endl;
    //Outputting contents of auditorium into 'A#Final.txt'.
    for(int x = 0; x < 3; x++) {
        stringstream audNum;
        audNum << (x+1);
        string name = "A" + audNum.str() + "Final.txt";
        ofstream outfile(name);
        Node<Seat *> *curNode = Auditoriums[x]->getHead();
        while (curNode) {
            if (curNode->getPayload()->getSeat() == 'A' && curNode != Auditoriums[x]->getHead()) {
                outfile << endl;
            }
            outfile << curNode->getPayload()->getTicketType();
            curNode = curNode->getRight();
        }
        outfile.close();
        DisplayReport(Auditoriums[x], x+1);
    }
    DisplayFinalReport(Auditoriums);
    return 0;
}
