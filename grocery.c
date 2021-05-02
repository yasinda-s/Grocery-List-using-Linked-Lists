
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct item{ //create a structure data type called item
    int id;
    char name[20];
    double cost;
    char category;
    struct item *next;
}item;

item * addItem(item * head); //function prototypes
void searchItem(item * head);
int changeCost(item *head);
void displayItems(item *head);
item * deleteItem(item *head);
item * sortItems(item *head);
int changeLen(item *head);

int main(void) {
    char fileName[100]; //to hold file name
    FILE *fp; //file pointer to open
    while(1){ //infinite loop that breaks when user enters the right file name and file type
        printf("Enter the name of the text file that contains the data with .txt extension : ");
        scanf("%s", fileName); //file name is scanned into the variable
        if((fp = fopen(fileName, "r+"))==NULL){
            puts("File Could not be opened! Please Retry!");
        }else{
            puts("File has been loaded successfully!");
            break;
        }
    }

    item * head = NULL; //main head node
    item * p = NULL;
    item * temp = NULL;
    item * newItem = NULL;
    item * changePrice = NULL;
    item * delItem = NULL;

    while(!feof(fp)){ //while the end of file haven't been reached
        const int check =getc(fp); //to make sure the last line is not filled in as empty
        if(check==EOF){
            break;
        }else{
            ungetc(check, fp);
        }
        temp = (item *)malloc(sizeof(item)); //allocate a temporary memory space of size item to temp
        fscanf(fp, "%d %s %lf %c",&(temp->id), (temp->name), &(temp->cost), &(temp->category)); //specify where to scan to - linked list nodes
        (temp->name)[0]=toupper((temp->name)[0]); //capitalize first letter of prod name
        temp->category=toupper(temp->category); //capitalize category
        temp->next=NULL; //we know the next is still null

        if(head==NULL){ //if first element is been scanned
            head = temp; //then head is temp
        }else{ //if this is second scan of item or higher
            p = head; //p becomes head
            while(p->next!=NULL){ //as long as there is a node next to p
              p=p->next; //make p that node
            } //when the position of p becomes null, then we add scanned node to the next node
            p->next = temp; //now next node of p becomes the scanned item
        }
    }
    fclose(fp); //close the file pointer (done with reading data as everything in ll now)

    int saveFlag = 0; //initialize to 0
    while(1){ //infinite loop till user decides to quit program
        int userTask;
        char sortorNot[10];
        int PreAdd,PostAdd;
        int PreDelete,PostDelete;
        int CostChange;
        printf("1. Add New Item\n2. Delete Item\n3. Change the cost of an item\n4. Search for item\n5. Display inventory details\n6. Quit\n");
        printf("Enter the task you want to execute :");
        scanf("%d",&userTask);
        if(userTask>0 && userTask<=6){
            switch(userTask){
                case 1: //add case
                    PreAdd = changeLen(head); //check length before add
                    head =addItem(head); //call function and save back changes on head
                    PostAdd = changeLen(head); //check length after add
                    if(PreAdd!=PostAdd){ //if there was an item added then we set set flag to 1
                        saveFlag=1;
                    }
                    break;
                case 2: //delete case
                    PreDelete = changeLen(head); //check length before del
                    head=deleteItem(head); //call function and save back changes on head
                    PostDelete = changeLen(head); //check length after del
                    if(PreDelete!=PostDelete){ //if item was deleted then save flag = 1
                        saveFlag=1;
                    }
                    break;
                case 3: //changing price
                    CostChange = changeCost(head); //call function and save back changes on head
                    if(CostChange==1){ //if price was changed, set save flag to one
                        saveFlag=1;
                    }
                    break;
                case 4: //search item
                    searchItem(head); //void function as there is no permanent change
                    break;
                case 5: //display items on list
                    displayItems(head); //void function as there is no permanent change
					printf("Would you like to sort your list? Type in 'Yes' or 'No' : ");
					scanf("%s",sortorNot);
					if (strcmp(sortorNot,"Yes")==0 || strcmp(sortorNot,"yes")==0 || strcmp(sortorNot,"YES")==0){ //to handle any type of yes
						sortItems(head); //if user wants to sort, then call function
					}
                    break;
                case 6: //quit
                    printf("Closing menu...\n");
                    break;
            }
        }else{ //any other input is a retry
            printf("Input out of range! Please retry\n");
        }
        if(userTask==6){
            if(saveFlag==1){ //check if changes were made
                printf("There have been some changes to the original grocery list! \n");
                char yesOrNo[20];
                printf("Would you like to save to file? Enter 'Yes' if you want to save the List :");
                scanf("%s", yesOrNo);
                if (strcmp(yesOrNo,"Yes")==0 || strcmp(yesOrNo,"yes")==0 || strcmp(yesOrNo,"YES")==0){ //handle all yes types
                    char fileWrite[50];
                    while(1){ //infinite till user gives write name to save on
                        printf("Enter the file name to write to with .txt extension :");
                        scanf("%s", fileWrite);
                        if(strcmp(fileName,fileWrite)==0){ //cant save on same name as loaded file condition
                            printf("Cant save over loaded file! Please Retry!\n");
                        }else{
                            FILE *writeTo = fopen(fileWrite,"w+");
                            while(head!=NULL){
                                if(head->next!=NULL){
                                    fprintf(writeTo,"%d %s %.2lf %c\n", head->id,head->name,head->cost,head->category);
                                }else{
                                    fprintf(writeTo,"%d %s %.2lf %c", head->id,head->name,head->cost,head->category);
                                }
                                head=head->next;
                            }
                            printf("Thank you for using the grocery list program! Have a nice day!");
                            return 0; //after save we can close the program
                        }
                    }
                }else{ //if user doesnt want to save
                    printf("The list will not be saved! Thank you!");
                    break;
                }
            }else{ //if saveflag wasnt 1
                printf("No changes detected on the grocery list! Thank you!\n");
                break;
            }
        }
    }
    return 0;
}

