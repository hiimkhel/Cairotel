#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;

void clearScreen() {
    system("cls"); 
    
}

tm stringToDate(const string& dateStr) {
    tm date = {};
    int day, month, year;
    char bar1, bar2;

    while (true) {
        stringstream ss(dateStr);
        ss >> year >> bar1 >> month >> bar2 >> day;

        // Check for correct format and separators
        if (bar1 != '-' || bar2 != '-' || ss.fail() || dateStr.size() != 10) {
            cout << "Invalid input of date or format. (YYYY-MM-DD)\nInput your date again: ";
            continue;
        }

        // Check for valid month and day ranges
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            cout << "Invalid input of date or format. (YYYY-MM-DD)\nInput your date again: ";
            continue;
        }

        // Handle months with only 30 days
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
            cout << "Invalid input of date or format. (YYYY-MM-DD)\nInput your date again: ";
            continue;
        }

        // Handle February (leap year check)
        if (month == 2) {
            bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
            if ((isLeapYear && day > 29) || (!isLeapYear && day > 28)) {
                cout << "Invalid input of date or format. (YYYY-MM-DD)\nInput your date again: ";
                continue;
            }
        }

        // If all validations pass, break out of the loop
        break;
    }

    // Convert to tm struct
    date.tm_year = year - 1900; // tm_year is years since 1900
    date.tm_mon = month - 1;    // tm_mon is 0-based
    date.tm_mday = day;

    mktime(&date); // Normalize tm struct

    return date;
}

// Guest class to hold info for a guest
// Node Guest
class Node{
private:
    string name;
    string dateStart;
    int room;
    string dateEnd;

    friend class Hotel;

    Node *next_guest;

public:

    // Initiallizing Guest info
    Node(string name, string dateStart, int room, string dateEnd)
        :name(name), dateStart(dateStart), room(room), dateEnd(dateEnd), next_guest(nullptr){}
    
    // Getter methods and setters
    string getName() const{
        return name;
    }
    void setName(const string& newName) {

        name = newName;
    }
    string getDateStart() const{
        return dateStart;
    }
    void setDateStart(const string& newDateStart) { 
        dateStart = newDateStart; 
    }
    
    int getRoom() const{
        return room;
    }
    void setRoom(int newRoom) { 
        room = newRoom; 
    }

    string getDateEnd() const{
        return dateEnd;
    }
    void setDateEnd(const string& newDateEnd){
        dateEnd = newDateEnd;
    }
    
    // Display guest info
    
    void display() const{
        cout<<"======Cairo Hotel======\n";
        cout << "Name: " << name <<"\n "
             << "Room: " << room <<"\n "
             << "Room Availability: Room is occupied from " << dateStart <<" till "<<dateEnd<<"\n";

    }

};

// Hotel class to manage registration
class Hotel{
private:
    Node* head;
    int currentRoom;
public: 
    Hotel() : head(nullptr), currentRoom(101) {}
    //Method to add guests
    void addGuest(const string& name, const string& dateStart,const string& dateEnd){
        Node* newNode = new Node(name, dateStart, currentRoom, dateEnd);
        if(!head){
            head = newNode;
        }
        else{
            Node* temp = head;
            while(temp->next_guest){
                temp = temp->next_guest;
            }
            temp->next_guest = newNode;
        }

        fstream file;
        file.open("LLHotel.csv", ios::app);

        if (file.is_open()) {
            file << name << "," << currentRoom << "," << dateStart << "," << dateEnd << "\n";
            file.close();
        } else {
            cerr << "Error: Unable to open file for writing.\n";
        }

        cout << "Guest " << name << " is assigned to Room #" << currentRoom << ".\n";
        cout << "Room #" << currentRoom << " is booked from " << dateStart << " till " << dateEnd << ".\n";

        // Increment room number for the next guest
        currentRoom++;
    
    }

    //Method to automate finding a room for a customer

