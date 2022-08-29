//Jamaica College Library Book Rental Report Generator
/*The following program accepts general information about a student and information related to their book.
  The program then prints out reports about book returns and overdue books.*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> 	
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Things to do...
- Write comments
*/

//macro definitions...
#define MAX_TEXT 256

//global variables...

char fileDir[MAX_TEXT];//stores the full directory of the file 
char fileName[MAX_TEXT]; //stores the name of the file
char returnBuffer[MAX_TEXT];// stores the return Date
char option; // stores the option selected by the main user
char c; // stores

int no_entries; // stores the number of books loaned
int no_overdue;// store the number of overdue books
int login_status;// stores the status of login
FILE *filehandler; //a file pointer
int overdue;

//structure declarations...
struct date {
int month; // stores the month
int day; // stores the day
int year; // stores the year
};//end of 'date' structure
//A structure called 'date' is created
struct borrower {
  char ID[10]; // stores the identificaiton no. of the student
  char fullName[MAX_TEXT]; // stores the name
  char className[MAX_TEXT]; // stores the class
  char bookTitle[MAX_TEXT]; //  stores the title of the book
  char bookID[10]; // stores the identification no. of the book
  char bookAuthor[MAX_TEXT]; //  stores the name of the book's author
  struct date borrowalDate; // the structure 'date' can now be called 'borrowalDate', which stores the date the book was borrowed
  struct date returnDate; // // the structure 'date' can now be called 'returnDate', which store the date the book should be returned
};//end of 'borrower' structure
// A structure called 'borrower' is created
struct borrower student; // the structure 'borrower' can now be called student
//struct borrower *ptr = &student; // the structure pointer 'ptr' is created and initialised.


//function prototypes...
void login(); //This function allows to user enter the program
void addBorrower(); //This function registers the student as well as the book information and passes the data on; to be stored in a file
void returnBooks(); //This function checks the data provided, for the due date of return for each book and compares it to the current date 
void gotoFunc(int x, int y); //This function sets cursor position on the screen
void hideCursor(); //This function makes the cursor invisible on the screen
void showCursor(); //This function makes the cursor visible on the screen
void menu(); //This function prints out a menu
void quit(); //This function prompts the user and exits the program
void overdueCheck(); // This function checks the file systems and stores the path of the corresponding books as overdue
int removeFile(char remove_Dir[]);// This function removes the folder directory passed to it
void viewOverdue();// This function displays all books overdue
void viewBooks();// This function displays all the books that were borrowed

//function declarations...
int main() //**
{
  option = '5';
  char PATH[MAX_TEXT];
  strcpy(PATH,"C:\\IA");

  FILE * file;
  file = fopen("C:\\IA\\init","r");
  if(file == NULL) mkdir(PATH);
  else fscanf(file,"%d",&no_entries);
  fclose(file);

  if(login_status != 1)login();

  overdueCheck();
  hideCursor();

  menu(); //This prints the options available
  option = getch();
  switch(option){
    case '1':
      system("cls");
      addBorrower();
      system("cls");
      main();
      break;
    case '2':
      returnBooks();
      break;
    case '3':
    viewBooks();
    break;
    case '4':
    viewOverdue();
    break;
    case '\e':
      quit();
      break;
    case '\b':
      quit();
      break;
    default:
      main();
    break;
  }
  return 0;
}// end of 'main' function
void gotoFunc (int x,int y)  //** sets cursor position on the screen
{
	COORD coord = {0, 0};
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

}//end of 'gotofunc' function
void showCursor() //** shows the cursor 
{
  CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = TRUE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}//end of 'showCursor' function
void hideCursor() //** hides the cursor
{
  CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}//end of 'hideCursor' function
