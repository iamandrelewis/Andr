/*
THINGS TO DO 

	CREATE FOLLOW UP PROTOCOLS FOR:
	=> update, view, & delete

	CREATE VALIDATION PROCESS FOR DATA ENTRY SECTIONS [CREATE APPOINTMENT, PATIENT REGISTRATION]
	FIX BUGS IN REGISTRATION INFO
	FIX BUGS IN PATIENTIDGENERATOR
	FIX BUGS IN APPOINTMENT CREATION
*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <unistd.h>

struct date //stores the date info in the format where, day as dd (integer), year yyyy (integer), month mmm (string)
{ 
    unsigned int dd, yyyy;
	char mmm[3];
};
struct RegistrationInfo //stores the patient info in the format where, ...
{
    char patient_id[128];//<--- This is gonna be 2 random no. 
	char fName[64];
    char lName[64];
    unsigned int age;// determins the health insurance coverage
    char gender;
    char email[30]; 
    char telephone[15];
	struct date dob; //calling the date structure 
};
struct appointment // stores the appointment info in the format where, ...
{
	char patient_id[9];
    char dow[9];// day of the week (only Mon-Fri)
    char timeofday[5];// time will be saved as a string "8:00-7:00" only
    char tov[15];// type of visit
    struct date doa; // date of appointment
};

// GLOBAL VARIABLES
int matchcounter = 0;// store the number of matches found about a particular patientID


// FUNCTION PROTOTYPES
struct appointment *__cdecl Search(char * patientID); // searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found
char *__cdecl patientIdGenerator(struct RegistrationInfo * patient);// accepts data on the patient and returns an ID corresponding to their name, age and year of birth
struct RegistrationInfo Registration();// accepts data on the patient and returns a struct with the corresponding information
struct appointment Create_app();// accepts data on the appointment details and returns a struct with the corresponding information
struct RegistrationInfo __cdecl searchPatient(char * patientID); // searches the file, 'patient.dat' for matching patientID and returns the struct with the patient that matches
int searchAppointment(struct appointment * details); // searchs the file, 'appointment.txt' for matching time and date of the struct given and returns the number of matches found

void Update();// updates the data of a selected appointment
void Delete(); // deletes the data of a selected appointment
void View(); // views the data of a selected appointment
void Exit(); // Exits the program

// FUNCTION DECLARATIONS
void menu()
{
	
	FILE * fptr;
	struct RegistrationInfo data;
	struct appointment info;
	char opt;

	getch();
	system("cls");// clears the screen
    //display screen
    printf("\t   Welcome to Dr. Mitchell's Medical Center\n");
    printf("\t\t Opening Days: Monday - Friday\n\n\n");
    printf("\t\t\t    MENU\n\n");
    printf("\t\tPlease select an option below\n");
    printf("------------------------------------------------------------\n");
    printf("\t\t    1  - Register patient info \n");
    printf("\t\t    2  - Create an appointment\n");
    printf("\t\t    3  - View appointment\n");//same as searching for the appointment
    printf("\t\t    4  - Update appointment\n");
    printf("\t\t    5  - Delete appointment\n");
    printf("\t\t    6  - Exit menu\n");
	printf("------------------------------------------------------------\n");
	printf("\tThese are the charges for the type of visit\n");
    printf("\tConsultation:\t\t$4500\n\tPrimary Care:\t\t$8000\n\tFollow-Up Visit:\t$9500\n\tUrgent Visit:\t\t$15500\n");
	printf("============================================================\n");
    printf("Enter number for the option chosen:\t\t\t");
	opt = getch();
    printf("\n------------------------------------------------------------\n");
	switch(opt)
	    {
		   	case '1':
		   	 	data = Registration();
				fptr = fopen("patient.dat","ab+");
				fwrite(&data,1,sizeof(data),fptr);
				fclose(fptr);
				menu();
				break; 		 
					
			case '2':
			 	info = Create_app();
				fptr = fopen("appointment.txt","a");
				fprintf(fptr,"%s %s %s %s %d %d %s\n", info);
				fclose(fptr);
				menu();
				break;
			case '3':
				View();
		   		 break;
		   	case '4':
		   		Update();
		   		break;
		   	case '5':
		   		Delete();
		   		 break;
		   	case '6'://exit option
				Exit();
		   		break;
		   		
		   	default:
				printf("no option.. please retype");
				getch();
				menu();
				break;

	    }
}
int main()
{
	menu();
   
	return 0;
}
int searchAppointment(struct appointment * details)
{
	int results = 0;
	struct appointment s;
	FILE * fptr;

	fptr = fopen("appointment.txt","r");
		while(fptr != NULL){
			if(feof(fptr)) break;
			fscanf(fptr,"%s %s %s %s %d %d %s", s.patient_id, s.dow, s.timeofday, s.tov, &s.doa.dd, &s.doa.yyyy, s.doa.mmm);
			if(strcmp(details->timeofday,s.timeofday) == 0 && strcmp(details->dow,s.dow) == 0 && s.doa.dd == details->doa.dd && s.doa.yyyy == details->doa.yyyy && strcmp(details->doa.mmm,s.doa.mmm) == 0 ) results++;
		}
	fclose(fptr);
	return results;
}
char *__cdecl patientIdGenerator(struct RegistrationInfo * patient)//
{

	static char ID[128];
	char ageBuffer[3];
	char yBuffer[5];
	char genderBuffer[3];
	char firstBuffer[3];
	char lastBuffer[3];

	sprintf(genderBuffer,"%c",patient->gender);
	sprintf(firstBuffer,"%c",toupper(patient->fName[0]));
	sprintf(lastBuffer,"%c",toupper(patient->lName[0]));
	sprintf(ageBuffer,"%d",patient->age);
	sprintf(yBuffer,"%d",patient->dob.yyyy);
	
	
	strcpy(ID,"");

	strcat(ID,firstBuffer);
	strcat(ID,lastBuffer);
	strcat(ID,ageBuffer);
	strcat(ID,genderBuffer);
	strcat(ID,yBuffer);

	return ID;
}
struct RegistrationInfo __cdecl searchPatient(char * patientID) //	
{
		FILE * fptr;
		struct RegistrationInfo patient;
		matchcounter = 0;

		fptr = fopen("patient.dat","rb"); // opens the file, 'patient.dat' in read-binary mode
			while(fptr != NULL) 
			{ // checks to see if the file has data stored
				if(feof(fptr)) break;
				fread(&patient,sizeof(patient),1,fptr); // reads the data within the file, and stores it into a struct variable called patient
				if (strcmp(patient.patient_id,patientID) == 0){ // compares the patient ID entered to an ID stored on file
					matchcounter++; // increments the 'matchcounter' variable
					break;// exits the loop
				}// end of if statement
			}// end of while loop
		fclose(fptr);// closes the file, 'patient.dat'

		return patient;
}
struct RegistrationInfo Registration() // 
{
	system("cls");	
	int size, i=0;
	struct RegistrationInfo patient;
	char opt;

		system("cls");//clears the screen
		printf("\tDr. Mitchell's Medical Center Appointment Program\n");
		printf("--------------------------------------------------------------\n");
		printf("==============       Patient Registration       ==============\n\n");
			
			printf("Enter Patient First Name: ");
			scanf("%s",patient.fName);
			printf("Enter Patient Last Name: ");
			scanf("%s",patient.lName);
			printf("Enter Patient Age: ");
			scanf("%u",&patient.age);
			printf("Enter Patient Gender: ");
			patient.gender = toupper(getch());
			printf("%c\n",patient.gender);
			printf("Enter Patient Email Address: ");
			scanf("%s",patient.email);
			printf("Enter Patient Phone Number: ");
			scanf("%s",patient.telephone);
			printf("Enter Date of Birth (dd-mmm-yyyy): ");
			scanf("%d%s%d",&patient.dob.dd,patient.dob.mmm,&patient.dob.yyyy);
			
			strcpy(patient.patient_id, patientIdGenerator(&patient));

			registrationPrompt:
				system("cls");
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("------------------------------------------------------------------\n\n");
				printf("================       Patient Registration       ================\n\n");
				printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
				printf("Patient First Name:\t\t\t%s\n",patient.fName);
				printf("Patient Last Name:\t\t\t%s\n",patient.lName);
				printf("Patient Age:\t\t\t\t%u\n",patient.age);
				printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
				printf("Patient Email Address:\t\t\t%s\n",patient.email);
				printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);
				printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
				printf("\n------------------------------------------------------------------\n\n");
				printf("\t\t\t[1] -\tEdit\n\t\t\t[2] -\tSave\n\t\t\t[3] -\tCancel\n");
				printf("\n===================================================================\n");
				printf("Please select an option:\t\t");
				opt = getch();
				switch (opt)
				{
				case '1': // 
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("================       Patient Registration       ================\n\n");
					printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
					printf("Patient First Name:\t\t\t%s\t\t\t[1]\n",patient.fName);
					printf("Patient Last Name:\t\t\t%s\t\t\t[2]\n",patient.lName);
					printf("Patient Age:\t\t\t\t%u\t\t\t[3]\n",patient.age);
					printf("Patient Gender:\t\t\t\t%c\t\t\t[4]\n", patient.gender);
					printf("Patient Email Address:\t\t\t%s\t\t\t[5]\n",patient.email);
					printf("Patient Phone Number:\t\t\t+1 (876) %s\t[6]\n",patient.telephone);
					printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\t\t[7]\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
					printf("\n------------------------------------------------------------------\n");
					printf("\t\t\t\t....");
					printf("\n===================================================================\n");
					printf("Please select an option:\t\t");
					getch();
					/* 'edit the entry' code goes here */
					break;
				case '2':
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("================       Patient Registration       ================\n\n");
					printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
					printf("Patient First Name:\t\t\t%s\n",patient.fName);
					printf("Patient Last Name:\t\t\t%s\n",patient.lName);
					printf("Patient Age:\t\t\t\t%u\n",patient.age);
					printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
					printf("Patient Email Address:\t\t\t%s\n",patient.email);
					printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);
					printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
					printf("\n-------------------------------------------------------------------\n");
					printf("\t\t     record saved successfully\n");
					printf("====================================================================\n");
					printf("\t\t   ...press any key to continue...\n");
					getch();
					break;
				case '3':
					menu();
					break;
				default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto registrationPrompt;
					break;
				}

	return patient;

}
struct appointment Create_app() //
{
		static char patientID[9];
		struct appointment details;
		struct appointment * results;
		struct RegistrationInfo patient;
		char opt;
		char opt2;
		char opt3;