item * addItem(item * head){
    item * newItem = NULL; //create new node
    item * last = head;
    newItem = (item *)malloc(sizeof(item)); //allocate space for newnode

    printf("Product ID : ");
    scanf("%d", &(newItem->id));
    printf("Product Name : ");
    scanf("%s", (newItem->name));
    (newItem->name)[0] = toupper((newItem->name)[0]); //capitalize
    printf("Product Cost : ");
    scanf("%lf", &(newItem->cost));
    printf("Product Category : ");
    scanf(" %c", &(newItem->category)); //now the new node is filled with the data
    newItem->category=toupper(newItem->category);

    newItem->next = NULL; //since this is the last node
    int dupCount=0; //to check for duplicates
    int nameCount=0; //to check for duplicates
    if(head==NULL){ //if head is null, then the new item should be the head
        head = newItem;
		printf("%s has been added into the database!\n",(newItem->name));
        return head;
    }
    else{
        if(last->id==newItem->id){
            dupCount++;
        }
        if(strcmp(last->name,newItem->name)==0){
            nameCount++;
        }
        while(last->next!=NULL){ //if head is not null, keep iterating to find node with no data and add there
            last = last->next;
            if(last->id==newItem->id){ //if we find duplicate increment dupcount
                dupCount++;
            }
            if(strcmp(last->name,newItem->name)==0){ //if we find duplicate increment namecount
                nameCount++;
            }
        }
        if(dupCount==0 && nameCount==0){ //if they are both zero, then we add to the next of the last node
            last->next = newItem;
            printf("%s has been added into the database!\n",(newItem->name));
        }else{
            printf("\nNot added as this is a duplicate! Try Again.\n");
        }
    }
    dupCount=0;
    nameCount=0;
    return head; //return back head with the added nodes
}

void searchItem(item * head){
    char searchName[100];
    printf("Enter the product name you want to search :");
    scanf("%s", searchName);
	searchName[0] = toupper(searchName[0]);

    int found=0; //counter to keep track if we found
    while (head!=NULL){ //iterate whole linked list
        if (strcmp(head->name,searchName)==0){ //if users and any node name is equal then it matches
            found++;
            printf("Here are the details of the item - \n"); //print the details
            printf("%d %s %.2lf %c\n", head->id, head->name, head->cost, head->category);
        }
        head = head->next; //if not found, go to the next node
    }
    if(found==0){ //if found==0 , we have no items in the linked list like user said, so print error and retry
        printf("The product name you entered is not on the list! Try Again.\n");
    }
}

