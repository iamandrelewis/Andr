/*
THINGS TO DO 

	CREATE VALIDATION PROCESS FOR DATA ENTRY SECTIONS [CREATE APPOINTMENT, PATIENT REGISTRATION]
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
	char patient_id[128];
    char dow[10];// day of the week (only Mon-Fri)
    char timeofday[6];// time will be saved as a string "8:00-7:00" only
    char tov[25];// type of visit
    struct date doa; // date of appointment
	double cost;
	char healthInsuranceStatus;
};

// GLOBAL VARIABLES
int matchcounter = 0;// store the number of matches found about a particular patientID
char monthsOftheYear[12][4] = {
	{'j','a','n','\0'},
	{'f','e','b','\0'},
	{'m','a','r','\0'},
	{'a','p','r','\0'},
	{'m','a','y','\0'},
	{'j','u','n','\0'},
	{'j','u','l','\0'},
	{'a','u','g','\0'},
	{'s','e','p','\0'},
	{'o','c','t','\0'},
	{'n','o','v','\0'},
	{'d','e','c','\0'}
};



// FUNCTION PROTOTYPES
struct appointment __cdecl Search(char * patientID); // searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found
char *__cdecl patientIdGenerator(struct RegistrationInfo * patient);// accepts data on the patient and returns an ID corresponding to their name, age and year of birth
struct RegistrationInfo Registration();// accepts data on the patient and returns a struct with the corresponding information
struct appointment Create_app();// accepts data on the appointment details and returns a struct with the corresponding information
struct RegistrationInfo __cdecl searchPatient(char * patientID); // searches the file, 'patient.dat' for matching patientID and returns the struct with the patient that matches
int searchAppointment(struct appointment * details); // searchs the file, 'appointment.txt' for matching time and date of the struct given and returns the number of matches found
double calcAppointmentCost(struct appointment * details, struct RegistrationInfo * info);//

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

	//getch();
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
	printf("\tThese are the charges for the types of visits\n");
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
					fprintf(fptr,"%s %s %s %s %d %d %s %lf %c\n", info.patient_id,info.dow,info.timeofday,info.tov,info.doa.dd,info.doa.yyyy,info.doa.mmm,info.cost,info.healthInsuranceStatus);
				fclose(fptr);
				menu();
				break;
			case '3':
				View();
				menu();
		   		 break;
		   	case '4':
		   		Update();
				menu();
		   		break;
		   	case '5':
		   		Delete();
				menu();
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
	struct RegistrationInfo patient;
	char opt;
	char opt2;

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
			printf("Enter Patient Gender [M/F]: ");
			registrationGenderPrompt1:
			patient.gender = toupper(getch());
			if (patient.gender == 'M'|| patient.gender == 'F') printf("%c\n",patient.gender);
			else goto registrationGenderPrompt1;
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
					registrationEditPrompt:
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
					printf("\t\t....press esc to continue.....");
					printf("\n===================================================================\n");
					printf("Please select an option:\t\t");
					opt2 = getch();
					switch (opt2)
					{
					case '1':
						system("cls");//clears the screen
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("--------------------------------------------------------------\n");
						printf("==============       Patient Registration       ==============\n\n");
						printf("Enter Patient First Name: ");
						scanf("%s",patient.fName);
						strcpy(patient.patient_id, patientIdGenerator(&patient));
						goto registrationEditPrompt;

						break;
					case '2':
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("================       Patient Registration       ================\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Patient First Name:\t\t\t%s\n",patient.fName);
						printf("Enter Patient Last Name: ");
						scanf("%s",patient.lName);
						strcpy(patient.patient_id, patientIdGenerator(&patient));
						goto registrationEditPrompt;

						break;
					case '3':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================       Patient Registration       ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Patient First Name:\t\t\t%s\n",patient.fName);
							printf("Patient Last Name:\t\t\t%s\n",patient.lName);
							printf("Enter Patient Age: ");
							scanf("%u",&patient.age);
							strcpy(patient.patient_id, patientIdGenerator(&patient));
							goto registrationEditPrompt;
						break;
					case '4':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================       Patient Registration       ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Patient First Name:\t\t\t%s\n",patient.fName);
							printf("Patient Last Name:\t\t\t%s\n",patient.lName);
							printf("Patient Age:\t\t\t\t%u\n",patient.age);
							printf("Enter Patient Gender [M/F]: ");
							registrationGenderPrompt2:
							patient.gender = toupper(getch());
							if (patient.gender == 'M'|| patient.gender == 'F') printf("%c\n",patient.gender);
							else goto registrationGenderPrompt2;
							strcpy(patient.patient_id, patientIdGenerator(&patient));
							goto registrationEditPrompt;
						break;
					case '5':
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("================       Patient Registration       ================\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Patient First Name:\t\t\t%s\n",patient.fName);
						printf("Patient Last Name:\t\t\t%s\n",patient.lName);
						printf("Patient Age:\t\t\t\t%u\n",patient.age);
						printf("Patient Gender:\t\t\t\t%c\n", patient.gender);
						printf("Enter Patient Email Address: ");
						scanf("%s",patient.email);
						goto registrationEditPrompt;
						break;
					case '6':
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
						printf("Enter Patient Phone Number: ");
						scanf("%s",patient.telephone);
						goto registrationEditPrompt;
						break;
					case '7':
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
						printf("Enter Date of Birth (dd-mmm-yyyy): ");
						scanf("%d%s%d",&patient.dob.dd,patient.dob.mmm,&patient.dob.yyyy);
						strcpy(patient.patient_id, patientIdGenerator(&patient));
						goto registrationEditPrompt;
						break;
					case '\e':
						goto registrationPrompt;
						break;
					default:
						goto registrationEditPrompt;
						break;
					}
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

		time_t today;
		struct tm * now;
		time(&today);
		now = localtime(&today);

		static char patientID[9];
		struct appointment details;
		struct RegistrationInfo patient;
		char buffer[3];
		char opt;
		char opt2;
		char opt3;
		char opt4;
		char opt5;
		char bufferCh;
		int i;
		int index;
		int digitChoice;
		int dummy;

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
				strcpy(details.patient_id,patient.patient_id);
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
				appointmentHealthInsurancePrompt:
				
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("--------------------------------------------------------------\n");
				printf("==============     Appointment Registration     ==============\n\n\n");
				printf("\t\t\t........\n\n");
				printf("\n---------------------------------------------------------------\n");
				printf("[Y]\t  ...do you have health insurance?...\t\t[N]");
				printf("\n===============================================================\n");
				printf("Please select an option:\t\t");
				opt5 = toupper(getch());
				if (opt5 == 'Y' || opt5 == 'N') details.healthInsuranceStatus = opt5;
				else goto appointmentHealthInsurancePrompt;
				system("cls");//clears the screen
				printf("\tDr. Mitchell's Medical Center Appointment Program\n");
				printf("--------------------------------------------------------------\n");
				printf("==============     Appointment Registration     ==============\n\n");
				printf("Select the weekday of the appointment: \n[1]\tMonday\n[2]\tTuesday\n[3]\tWednesday\n[4]\tThursday\n[5]\tFriday\n\n");
				DayoftheWeekPrompt:
				opt3 = getch();
				switch (opt3)
				{
				case '1':
					strcpy(details.dow,"Monday");
					break;
				case '2':
					strcpy(details.dow,"Tuesday");
					break;
				case '3':
					strcpy(details.dow,"Wednesday");
					break;
				case '4':
					strcpy(details.dow,"Thursday");
					break;
				case '5':
					strcpy(details.dow,"Friday");
					break;	
				default:
					goto DayoftheWeekPrompt;
					break;
				}
				printf("Select the type of visit:\n[1]\tConsultation\n[2]\tPrimary Care\n[3]\tFollow-Up Visit\n[4]\tUrgent Visit\n\n");
				TypeofVisitPrompt:
				opt4 = getch();
				switch (opt4)
				{
				case '1':
					strcpy(details.tov,"Consultation");
					break;
				case '2':
					strcpy(details.tov,"Primary_Care");
					break;
				case '3':
					strcpy(details.tov,"Follow-Up_Visit");
					break;
				case '4':
					strcpy(details.tov,"Urgent_Visit");
					break;
				default:
					goto TypeofVisitPrompt;
					break;
				}
				fflush(stdin);//flushes the output buffer
				memset(details.timeofday,0,0);// clears the string
				printf("Enter the time of day (XX:00 - 24hr): ");
				TimeofDayPrompt1:
				bufferCh = getch();
				if(bufferCh == '1' || bufferCh == '0'){
					buffer[0] = bufferCh;
					printf("%c",buffer[0]);
				}
				else {
					goto TimeofDayPrompt1;
				}
				TimeofDayPrompt2:
				bufferCh = getch();
				if(isdigit(bufferCh))
				{
						buffer[1] = bufferCh;
						buffer[2] = '\0';
						printf("%c",buffer[1]);
				}
				else {
					goto TimeofDayPrompt2;
				}
				sscanf(buffer,"%d",&digitChoice);
				if (digitChoice >= 8 && digitChoice <= 19)
				{
					sprintf(details.timeofday,"%s:00",buffer);
					printf(":00\n");
				}
				else {
					printf("\b \b\b \b");
					printf("enter a valid time:( we're open 8AM - 7PM )");
					getch();
					printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b");
					goto TimeofDayPrompt1;
				}
				
				
				
				printf("Enter the appointment date (dd-mmm-yyyy): ");
				DatePrompta:
				i = 0;
				scanf("%d%s%d",&details.doa.dd,details.doa.mmm,&details.doa.yyyy);
				while (details.doa.mmm[i] != '\0')
				{
					details.doa.mmm[i] = tolower(details.doa.mmm[i]);
					i++;
				}
				for(index = 0; index < 12; index++){
					
					if(strcmp(details.doa.mmm,monthsOftheYear[index]) == 0){
						if(index == 0 || index == 2 || index == 4 || index == 6 || index == 7 || index == 9 || index ==11 )
						{
							if (details.doa.dd > 31 || details.doa.dd <= 0 )
							{
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
								printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
								goto DatePrompta;
							}
							
						}
						else if(index == 1)
						{
							if(details.doa.yyyy % 4 == 0){
								if (details.doa.dd > 29 || details.doa.dd <= 0) {
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
									printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
									getch();
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
									goto DatePrompta;
									} 
							}
							else if (details.doa.dd > 28 || details.doa.dd <= 0){
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
								printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
								goto DatePrompta;
								break;
							}
						}
						else if(index == 3|| index == 5 || index == 8 || index == 10)
						{
							if(details.doa.dd > 30 || details.doa.dd <= 0){
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
								printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
								goto DatePrompta;
							}
						}
						break;
					}
					else if (index == 11)
					{
						printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
						printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
						getch();
						printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
						goto DatePrompta;
						break;
					}
				}

				if (details.doa.yyyy >= (now->tm_year+1900)){
					if(details.doa.yyyy > (now->tm_year+1900))
					{
						dummy=1;
					}
					else if (details.doa.yyyy == (now->tm_year+1900))
					{
						if(index >= now->tm_mon){
							if(index == now->tm_mon){
								if (details.doa.dd >= now->tm_mday)
								{
										dummy = 1;							
								}
								else {
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
									printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
									getch();
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
									goto DatePrompta;
								}
							}
							else if (index > now->tm_mon){
								dummy = 1;
							}
						}
						else {
							printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
							printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
							getch();
							printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
							goto DatePrompta;
						}
					}
				}
				else {
					printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
					printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
					getch();
					printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
					goto DatePrompta;
				}

				if (searchAppointment(&details) < 1)
				{
					appointmentRegPrompt:
					system("cls");
					details.cost = calcAppointmentCost(&details,&patient);
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("================     Appointment Registration     ================\n\n");
					printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
					printf("Day of the Week:\t\t\t\t%s\n",details.dow);
					printf("Time Period (XX:00 - 24hr):\t\t\t%s\n",details.timeofday);
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
						printf("Day of the Week:\t\t\t%s\t\t[1]\n",details.dow);
						printf("Time Period (XX:00 - 24hr):\t\t\t%s\t\t[2]\n",details.timeofday);
						printf("Type of Visit:\t\t\t\t%s\t\t[3]\n",details.tov);
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
							printf("Day of the Week:\t\t\t%s\t\t[1]\n",details.dow);
							printf("Enter the time of day (XX:00 - 24hr): ");
							TimeofDayPrompt1a:
							bufferCh = getch();
							if(bufferCh == '1' || bufferCh == '0'){
								buffer[0] = bufferCh;
								printf("%c",buffer[0]);
							}
							else {
								goto TimeofDayPrompt1a;
							}
							TimeofDayPrompt2a:
							bufferCh = getch();
							if(isdigit(bufferCh)){
								buffer[1] = bufferCh;
								buffer[2] = '\0';
								printf("%c",buffer[1]);
							}
							else {
								goto TimeofDayPrompt2a;
							}
							sscanf(buffer,"%d",&digitChoice);
							if (digitChoice >= 8 && digitChoice <= 19)
							{
								sprintf(details.timeofday,"%s:00",buffer);
								printf(":00\n");
							}
							else {
								printf("\b \b\b \b");
								printf("enter a valid time:( we're open 8AM - 7PM )");
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b");
								goto TimeofDayPrompt1a;
							}
							goto editAppointment;
							break;
					
						case '1':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Select the weekday of the appointment: \n[a]\tMonday\n[b]\tTuesday\n[c]\tWednesday\n[d]\tThursday\n[e]\tFriday\n\n");
							DayoftheWeekPrompt2:
							opt3 = getch();
							switch (opt3)
							{
							case 'a':
								strcpy(details.dow,"Monday");
								break;
							case 'b':
								strcpy(details.dow,"Tuesday");
								break;
							case 'c':
								strcpy(details.dow,"Wednesday");
								break;
							case 'd':
								strcpy(details.dow,"Thursday");
								break;
							case 'e':
								strcpy(details.dow,"Friday");
								break;	
							default:
								goto DayoftheWeekPrompt2;
								break;
							}
							printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
							printf("Type of Visit:\t\t\t\t%s\t\t[3]\n",details.tov);
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
							printf("Select the type of visit:\n[a]\tConsultation\n[b]\tPrimary Care\n[c]\tFollow-Up Visit\n[d]\tUrgent Visit\n\n");
							TypeofVisitPrompt2:
							opt4 = getch();
							switch (opt4)
							{
							case 'a':
								strcpy(details.tov,"Consultation");
								break;
							case 'b':
								strcpy(details.tov,"Primary_Care");
								break;
							case 'c':
								strcpy(details.tov,"Follow-Up_Visit");
								break;
							case 'd':
								strcpy(details.tov,"Urgent_Visit");
								break;
							default:
								goto TypeofVisitPrompt2;
								break;
							}
							goto editAppointment;
							break;
						case '4':
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
							printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
							printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
							printf("Type of Visit:\t\t\t\t%s\t\t[3]\n",details.tov);
							printf("Enter the appointment date (dd-mmm-yyyy): ");
							DatePromptb:
							i = 0;
							scanf("%d%s%d",&details.doa.dd,details.doa.mmm,&details.doa.yyyy);
							while (details.doa.mmm[i] != '\0')
							{
								details.doa.mmm[i] = tolower(details.doa.mmm[i]);
								i++;
							}
							for(index = 0; index < 12; index++){
								
								if(strcmp(details.doa.mmm,monthsOftheYear[index]) == 0){
									if(index == 0 || index == 2 || index == 4 || index == 6 || index == 7 || index == 9 || index == 11 )
									{
										if (details.doa.dd > 31 || details.doa.dd <= 0 )
										{
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePromptb;
										}
										
									}
									else if(index == 1)
									{
										if(details.doa.yyyy % 4 == 0){
											if (details.doa.dd > 29 || details.doa.dd <= 0) {
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
												printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
												getch();
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
												goto DatePromptb;
												} 
										}
										else if (details.doa.dd > 28 || details.doa.dd <= 0){
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePromptb;
											break;
										}
									}
									else if(index == 3|| index == 5 || index == 8 || index == 10)
									{
										if(details.doa.dd > 30 || details.doa.dd <= 0){
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePromptb;
										}
									}
									break;
								}
								else if (index == 11)
								{
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
									printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
									getch();
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
									goto DatePromptb;
									break;
								}
							}

							if (details.doa.yyyy >= (now->tm_year+1900)){
								if(details.doa.yyyy > (now->tm_year+1900))
								{
									dummy=1;
								}
								else if (details.doa.yyyy == (now->tm_year+1900))
								{
									if(index >= now->tm_mon){
										if(index == now->tm_mon){
											if (details.doa.dd >= now->tm_mday)
											{
													dummy = 1;							
											}
											else {
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
												printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
												getch();
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
												goto DatePromptb;
											}
										}
										else if (index > now->tm_mon){
											dummy = 1;
										}
									}
									else {
										printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
										printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
										getch();
										printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
										goto DatePromptb;
									}
								}
							}
							else {
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
								printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
								goto DatePromptb;
							}

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
						printf("Type of Visit:\t\t\t\t%s\n",details.tov);
						printf("Appointment Date:\t\t\t\t%d-%s-%d\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
						printf("\n-------------------------------------------------------------------\n");
						printf("\t\t     record saved successfully\n");
						printf("====================================================================\n");
						printf("\t\t   ...press any key to continue...\n");
						getch();
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("================       Appointment Receipt        ================\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Appointment Date:\t\t\t\t%d-%s-%d\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
						printf("Day of the Week\t\t\t%s\n",details.dow);
						printf("Time Period:\t\t\t%s\n\n",details.timeofday);
						printf("Type of Visit:\t\t\t\t%s\n\n\n",details.tov);
						printf("Amount Due:\t\t%0.3lf\n",details.cost);
						printf("\n-------------------------------------------------------------------\n");
						printf("\t\t........\n");
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
					goto DatePrompta;
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
struct appointment __cdecl Search(char * patientID) //
{
	
	struct appointment searchResults[100];
	static struct appointment searchResult;
	struct appointment s;
	static fpos_t position;
	int i = 0;
	int counter = 0;
	matchcounter = 0;
	char opt[2];
	int digitChoice;
	char maxChoice[2];
	unsigned int pageNum = 1;
	unsigned int currPageNum = 1;
	int stopStatus = 0;	

	searchProcedure:
	if(i >= 99 ) stopStatus = 1;
	i = 0;
	FILE * fptr = fopen("appointment.txt","r");
		while(fptr != NULL)
		{
			if(feof(fptr)) break;
			if (position != 0) fsetpos(fptr,&position);

			fscanf(fptr,"%s %s %s %s %d %d %s %lf %c\n", s.patient_id, s.dow, s.timeofday, s.tov, &s.doa.dd, &s.doa.yyyy, s.doa.mmm, &s.cost, &s.healthInsuranceStatus);
			if (strcmp(s.patient_id,patientID) == 0 )
			{ 
				matchcounter++;
				if(i < 100)
				{
						strcpy(searchResults[i].patient_id,s.patient_id);
						strcpy(searchResults[i].doa.mmm,s.doa.mmm);
						strcpy(searchResults[i].dow,s.dow);
						strcpy(searchResults[i].tov,s.tov);
						strcpy(searchResults[i].timeofday,s.timeofday);
						searchResults[i].doa.dd = s.doa.dd;
						searchResults[i].doa.yyyy = s.doa.yyyy;
						searchResults[i].cost = s.cost;
						searchResults[i].healthInsuranceStatus = s.healthInsuranceStatus;
						i++;
				}
				else 
				{
					if ( i > 100 ) fgetpos(fptr,&position);
					i = 0;
				}
			}
	
		}
	fclose(fptr);

	i = 0;
	if(matchcounter < 1){
		printf("\n\n \t\t\t no appointments found\n\n\n");
		printf("------------------------------------------------------------------\n");
		printf("\t\t  ...press any key to continue...\n");
		printf("==================================================================\n");
		getch();
		menu();/* returns to the main menu*/ 
	}
	else 
	{
		pageNum = matchcounter/9;
		if (matchcounter % 9 > 0) pageNum++;
		if(matchcounter > 1) {
			while ( i <= matchcounter)
			{				
					if(i >= 99 && stopStatus == 0) goto searchProcedure;
					if( strcmp(patientID,searchResults[i].patient_id) == 0 && counter <= 9) {
						printf("\t%d-%s-%d %s %s %s\t\t[%d]\n",searchResults[i].doa.dd, searchResults[i].doa.mmm, searchResults[i].doa.yyyy, searchResults[i].dow,searchResults[i].timeofday,searchResults[i].tov,counter);
						counter++;
						i++;
						
					}
					else {					
						printf("\n-----------------------------------------------------------------\n");
						if(currPageNum > 1 && pageNum > 1)printf("[<- 'a' ]\t\t");
						else printf("\t\t\t  ");
						printf("Page [%d/%d]",currPageNum,pageNum);
						if(currPageNum < pageNum && pageNum > 1) printf("\t\t[ 'd' ->]");
						printf("\n==================================================================\n");
						printf("please select an option:\t\t");

						searchPrompt:
							opt[0] = getch();

							if( isdigit(opt[0]) ) {

								sscanf(opt,"%d", &digitChoice);
								if(digitChoice < counter){
									searchResult = searchResults[(digitChoice+(currPageNum-1)) + ((currPageNum-1)*9)];
									break;
								}
								else goto searchPrompt;

							}
							else {
								if(pageNum> 1)
								{
									if(opt[0] == 'a' && currPageNum <= pageNum && currPageNum > 1){
										/*Allow*/
										
										system("cls");
										printf("\tDr. Mitchell's Medical Center Appointment Program\n");
										printf("------------------------------------------------------------------\n\n");
										printf("==============        Appointment Information       ==============\n\n");
										i = i - (counter + (((currPageNum-1)*9)+1));
										counter = 0;
										currPageNum--;
										continue;
									
									}
									if(opt[0] == 'd' && currPageNum <= pageNum && pageNum > 1 && currPageNum < pageNum){
										system("cls");
										printf("\tDr. Mitchell's Medical Center Appointment Program\n");
										printf("------------------------------------------------------------------\n\n");
										printf("==============        Appointment Information       ==============\n\n");
										counter = 0;
										currPageNum++;
										continue;
									}
									else{
										goto searchPrompt;
									}
								}
							}
							//break;
					}
					
			}
		}
		else searchResult = searchResults[0];
	}

	return searchResult;
}
double calcAppointmentCost(struct appointment * details, struct RegistrationInfo * info)
{
	double cost;
	const double consultation = 4500.00;
	const double primaryCare = 8000.00;
	const double followUp = 9500.00;
	const double urgentVisit = 15500.00;
	double coverage;

	if(details->healthInsuranceStatus == 'Y'){

		if(info->age <= 11 && info->age >= 0) coverage = 0.4;
		else if(info->age <= 18 && info->age >= 12) coverage = 0.35;
		else if (info->age <= 25 && info->age >= 19) coverage = 0.3;
		else if (info->age <= 45 && info->age >= 26) coverage = 0.25;
		else if (info->age <= 100 && info->age >= 46) coverage = 0.2;

		if(strcmp(details->tov,"Consultation") == 0) cost = consultation - (consultation*coverage);
		else if(strcmp(details->tov,"Primary_Care") == 0) cost = primaryCare - (primaryCare*coverage);
		else if(strcmp(details->tov,"Follow-Up_Visit") == 0) cost = followUp - (followUp*coverage);
		else if(strcmp(details->tov,"Urgent_Visit") == 0) cost = urgentVisit - (urgentVisit*coverage);
	}
	else {
		if(strcmp(details->tov,"Consultation") == 0) cost = consultation;
		else if(strcmp(details->tov,"Primary_Care") == 0) cost = primaryCare;
		else if(strcmp(details->tov,"Follow-Up_Visit") == 0) cost = followUp;
		else if(strcmp(details->tov,"Urgent_Visit") == 0) cost = urgentVisit;
		}
	return cost;
}

