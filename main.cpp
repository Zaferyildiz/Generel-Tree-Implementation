/* @Author  Student Name: <Zafer Yıldız>
 * Student ID: <150170054>
 * Date: <21/12/2019> */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct tree;                                            //The network
struct base_station;
struct mobile_host;
struct child_base_stations;                             //To hold child base stations of a base station
struct child_mobile_hosts;                              //To hold mobile hosts of a base station
struct Stack_node;
struct Stack;                                           //I've used stack structure to hold the shortest path.

struct Stack_node{
    base_station* node;
    Stack_node* next;
};

struct Stack{
    Stack_node* head;
    void create();
    void push(base_station* node);
    void close();
};

void Stack::create() {
    head = NULL;
}

void Stack::push(base_station *node) {
    Stack_node* newnode = new Stack_node;
    newnode->node = node;
    newnode->next = head;
    head = newnode;
}

void Stack::close() {
    Stack_node *p;
    while (head){
        p = head;
        head = head->next;
        delete p;
    }
}

struct child_base_stations {                                //I've used a linked list structure to hold child BSs of a base station
    base_station *headchildBS;                              //First child BS of a base station
    void create();
    void add(base_station *newnode);
};

struct child_mobile_hosts {                                 //I've used a linked list structure to hold child MH of a base station
    mobile_host *headchildMH;                               //First MH of a base station
    void create();
    void addMH(mobile_host *newnode);
};

struct base_station{
    int id;                                                 //Id of the base station
    int parent;                                             //Id of the parent base station
    base_station* next;                                     //It points sibling base station
    base_station* parentBS;                                 //To hold the adress of parent base station
    void create2();
    struct child_base_stations childBS;                     //I've used a linked list structure to hold child BSs of a base station
    struct child_mobile_hosts childMH;                      //I've used a linked list structure to hold MHs of a base station
};

struct mobile_host {                                        //Node structure of a mobile host
    mobile_host *next;                                      //It points sibling MH
    int id;
    int parent;
};

void base_station::create2(){                               //It initializes the base station structure
    id = 0;
    parent = 0;
    next = NULL;
    childBS.headchildBS = NULL;
    childMH.headchildMH = NULL;
}

void child_base_stations::create() {                        //It initializes the linked list of child base stations
    headchildBS = NULL;
}

void child_base_stations::add(base_station *newnode) {      //It adds a new base station to the linked list
    if (headchildBS == NULL) {                              //If head in NULL that means the linked list is empty and head should be point newnode
        headchildBS = newnode;
        newnode->next = NULL;
    }else{                                                  //If the linked list is not empty
        base_station *temp = headchildBS;                   //I've used a temp ptr to specify the correct place for the newcoming node
        while(temp->next != NULL){                          //The correct place is last position, so I've slided temp ptr to the end of the linked list
            temp = temp->next;
        }
        temp->next = newnode;                               //And I've added the newcoming node as the last node of linked list
        newnode->next = NULL;
    }
}

void child_mobile_hosts::create() {                         //It initializes the linked list of child mobile hosts
    headchildMH = NULL;
}

void child_mobile_hosts::addMH(mobile_host *newnodem){      //It adds a new MH to the child MH linked list of a base station
    if(headchildMH == NULL){                                //If the head is NULL, that means linked list is empty
        headchildMH = newnodem;                             //So, we should add newcoming node as the first node
        newnodem->next = NULL;
    }else{                                                  //If the linked list is not empty
        mobile_host *temp = headchildMH;                    //I should find the correct place for the newcoming node
        while(temp->next != NULL){                          //I've slided temp ptr to the end of the linked list
            temp = temp->next;
        }
        temp->next = newnodem;                              //Finally, I've made necessary connections to add the newcoming node to the end of the linked list
        newnodem->next = NULL;
    }
}

struct tree{                                                //Network structure
    base_station *root;                                     //Central controller
    void create();                                          //It initializes the content of the central controller
    void insertBS(base_station *node);                      //It adds new BS to the network
    void insertMH(mobile_host *node);                       //It add new MH to the network
    void search(base_station *root, int id);                //It traverses the whole network to find a specific base station
    void traverseBS_for_MH(base_station *find, int id);     //It searches a specific MH on the network
    void find_path(base_station* node);                     //It finds the path of a base station from the central controller
    struct Stack path;                                      //It holds the path of a BS from the central controller
};

void tree::create(){                                        //It initializes the central controller
    base_station *newnode = new base_station;               //I've implemented the CC as a base stations that has no MH.
    newnode->create2();                                     //Function call for initialization
    newnode->parent = -1;                                   //I've specified the parent of the CC as -1
    newnode->childBS.create();                              //It creates child BS linked list of the CC
    newnode->childMH.create();                              //It creates child MH linked list of the CC (NULL)
    root = newnode;
    root->parentBS = NULL;                                  //CC has no parent BS
    path.create();
}

base_station *target = new base_station;                    //I've used global base station to hold the adress of the node that I'm looking for

void tree::search(base_station *find, int id){              //It searches for a specific node in the whole network
    if (find != NULL) {                                     //If find is not NULL
        if (find->id == id){                                //And if the id of the node I'm looking now is equal to id I am searching
            target = find;                                  //I updated the target, because I found the node I want to find
            return;
        }
        search(find->childBS.headchildBS, id);              //I called the same function for the child of the node I am looking now
        search(find->next, id);                             //and for the sibling
    }
}