int changeCost(item *head){
    int costChange=0;
    int prodID;
    double newPrice;
    printf("Enter an existing ID number :");
    scanf("%d", &prodID);
    printf("Enter the new value :");
    scanf("%lf", &newPrice);
    int match = 0;
    item * temp = head; //make copy of the head

    while(temp!=NULL){ //iterate whole ll
        if(temp->id==prodID){ //if we find the id user wants to change cost of
            match++; //increment match
        }
        temp=temp->next; //if did not find, go to next node
    }
    if (match==1){
        while (head!=NULL){
            if (head->id==prodID){
                    if(head->cost==newPrice){ //if the price is anyways the same, then no need to update save flag
                        head->cost=newPrice;
                        printf("The price is still the same!\n");
                    }else{ //else we need to update saveflag
						head->cost=newPrice;
                        printf("%s now costs %.2lf\n", head->name, head->cost);
                        costChange++; //change of cost took place so increment that
                    }
            }
            head = head->next;
        }
    }else{ //if match is not 1, then what user entered was not in the list
        printf("%d is not an Product ID in the List! Try Again!\n",prodID);
    }
    return costChange; //return the cost change, (1 or 0) back so that it helps with the save flag
}

void displayItems(item *head){
    int nodeNum =0;
	if(head==NULL){
		printf("There are no items in your grocery list!\n");
	}else{
		printf("Prod ID | Product Name | Product Price | Product Category \n");
		while(head!=NULL){
			//printf("Node %d - %d %s %lf %c\n",nodeNum, head->id, head->name, head->cost, head->category);
			printf("%d %16s %13.2lf %11c\n",head->id, head->name, head->cost, head->category);
			head = head->next;
			nodeNum++;
		}
	}
}