    //Return Number of Days to Stay Function
    int getDaysDuration(string dateStart, string dateEnd){
        tm startDate = stringToDate(dateStart);
        tm endDate = stringToDate(dateEnd);
        time_t start = mktime(&startDate);
        time_t end = mktime(&endDate);

        double differenceInSeconds = difftime(end, start);

    int stayDuration = differenceInSeconds / (60 * 60 * 24);

    return stayDuration;
        
    }
    // Check out Customer Function (Pass)
    bool checkOut(int roomToDelete) {
        bool isDeleted = false;
        if(head == NULL){
            return false;
        }
        else{
            if(roomToDelete == head->room){ //if the room is the first node
                Node *temp =head;
                head = head->next_guest;
                isDeleted = true;
                delete temp;
            }
            else{
                Node *temp = head;
                Node *temp2 = head->next_guest;
                while(temp2 != NULL){
                    if(roomToDelete == temp2->room){
                        temp->next_guest = temp2->next_guest;

                        isDeleted = true;
                        delete temp2;
                        break;
                    }
                    temp = temp2;
                    temp2 = temp->next_guest;
                }

            }
            return isDeleted;
        }
    }
    //Display All Room Function (Pass)
    void displayAllRoomAvailability(){
        int roomOccupied = 0;
        if(head == NULL){
            cout<< "No booked guest yet.\n";
        }
        else{
            Node* temp = head;
            while(temp != NULL){
                roomOccupied++;
                temp = temp->next_guest;
            }
            cout<<"Number of Room Booked: "<< roomOccupied << "\n";
        }
    }
    //Search Guest Info (Pass)
    void searchGuest(int roomToSearch){
        Node* temp = head;
        if(head == NULL){
            cout<<"No booked guest yet.\n";

        }
        else{
            bool isFound = false;
            while(temp != NULL){
                if(roomToSearch == temp->room){
                    temp->display();
                    isFound = true;
                }
                temp = temp->next_guest;
            }
            if(!isFound){
                cout<<"Guest at Room #" <<roomToSearch << " can't be found.\n ";
            }
        }
        
   
    }
    //Edit guest Info Function (Pass)
    bool editGuestInfo(int roomToEdit){
        bool isFound = false;
        if(head==NULL){
            cout<<"No booked guest yet. \n";
            return isFound;
        }
        else{
            Node *temp = head;
            while(head != NULL){
                if(roomToEdit == temp->room){
                    cout<<"Enter new Guest Name (Press Enter to not change): ";
                    cin>>temp->name;
                    cout<<"Change start booking schedule (Press Enter to not change): ";
                    cin>>temp->dateStart;
                    cout<<"Change end booking schedule (Press Enter to not change): ";
                    cin>>temp->dateEnd;

                    cout<<"Guest Updated Succesfully";
                    isFound = true;
                }
                temp = temp->next_guest;
            }
            return isFound;
        }
    }
    //Remove Guest Function (Pass)
    bool removeGuest(int roomToRemove){
        if(head == NULL){
            return false;
        }
        else{
            if(roomToRemove == head->room){ //if the room is the first node
                Node *temp =head;
                head = head->next_guest;
                return true;
                delete temp;
            }
            else{
                Node *temp = head;
                Node *temp2 = head->next_guest;
                while(temp2 != NULL){
                    if(roomToRemove == temp2->room){
                        temp->next_guest = temp2->next_guest;
                        delete temp2;
                        break;
                    }
                    temp = temp2;
                    temp2 = temp->next_guest;
                }

            }
            return true;
        }
    }
    //Method to Display all guests (Pass)
    void displayGuest() const{
        Node* temp = head;
        if(!temp){
            cout<<"No booked guest yet.\n";
            return;
        }
        while(temp){
            temp->display();
            temp = temp->next_guest;
        }
    }

};
class Menu{//class that checks whether the user is a customer and staff
protected:
    Hotel &hotelReference;
private:
    void displayStaffOrCustomer(int choice){

            if(choice == 1){
                isCustomer = true;
                customerMode();
            }else if(choice == 2){
                isStaff = true;
                checkStaffPassword();
        
            }
            else{
                cout<<"Invalid input. Please enter 1 or 2 only!";
            }
    }

    void checkStaffPassword(){
        string inputPassword;
        const string staffPassword = "cairobabies123";

        cout<<"Enter staff's password: ";
        cin>> inputPassword;

        if(inputPassword == staffPassword){
            staffMode();
        }
        else{
            cout<<"Incorrect Password. Returning to Main Menu...\n";
            isStaff = false;
        }
    }

