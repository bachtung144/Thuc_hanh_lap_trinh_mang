#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>

char fileName[] = "nguoidung.txt";
typedef struct list {
  	char username[30];
	char password[30];
	int status;
    int login ;
    char homePage[30];
	struct list *next;
}list;

list*head = NULL;

void menu(){
    printf("USER MANAGEMENT PROGRAM \n");
	printf("----------------------------------- \n");
	printf("1. Register \n");
	printf("2. Activate \n");
	printf("3. Sign in \n");
	printf("4. Search \n");
    printf("5. Change passowrd \n");
    printf("6. Sign out \n");
    printf("7. Homepage with domain name \n");
    printf("8. Homepage with IP address \n");
	printf("Your choice (1-8, other to quit): \n");
}

void insertUser(char username[30], char password[30], char homePage[30], int status,int login){
	list *tmp;
	tmp=(list *)malloc(sizeof(list));
	strcpy(tmp->username, username);
	strcpy(tmp->password, password);
	tmp->status = status;
    strcpy(tmp->homePage, homePage);
	tmp->next = head;
    tmp ->login = login;
	head = tmp;
}

void readFile(char fileName[]){
    FILE *fptr;
    char *username;
    char *password;
    char *homePage;
    int status;
    list *acc;
    username = (char *)malloc(30);
	password = (char *)malloc(30);
    homePage = (char *)malloc(30);
    fptr=fopen(fileName,"r+");
    if(fptr == NULL){
		printf("not found file \n");
		return;
	}
    while (1)
    {
        fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
        fscanf(fptr,"%s",homePage);
		if(feof(fptr)) break;
        else insertUser(username, password, homePage, status,0);
    }
}

void writeFile(char fileName[]){
	FILE *fptr;
	list *tmp;
    tmp = head;
    fptr=fopen(fileName,"w+");
    while(tmp){
    	fprintf(fptr, "%s %s %d %s", tmp->username, tmp->password, tmp->status, tmp->homePage);
    	fprintf(fptr, "\n");
    	tmp=tmp->next;
    }
    fclose(fptr);
}

list *find(char name[20]){
   list *current = head;

   if(head == NULL) return NULL;
   while(strcmp(current->username, name) != 0){
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
      }
   }      
   return current;
}

// chuc nang 1 dang ki tai khoan
void registerAccount(){
    char username[30];
    char password[30];
    char homePage[30];
    printf("Pls input Username: "); 
    scanf("%s", username);
	if(find(username) != NULL)
        printf("Account existed\n");	
	else{
        printf("Pls inputPassword:"); 
        scanf("%s", password); 
        printf("Pls input homePage: "); 
        scanf("%s", homePage);
		insertUser(username, password, homePage, 2, 0);
		writeFile(fileName);
        printf("Successful registration. Activation required.\n");
	}
}

// chuc nang 2 active tai khoan
void activeAccount(){
    char username[30];
    char password[30];
    printf("Pls input Username: "); 
    scanf("%s", username);
    printf("\n");
    printf("Pls inputPassword:"); 
    scanf("%s", password);
    int checkCode = 0;
    char code[20];
    list * curr = find(username);
    while (1)
    {
        printf("Pls input code :");
        scanf("%s",code);
        if (strcmp(code,"LTM121216") !=0)
        {
            printf("Account is not activated\n");
            checkCode ++;
            if (checkCode ==  3)
            {
            printf("Activation code is incorrect.\nAccount is blocked\n");
            curr -> status = 0;
            writeFile(fileName);
            break;
            }
        }else {
            printf("Account is activated !\n") ;
            curr -> status = 1;
            writeFile(fileName);
            break;
        }
    }   
}

// chuc nang 3 sign in
void signIn(){
    char username[30];
    char password[30];
    int checkPass = 0;
    printf("Pls input Username: "); 
    scanf("%s", username);
    printf("\n");
    list * curr = find(username);
    if (curr == NULL) printf("Cannot find account\n");
    else if (curr -> status == 0) printf("Account is blocked\n");
    else if (curr -> status == 2) printf("Account is not actived\n");
    else
    {
        while (1)
        {
           printf("Pls inputPassword:"); 
           scanf("%s", password);
           if (strcmp(password,curr->password) != 0)
           {
               checkPass ++;
               printf("Password is incorrect\n");
               if (checkPass == 3)
               {
                   printf("Password is incorrect. Account is blocked\n");
                   curr -> status = 0;
                   writeFile(fileName);
                   break;
               }
           }
           else
           {
               printf("Hello %s\n",username); //thieu dau cach
               curr -> login = 1;
               break;
           }
        }   
    }
}