void Update() //
{

		time_t today;
		struct tm * now;
		time(&today);
		now = localtime(&today);

		struct appointment ptr;
		struct appointment details;
		struct appointment s;
		struct RegistrationInfo patient;

		FILE * fptr;
		FILE * fptr2;
		char patientID[128];
		char bufferCh;
		char buffer[3];
		int i = 1;
		int index;
		int dummy;
		int digitChoice;
		char opt;
		char opt2;
		char opt3;
		char opt4;

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
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============          Edit Appointment(s)         ==============\n\n");
					ptr = Search(patient.patient_id);
					details = ptr;
					updateAppointment:
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============          Edit Appointment(s)         ==============\n\n");
					printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
					printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
					printf("Time Period (XX:00 - 24hr):\t\t\t%s\t\t[2]\n",details.timeofday);
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
						printf("==============          Edit Appointment(s)         ==============\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
						printf("Enter the time of day (XX:00 - 24hr): ");
						TimeofDayPrompt1c:
							bufferCh = getch();
							if(bufferCh == '1' || bufferCh == '0'){
								buffer[0] = bufferCh;
								printf("%c",buffer[0]);
							}
							else {
								goto TimeofDayPrompt1c;
							}
							TimeofDayPrompt2c:
							bufferCh = getch();
							if(isdigit(bufferCh)){
								buffer[1] = bufferCh;
								buffer[2] = '\0';
								printf("%c",buffer[1]);
							}
							else {
								goto TimeofDayPrompt2c;
							}
							sscanf(buffer,"%d",&digitChoice);
							if (digitChoice >= 8 && digitChoice <= 19)
							{
								sprintf(details.timeofday,"%s:00",buffer);
								printf(":00\n");
							}
							else {
								printf("\b \b\b \b");
								printf("enter a valid time:( we're open 8AM - 7PM )");
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b");
								goto TimeofDayPrompt1c;
							}
						goto updateAppointment;
						break;
				
					case '1':
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("==============          Edit Appointment(s)         ==============\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Select the weekday of the appointment: \n[a]\tMonday\n[b]\tTuesday\n[c]\tWednesday\n[d]\tThursday\n[e]\tFriday\n\n");
						DayoftheWeekPrompt2a:
						opt3 = getch();
						switch (opt3)
						{
						case 'a':
							strcpy(details.dow,"Monday");
							break;
						case 'b':
							strcpy(details.dow,"Tuesday");
							break;
						case 'c':
							strcpy(details.dow,"Wednesday");
							break;
						case 'd':
							strcpy(details.dow,"Thursday");
							break;
						case 'e':
							strcpy(details.dow,"Friday");
							break;	
						default:
							goto DayoftheWeekPrompt2a;
							break;
						}
						printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
						printf("Type of Visit\t\t\t\t%s\t\t[3]\n",details.tov);
						printf("Appointment Date:\t\t\t\t%d-%s-%d\t\t[4]\n", details.doa.dd,details.doa.mmm,details.doa.yyyy);
						printf("\n------------------------------------------------------------------\n");
						printf("\t\t\t\t....");
						printf("\n===================================================================\n");
						goto updateAppointment;
						break;
					case '3':
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("==============          Edit Appointment(s)         ==============\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
						printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
						printf("Select the type of visit:\n[a]\tConsultation\n[b]\tPrimary Care\n[c]\tFollow-Up Visit\n[d]\tUrgent Visit\n\n");
						TypeofVisitPrompt2a:
						opt4 = getch();
						switch (opt4)
						{
						case 'a':
							strcpy(details.tov,"Consultation");
							break;
						case 'b':
							strcpy(details.tov,"Primary_Care");
							break;
						case 'c':
							strcpy(details.tov,"Follow-Up_Visit");
							break;
						case 'd':
							strcpy(details.tov,"Urgent_Visit");
							break;
						default:
							goto TypeofVisitPrompt2a;
							break;
						}
						details.cost = calcAppointmentCost(&details,&patient);
						goto updateAppointment;
						break;
					case '4':
						updateAppointmentDatePrompt:
						system("cls");
						printf("\tDr. Mitchell's Medical Center Appointment Program\n");
						printf("------------------------------------------------------------------\n\n");
						printf("==============          Edit Appointment(s)         ==============\n\n");
						printf("Patient ID:\t\t\t\t[%s]\n\n\n",patient.patient_id);
						printf("Day of the Week\t\t\t%s\t\t[1]\n",details.dow);
						printf("Time Period:\t\t\t%s\t\t[2]\n",details.timeofday);
						printf("Type of Visit\t\t\t\t%s\t\t[3]\n",details.tov);
						printf("Enter the appointment date (dd-mmm-yyyy): ");
						DatePrompt1a:
							i = 0;
							scanf("%d%s%d",&details.doa.dd,details.doa.mmm,&details.doa.yyyy);
							while (details.doa.mmm[i] != '\0')
							{
								details.doa.mmm[i] = tolower(details.doa.mmm[i]);
								i++;
							}
							for(index = 0; index < 12; index++){
								
								if(strcmp(details.doa.mmm,monthsOftheYear[index]) == 0){
									if(index == 0 || index == 2 || index == 4 || index == 6 || index == 7 || index == 9 || index == 11 )
									{
										if (details.doa.dd > 31 || details.doa.dd <= 0 )
										{
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePrompt1a;
										}
										
									}
									else if(index == 1)
									{
										if(details.doa.yyyy % 4 == 0){
											if (details.doa.dd > 29 || details.doa.dd <= 0) {
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
												printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
												getch();
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
												goto DatePrompt1a;
												} 
										}
										else if (details.doa.dd > 28 || details.doa.dd <= 0){
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePrompt1a;
											break;
										}
									}
									else if(index == 3|| index == 5 || index == 8 || index == 10)
									{
										if(details.doa.dd > 30 || details.doa.dd <= 0){
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
											printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
											getch();
											printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
											goto DatePrompt1a;
										}
									}
									break;
								}
								else if (index == 11)
								{
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
									printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
									getch();
									printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b");
									goto DatePrompt1a;
									break;
								}
							}

							if (details.doa.yyyy >= (now->tm_year+1900)){
								if(details.doa.yyyy > (now->tm_year+1900))
								{
									dummy=1;
								}
								else if (details.doa.yyyy == (now->tm_year+1900))
								{
									if(index >= now->tm_mon){
										if(index == now->tm_mon){
											if (details.doa.dd >= now->tm_mday)
											{
													dummy = 1;							
											}
											else {
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
												printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
												getch();
												printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
												goto DatePrompt1a;
											}
										}
										else if (index > now->tm_mon){
											dummy = 1;
										}
									}
									else {
										printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
										printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
										getch();
										printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
										goto DatePrompt1a;
									}
								}
							}
							else {
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b");
								printf("%c[1A\t\t\t\t\t: enter a valid date!",033);
								getch();
								printf("\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b \b\b \b\b \b\b \b");
								goto DatePrompt1a;
							}

						if (searchAppointment(&details) > 1)
						{
							system("cls");
							printf("\tDr. Mitchell's Medical Center Appointment Program\n");
							printf("------------------------------------------------------------------\n\n");
							printf("================     Appointment Registration     ================\n\n");
							printf("\n-------------------------------------------------------------------\n");
							printf("\t\t     appointment slot already taken\n");
							printf("====================================================================\n");
							printf("\t\t   ...press any key to continue...\n");
							getch();
							goto updateAppointmentDatePrompt;
						}
						else{
						printf("\n------------------------------------------------------------------\n");
						printf("\t\t\t\t....");
						printf("\n===================================================================\n");
						details.cost = calcAppointmentCost(&details,&patient);
						goto updateAppointment;
						}
						break;
					case '\e':
							fptr2 = fopen("appointment__buffer.txt","a");
								fptr = fopen("appointment.txt","r");
									while(!feof(fptr)){
										fscanf(fptr,"%s %s %s %s %d %d %s %lf %c\n", s.patient_id, s.dow, s.timeofday, s.tov, &s.doa.dd, &s.doa.yyyy, s.doa.mmm,&s.cost,&s.healthInsuranceStatus);
										//searches for the appointment record that matches the previously selected appointment
										if(strcmp(s.patient_id,ptr.patient_id) == 0 && strcmp(s.timeofday,ptr.timeofday) == 0 && strcmp(s.tov,ptr.tov) == 0 && s.doa.dd == ptr.doa.dd && s.doa.yyyy == ptr.doa.yyyy && strcmp(s.doa.mmm,ptr.doa.mmm) == 0 && s.cost == ptr.cost && s.healthInsuranceStatus == ptr.healthInsuranceStatus) 
										{
											fprintf(fptr2,"%s %s %s %s %d %d %s %lf %c\n",details.patient_id, details.dow, details.timeofday, details.tov, details.doa.dd, details.doa.yyyy, details.doa.mmm,details.cost,details.healthInsuranceStatus);
											//appends the newly updated information into the string
										}
										else {
											fprintf(fptr2,"%s %s %s %s %d %d %s %lf %c\n",s.patient_id, s.dow, s.timeofday, s.tov, s.doa.dd, s.doa.yyyy, s.doa.mmm,s.cost,s.healthInsuranceStatus);
										}
									}
									
								fclose(fptr);
							fclose(fptr2);
							remove("appointment.txt");//deletes the file, 'appointment.txt'
							rename("appointment__buffer.txt","appointment.txt");// renames the file,'appointment__buffer.txt' to 'appointment.txt'

						break;
					default:
						printf("\n------------------------------------------------------------\n");
						printf("no option.. please retype");
						getch();
						goto updateAppointment;
						break;
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
		struct appointment ptr;
		struct RegistrationInfo patient;
		char patientID[128];
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
					View();
					break;
				case '2':
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============         View Appointment(s)          ==============\n\n");
					ptr = Search(patient.patient_id); // searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found, and stores the address into the pointer, 'ptr' 
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
					printf("Patient Phone Number:\t\t\t+1 (876) %s\n\n",patient.telephone);
					printf("==============        Appointment Information       ==============\n\n");
					printf("Appointment Date:\t\t%d-%s-%d\t%s\n",ptr.doa.dd, ptr.doa.mmm, ptr.doa.yyyy,ptr.dow);
					printf("Type of Visit:\t\t\t%s\n",ptr.tov);
					printf("Appointment Time:\t\t%s\n\n",ptr.timeofday);
					printf("Amount Due:\t\t\t$%0.3lf\n",ptr.cost);
					printf("\n----------------------------------------------------------------\n");
					printf("\t\t...press any key to continue...");
					printf("\n================================================================\n");
					getch();
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
		struct appointment ptr;
		struct appointment s;
		struct RegistrationInfo patient;
		FILE * fptr;
		FILE * fptr2;

		char patientID[128];
		int i = 0;
		char opt;
		char opt2;
	
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
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============         Delete Appointment(s)        ==============\n\n");
					ptr = Search(patient.patient_id);// searchs the file, 'appointment.txt' for the matching patientID given and returns the struct of the matches found, and stores the address into the pointer, 'ptr' 
					system("cls");
					printf("\tDr. Mitchell's Medical Center Appointment Program\n");
					printf("------------------------------------------------------------------\n\n");
					printf("==============           Delete Appointment         ==============\n\n");
					printf("Appointment Date:\t\t%d-%s-%d\t\t%s\n\n",ptr.doa.dd, ptr.doa.mmm, ptr.doa.yyyy,ptr.dow);
					printf("Appointment Visit:\t\t%s\n",ptr.tov);
					printf("Appointment Time:\t\t%s\n",ptr.timeofday);
					printf("\n----------------------------------------------------------------\n");
					printf("[Y]\t\t...do you want to continue?...\t\t[N]");
					printf("\n================================================================\n");
					printf("please select an option:\t\t");
					deleteAppointmentPrompt:
					opt2 = toupper(getch());
					if( opt2 == 'Y' || opt2 == 'N'){
						if(opt2 == 'Y') {

							fptr2 = fopen("appointment__buffer.txt","a");
								fptr = fopen("appointment.txt","r");
									while(!feof(fptr)){
										fscanf(fptr,"%s %s %s %s %d %d %s %lf %c\n", s.patient_id, s.dow, s.timeofday, s.tov, &s.doa.dd, &s.doa.yyyy, s.doa.mmm,&s.cost,&s.healthInsuranceStatus);
										if(strcmp(s.patient_id,ptr.patient_id) == 0 && strcmp(s.timeofday,ptr.timeofday) == 0 && strcmp(s.tov,ptr.tov) == 0 && s.doa.dd == ptr.doa.dd && s.doa.yyyy == ptr.doa.yyyy && strcmp(s.doa.mmm,ptr.doa.mmm) == 0 && s.cost == ptr.cost && s.healthInsuranceStatus == ptr.healthInsuranceStatus)
										{
											continue;
										}
										else {
											fprintf(fptr2,"%s %s %s %s %d %d %s %lf %c\n",s.patient_id, s.dow, s.timeofday, s.tov, s.doa.dd, s.doa.yyyy, s.doa.mmm,s.cost,s.healthInsuranceStatus);
											if(!feof(fptr2)) fprintf(fptr2,"\n");
										}
									}
									
								fclose(fptr);
							fclose(fptr2);
							remove("appointment.txt");
							rename("appointment__buffer.txt","appointment.txt");
						}
						if(opt2 == 'N') menu();
					}
					else goto deleteAppointmentPrompt;
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