//Author: 1200704-Jihad Anfous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//passenger Linked List node:
struct passenger
{
    int studentID, travelDate;
    char travelTime[100],from[150],to[150];
    struct passenger *next;
};

//info of the Bus:
struct Bus
{
    int busNum,date,capacity;
    float ticketPrice;
    char departureTime[100],from[150],to[150];
    struct passenger *head;
};

//function prototypes:
void readPassData(struct passenger *p);
struct passenger* makeEmptyPass(struct passenger *p);
void DeletePassList(struct passenger* p);
void insertPassData(struct passenger *p,struct passenger *newNode);
int numOfBusses();
void fillBusInfo(struct Bus busses[]);
void menu();
void printLine();
int isEmpty(struct passenger *p);
void printPassList(struct passenger *p);
int isLast(struct passenger *x, struct passenger *p);
void printBusInfo(struct Bus busses[], int *size);
void assignPassengers(struct Bus bus[],struct passenger *p,int *size,struct passenger *unmatched);
void insertPassToBus(struct Bus bus, int capacity, struct passenger *newNode);
int findBus(int busNum , struct Bus bus[], int *size);
void addNewPass(struct passenger *p);
struct passenger* findPrev(struct passenger *p, int studentID);
void deletePass(struct passenger *p,struct passenger *unmatched,struct Bus bus[],int *size);
void deleteBusNum(struct Bus bus[], int *size,struct passenger *unmatched);
void deleteNode(struct passenger *p, int id);
void printLine2();
void specificBus(struct Bus busses[],int *size);