    void customerMode(){
        //book a room, cancel booking, 
        do{
            cout<<"</Customer Mode>\n";
            cout<<"1) Book a room.\n";
            cout<<"2) Check Out.\n";
            cout<<"3) Return to the Main Menu.\n";
            cout<<"> ";
            cin>> customerChoice;

            switch(customerChoice){
                case 1:
                    addGuestOption();
                    break;
                case 2:
                    checkOutOption();
                    break;
                default:
                    cout<<"Invalid Choice. Please try again.\n";
                    break;
            }
        }while(customerChoice != 3);
        
    }

    void staffMode(){
        do{
             cout<<"</Staff Mode>\n";
            cout<<"1) Search Guest\n";
            cout<<"2) Edit Guest Info\n";
            cout<<"3) Remove Guest\n";
            cout<<"4) Display All Guests\n";
            cout<<"5) Room Availability\n";
            cout<<"6) Return to Main Menu\n";
            cout<<"> ";
            cin>> staffChoice;
            switch (staffChoice){
                case 1:
                    searchGuestOption();
                    break;
                case 2:
                    editGuestInfoOption();
                    break;
                case 3:
                    removeGuestOption();
                    break;
                case 4:
                    displayAllGuestOption();
                    break;
                case 5:
                    roomAvailabilityOption();
                    break;
                case 6:
                    cout<<"Returning to the Main Menu...\n";
                    break;
            
                default:
                    cout<<"Invalid choice. Try again.\n";
                    break;
            }
        }while(staffChoice != 6);
       
    
    }
    //customer mode functions
    void addGuestOption(){
        string name, dateStart, dateEnd;

        cout<<"Enter your name: ";
        cin>> name;
        cout<<"Start of booking (YYYY-MM-DD): ";
        cin>> dateEnd;
        cout<<"End of booking(YYYY-MM-DD): ";
        cin>> dateStart;
        hotelReference.addGuest(name, dateStart, dateEnd);
       
        cout<<"Booking successful! Returning to Customer Menu...\n";
        system("pause");
    }

    void checkOutOption(){
        int roomToCancel;
        cout<<"Enter your room number: ";
        cin>> roomToCancel;

        bool isCheckOut = hotelReference.checkOut(roomToCancel);
        cout<<"Checking out for Room #" << roomToCancel << " is being process. Please wait.\n";
        if(isCheckOut){
          cout<<"Room #"<< roomToCancel <<" check out completed.\n";
          cout<<"Thank you for staying.\n";
        }
        else{
            cout<<"Room is not found.\n";
        }
    
    }
    
    //staff mode functions
    void searchGuestOption(){
        int roomToSearch;
        cout<<"Enter room number to search: ";
        cin>> roomToSearch;
        
        hotelReference.searchGuest(roomToSearch);
    }
    void editGuestInfoOption(){
        int roomToEdit;
        cout<<"Enter guest room# to edit info: ";
        cin>>roomToEdit;

        hotelReference.editGuestInfo(roomToEdit);

        
    }
    void removeGuestOption(){
        int roomToRemove;
        cout<<"Enter room to remove from the bookings list: ";
        cin>> roomToRemove;

        bool isRemove = hotelReference.removeGuest(roomToRemove);
        cout<<"Removing Room #" <<roomToRemove<<"  from the list...\n";
        if(isRemove){
            cout<<"Room #"<< roomToRemove <<" has been deleted from the list.\n";
           
        }
        else{
            cout<<"Room #"<< roomToRemove <<" has not found.\n";
            cout<<"Removing from list unsucccessful.\n";
        }
    }
    void displayAllGuestOption(){
        cout<<"***********Cairo Hotel System Guests list***********\n";
        hotelReference.displayGuest();
    }
    void roomAvailabilityOption(){  
        cout<<"***********Cairo Hotel Room Availability***********\n";
        hotelReference.displayAllRoomAvailability();
    }
public:
    Menu(Hotel &hotelReference) : hotelReference(hotelReference){}
    int choice, customerChoice, staffChoice;
    bool isCustomer, isStaff, isValidPassword = false;

    void runMenu(){
        do{
            cout<<"Welcome to Cairo Hotel\n";
            cout<<"1) Customer\n";
            cout<<"2) Staff\n";
            cout<<"3) Exit\n";
            cin>> choice;
            if(choice == 3){
                cout<<"Exiting program. Thank you for using Cairo Hotel System.\n";
                break;
            }
            displayStaffOrCustomer(choice);
        }while(choice != 3);
        
    }
    
};

int main(){
    Hotel hotel;
    Menu menu(hotel);
    menu.runMenu();
    return 0;
}