item * sortItems(item *head)
{
    int userSort;
    printf("How do you want to sort the list?\n 1. ProdID \n 2. Cost \n 3. Product Category \n 4. Product Name \n Enter your choice : ");
    scanf("%d",&userSort); //save what int user gave, this will determine which case (sorting) on the switch
    item *size = head;
    int sizell=0;
    while(size!=NULL){ //check the size of the linked list to run iterations (bubble sort)
        size=size->next;
        sizell++;
    }
    switch(userSort){ //will be swapping all data on node as a whole
        item * temp;
        int Valueid;
        char Valuename[20];
        double Valuecost;
        char Valuecategory;
        int AscDesc;
        case 1: //sort for ProdID
            printf("Press 1 for Ascending, 2 for Descending \n Enter your choice :");
            scanf("%d",&AscDesc);
            if(AscDesc==1){ //prod id ascending
                while(head!=NULL){
                temp = head;
                for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                    while(temp->next!=NULL){
                    if(temp->id >= temp->next->id){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }else{ //prod id desending
                while(head!=NULL){
                temp = head;
                for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                    while(temp->next!=NULL){
                    if(temp->id <= temp->next->id){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
            }
            }
        break;
        case 2: //cost
            printf("Press 1 for Ascending, 2 for Descending :\n");
            scanf("%d",&AscDesc);
            if(AscDesc==1){//cost ascending
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(temp->cost >= temp->next->cost){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }else{ //cost descending
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(temp->cost <= temp->next->cost){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }
        break;
        case 3: //sort category
            printf("Press 1 for Ascending, 2 for Descending :\n");
            scanf("%d",&AscDesc);
            if(AscDesc==1){
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(temp->category >= temp->next->category){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }else{ //category descending
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(temp->category <= temp->next->category){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }
            break;
        case 4: //descending for prod category
            printf("Press 1 for Ascending, 2 for Descending :\n");
            scanf("%d",&AscDesc);
            if(AscDesc==1){
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(strcmp(temp->name,temp->next->name)>0){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
            }
            }else{
            while(head!=NULL){
            temp = head;
            for(int i=0;i<sizell;i++){ //by the time for loop runs through the size of linked list nodes, we are certain all items have been sorted
                while(temp->next!=NULL){
                    if(strcmp(temp->name,temp->next->name)<0){
                        Valueid = temp->id; //make copy of id
                        strcpy(Valuename,temp->name); //make copy of name
                        Valuecost = temp->cost; //make copy of price
                        Valuecategory = temp->category; //make copy of category

                        temp->id = temp->next->id; //change where temp points to
                        strcpy(temp->name, temp->next->name);
                        temp->cost = temp->next->cost;
                        temp->category = temp->next->category;

                        temp->next->id = Valueid; //copy all data
                        strcpy(temp->next->name,Valuename);
                        temp->next->cost = Valuecost;
                        temp->next->category = Valuecategory;
                    }
                    temp = temp->next;
                    }
                    temp=head;
                }
                head=head->next;
                }
            }
            break;
        }
    return head;
}

item * deleteItem(item *head){
        int delProd;
		int found=0;
        printf("Enter the Product ID of the element you want to delete :");
        scanf("%d", &delProd);
		
        item *check = head;

        while (check!=NULL){ //iterate whole linked list
            if (check->id==delProd){ //if users prod id is in the node
                found++;
            }
            check = check->next; //if not found, go to the next node
        }

        if(found==0){ //item not in database
            printf("The Prod ID is not found in the database!\n");
            return head;
        }	


        item *last = NULL; //set last to null initially
        item *headPtr = head; //copy of head

        if (head == NULL) { //if head is null, then we have nothing to delete
                return head;

        }
        item *prev;
        item *curr = head;
        item *head1 = head;
        item *sLast = head1;
        item *temp;

        int counter=0;
        if(curr->next==NULL){ //if only node's next is empty
            if(curr->id == delProd){
                char temp1[30]; //make copy for the print message
                strcpy(temp1,curr->name);
                free(curr); //free the head
                printf("%s Has been deleted!\n",temp1);
                return NULL; //return empty list (NULL)
            }
        }
        last = head1->next; //make last the next of head copys
        head1 = last->next; //now get head to be the next of the last node
        while(head1!=NULL){
            sLast = last; //to locate second last node after loop
            last = head1; //locate last node after loop
            head1=head1->next; //advance head1 to next node
        }
        if((curr->next)->next==NULL){ //if 2 nodes left only
            if(delProd==curr->id){ //user deletes first node
                head=last; //last becomes head
                printf("%s Has been deleted!\n",curr->name);
                return head;
            }else if(delProd==(curr->next)->id){ //if user deletes second node
                char temp[30]; //make copy of name to print (will be gone after condition fully executes)
                strcpy(temp,(curr->next)->name);
                head=curr; //head is now curr
                head->next=NULL; //and next becomes empty
                printf("%s Has been deleted!\n",temp);
                return head;
            }
        }else{
            while (curr != NULL) { //while head is not null
                    if (curr->id == delProd) { //handle head deletion
                            if (counter==0) { //for the first time (counter is still 0)
                                    temp=last;
                                    temp->next=curr->next;
                                    sLast->next=NULL;
                                    head=temp;
                                    printf("%s Has been deleted!\n",curr->name);
                                    return head;
                            }else if(curr==sLast){ //concern for second last node
                                    prev->next=last;
                                    printf("%s Has been deleted!\n",curr->name);
                                    return head;
                            }
                            else{ //other nodes
                                    prev->next = last;
                                    last->next = curr->next;
                                    sLast->next = NULL;
                                    printf("%s Has been deleted!\n",curr->name);
                                    return head;
                            }
                    }
                    counter++;
                    prev = curr; //now previous becomes curr
                    curr = curr->next; //and curr advances to next node
            }
        }
}

int changeLen(item *head){ //function to help with the saving flag
    int count=0;
    item * temp = head;
    while(temp!=NULL){
        count++;
        temp=temp->next;
    }

    return count; //if count was diff before task on menu, then we should change saveflag
}