int main()
{
    struct passenger *p,*unmatched;//pointers on the passenger structure
    p=NULL;//assigned it to NULL to avoid assigning random values to it
    p = makeEmptyPass(p);//Build the header of the linked list
    unmatched = NULL;
    unmatched = makeEmptyPass(unmatched);
    int *size;
    int temp = numOfBusses();
    size = &temp;
    struct Bus busses[*size];//made the size as pointer to control it when I use delete Bus
    //assigning NULL value to all the headers of passengers linked list in Bus
    for(int i=0; i<numOfBusses(); i++)
    {
        busses[i].head=NULL;
    }
    int choice = 0;
    printf("Welcome to my station program..\n");
    printf("choose what you want from the following menu:\n");
    printLine2();
    menu();
    printLine2();
    scanf("%d",&choice);
    //to keep choosing from the menu until number 9 is entered
    while(choice!=9)
    {
        if(choice==1)
        {
            printLine2();
            fillBusInfo(busses);//reading info from bus file and store it in busses array
            printBusInfo(busses,size);//to print all the info of the busses
            printf("\n");
            menu();
            //printLine();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==2)
        {
            printLine();
            readPassData(p);//reading info from passenger file and store it in linked list p
            printPassList(p);//print all the info of the passengers
            printf("\n");
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==3)
        {
            printLine2();
            assignPassengers(busses,p,size,unmatched);/*to assign the passengers to the correct busses and fill the unmatched linked list
                                                        with the passengers that there is no bus for them*/
            printLine2();
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==4)
        {
            printLine2();
            specificBus(busses,size);
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==5)
        {
            printLine2();
            printf("the unmatched passengers list:\n\n");
            printLine();
            printPassList(unmatched);//print the unmatched passengers linked list
            printf("\n");
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==6)
        {
            printLine2();
            addNewPass(p);//add new passenger to linked list p
            printLine2();
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else if(choice==7)
        {
            printLine2();
            deletePass(p,unmatched,busses,size);//delete passenger from linked list p
            printLine2();
            menu();
            printLine2();
            printf("\n");
            scanf("%d",&choice);
        }
        else if(choice==8)
        {
            printLine2();
            deleteBusNum(busses,size,unmatched);//delete bus from the busses array by shifting the elements of the array
            printf("\n");
            menu();
            printLine2();
            scanf("%d",&choice);
        }
        else
        {
            printf("you have to choose numbers from the list only.\n");
            scanf("%d",&choice);
        }
    }
    return 0;
}


void menu()
{
    printf("1. Load the bus information file\n"
           "2. Load the passenger information file\n"
           "3. Assign passengers and print assignment information of all busses\n"
           "4. Print a specific bus information along with its passengers information\n"
           "5. Print unmatched passengers\n"
           "6. Add new passenger\n"
           "7. Delete passenger\n"
           "8. Delete bus number\n"
           "9. Exit\n");

}

//read passengers data from passengers file and store it in linked list of passengers
void readPassData(struct passenger *p)
{
    char line[1000];
    FILE *passengerFile;
    passengerFile = fopen("passengers.txt","r");//open passengers.txt file to read only
    struct passenger *newNode;
    newNode = (struct passenger*)malloc(sizeof(struct passenger));//building new passenger Node
    p->next = newNode;//putting newNode after the header of the passengers linked list
    newNode->next=NULL;//making the next of the newNode NULL which means it will be the last node
    int length = 0;
    while(!feof(passengerFile))//!feof means not the end of the file
    {
        fgets(line,1000,passengerFile);//read full line of data from passengers.txt and store it in line
        length = strlen(line);//length of the line string
        if(line[length-1]=='\n')//to avoid reading new line character
        {
            line[length-1]='\0';
        }
        //splitting the line and store each data to it's correct place in the newNode
        newNode->studentID = atoi(strtok(line,"#"));//split and converts from string to integer
        newNode->travelDate = atoi(strtok(NULL,"#"));
        strcpy(newNode->travelTime, strtok(NULL,"#"));
        strcpy(newNode->from, strtok(NULL,"#"));
        strcpy(newNode->to, strtok(NULL,"#"));
        insertPassData(p,newNode);//to add the newNode to linked list p
        newNode = (struct passenger*)malloc(sizeof(struct passenger));//build another new node to store in it the next data
    }
    fclose(passengerFile);//close the passengers.txt file
}

//read from bus file and store in busses array
void fillBusInfo(struct Bus busses[])
{
    int index = 0;
    char line[1000];
    FILE *BusFile;
    BusFile = fopen("busses.txt","r");//open busses.txt file for reading only
    int length = 0;
    while(!feof(BusFile))
    {
        fgets(line,1000,BusFile);
        length = strlen(line);
        if(line[length-1]=='\n')//to avoid reading new line character
        {
            line[length-1]='\0';
        }
        busses[index].busNum = atoi(strtok(line,"#"));
        busses[index].date = atoi(strtok(NULL,"#"));
        strcpy(busses[index].departureTime,strtok(NULL,"#"));
        strcpy(busses[index].from,strtok(NULL,"#"));
        strcpy(busses[index].to,strtok(NULL,"#"));
        busses[index].ticketPrice = atof(strtok(NULL,"#"));//to split and convert the string into float and store it in bus's ticketPrice
        busses[index].capacity = atoi(strtok(NULL,"#"));
        index++;
    }
    fclose(BusFile);//close busses.txt file
}

//know the number of busses by reading line count of data in bus file
int numOfBusses()
{
    int counter = 0;
    char line [1000];
    FILE *BusFile;
    BusFile = fopen("busses.txt","r");
    while(!feof(BusFile))
    {
        fgets(line,1000,BusFile);
        counter++;
    }
    fclose(BusFile);

    return counter;
}

/*make a header for the linked list if there was no head for it and check if
it's used before to empty it and use it*/
struct passenger* makeEmptyPass(struct passenger *p)
{
    if(p!=NULL)
    {
        DeletePassList(p);//delete all the linked list if it was used before
    }
    p = (struct passenger*)malloc(sizeof(struct passenger));//build the header

    if(p==NULL)
    {
        printf("memory is out of space\n");
    }
    p->next=NULL;//make the next of the header point on NULL
    return p;//return the header
}

//delete all nodes of the linked list
void DeletePassList(struct passenger* p)
{
    struct passenger *temp,*x;
    x = p->next;
    p->next=NULL;

    while(x!=NULL)//to traverse all the nodes in the linked list
    {
        temp = x->next;
        free(x);//delete the node
        x = temp;
    }
}

//insert node at the last of the passenger linked list
void insertPassData(struct passenger *p,struct passenger *newNode)
{
    struct passenger *temp;
    temp = p;
    while(temp->next!=NULL)//to get the last node
    {
        temp = temp->next;//to keep traversing the the nodes of the linked list
        if(temp->next==NULL)//if it's true this means that we've reached the last node
        {
            temp->next = newNode;//make the newNode after the last node
            newNode->next = NULL;//make the next of the newNode points on NULL
        }
    }
}

//print all the data of the passenger linked list
void printPassList(struct passenger *p)
{
    struct passenger *x;
    x = p;
    if(isEmpty(x))
    {
        printf("The List is Empty.\n");
    }
    else
    {
        printf("Student ID  \t Travel Date  \t Travel Time  \t From  \t\t To\n");
        printLine();
        do
        {
            x = x->next;
            printf("%d  \t %d  \t %s  \t %s  \t %s\n",x->studentID,x->travelDate,x->travelTime,x->from,x->to);
            printLine();
        }while(!isLast(x,p));
    }
}

//check if the passenger linked list is empty
int isEmpty(struct passenger *p)
{
    return p->next==NULL;
}

//check if the node is the last node of the passenger linked list
int isLast(struct passenger *x, struct passenger *p)
{
    return x->next==NULL;
}

void specificBus(struct Bus busses[],int *size)
{
    struct passenger *temp;
    int busNum = 0;
    printf("Enter the bus number:");
    scanf("%d",&busNum);
    int index = findBus(busNum,busses,size);//get the index of the wanted bus
    if(index!=-1)
    {
       if(busses[index].head==NULL)
       {
           busses[index].head = (struct passenger*)malloc(sizeof(struct passenger));
           busses[index].head->next = NULL;
       }
       temp = busses[index].head;//header of the assigned passengers linked list
       printf("Bus Number: %d , Date: %d , Time: %s , From: %s , To: %s\n",busses[index].busNum,busses[index].date,
                                                                           busses[index].departureTime,busses[index].from,
                                                                           busses[index].to);
       if(temp->next!=NULL)//to make sure that the node is not the last
       {
            printf("passengers ID: ");
            //print the info of all the nodes that are in the linked list
            do
            {
                temp = temp->next;
                printf("%d\t",temp->studentID);
            }while(!isLast(temp,busses[index].head));
            printf("\n");
            printLine();
       }
       else
       {
            printf("There is no passengers in the bus\n");
            printLine();
       }

    }
    else if(index==-1)//findBus function returns -1 if the bus is not found
    {
        printf("The bus is not in the list\n");
        printLine();
    }

}

void printLine()
{
    printf("--------------------------------------------------------------------------\n");
}

//print all the info of the bus
void printBusInfo(struct Bus busses[], int *size)
{
    printf("Bus Number  \t Date  \t\t Departure Time  \t From  \t\t To  \t\tTicket Price  \t Capacity\n");
    printf("--------------------------------------------------------"
           "-----------------------------------------------------------\n");
    for(int i=0; i<numOfBusses();i++)
    {
        printf("%d  \t\t %d  \t      %s  \t\t %s  \t %s  \t %.2f NIS  \t  %d\n",busses[i].busNum,busses[i].date,busses[i].departureTime,
                                                                                busses[i].from,busses[i].to,
                                                                                busses[i].ticketPrice,busses[i].capacity);
        printf("--------------------------------------------------------"
               "-----------------------------------------------------------\n");
    }
}

//to assign the passengers to the right bus
void assignPassengers(struct Bus bus[],struct passenger *p,int *size,struct passenger *unmatched)
{

    //creating headers of passengers linked list for all the busses
    for(int i=0; i<numOfBusses(); i++)
    {
        bus[i].head = (struct passenger*)malloc(sizeof(struct passenger));
        bus[i].head->next = NULL;
    }
    struct passenger *temp,*temp1,*temp2,*temp3,*newNode,*newNode2;
    newNode = (struct passenger*)malloc(sizeof(struct passenger));
    newNode2 = (struct passenger*)malloc(sizeof(struct passenger));
    temp1 = p;
    temp3 = unmatched;
    int i,hoursP,minP,hoursB,minB,insertFlag;
    char temporaryString[10];//to store in it the time that I want to split
    while(temp1->next!=NULL)
    {
        temp1 = temp1->next;

        insertFlag=1;

        strcpy(temporaryString,temp1->travelTime);

        hoursP = atoi(strtok(temporaryString,":"));//split the time of the passenger into hours and convert it into integer

        minP = atoi(strtok(NULL,":"));//split the time of the passenger into minutes and convert it into integer

        for(i=0; i<*size; i++)
        {
            strcpy(temporaryString,bus[i].departureTime);

            hoursB = atoi(strtok(temporaryString,":"));//split the time of each bus and convert it into integer

            minB = atoi(strtok(NULL,":"));

            if(temp1->travelDate==bus[i].date && ((hoursP<hoursB)||(hoursP==hoursB && minP<=minB))
                && strcmp(temp1->from,bus[i].from)==0  && strcmp(temp1->to,bus[i].to)==0)/*check if the date , time ,location and
                                                                                           destination of the bus matches with
                                                                                           the passenger*/
            {
                if(bus[i].head->next==NULL)//if the passenger linked list of the bus is empty
                {
                    temp2 = bus[i].head;
                    newNode->next = temp2->next;
                    temp2->next = newNode;
                }
                else
                {
                    while(temp2->next!=NULL)//if it's not empty go to the last node of it
                    {
                        temp2 = temp2->next;
                    }
                }
                newNode->studentID = temp1->studentID;
                newNode->travelDate = temp1->travelDate;
                strcpy(newNode->travelTime,temp1->travelTime);
                strcpy(newNode->from,temp1->from);
                strcpy(newNode->to,temp1->to);
                insertPassToBus(bus[i],bus[i].capacity,newNode);//insert the node to the passenger linked list of the bus
                insertFlag = 0;
                //printf("The passenger who has this ID: %d has been assigned to bus %d\n",temp1->studentID,bus[i].busNum);
                deleteNode(p,temp1->studentID);//delete the node from passenger linked list after assigning it to the correct bus
                newNode = (struct passenger*)malloc(sizeof(struct passenger));
                break;
            }
        }
        if(insertFlag!=0)//if it's 0 means that the node has been assigned
                        //if it's 1 means that the node has not been assigned so insert it into the unmatched linked list
        {
            while(temp3->next!=NULL)
            {
                temp3 = temp3->next;
            }
            newNode2->next = temp3->next;
            temp3->next = newNode2;
            newNode2->studentID = temp1->studentID;
            newNode2->travelDate = temp1->travelDate;
            strcpy(newNode2->travelTime,temp1->travelTime);
            strcpy(newNode2->from,temp1->from);
            strcpy(newNode2->to,temp1->to);
            insertPassData(unmatched,newNode2);//insert the node to the unmatched linked list
            deleteNode(p,temp1->studentID);
            newNode2 = (struct passenger*)malloc(sizeof(struct passenger));
        }
    }
    for(int i=0; i<*size; i++)
    {
        printf("Bus Number: %d , Date: %d , Time: %s , From: %s , To: %s\n",bus[i].busNum,bus[i].date,
                                                                           bus[i].departureTime,bus[i].from,
                                                                           bus[i].to);

        temp = bus[i].head;//header of the assigned passengers linked list
        if(temp->next!=NULL)//to make sure that the node is not the last
        {
            printf("passengers ID: ");
            //print the info of all the nodes that are in the linked list
            do
            {
                temp = temp->next;
                printf("%d\t",temp->studentID);
            }while(!isLast(temp,bus[i].head));
            printf("\n");
            printLine();
       }
       else
       {
            printf("There is no passengers in the bus\n");
            printLine();
       }
    }
}

//delete the node from passenger linked list depending on the passenger ID
void deleteNode(struct passenger *p, int id)
{
    struct passenger *temp,*prev;
    prev = findPrev(p,id);//find the previous node of the node that we want to delete
    if(!isLast(prev,p))//if the previous node is not the last node...delete the node that becomes after it
    {
        temp = prev->next;
        prev->next = temp->next;//bypass delete cell
        free(temp);//delete the node from the memory
    }
}

//insert passenger node to passenger linked list of bus depending on the capacity
void insertPassToBus(struct Bus bus, int capacity, struct passenger *newNode)
{
    struct passenger *temp;
    temp = bus.head;
    int numOfPass = 0;
    while(temp->next!=NULL)//if the passenger linked list of the buss is not empty count how many nodes inside it
    {
        temp = temp->next;
        numOfPass++;
    }
    if(numOfPass>0 && numOfPass<capacity)//if the counted nodes are less than the capacity, add the node to the linked list
    {
        insertPassData(bus.head,newNode);
    }
    return;//return from the function without doing anything
}

//find the specified bus by using it's number
int findBus(int busNum , struct Bus bus[], int *size)
{
    for(int i=0;i<*size;i++)
    {
        if(bus[i].busNum==busNum)
        {
            return i;//return it's index if it's found
        }
    }
    return -1;//if the bus is not found
}

//add new passenger to the passenger linked list
void addNewPass(struct passenger *p)
{
    struct passenger *newNode;
    newNode = (struct passenger*)malloc(sizeof(struct passenger));
    struct passenger *temp,*tempD,*previous;
    if(p->next!=NULL)//if the linked list is not empty
    {
        temp=p;
        insertPassData(temp,newNode);//insert the newNode to the passenger linked list
    }
    else//if the passenger linked list is empty
    {
        temp = p;
        temp->next = newNode;
        newNode->next = NULL;
        insertPassData(temp,newNode);
    }
    int studentID = 0;
    printf("Enter Student ID:");
    scanf("%d",&studentID);
    while(temp->next!=NULL)
    {
        if(temp->studentID==studentID)//to avoid repetition
        {
            printf("The student ID exists, you can't repeat it.\n");
            while(temp->next->next!=NULL)
            {
                temp = temp->next;
            }
            tempD = temp->next;
            temp->next = NULL;
            free(tempD);/*delete the empty node that I've inserted into the passenger
                          if there was another node has the same passenger ID*/
            return;
        }
        temp = temp->next;
    }
    //else fill the passenger information and add it to the linked list
    newNode->studentID = studentID;
    printf("Enter the travel Date as follows(ddmmyy):");
    scanf("%d",&newNode->travelDate);
    printf("Enter the travel Time in 24-hour format:");
    scanf("%s",newNode->travelTime);
    printf("Enter the location of the student:");
    getchar();
    scanf("%[^\n]",newNode->from);
    printf("Enter the destination of the student:");
    getchar();
    scanf("%[^\n]",newNode->to);
    printf("The passenger has been added successfully\n");
    printLine();
    printPassList(p);
}

//to find the previous node of a specified node
struct passenger* findPrev(struct passenger *p, int passID)
{
    struct passenger *temp;
    temp = p;
    while(temp->next!=NULL && temp->next->studentID!=passID)//while the node is not the last and the data of the next node is not equal to the ID
    {
        temp = temp->next;
    }
    return temp;
}

//check in which list the passenger is, then delete it
void deletePass(struct passenger *p,struct passenger *unmatched,struct Bus bus[],int *size)
{
    int studentID = 0;
    printf("Enter the ID number of the passenger you want to delete:");
    scanf("%d",&studentID);
    struct passenger *previous,*temp;
    printLine2();
    if(p->next!=NULL)//if the passenger linked list is not empty
    {
        temp = p;
        while(temp->next!=NULL)
        {
            temp = temp->next;
            if(temp->studentID==studentID)//if the node has the same inserted passenger ID
            {
                //deletion:
                previous = findPrev(p,studentID);
                if(!isLast(previous,p))
                {
                    deleteNode(p,studentID);
                    printf("The passenger has been deleted successfully from the passengers List\n\n");
                    printLine2();
                    printPassList(p);
                    return;
                }
                else
                {
                    printf("The passenger is not in the list\n");
                    return;
                }
            }
        }
    }

    if(unmatched->next!=NULL)// if the unmatched linked list is not empty
    {
        temp = unmatched;
        while(temp->next!=NULL)
        {
            temp = temp->next;
            if(temp->studentID==studentID)//if the node has the same inserted passenger ID
            {
                //deletion:
                previous = findPrev(unmatched,studentID);
                if(!isLast(previous,unmatched))
                {
                    deleteNode(unmatched,studentID);
                    printf("The passenger has been deleted successfully from the unmatched List\n\n");
                    printf("The unmatched List: \n");
                    printLine();
                    printPassList(unmatched);
                    return;
                }
                else
                {
                    printf("The passenger is not in the list\n");
                    return;
                }
            }
        }
    }

    for(int i=0; i<*size; i++)//to check all the busses
    {
        if(bus[i].head->next!=NULL)
        {
            temp = bus[i].head;
            while(temp->next!=NULL)
            {
                temp = temp->next;
                if(temp->studentID==studentID)//check if the node has the same inserted passenger ID
                {
                    //deletion:
                    previous = findPrev(bus[i].head,studentID);
                    if(!isLast(previous,bus[i].head))
                    {
                        deleteNode(bus[i].head,studentID);
                        printf("the passenger has been deleted successfully from bus %d\n",bus[i].busNum);
                        printf("The passengers that are in bus %d:\n",bus[i].busNum);
                        if(bus[i].head->next!=NULL)
                        {
                            printLine();
                            printPassList(bus[i].head);
                            return;
                        }
                        else
                        {
                            printf("There is no passengers\n");
                            return;
                        }
                    }
                    else
                    {
                        printf("The passenger is not in the list\n");
                        return;
                    }
                }
            }
        }
    }
    printf("The passenger is not in the list\n");
    return;

}

//delete bus from the busses array and add the passengers assigned to it to the unmatched list
void deleteBusNum(struct Bus bus[], int *size,struct passenger *unmatched)
{
    int busNum = 0;
    struct passenger *newNode,*temp;
    printf("Enter the number of the bus you want to delete:");
    scanf("%d",&busNum);
    int index = findBus(busNum,bus,size);
    if(*size>0)//to check that there is busses
    {
        if(index>=0)//to make sure that the bus exists
        {
            if(bus[index].head==NULL)/*to avoid the runtime error that happens
                                        when the passengers are not assigned to the busses*/
            {
                bus[index].head = (struct passenger*)malloc(sizeof(struct passenger));
                bus[index].head->next = NULL;
            }
            if(bus[index].head->next!=NULL)//check if the bus has passengers in it
            {
                temp = bus[index].head;
                while(temp->next!=NULL)//to transfer the passengers to the unmatched list
                {
                    temp = temp->next;
                    newNode = (struct passenger*)malloc(sizeof(struct passenger));
                    newNode->studentID = temp->studentID;
                    newNode->travelDate = temp->travelDate;
                    strcpy(newNode->travelTime,temp->travelTime);
                    strcpy(newNode->from,temp->from);
                    strcpy(newNode->to,temp->to);
                    insertPassData(unmatched,newNode);//insert the newNode to the unmatched
                }
                DeletePassList(bus[index].head);//delete the passenger linked list of the bus
            }
            for(int i = index; i<*size-1; i++)
            {
                bus[i] = bus[i+1];//shift the elements of the array to the left
            }
            *size = *size-1;//decrement the size of the array by 1 because of shifting

            //print the information of all the busses after shifting
            printf("The bus has been deleted successfully.\n\n");
            printLine2();
            printf("Bus Number  \t Date  \t\t Departure Time  \t From  \t\t To  \t\tTicket Price  \t Capacity\n");
            printLine2();
            for(int i=0; i<*size;i++)
            {
                 printf("%d  \t\t %d  \t      %s  \t\t %s  \t %s  \t %.2f NIS  \t  %d\n",bus[i].busNum,bus[i].date,bus[i].departureTime,
                                                                                         bus[i].from,bus[i].to,
                                                                                         bus[i].ticketPrice,bus[i].capacity);
                 printLine2();
            }
        }
        else
            printf("The bus you want to delete is not in the List.\n");

    }
    else
    {
        printf("There is no busses to delete.\n");
        return;
    }
}

void printLine2()
{
     printf("--------------------------------------------------------"
            "-----------------------------------------------------------\n");
}