//chuc nang 4 tim kiem 
void search(){
    char username[30];
    char password[30];
    int checkPass = 0;
    printf("Pls input Username: "); 
    scanf("%s", username);
    printf("\n");
    list * curr = find(username);
    if (curr == NULL) printf("Cannot find account\n");
    else
    {
        if(curr -> status == 1) printf("Account is active\n");
        else if(curr -> status == 0) printf("Account is blocked\n");
        else printf("Account is not active\n");
    } 
}

//chuc nang 5 change password
void changePass(){
    char username[30];
    char password[30];
    char newPass[30];

    printf("Pls input Username: "); 
    scanf("%s", username);
    printf("\n");
    printf("Pls input password: "); 
    scanf("%s", password);
    printf("\n");
    printf("Pls input newPass: "); 
    scanf("%s", newPass);
    printf("\n");
    list * curr = find(username);
    if (strcmp(curr->password,password) != 0)
        printf("Currentpassword is incorrect.Please try again\n");
    else
    {
        printf("Password is changed\n");
        strcpy(curr->password, newPass);
        writeFile(fileName);
    }
}

//chuc nang 6 signOut
void signOut(){
    char username[30];
    printf("Pls input Username: "); 
    scanf("%s", username);
    printf("\n");
    list * curr = find(username);
    if (curr == NULL) printf("Cannot find account\n");
    else
    {
        if (curr->login == 0) printf("Account is not sign in\n");
        else
        {
            curr ->login = 0;
            printf("Goodbye %s\n",username);
        }
    }
}

int checkHostnameOrIp(char *info)
{
	if(info[1] > 47 && info[1] < 58)
		return 1; //is IP
	else
		return 0; // is Hostname
}

//chuc nang 7 convert domain to ip
int handleDomain() 
{
    char username[30];
    char ip[100];
    printf("Pls input Username: "); 
    scanf("%s", username);
    list * curr = find(username);
    if (curr ->login == 0) printf(" you are not login\n");
    else
    {
        if (checkHostnameOrIp(curr->homePage) == 1)
        {
            printf("this is IP \n");
        }
        else
        {
            struct hostent *he;     
            struct in_addr **addr_list;     
            int i;     
            if ((he = gethostbyname(curr->homePage)) == NULL)     
            {
                printf("Not found information.\n");
            }     
            addr_list = (struct in_addr **) he->h_addr_list;
            for(i = 0; addr_list[i] != NULL; i++)
            {
                strcpy(ip , inet_ntoa(*addr_list[i]) );
            }
            printf("IP : %s \n",ip);
        }
    }
    
    
}

//chuc nang 8 convert ip to domain name
void handleIP() {
    char hostname[100];
    char username[30];
    printf("Pls input Username: "); 
    scanf("%s", username);
    list * curr = find(username);
    if (curr ->login == 0) printf(" you are not login\n");
    else
    {
        if (checkHostnameOrIp(curr->homePage) == 0)
        {
            printf("this is hostname\n");
        }
        else
        {
            struct in_addr addr;
            struct hostent* esu;
            inet_aton(curr->homePage, &addr);
            esu = gethostbyaddr(((const char*)&addr),sizeof(addr), AF_INET);
            if(esu == NULL)
            {
                printf("Not found information\n");
            }
            strcpy(hostname, esu->h_name);
            printf("host name : %s\n",hostname);
        }
    }  
}

int main(int argc, char const *argv[])
{
    int choice;
    readFile(fileName);
    do
    {
        menu();
        scanf("%d",&choice);
        switch (choice)
        {
            case 1: registerAccount(); break;
            case 2: activeAccount(); break;
            case 3: signIn(); break;
            case 4: search(); break;
            case 5: changePass(); break;
            case 6: signOut(); break;
            case 7: handleDomain(); break;
            case 8: handleIP(); break;
        default:
            break;
        }
    } while (choice > 0 & choice < 9);
    return 0;
}
