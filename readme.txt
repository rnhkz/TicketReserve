Compile:
Required: Cmake, make

From within TicketReserve directory:
>cmake *
>make
>./Project4

*Files A1.txt, A2.txt, A3.txt, and userDB.dat must be in same folder as executable (Located in */cmake-build-debug).

1. What is this?
A program that simulates adding/removing seat reservations to a virtual auditorium. The auditorium is visualized through the command prompt.

2. How to use:
    2.1 Input:
    Upon running the program, there will be a prompt for an input file.
        input: A grid of ascii characters representing an auditorium. This is the initial state of the auditorium.
            Where:
            . = Empty seat
            C = Child
            A = Adult
            S = Senior
        Each row should have the same number of characters. Each column should have the same number of characters.
        Example files included: A1.txt, A2.txt, A3.txt

    2.2 Usage:
    You will be prompted for a username and password.
    Usernames and passwords are stored in "userdb.dat" file in the format "USERNAME<SPACE>PASSWORD".

    Defaults are:
        user1 password - for normal menu
        admin hunter2 - for admin menu
    *Upon entering a username, you get three tries for the password before returning to the prompt for a username.

    Normal menu:
        1. Reserve Seats
            Lets you reserve a row of seats in a selected auditorium. Asks for starting row number and letter, then
            number of adult, child, and senior tickets.
        2. View Orders
            - Prints all orders made by user to screen.
        3. Update Order
            Lets you add or delete tickets to a certain order. Also lets you cancel the order entirely.
        4. Display Receipt
            - Prints orders + total cost of all orders.
        5. Log Out
            - Logs the current user out. Returns to login prompt.

    Admin menu:
        1. Print Report
            - Prints a report, detailing the types of seats (Empty, reserve, adult, child, senior), and the total cost
              of all the seats.
        2. Logout
            - Logs the admin out. Returns to login prompt.
        3. Exit
            - Exits the program. Prints report and writes the new reservations to a file.
              <AuditoriumName>Final.txt