bool found = false;                                         //I've used a global boolean variable to hold whether I've found the node I'm searching for or not

void tree::traverseBS_for_MH(base_station *find, int id){   //I've used this function to search for a MH in the network
                                                            //(when sending a message I have to control whether there is a MH I will send a message or not)
    if(!found){                                             //If the MH is not found
        if (find != NULL) {                                 //And the base station of this MH is not NULL (A BS can not be NULL it is because of recursive control)
            cout<<find->id<<" ";                            //I am printing the id of the node I'm looking now
            mobile_host* temp = find->childMH.headchildMH;  //And I've used a temp ptr to control whether the child MH linked list of this BS includes the MH I'm searching for
            while(temp){                                    //I'm traversing on the child MH linked list of this BS
                if(temp->id == id){                         //If the temp's id is equal to id I'm searching for
                    target = find;                          //I've updated target(global base station)
                    found = true;                           //And I've updated found as true
                    return;
                }else{
                    temp = temp->next;                      //If temp's id is not equal to id then I've slided the temp on the linked list
                }
            }
            traverseBS_for_MH(find->childBS.headchildBS, id); //I've called the same function for the child of the BS I'm looking now
            traverseBS_for_MH(find->next, id);                //And for the sibling
        }
    }
}

void tree::insertBS(base_station *node){                    //It inserts a new Bs to the network
    int parent = node->parent;
    search(root, parent);                              //I've used search function to find parent BS of the BS I want to add
    base_station *temp = target;                            //After I've found
    temp->childBS.add(node);                                //I've add the newcoming BS as the child of the node I've found
}

void tree::insertMH(mobile_host *node){                     //It inserts a new MH to the network
    int parent = node->parent;
    base_station *newnode = new base_station;
    newnode->create2();
    newnode->id = parent;
    newnode->next = NULL;
    search(root, parent);                              //I've used search function again to find the parent BS of the newcoming MH
    base_station *temp = new base_station;
    temp->create2();
    temp = target;
    temp->childMH.addMH(node);                              //I've added newcoming MH as the child MH of the Bs I've found by search function
}

void tree::find_path(base_station *node){                   //It finds the path for the BS from the CC and holds it in a stack
    base_station* temp = new base_station;
    temp->create2();
    temp = node;
    while(temp->id != 0){                                   //Until temp reaches to the CC
        path.push(temp);                                    //I've pushed the BS on road to the path
        temp = temp->parentBS;                              //I've moved up to the parent BS
    }
    path.push(root);                                  //To print "0", I've finally pushed the root(CC)
    while(path.head){                                       //Then, I've printed the path
        cout<<path.head->node->id<<" ";
        path.head = path.head->next;
    }
    path.close();                                           //And, I've deleted path.
}

int main(int argc, char* argv[]) {
    char* file1 = argv[1];
    char* file2 = argv[2];

    struct tree network;                                    //Network is created
    network.create();

    ifstream network_file(file1);
    if(network_file.is_open()){                             //If the file is opened in a correct way
        while(!network_file.eof()){                         //Until it reaches the end of file
            string type;
            int id,parent;
            network_file>>type>>id>>parent;                     //It reads a string, id and parent of the BS or MH
            if(type != "" && type != "\n"){                     //If the type is not a new line or empty string
                if(type == "BS"){                               //And if the type is BS
                    base_station *newnode = new base_station;   //I've created a new BS
                    newnode->create2();
                    newnode->id = id;
                    newnode->parent = parent;
                    newnode->next = NULL;
                    newnode->childBS.create();
                    newnode->childMH.create();
                    network.search(network.root,parent);        //I've used search function to find the parent BS of the BS I want to insert(Target is updated)
                    newnode->parentBS = target;                 //I've assigned target to the parentBS of the BS I want to add
                    network.insertBS(newnode);                  //I've inserted newnode by using insertBS function
                }else if(type == "MH"){                         //If the type is MH
                    mobile_host *newnode = new mobile_host;     //I've created a new MH
                    newnode->id = id;
                    newnode->parent = parent;
                    newnode->next = NULL;
                    network.insertMH(newnode);                  //I've inserted MH by using insertMH function
                }
            }
        }
    }else{
        cout<<"File could not be opened\n";
    }
    network_file.close();

    ifstream message_file(file2);
    if(message_file.is_open()){
        while(!message_file.eof()){
            string input;
            getline(message_file,input);                      //I've read a line from the messages file as a string
            if(input != "" && input != "\n"){                       //If the type is not equal to empty string or new line
                int pos = input.find(">");                       //I've divided string into two part(before ">" and after ">")
                string message = input.substr(0,pos);          //Actual message that will be send to a MH is the first part of the string
                string mh_str = input.substr(pos+1);
                stringstream str(mh_str);                           //I've used stringstream to convert string to integer
                int mh = 0;
                str >> mh;
                cout<<"Traversing:";
                found = false;                                      //Global variable that holds whether MH is found or not
                network.traverseBS_for_MH(network.root,mh);         //I'am searching MH in the network by this function
                cout<<endl;
                if(found == false){                                 //If the MH is not found in the network
                    cout<<"Can not be reached the mobile host mh_"<<mh<<" at the moment"<<endl;
                }else{                                              //If the MH is found
                    cout<<"Message:"<<message<<" To:";
                    network.find_path(target);                      //It finds path and then prints the path
                    cout<<"mh_"<<mh<<endl;
                }
            }
        }
    }else{
        cout<<"File could not be opened\n";
    }
    message_file.close();
    return 0;
}