void menu() //** This prints the main menu to the screen
{ 
  char plural;
  int i = 0;
  
  if(overdue >1 || overdue == 0)plural = 's';
  else plural = '\0';

  system("cls");
  hideCursor();
  gotoFunc(84,10);
  printf("JAMAICA COLLEGE LIBRARY MANAGEMENT PROGRAM");
  gotoFunc(73,11);
  printf("=================================================================");
  gotoFunc(84,13);
  printf("record book loan.......................[1]");
  gotoFunc(84,14);
  printf("record book return.....................[2]");
  gotoFunc(84,15);
  printf("view unreturned books..................[3]");
  gotoFunc(84,16);
  printf("view overdue books.....................[4]");
  gotoFunc(73,21);
  printf("=================================================================");
  gotoFunc(73,22);
  printf("[Esc.]");
  gotoFunc(104,22);
  printf("quit");
  gotoFunc(131,22);
  printf("[Bksp.]");
  gotoFunc(73,19);
  printf("-----------------------------------------------------------------");
  gotoFunc(95,20);
  printf("You have %d book%c overdue!",overdue,plural);
 
}//end of 'menu' function
void addBorrower() //** This function accepts the information about the student and book and writes it to a file  
{
    //gets the time the book is being borrowed
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    student.borrowalDate.day = tm.tm_mday; // stores the day that the book was borrowed
    student.borrowalDate.month = tm.tm_mon+1; // stores the month that the book was borrowed
    student.borrowalDate.year = tm.tm_year+1900; // stores the year that the book was borrowed
    
    char Year[25];
    int i; // index variable
    int clear; // index variable
    int id_edit = 0; // 
    int name_edit = 0; // status variable
    int class_edit = 0; // status variable
    int bookid_edit = 0; // status variable
    int bookname_edit = 0; //
    int author_edit = 0; // 
    int returndate_edit = 0; // 
    int from_option = 0; //


    //file operation variables
    FILE * filehandler; //
    char folderName[MAX_TEXT]; //
    char folderDir[MAX_TEXT];
    char fileName[MAX_TEXT];
    char folder[MAX_TEXT];
    char PATH[MAX_TEXT];

    strcpy(PATH,"C:\\IA\\entries");
    if(no_entries == 0)
    {
      mkdir(PATH);
    }
    showCursor();

    //General User Interface
    system("cls"); // clears the screen
    gotoFunc(78,10);//sets cursor position
    printf("Jamaica College Library Management Program: BOOK LOAN");
    gotoFunc(73,11);// sets cursor position
    printf("=================================================================");
    gotoFunc(100,13);// sets cursor position
    printf("student data:");
    gotoFunc(73,24);// sets cursor position
    printf("-----------------------------------------------------------------");
    gotoFunc(102,25);// sets cursor position
    printf("%d/%d/%d", student.borrowalDate);//prints the current date
    gotoFunc(73,26);// sets cursor positon
    printf("=================================================================");
    gotoFunc(73,27);
    printf("[Esc.]");
    gotoFunc(103,27);
    printf("cancel");
    gotoFunc(132,27);
    printf("[Del.]");

    student_id:
       i = 0; // initializes variable
        gotoFunc(84,14);
        printf("enter id #: ");   
        if(id_edit == 0)
        { 
          printf("                                                                                                                       ");
          gotoFunc(113,14);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isdigit(c) != FALSE && i < 8)
              {
               student.ID[i] = c;
                printf("%c",student.ID[i]);
                i++;
              }
              if((int)c == 8 && i > 0)
              {
               student.ID[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
               student.ID[i] = '\0';	
                break;
              }
              if((int)c == 27 ||(int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          id_edit = 1;
          if(from_option == 1) goto edit;
        }
    student_name: 
        i = 0; // initializes variable
        gotoFunc(84,15);
        printf("enter full name: ");
        if(name_edit == 0)
        {
          printf("                                                                                                                       ");
          gotoFunc(113,15);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isalpha(c) != FALSE || isspace(c) != FALSE)
              {
                student.fullName[i] = c;
                printf("%c",student.fullName[i]);
                i++;
              }//end of if statement
              if((int)c == 8)
              {
                student.fullName[i] = '\0';	
                printf("\b \b");
                i--;
              }//end of if statement
              if((int)c == 13 && i > 0)
              {
                student.fullName[i] = '\0';	
                break;
              }//end of if statement
              if(i<0)
              { 
                id_edit = 0; 
                goto student_id;
              }//end of if statement
              if((int)c == 27 || (int)c == 83)
              {
                main();
              }
            }//end of if statement
          }//end of while loop
          name_edit = 1;
          if(from_option == 1) goto edit;
    }
    student_class:
        i = 0; // initializes variable
        gotoFunc(84,16);
        printf("enter class name: ");
        if(class_edit == 0)
       { 
          printf("                                                                                                                       ");
          gotoFunc(113,16);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isalnum(c) != FALSE || isspace(c) != FALSE)
              {
                student.className[i] = c;
                printf("%c",student.className[i]);
                i++;
              }
              if((int)c == 8)
              {
                student.className[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
                student.className[i] = '\0';	
                break;
              }
              if(i<0)
              { 
                name_edit = 0; 
                goto student_name;
              }
              if((int)c == 27 || (int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          class_edit = 1;
          if(from_option == 1) goto edit;
       }
    gotoFunc(101,18);// sets cursor position
    printf("book data:");
    book_id:
        i = 0; // initializes variable
        gotoFunc(84,19);
        printf("enter id #: ");
        if(bookid_edit == 0)
        {
          printf("                                                                                                                       ");
          gotoFunc(113,19);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isdigit(c) != FALSE)
              {
                student.bookID[i] = c;
                printf("%c",student.bookID[i]);
                i++;
              }
              if((int)c == 8)
              {
                student.bookID[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
                student.bookID[i] = '\0';	
                break;
              }
              if(i<0)
              { 
                class_edit = 0; 
                goto student_class;
              }
              if((int)c == 27 || (int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          bookid_edit = 1;
          if(from_option == 1) goto edit;
        }
    book_title:
        i = 0; // initializes variable
        gotoFunc(84,20);
        printf("enter title: ");
        if(bookname_edit == 0 )
        {
          printf("                                                                                                                       ");
          gotoFunc(113,20);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isalnum(c) != FALSE || isspace(c) != FALSE)
              {
                student.bookTitle[i] = c;
                printf("%c",student.bookTitle[i]);
                i++;
              }
              if((int)c == 8)
              {
                student.bookTitle[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
                student.bookTitle[i] = '\0';	
                break;
              }
             if(i<0)
              { bookid_edit = 0; 
                goto book_id;
              }
              if((int)c == 27 || (int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          bookname_edit = 1;
          if(from_option == 1) goto edit;
      }
    author_name:
        i = 0; // initializes variable
        gotoFunc(84,21);
        printf("enter author's name: ");
        if(author_edit == 0)
        {
          printf("                                                                                                                       ");
          gotoFunc(113,21);
          while(1)
          {
            if(kbhit())
            {        
              c = getch();
              if(isalpha(c) != FALSE || isspace(c) != FALSE)
              {
                student.bookAuthor[i] = c;
                printf("%c",student.bookAuthor[i]);
                i++;
              }
              if((int)c == 8)
              {
                student.bookAuthor[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
                student.bookAuthor[i] = '\0';	
                break;
              }
             if(i<0)
              { 
                bookname_edit = 0; 
                goto book_title;
              }
              if((int)c == 27 || (int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          author_edit = 1;
          if(from_option == 1) goto edit;
        }
    return_date: 
        i = 0; // initializes variable
        clear = 0; // initializes variable
        gotoFunc(84,22);
        printf("enter return date: (NOTE: the date format is DD/MM)");
        if(returndate_edit == 0)
        {
          while(1)
          {
            if(kbhit())
            {
              if (clear == 0) for (clear = 0; clear < 32; clear++) 
              {
                printf("\b \b");
              }//end of if statement
              if(i < 1)
              {
                printf("                                                                                                                       ");
                gotoFunc(113,22);
              }
          
              c = getch();
              if(isdigit(c) != FALSE || c == '/' )
              {
                returnBuffer[i] = c;
                printf("%c",returnBuffer[i]);
                i++;
              }
              if((int)c == 8)
              {
                returnBuffer[i] = '\0';	
                printf("\b \b");
                i--;
              }
              if((int)c == 13 && i > 0)
              {
                returnBuffer[i] = '\0';
                break;
              }
              if(i<0)
              { 
                author_edit = 0; 
                goto author_name;
              }
              if((int)c == 27 || (int)c == 83)
              {
                  main();
              }
            }//end of if statement
          }//end of while loop
          returndate_edit = 1;
          if(from_option == 1) goto edit;
          sprintf(Year,"/%d", student.borrowalDate.year);
          strcat(returnBuffer,Year);
          sscanf(returnBuffer,"%d/%d/%d", &student.returnDate.day, &student.returnDate.month, &student.returnDate.year);
        }

  char ch;
  edit:
    hideCursor();
    do 
    {
      
      if(from_option == 1) // cleans up the User Interface
      {
          gotoFunc(92,25); // sets cursor position
          printf("                                 "); // prints a space 
          gotoFunc(102,25);// sets cursor position
          printf("%d/%d/%d", student.borrowalDate); // prints the current date
          gotoFunc(78,14); // sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,15); // sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,16); // sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,19); // sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,20); // sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,21);// sets cursor position
          printf("   "); // prints a space 
          gotoFunc(78,22); // sets cursor position
          printf("   "); // prints a space 
      }
      gotoFunc(73,27);// sets cursor position
      printf("[ Y. ]");
      gotoFunc(102,27);// sets cursor position
      printf("continue?");
      gotoFunc(132,27);// sets cursor position
      printf("[ N. ]");
      gotoFunc(131,27);// sets cursor position
      printf(" ");
      ch = getch();
      ch = toupper(ch);
      if (ch == 'Y')
      {
          //file operations here
          no_entries++; // updates the number of entries made
          strcpy(folderDir,PATH); //copies the directory 
          sprintf(folderName,"\\%d",no_entries);
          strcat(folderDir,folderName);
          strcpy(folder,folderDir);
          mkdir(folderDir);

          strcpy(fileName,"\\1");
          strcat(folderDir,fileName);
          
          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.ID);
          fclose(filehandler);

          strcpy(folderDir,folder);
          strcpy(fileName,"\\2");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.fullName);
          fclose(filehandler);

          strcpy(folderDir,folder);
          strcpy(fileName,"\\3");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.className);
          fclose(filehandler);

          strcpy(folderDir,folder);
          strcpy(fileName,"\\4");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.bookID);
          fclose(filehandler);

          strcpy(folderDir,folder);
          strcpy(fileName,"\\5");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.bookTitle);
          fclose(filehandler);

          strcpy(folderDir,folder);
          strcpy(fileName,"\\6");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%s",student.bookAuthor);
          fclose(filehandler);
          
          strcpy(folderDir,folder);
          strcpy(fileName,"\\7");
          strcat(folderDir,fileName);

          filehandler = fopen(folderDir,"w");
          fprintf(filehandler,"%d\\%d\\%d",student.returnDate.day,student.returnDate.month,student.borrowalDate.year);
          fclose(filehandler);

          //updates the current index count
          filehandler = fopen("C:\\IA\\init","w");
          fprintf(filehandler,"%d",no_entries);
          fclose(filehandler);

        if(from_option == 1)
        { 
          gotoFunc(92,25);// sets cursor position
          printf("                                 ");
        }
        gotoFunc(92,25);// sets cursor position
        printf("record entered successfully!");
        gotoFunc(73,27);// sets cursor position
        printf("......");
        gotoFunc(94,27);// sets cursor position
        printf("press any key to continue");
        gotoFunc(132,27);// sets cursor position
        printf("......");
        getch();
        system("cls");
        main();
      }
      if (ch == 'N')
      {
          no:
            from_option = 1;
            gotoFunc(92,25);// sets cursor position
            printf("select a number to edit the field");
            gotoFunc(73,27);// sets cursor position
            printf("[Esc.]");
            gotoFunc(102,27);// sets cursor position
            printf("         ");
            gotoFunc(104,27);// sets cursor position
            printf("cancel");
            gotoFunc(131,27);// sets cursor position
            printf("[Bksp.]");
            gotoFunc(78,14);// sets cursor position
            printf("[1]");
            gotoFunc(78,15);// sets cursor position
            printf("[2]");
            gotoFunc(78,16);// sets cursor position
            printf("[3]");
            gotoFunc(78,19);// sets cursor position
            printf("[4]");
            gotoFunc(78,20);// sets cursor position
            printf("[5]");
            gotoFunc(78,21);// sets cursor position
            printf("[6]");
            gotoFunc(78,22);// sets cursor position
            printf("[7]");
            option = getch();
            switch (option)
            {
              case'1':
                id_edit = 0;
                showCursor();
                goto student_id;
                break;
              case'2':
                name_edit = 0; // status variable
                showCursor();
                goto student_name;
                break;
              case'3':
                class_edit = 0; // status variable
                showCursor();
                goto student_class;
                break;
              case'4':
                bookid_edit = 0; // status variable
                showCursor();
                goto book_id;
                break;
              case'5':
                bookname_edit = 0;
                showCursor();
                goto book_title;
                break;
              case'6':
                author_edit = 0;
                showCursor();
                goto author_name;
                break;
              case'7':
                returndate_edit = 0;
                showCursor();
                goto return_date;
                break;
              case '\e':
               goto edit;
              break;
              case '\b':
               goto edit;
              break;

              default:
                goto no;
                break;
          }
      }
      if (ch == '\e' || ch == '\b') main();
    } while(ch != 'Y' || ch != 'N' || ch != '\e'|| ch != '\b');
}//end of 'addBorrower' function
void returnBooks() //** This function searches throughout the file system for a entered value and removes the directory with said value
{
    showCursor();
    char buffer[256];
    char search_item[256];
    char search_File[256];
    char search_Folder[256];
    char search_Dir[256];
    int option;
    char option_File[256];
    char folderDir[256];
    char folderName[256];
    char fileName[256];
    char PATH[256];
    char checker;

    struct date DUE;
    char ch;
 
    FILE * fptr;
    FILE * file;
    FILE * fileptr;
    FILE * filehandle;

    int i = 0;
    int j = 0;
    int f = 0;
    int a = 0;
    int b = 0;
    char dd;
    int ii = 0;

    int matches = 0;
    int match_status = 0;
    int results = 0;
    int occupied = 0;

    int no_matchesentries = 1;
    int no_matches = 0;
    int tot_matchentries = 0;
    int tot_matches = 0;
    int stop_status = 0;

   strcpy(PATH,"C:\\IA");
   strcpy(search_Dir,PATH);
   strcat(search_Dir,"\\search");

   //General User Interface
    system("cls"); // clears the screen
    gotoFunc(78,10);//sets cursor position
    printf("Jamaica College Library Management Program: BOOK RETURN");
    gotoFunc(73,11);// sets cursor position
    printf("=================================================================");
    gotoFunc(84,15);// sets cursor position
    printf("search:");
    gotoFunc(73,19);
    printf("-----------------------------------------------------------------");
    gotoFunc(73,21);// sets cursor positon
    printf("=================================================================");
    gotoFunc(73,22);
    printf("[Esc.]");
    gotoFunc(103,22);
    printf("cancel");
    gotoFunc(103,20); 
    printf(".....");
    gotoFunc(132,22);
    printf("[Del.]");
    gotoFunc(98,15);
    while(1) //accepts
    {
      if(kbhit())
      {        
        c = getch();
        if(isalnum(c) != FALSE || isspace(c) != FALSE)
        {
          search_item[i] = c;
          printf("%c",search_item[i]);
          i++;
        }
        if((int)c == 8 && i > 0)
        {
          search_item[i] = '\0';	
          printf("\b \b");
          i--;
        }
        if((int)c == 13 && i > 0)
        {
          i++;
          search_item[i] = '\0';
          break;
        }
        if((int)c == 27 ||(int)c == 83)
        {
            removeFile("C:\\IA\\search");
            main();
        }
      }//end of if statement
    }//end of while loop
    strcat(search_item,"*"); 
    i = 0;
    while(i < no_entries)//searchs each folder
    {
        strcpy(buffer,"");
        i++;

        strcpy(folderDir,PATH);
        sprintf(folderName,"\\entries\\%d",i);
        strcat(folderDir,folderName);
        strcpy(search_Folder,folderDir);

        while(stop_status == 0)//searchs each file
        {
            j++;
            sprintf(fileName,"\\%d",j);
            strcat(folderDir,fileName);
            
            file = fopen(folderDir,"r");
            fscanf(file,"%s",buffer);
            fclose(file);
            
            while(search_item[f] != '*')
            {
                if( toupper(buffer[f]) == toupper(search_item[f]))
                {
                    matches++;
                }
                if(search_item[f] == '*')
                {
                  break;
                }
                else f++;
            } 
            if(matches > 0 && matches == f-1) //checks if the # of matches is equal to the length of the string entered
            {  
                if(no_matches == 0)
                {
                    mkdir(search_Dir); // creates a folder
                }
                tot_matchentries++;
                match_status = 1;
                stop_status = 1;

                sprintf(search_File,"\\%d",no_matchesentries);  
                strcat(search_Dir,search_File);

                if(no_matches == 0)
                {
                    mkdir(search_Dir); // creates a folder
                }

                no_matches++;
                tot_matches++;
                
                strcpy(search_File,"");
                sprintf(search_File,"\\%d",no_matches);
                strcat(search_Dir,search_File);

                if(no_matches == 0)
                {
                    mkdir(search_Dir); // creates a folder
                }
                
                fptr = fopen(search_Dir,"w");
                fprintf(fptr,"%s",search_Folder);
                fclose(fptr);

            }
            if (match_status == 1)
            {
                results++;
            }
            if(j >= 7) stop_status = 1;
            f = 0;
 
            strcpy(folderDir,PATH);
            sprintf(folderName,"\\entries\\%d",i);
            strcat(folderDir,folderName);

            strcpy(search_Dir,PATH);
            strcat(search_Dir,"\\search");
            match_status = 0;
            matches = 0;
        }
        j = 0;
        stop_status = 0;
        
        if(no_matches >= 9)
        {
            no_matchesentries++;
            no_matches = 0;
        }
    }
        
        strcpy(buffer,"");//initiialises buffer variable
        strcpy(search_Dir,PATH);//intialises search_Dir variable with "C:\IA"
        strcat(search_Dir,"\\search"); //concatenates the search_Dir variable with search, so it becomes "C:\IA\search"
        strcpy(folderDir,"");
        strcpy(fileName,"");
        strcpy(search_Folder,"");

        i = 0; // initialises i
        j = 0; // initialises j
        a = 1; // initialises a 
        f = 14;
        stop_status = 0;
        hideCursor();
        system("cls"); // clears the screen
       
        ha: //prints out results  
          gotoFunc(78,10);//sets cursor position
          printf("Jamaica College Library Management Program: BOOK RETURN");
          gotoFunc(73,11);// sets cursor position
          printf("=================================================================");
          gotoFunc(84,13);
          if(results > 1) 
          while (no_matchesentries >= a)
              {
                  
                  strcpy(search_Dir,PATH);
                  strcat(search_Dir,"\\search");

                  sprintf(search_Folder,"\\%d",a);
                  strcat(search_Dir,search_Folder);

                  while(1) //prints from the folder
                  {
                      b++;

                      sprintf(search_File,"\\%d",b);
                      strcat(search_Dir,search_File);
                      
                      fileptr = fopen(search_Dir,"r");
                      if(fileptr == NULL) stop_status = 1;
                      else fscanf(fileptr,"%s",folderDir);
                      fclose(fileptr);

                      if(stop_status == 0)
                      { 
                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\1");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%s",student.ID);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\2");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%s",student.fullName);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\3");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%s",student.className);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\4");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%s",student.bookID);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\5");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%[^\n]s",student.bookTitle);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\6");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%s",student.bookAuthor);
                        fclose(filehandle);

                        strcpy(fileName,folderDir);
                        strcat(fileName,"\\7");

                        filehandle = fopen(fileName,"r");
                        fscanf(filehandle,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);
                        fclose(filehandle);

                        gotoFunc(84,f);
                        printf("%s\t\t%s\t\t[%d]",student.ID,student.fullName,b);
                    }
                      if(b == 9) stop_status =1;
                      f++;
                      i = 0;
                      strcpy(search_Dir,PATH);
                      strcat(search_Dir,"\\search");
                      sprintf(search_Folder,"\\%d",a);
                      strcat(search_Dir,search_Folder);
                      char ch;

                      if( stop_status == 1)
                      {   
                          //f = 14;
                          results:
                              gotoFunc(73,24);
                              printf("-----------------------------------------------------------------");
                              gotoFunc(73,26);// sets cursor positon
                             
                              if(a > 1)
                              {
                                gotoFunc(73,27);
                                printf("[ <- ]");
                              }
                              else
                              {
                                gotoFunc(73,27);
                                printf("......");
                              }
                              gotoFunc(84,27);
                              printf("press any number listed above to continue");
                              gotoFunc(90,25);
                              printf("%d results found\t [page: %d / %d] \n\n",results,a,no_matchesentries);
                              gotoFunc(73,26);
                              printf("=================================================================");
                              if(a < no_matchesentries)
                              {
                                gotoFunc(132,27);
                                printf("[ -> ]");
                              }
                              else
                              {
                                gotoFunc(132,27);
                                printf("......");
                              }
                              
                              //gotoFunc(113,15);
                              //printf("\n\n%d\t\t[%d]\t\t%d",b, stop_status,a);
                              
                              j = 0;
                              strcpy(option_File,"");
                              while(1) //accepts option
                              {
                                      
                                      if(kbhit())
                                      {
                                      dd = getch();
                                      if(isdigit(dd) != FALSE)
                                      {
                                          option_File[j]  = dd;
                                          //printf("%c",option_File[j]);
                                          j++;
                                      }
                                      if( j > 0)
                                      {   
                                          
                                          option_File[j] = '\0';
                                          j++;
                                          break;
                                      }
                                      if((int)dd == 77)
                                      {
                                          if(no_matchesentries > a) 
                                          {
                                              system("cls");
                                              if(b == 10 || b < 10 && stop_status == 1)
                                              { 
                                                  b = 0;
                                                  stop_status = 0;

                                              }
                                              a++;
                                              goto ha;
                                          }
                                      } 
                                      if((int)dd == 75 )
                                      {
                                          
                                          if(no_matchesentries >= a && a > 1) 
                                          {
                                              system("cls");
                                              if(b == 10 || b < 10 && stop_status == 1)
                                              { 
                                                  b = 0;
                                                  stop_status = 0;

                                              }
                                              a--;
                                              goto ha;
                                          }
                                      }
                                      }if((int)dd == 27 ||(int)dd == -32)
                                      {
                                          strcpy(search_Dir,PATH);
                                          strcat(search_Dir,"\\search");
                                          printf("\n\n\n\n");
                                          removeFile(search_Dir);  
                                          main();
                                      }
                              }
                              dd = '\0';

                              sscanf(option_File,"%d",&option);
                              if(option <= b && option > 0)
                              {  
                                  sprintf(option_File,"C:\\IA\\search\\%d\\%d",a,option);
                                  system("cls");

                                  fptr = fopen(option_File,"r");
                                  fscanf(fptr,"%s",search_Folder);
                                  fclose(fptr);

                                  strcpy(folderDir,search_Folder);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\1");
                                    
                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%s",student.ID);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\2");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%[^\n]s",student.fullName);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\3");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%s",student.className);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\4");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%s",student.bookID);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\5");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%[^\n]s",student.bookTitle);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\6");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%[^\n]s",student.bookAuthor);
                                  fclose(filehandle);

                                  strcpy(fileName,folderDir);
                                  strcat(fileName,"\\7");

                                  filehandle = fopen(fileName,"r");
                                  fscanf(filehandle,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);
                                  fclose(filehandle);
                                  
                                  this:
                                  gotoFunc(78,10);//sets cursor position
                                  printf("Jamaica College Library Management Program: BOOK RETURN");
                                  gotoFunc(73,11);// sets cursor position
                                  printf("=================================================================");
                                  gotoFunc(84,13);
                                  printf("student ID:\t\t\t%s",student.ID);
                                  gotoFunc(84,14);
                                  printf("student class:\t\t%s",student.className);
                                  gotoFunc(84,15);
                                  printf("Name:\t\t\t%s",student.fullName);
                                  gotoFunc(84,16);
                                  printf("Author:\t\t\t%s",student.bookAuthor);
                                  gotoFunc(84,17);
                                  printf("Book Title:\t\t\t%s",student.bookTitle);
                                  gotoFunc(73,19);
                                  printf("-----------------------------------------------------------------");
                                  gotoFunc(98,20);
                                  printf("return this book");
                                  gotoFunc(73,21);// sets cursor positon
                                  printf("=================================================================");
                                  gotoFunc(103,22);// sets cursor positon
                                  printf("continue?");
                                  gotoFunc(73,22);
                                  printf("[ Y. ]");
                                  gotoFunc(132,22);
                                  printf("[ N. ]");
                                      ch = getch();
                                      ch = toupper(ch);
                                      
                                      if(ch == 'Y' || ch == 'N')
                                      {
                                          if(ch == 'Y')
                                          {   system("cls");
                                              removeFile(folderDir);
                                              main();
                                          }
                                          if(ch == 'N')
                                          {
                                              system("cls");
                                              f = 14;
                                              b = 0;
                                              j = 0;
                                              dd = '\0';
                                              strcpy(option_File,"");
                                              stop_status = 0; 
                                              goto ha;
                                          }
                                      } else goto this;

                              } 
                              else 
                              {   
                                  system("cls");
                                  f = 14;
                                  b = 0;
                                  j = 0;
                                  dd = '\0';
                                  strcpy(option_File,"");
                                  stop_status = 0; 
                                  goto ha;
                              }
                    }
                      
              }
              
            }
          else if(results <= 0)
          {
            
            here:
                
                gotoFunc(96,15);
                printf("sorry, no results found!");
                gotoFunc(73,19);
                printf("-----------------------------------------------------------------");
                gotoFunc(73,21);// sets cursor positon
                printf("=================================================================");
                gotoFunc(73,22);
                printf("[ Y. ]");
                gotoFunc(102,22);
                printf("try again?");
                gotoFunc(103,20);
                printf(".....");
                gotoFunc(132,22);
                printf("[ N. ]");
                checker = getch();
                checker = toupper(checker);
                if(checker == 'Y'|| checker == 'N')
                {
                  if(checker == 'Y') returnBooks();
                  if(checker == 'N') main();
                }else goto here;
          }
          else if(results  == 1)
          {
              strcpy(option_File,"C:\\IA\\search\\1\\1");
              system("cls");
              
              fptr = fopen(option_File,"r");
              fscanf(fptr,"%s",search_Folder);
              fclose(fptr);

              strcpy(folderDir,search_Folder);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\1");
                
              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%s",student.ID);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\2");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%[^\n]s",student.fullName);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\3");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%s",student.className);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\4");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%s",student.bookID);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\5");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%[^\n]s",student.bookTitle);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\6");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%[^\n]s",student.bookAuthor);
              fclose(filehandle);

              strcpy(fileName,folderDir);
              strcat(fileName,"\\7");

              filehandle = fopen(fileName,"r");
              fscanf(filehandle,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);
              fclose(filehandle);
              
              going:
              gotoFunc(78,10);//sets cursor position
              printf("Jamaica College Library Management Program: BOOK RETURN");
              gotoFunc(73,11);// sets cursor position
              printf("=================================================================");
              gotoFunc(84,13);
              printf("student ID: %s",student.ID);
              gotoFunc(84,14);
              printf("student class: %s",student.className);
              gotoFunc(84,15);
              printf("Name: %s",student.fullName);
              gotoFunc(84,16);
              printf("Author: %s",student.bookAuthor);
              gotoFunc(84,17);
              printf("Book Title: %s",student.bookTitle);
              gotoFunc(73,19);
              printf("-----------------------------------------------------------------");
              gotoFunc(98,20);
              printf("return this book");
              gotoFunc(73,21);// sets cursor positon
              printf("=================================================================");
              gotoFunc(103,22);// sets cursor positon
              printf("continue?");
              gotoFunc(73,22);
              printf("[ Y. ]");
              gotoFunc(132,22);
              printf("[ N. ]");
                  ch = getch();
                  ch = toupper(ch);
                  
                  if(ch == 'Y' || ch == 'N')
                  {
                      if(ch == 'Y')
                      {   system("cls");
                          removeFile(folderDir);
                      }
                      if(ch == 'N')
                      {
                          returnBooks();
                      }
                  } else goto going;
          }
    
    bye_bye:
        strcpy(search_Dir,PATH);
        strcat(search_Dir,"\\search");
        removeFile(search_Dir);        

}//end of 'unreturnedBooks' function
void quit() //** This function prompts the user to confirm its request to leave the program
{
      char ch;
      system("cls");
      prompt:
          gotoFunc(84,10);
          printf("JAMAICA COLLEGE LIBRARY MANAGEMENT PROGRAM");
          gotoFunc(73,11);
          printf("=================================================================");
          gotoFunc(73,21);
          printf("=================================================================");
          gotoFunc(73,22);
          printf("[ Y. ]");
          gotoFunc(84,16);
          printf("are you sure you want to leave the program?");
          gotoFunc(100,22);
          printf("continue?");
          gotoFunc(132,22);
          printf("[ N. ]");
          ch = getch();
          ch = toupper(ch);
          if(ch == 'Y' || ch == 'N'){
            if(ch == 'Y'){
              removeFile("C:\\IA\\overdue");
              system("cls");
              exit(0);
            }
            if(ch == 'N')
            {
              main();
            }
          }
          else goto prompt;
      
      
}
void overdueCheck() //** This function checks if a book is overdue or not and records its path
{  
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  
  int check_status;
  int stop_status = 0;
  int no_days;
  float late_fee;
  int i = 0;
  FILE * fptr;
  FILE *filehandler;
  char fileDir[MAX_TEXT];
  char overdueDir[MAX_TEXT];
  char DATE[MAX_TEXT];
  struct date DUE;
  struct date CURRENT;
  no_overdue = 0;
  
  const int monthDays[12]
    = { 31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31 };

  CURRENT.day = tm.tm_mday; // stores the day that the book was borrowed
  CURRENT.month = tm.tm_mon+1; // stores the month that the book was borrowed
  CURRENT.year = tm.tm_year+1900; // stores the year that the book was borrowed

  int CURRENT__leap_years = 0;
  int DUE__leap_years = 0;

  long int CURRENT__noDays = 0;
  long int DUE__noDays = 0;

  

  //fptr = fopen("C:\\IA\\overdue\\init","r");
  //if(fptr == NULL)
  //{
    mkdir("C:\\IA\\overdue");
  //}
  //else fscanf(fptr,"%d",&no_overdue);
  //fclose(fptr);
    
  while(i <= no_entries)
  {
    i++;
    sprintf(fileDir,"C:\\IA\\entries\\%d\\7",i);

    fptr = fopen(fileDir,"r");
    if(fptr == NULL) stop_status = 1;
    else fscanf(fptr,"%s",DATE);
    fclose(fptr);

    sscanf(DATE,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);

    system("cls");
    //sprintf(fileDir,"C:\\IA\\entries\\%d\\8",i);
    /*
    filehandler = fopen(fileDir,"r");
    if(filehandler == NULL && stop_status == 0) check_status = 0;
    else check_status = 1;
    fclose(filehandler);
    */
       if (/*check_status == 0 &&*/ stop_status == 0 )
       {
          CURRENT__noDays = CURRENT.year * 365 + CURRENT.day;
          DUE__noDays = DUE.year * 365 + DUE.day;
          for (int a = 0; a < CURRENT.month; a++) CURRENT__noDays += monthDays[a];
          if (CURRENT.month <= 2) CURRENT.year--;
          CURRENT__leap_years = (CURRENT.year / 4) - (CURRENT.year / 100) + (CURRENT.year/400);
          CURRENT__noDays += CURRENT__leap_years;
          for (int a = 0; a < DUE.month; a++) DUE__noDays += monthDays[a];
          if (DUE.month <= 2) DUE.year--;
          DUE__leap_years = (DUE.year / 4) - (DUE.year / 100) + (DUE.year/400);
          DUE__noDays += DUE__leap_years;

          no_days = DUE__noDays - CURRENT__noDays;
          
        if(no_days < 0)
        { 
          no_overdue++;
          //check_status++;         

         sprintf(fileDir,"C:\\IA\\entries\\%d",i);
         sprintf(overdueDir,"C:\\IA\\overdue\\%d",no_overdue);
        
          if ( stop_status == 0 )
          {
            fptr = fopen(overdueDir,"w");
          fprintf(fptr,"%s\t\t%d",fileDir,abs(no_days));
          fclose(fptr);
            
          /*strcat(fileDir,"\\7");
          fptr = fopen(fileDir,"w");
          fprintf(fptr,"%d\\%d\\%d",DUE.day,DUE.month,DUE.year);
          fclose(fptr);
          */
          /*sprintf(fileDisr,"C:\\IA\\entries\\%d\\8",i);
          fptr = fopen(fileDir,"w");
          fprintf(fptr,"%d",check_status);
          fclose(fptr);
          */
          }
       }
      }
    DUE.day = 0;
    DUE.month = 0;
    DUE.year = 0;
    check_status = 0;
    stop_status = 0;
    overdue = no_overdue;
  }
  //removeFile("C:\\IA\\overdue");
}
int  removeFile(char remove_Dir[]) //** This function removes the directory that is passed to it
{
    char remove_cmd[MAX_TEXT];
    strcpy(remove_cmd,"rmdir /s /q \"");
    strcat(remove_cmd,remove_Dir);
    strcat(remove_cmd,"\"");  
    system(remove_cmd);
    return 0;
}
void viewOverdue() //** This function prints out all the data about overdue books, including the student information, the book information, no of days overdue and the late fee tendered
{   
    hideCursor();
    int i = 0;
    int b = 0;
    int a = 16;
    int stop_status = 0;
    char overdueDir[MAX_TEXT];
    struct date DUE;
    int no_days;
    float late_fee;
    FILE * fptr;
    FILE * filehandle;
    if(no_overdue > 0)
   {
    system("cls");
    gotoFunc(86,10);
    printf("JAMAICA COLLEGE LIBRARY MANAGEMENT PROGRAM: OVERDUE BOOK REPORT");
    gotoFunc(20,11);
    printf("==============================================================================================================================================================================================================");
    gotoFunc(21,12);
    printf("ID\t\t\tClass\t\t\tName\t\t\t\tAuthor\t\t\t\t\tTitle\t\t\t\t\t\tDays Overdue\t\t\tLate Fee");
    gotoFunc(20,13);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
        while(i < no_overdue)
        {
          i++;

          sprintf(overdueDir,"C:\\IA\\overdue\\%d",i);
          
          filehandle = fopen(overdueDir,"r");
          fscanf(filehandle,"%s\t\t%d",fileDir,&no_days);
          fclose(filehandle);

          late_fee = no_days * 50.00;

          strcpy(fileName,fileDir);
          strcat(fileName,"\\1");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%s",student.ID);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\2");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.fullName);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\3");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.className);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\4");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookID);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\5");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookTitle);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\6");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookAuthor);
          fclose(filehandle);

          strcpy(fileName,fileDir);
          strcat(fileName,"\\7");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);
          fclose(filehandle);

        gotoFunc(21,a);
        printf("%s",student.ID);
        gotoFunc(41,a);
        printf("%s",student.className);
        gotoFunc(64,a);
        printf("%s",student.fullName);
        gotoFunc(97,a);
        printf("%s",student.bookAuthor);
        gotoFunc(137,a);
        printf("%s",student.bookTitle);
        gotoFunc(188,a);
        printf("%d",no_days);
        gotoFunc(216,a);
        printf("%0.2f",late_fee);
        a++;
    }      
    printf("\n\n\t\t    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
    printf("\t\t    ==============================================================================================================================================================================================================\n");
    printf("\t\t    ..................................\t\t\t\t\t\t\t    press any key to continue    \t\t\t\t\t\t\t..................................");
    gotoFunc(20,24);
    getch();
    main();
    }
    else
    {
      system("cls");
      gotoFunc(76,10);//sets cursor position
      printf("Jamaica College Library Management Program: OVERDUE BOOKS");
      gotoFunc(73,11);// sets cursor position
      printf("=================================================================");
      gotoFunc(93,16);// sets cursor position
      printf("no overdue entries found.");
      gotoFunc(73,21);// sets cursor positon
      printf("=================================================================");
      gotoFunc(73,22);
      printf("......");
      gotoFunc(94,22);
      printf("press any key to continue");
      gotoFunc(132,22);
      printf("......");
      getch();
      main();
    }
}
void viewBooks() //** This function prints out all the data about the unreturned books, including the student information and the book information
{
   hideCursor();
     hideCursor();
    int i = 0;
    int b = 0;
    int a = 16;
    int stop_status = 0;
    char folderDir[MAX_TEXT];
    struct date DUE;
    FILE * fptr;
    FILE * filehandle;
    if(no_entries > 0)
   {
    system("cls");
    gotoFunc(86,10);
    printf("JAMAICA COLLEGE LIBRARY MANAGEMENT PROGRAM: UNRETURNED BOOK REPORT");
    gotoFunc(20,11);
    printf("==============================================================================================================================================================================================================");
    gotoFunc(21,12);
    printf("ID\t\t\tClass\t\t\tName\t\t\t\tAuthor\t\t\t\t\tTitle\t\t\t\t\t\t    Due Date [D/M/YYYY]");
    gotoFunc(20,13);
    printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
        while(i < no_entries)
        {
          strcpy(student.ID,"");
          strcpy(student.fullName,"");
          strcpy(student.className,"");
          strcpy(student.bookID,"");
          strcpy(student.bookTitle,"");
          strcpy(student.bookAuthor,"");
          DUE.day = 0;
          DUE.month = 0;
          DUE.year = 0;
          i++;

          sprintf(folderDir,"C:\\IA\\entries\\%d",i);
          
          strcpy(fileName,folderDir);
          strcat(fileName,"\\1");

          filehandle = fopen(fileName,"r");
          if(filehandle == NULL)stop_status  = 1;
          fscanf(filehandle,"%s",student.ID);
          fclose(filehandle);

          if (stop_status == 1 && i == no_entries && a == 16)
          {
            fptr = fopen("C:\\IA\\init","w");
            fprintf(fptr,"0");
            fclose(fptr);
            goto end;
          }

          strcpy(fileName,folderDir);
          strcat(fileName,"\\2");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.fullName);
          fclose(filehandle);

          strcpy(fileName,folderDir);
          strcat(fileName,"\\3");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.className);
          fclose(filehandle);

          strcpy(fileName,folderDir);
          strcat(fileName,"\\4");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookID);
          fclose(filehandle);

          strcpy(fileName,folderDir);
          strcat(fileName,"\\5");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookTitle);
          fclose(filehandle);

          strcpy(fileName,folderDir);
          strcat(fileName,"\\6");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%[^\n]s",student.bookAuthor);
          fclose(filehandle);

          strcpy(fileName,folderDir);
          strcat(fileName,"\\7");

          filehandle = fopen(fileName,"r");
          fscanf(filehandle,"%d\\%d\\%d",&DUE.day,&DUE.month,&DUE.year);
          fclose(filehandle);

        if (stop_status == 0)
        {
        gotoFunc(21,a);
        printf("%s",student.ID);
        gotoFunc(41,a);
        printf("%s",student.className);
        gotoFunc(64,a);
        printf("%s",student.fullName);
        gotoFunc(97,a);
        printf("%s",student.bookAuthor);
        gotoFunc(137,a);
        printf("%s",student.bookTitle);
        gotoFunc(188,a);
        printf("%d/%d/%d",DUE);
        gotoFunc(216,a);
        //printf("%0.2f",late_fee);
        a++;
        }
    }      
    printf("\n\n\t\t    --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
    printf("\t\t    ==============================================================================================================================================================================================================\n");
    printf("\t\t    ..................................\t\t\t\t\t\t\t    press any key to continue    \t\t\t\t\t\t\t..................................");
    gotoFunc(20,24);
    getch();
    main();
    }
    else
    {
      end:
        system("cls");
        gotoFunc(76,10);//sets cursor position
        printf("Jamaica College Library Management Program: OVERDUE BOOKS");
        gotoFunc(73,11);// sets cursor position
        printf("=================================================================");
        gotoFunc(93,16);// sets cursor position
        printf("no book entries found.");
        gotoFunc(73,21);// sets cursor positon
        printf("=================================================================");
        gotoFunc(73,22);
        printf("......");
        gotoFunc(94,22);
        printf("press any key to continue");
        gotoFunc(132,22);
        printf("......");
        getch();
        main();
    }
}
void login() //This function accepts a specific username and password, which is used to gain access to the features of the program
{
char username[MAX_TEXT];
char password[MAX_TEXT];
int i = 0;
int a = 0;
showCursor();
strcpy(username,"");
strcpy(password,"");

  system("cls");
  gotoFunc(84,10);
  printf("JAMAICA COLLEGE LIBRARY MANAGEMENT PROGRAM");
  gotoFunc(73,11);
  printf("=================================================================");
  gotoFunc(73,21);
  printf("=================================================================");
  gotoFunc(73,22);
  printf("[Esc.]");
  gotoFunc(104,22);
  printf("quit");
  gotoFunc(131,22);
  printf("[Bksp.]");
  gotoFunc(73,13);
  printf("-----------------------------------------------------------------");
  gotoFunc(103,20);
  printf(".....");
  gotoFunc(73,19);
  printf("-----------------------------------------------------------------");
  gotoFunc(100,12);
  printf("hello there!");
  gotoFunc(84,15);
  printf("enter username:");
  gotoFunc(113,15);
  i = 0;
   while(1)
    {
      if(kbhit())
      {        
        c = getch();
        if(isalnum(c) != FALSE || isspace(c) != FALSE)
        {
          username[i] = c;
          printf("%c",username[i]);
          i++;
        }
        if((int)c == 8 && i > 0)
        {
          username[i] = '\0';	
          printf("\b \b");
          i--;
        }
        if((int)c == 13 && i > 0)
        {
         username[i] = '\0';	
          break;
        }
        if((int)c == 27 ||(int)c == -32)
        {
            quit();
        }
      }//end of if statement
    }//end of while loop
  gotoFunc(84,16);
  printf("enter password:");
  gotoFunc(113,16);
  a = 0;
   while(1)
    {
      if(kbhit())
      {        
        c = getch();
        if(isalnum(c) != FALSE || isspace(c) != FALSE)
        {
          password[a] = c;
          printf("*");
          a++;
        }
        if((int)c == 8 && a > 0)
        {
          password[a] = '\0';	
          printf("\b \b");
          i--;
        }
        if((int)c == 13 && a > 0)
        {
          password[a] = '\0';	
          break;
        }
        if((int)c == 27 ||(int)c == -32)
        {
            quit();
        }
      }//end of if statement
    }//end of while loop
  hideCursor();
  
  if( strcmp(username,"admin\0") != -1 && strcmp(password,"JCPassword\0") != -1 )
  {
    login_status = 1;
  } 
  else
  {
    //printf("%d\t\t%d",strcmp(username,"admin\0"),strcmp(password,"JCPassword\0"));
    login_status = 0;
    gotoFunc(73,22);
    printf(".......");
    gotoFunc(94,22);
    printf("press any key to continue");
    gotoFunc(131,22);
    printf(".......");
    gotoFunc(90,20); 
    printf("incorrect password or username");
    gotoFunc(113,15);
    getch();
    login();
    
  }
}