		system("cls");//clears the screen
		printf("\tDr. Mitchell's Medical Center Appointment Program\n");
		printf("--------------------------------------------------------------\n");
		printf("==============     Appointment Registration     ==============\n\n");
		printf("Enter the patient ID:  ");
		scanf("%s", patientID);
		
		patient = searchPatient(patientID);

		if(matchcounter < 1){ //
			printf("\n\n\nno results found\n");
			printf("--------------------------------------------------------------\n");
			printf("...press any key to continue...\n");
			getch();
			menu();
		}
		else {
				appointmentPrompt:
				system("cls");
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("------------------------------------------------------------------\n\n");
				printf("================        Patient Information       ================\n\n");
				printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
				printf("Patient First Name:\t\t\t%s\n",patient.fName);
				printf("Patient Last Name:\t\t\t%s\n",patient.lName);
				printf("Patient Age:\t\t\t\t%u\n",patient.age);
				printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
				printf("Patient Email Address:\t\t\t%s\n",patient.email);
				printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);
				printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
				printf("\n------------------------------------------------------------------\n\n");
				printf("\t\t\t[1] -\tTry Again\n\t\t\t[2] -\tContinue\n\t\t\t[3] -\tCancel\n");
				printf("\n===================================================================\n");
				printf("Please select an option:\t\t");
				opt = getch();
				switch (opt)
				{
				case '1': // 
					Create_app();
					break;
				case '2':

				system("cls");//clears the screen
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("--------------------------------------------------------------\n");
				printf("==============     Appointment Registration     ==============\n\n");
				printf("Enter the day for an appointment :");
				scanf("%s",details.dow);
				printf("What time of day (XX: 00 [24 hr]): ");
				scanf("%s",details.timeofday);
				printf("Enter type of visit:\n [1]\tConsultation\n[2]\tPrimary Care\n[3]\tFollow-Up Visit\n[4]\tUrgent Visit\n\n");
				opt3 = getch();
				switch (opt3)
				{
				case '1':
					/* code */
					break;
				case '2':
					/* code */
					break;
				case '3':
					/* code */
					break;
				case '4':
					/* code */
					break;
				default:
					break;
				}
				printf("Enter the appointment date (dd-mmm-yyyy): ");
				scanf("%d%s%d",&details.doa.dd,details.doa.mmm,&details.doa.yyyy);

				if (searchAppointment(&details) < 1)
				{
					appointmentRegPrompt:
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("================     Appointment Registration     ================\n\n");
					printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
					printf("Day of the Week\t\t\t%s\n",details.dow);
					printf("Time Period (XX:00 [24hr]):\t\t\t%s\n",details.timeofday);
					printf("Type of Visit\t\t\t\t%s\n",details.tov);
					printf("Appointment Date:\t\t\t\t%d-%s-%d\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
					printf("\n------------------------------------------------------------------\n\n");
					printf("\t\t\t[1] -\tEdit\n\t\t\t[2] -\tSave\n\t\t\t[3] -\tCancel\n");
					printf("\n===================================================================\n");
					printf("Please select an option:\t\t");
					opt = getch();
					switch (opt)
					{
					case '1': // 
						editAppointment:
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("================     Appointment Registration     ================\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
						printf("Time Period (XX:00 [24hr]):\t\t\t%s\t\t[2]\n",details.timeofday);
						printf("Type of Visit\t\t\t\t%s\t\t[3]\n",details.tov);
						printf("Appointment Date:\t\t\t\t%d-%s-%d\t\t[4]\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
						printf("\n------------------------------------------------------------------\n");
						printf("\t\t....press esc to continue.....");
						printf("\n===================================================================\n");
						printf("Please select an option:\t\t");
						opt2 = getch();

						switch (opt2)
						{
						case '2':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
							printf("Time Period (XX:00 [24hr]): ");
							scanf("%s",details.timeofday);
							printf("Type of Visit\t\t\t\t%s\t\t[3]\n",details.tov);
							printf("Appointment Date:\t\t\t\t%d-%s-%d\t\t[4]\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
							printf("\n------------------------------------------------------------------\n");
							printf("\t\t\t\t....");
							printf("\n===================================================================\n");
							goto editAppointment;
							break;
					
						case '1':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Day of the Week: ");
							scanf("%s",details.dow);
							printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
							printf("Type of Visit\t\t\t\t%s\t\t[3]\n",details.tov);
							printf("Appointment Date:\t\t\t\t%d-%s-%d\t\t[4]\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
							printf("\n------------------------------------------------------------------\n");
							printf("\t\t\t\t....");
							printf("\n===================================================================\n");
							goto editAppointment;
							break;
						case '3':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
							printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
							printf("Type of Visit: ");
							scanf("%s",details.tov);
							printf("Appointment Date:\t\t\t\t%d-%s-%d\t\t[4]\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
							printf("\n------------------------------------------------------------------\n");
							printf("\t\t\t\t....");
							printf("\n===================================================================\n");
							goto editAppointment;
							break;
						case '\e':
							goto appointmentRegPrompt;
							break;
						default:
							printf("\n------------------------------------------------------------\n");
							printf("no option.. please retype");
							getch();
							goto editAppointment;
							break;
						}

						break;
					case '2':
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("================     Appointment Registration     ================\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Day of the Week\t\t\t%s\n",details.dow);
						printf("Time Period:\t\t\t%s\n",details.timeofday);
						printf("Type of Visit\t\t\t\t%s\n",details.tov);
						printf("Appointment Date:\t\t\t\t%d-%s-%d\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
						printf("\n-------------------------------------------------------------------\n");
						printf("\t\t     record saved successfully\n");
						printf("====================================================================\n");
						printf("\t\t   ...press any key to continue...\n");
						getch();
						break;
					case '3':
						menu();
						break;
					default:
							printf("\n------------------------------------------------------------\n");
							printf("no option.. please retype");
							getch();
							goto appointmentRegPrompt;
						break;
					}
				}
				else {
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("================     Appointment Registration     ================\n\n");
					printf("\n-------------------------------------------------------------------\n");
					printf("\t\t     appointment slot already taken\n");
					printf("====================================================================\n");
					printf("\t\t   ...press any key to continue...\n");
					getch();
					menu();
				}
					break;
				case '3':
					menu();
					break;
				default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto appointmentPrompt;
					break;
				}//end of switch-case statement
			}

	return details;
}
struct appointment *__cdecl Search(char * patientID) //
{
	
	static struct appointment searchResults[100];
	struct appointment s;
	static fpos_t position;
	static int i = 0;
	matchcounter = 0;

	FILE * fptr = fopen("appointment.txt","r");

		while(fptr != NULL)
		{
			if(feof(fptr)) break;

			if (position != 0) fsetpos(fptr,&position);

			fscanf(fptr,"%s %s %s %s %d %d %s", s.patient_id, s.dow, s.timeofday, s.tov, &s.doa.dd, &s.doa.yyyy, s.doa.mmm);
			if (strcmp(patientID,s.patient_id) == 0 ) matchcounter++;

			if(i <= 100)
			{
				if (strcmp(patientID,s.patient_id) == 0 ) 
				{
					strcpy(searchResults[i].patient_id,s.patient_id);
					strcpy(searchResults[i].doa.mmm,s.doa.mmm);
					strcpy(searchResults[i].dow,s.dow);
					strcpy(searchResults[i].tov,s.tov);
					searchResults[i].doa.dd = s.doa.dd;
					searchResults[i].doa.yyyy = s.doa.yyyy;
					i++;
				}
			}
			else 
			{
				if ( i >= 100 )fgetpos(fptr,&position);
				i = 0;
			}

		}
		
	fclose(fptr);

	return searchResults;
}

void Update() //
{
		struct appointment (*ptr)[100];
		struct RegistrationInfo patient;
		char patientID[9];
		int i = 1;
		char opt;
		char opt2;
	
		/* prompt to search for patient info*/
	
		system("cls");//clears the screen
		printf("\tDr. Mitchell's Medical Center Appointment Program\n");
		printf("--------------------------------------------------------------\n");
		printf("==============        Edit Appointment(s)       ==============\n\n");
		printf("Enter the patient ID:  ");
		scanf("%s", patientID);
		
		patient = searchPatient(patientID);

		if(matchcounter < 1){ //
			printf("\n\n\nno results found\n");
			printf("--------------------------------------------------------------\n");
			printf("...press any key to continue...\n");
			getch();
			menu();
		}
		else {
				updatePrompt:
				system("cls");
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("------------------------------------------------------------------\n\n");
				printf("==============          Patient Information         ==============\n\n");
				printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
				printf("Patient First Name:\t\t\t%s\n",patient.fName);
				printf("Patient Last Name:\t\t\t%s\n",patient.lName);
				printf("Patient Age:\t\t\t\t%u\n",patient.age);
				printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
				printf("Patient Email Address:\t\t\t%s\n",patient.email);
				printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);
				printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
				printf("\n------------------------------------------------------------------\n\n");
				printf("\t\t\t[1] -\tTry Again\n\t\t\t[2] -\tContinue\n\t\t\t[3] -\tCancel\n");
				printf("\n===================================================================\n");
				printf("Please select an option:\t\t");
				opt = getch();
				switch (opt)
				{
				case '1': // 
					Update();
					break;
				case '2':

					ptr = Search(patientID);
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============          Edit Appointment(s)         ==============\n\n");

					if(matchcounter < 1){ //
						printf("\n\n\nno appointments found\n");
						printf("--------------------------------------------------------------\n");
						printf("...press any key to continue...\n");
						getch();
						menu();
					}
					else
					{
						/*Update appointment procedure code goes here*/
						while(i <= matchcounter)
						{
							system("cls");// clears the screen
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("==============          Edit Appointment(s)         ==============\n\n\n");
							printf("%s\t%s\t%d-%s-%d.............[%d]\n",(*ptr + i)->dow,(*ptr + i)->timeofday,(*ptr+i)->doa.dd,(*ptr+i)->doa.mmm,(*ptr+i)->doa.yyyy,i);
							if(i >= 9 || i == matchcounter)
							{
								opt2 = getch();
								switch (opt2)
								{
									case '0':
										/* code */
										break;
									case '1':
										/* code */
										break;
									case '2':
										/* code */
										break;
									case '3':
										/* code */
										break;
									case '4':
										/* code */
										break;
									case '5':
										/* code */
										break;
									case '6':
										/* code */
										break;
									case '7':
										/* code */
										break;
									case '8':
										/* code */
										break;
									case '9':
										/* code */
										break;
								default:
									break;
								}
							}
						}
					}

					break;
				case '3':
					menu();
					break;
				default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto updatePrompt;
					break;
				}
		}

}
void View()   //
{
		struct appointment *ptr;
		struct RegistrationInfo patient;
		char patientID[9];
		int i = 1;
		char opt;
	
		/* prompt to search for patient info*/
	
		system("cls");//clears the screen
		printf("\tDr. Mitchell's Medical Center Appointment Program\n");
		printf("--------------------------------------------------------------\n");
		printf("==============       View Appointment(s)        ==============\n\n");
		printf("Enter the patient ID:  ");
		scanf("%s", patientID);
		
		patient = searchPatient(patientID); // Searches the file, 'patient.dat' for a struct with a value matching the variable, 'patientID' and stores the data into the struct, 'patient'

		if(matchcounter < 1){ //
			printf("\n\n\nno results found\n");
			printf("--------------------------------------------------------------\n");
			printf("...press any key to continue...\n");
			getch();
			menu(); // returns to the main menu
		}
		else {
				viewPrompt:
				system("cls");
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("------------------------------------------------------------------\n\n");
				printf("==============          Patient Verification        ==============\n\n");
				printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
				printf("Patient First Name:\t\t\t%s\n",patient.fName);
				printf("Patient Last Name:\t\t\t%s\n",patient.lName);
				printf("Patient Age:\t\t\t\t%u\n",patient.age);
				printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
				printf("Patient Email Address:\t\t\t%s\n",patient.email);
				printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);
				printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);
				printf("\n------------------------------------------------------------------\n\n");
				printf("\t\t\t[1] -\tTry Again\n\t\t\t[2] -\tContinue\n\t\t\t[3] -\tCancel\n");
				printf("\n===================================================================\n");
				printf("Please select an option:\t\t");
				opt = getch();
				switch (opt)
				{
				case '1': // 
					View();
					break;
				case '2':
					ptr = Search(patientID); // searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found, and stores the address into the pointer, 'ptr' 
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============         View Appointment(s)          ==============\n\n");

					if(matchcounter < 1){ //
						printf("\n\n\nno appointments found\n");
						printf("--------------------------------------------------------------\n");
						printf("...press any key to continue...\n");
						getch();
						menu();// returns to the main menu
					}
					else
					{
						/*View appointment procedure code goes here*/
					}
					break;
				case '3':
					menu();// returns the main menu
					break;
				default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto viewPrompt;
					break;
				}
		}
}
void Delete() //
{
		struct appointment (*ptr)[100];
		struct RegistrationInfo patient;
		char patientID[9];
		int i = 0;
		char opt;
	
		system("cls");//clears the screen
		printf("\tDr. Mitchell's Medical Center Appointment Program\n");
		printf("--------------------------------------------------------------\n");
		printf("==============     Delete Appointment(s)     ==============\n\n");
		printf("Enter the patient ID:  ");
		scanf("%s", patientID);// Accepts the patient ID and stores into the variable 'patientID'
		
		patient = searchPatient(patientID); // Searches the file, 'patient.dat' for a struct with a value matching the variable, 'patientID' and stores the data into the struct, 'patient'

		if(matchcounter < 1){ //Checks the number of matches found in the file
			printf("\n\n\nno results found\n");
			printf("--------------------------------------------------------------\n");
			printf("...press any key to continue...\n");
			getch();
			menu();// returns to the main menu
		}// end of 'if' statement
		else {
				deletePrompt:
				system("cls");//clears the screen
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("------------------------------------------------------------------\n\n");
				printf("==============          Patient Verification        ==============\n\n");
				printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);// Prints the patient ID
				printf("Patient First Name:\t\t\t%s\n",patient.fName); // Prints the patient First Name
				printf("Patient Last Name:\t\t\t%s\n",patient.lName);// Prints the patient Last Name
				printf("Patient Age:\t\t\t\t%u\n",patient.age);// Prints the patient Age
				printf("Patient Gender:\t\t\t\t%c\n", patient.gender);// Prints the patient Gender
				printf("Patient Email Address:\t\t\t%s\n",patient.email);// Prints the patient email
				printf("Patient Phone Number:\t\t\t+1 (876) %s\n",patient.telephone);// Prints the patient Phone Number
				printf("Patient Date of Birth (dd-mmm-yyyy):\t%d-%s-%d\n",patient.dob.dd,patient.dob.mmm,patient.dob.yyyy);// Prints the patient Date of Birth
				printf("\n------------------------------------------------------------------\n\n");
				printf("\t\t\t[1] -\tTry Again\n\t\t\t[2] -\tContinue\n\t\t\t[3] -\tCancel\n");//Prints the options panel
				printf("\n===================================================================\n");
				printf("Please select an option:\t\t");
				opt = getch();// Accepts the desired option and stores it in the variable, 'opt'
				switch (opt)
				{
				case '1': // 
					Delete();// re-runs the function to return to the first instance
					break;
				case '2':
					ptr = Search(patientID);// searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found, and stores the address into the pointer, 'ptr' 
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============         Delete Appointment(s)        ==============\n\n");

					if(matchcounter < 1){ //Checks the number of matches found in the file
						printf("\n\n\nno appointments found\n");
						printf("------------------------------------------------------------------\n");
						printf("...press any key to continue...\n");
						getch();
						menu(); //returns to the main menu
					}
					else
					{
						/*Delete appointment procedure code goes here*/
						system("cls");
						while(i < matchcounter)
						{
							
							printf("");
							i++;
							if(i == 9) getch();
						}

					}
					break;
				case '3':
					menu();
					break;
				default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto deletePrompt;
					break;
				}
		}
}
void Exit()   //
{
 	system("cls");
	printf("BYE!!");
	sleep(1); // delays the code for 10 seconds
	exit(0);
}