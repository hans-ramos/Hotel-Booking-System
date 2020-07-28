#include <stdio.h>
#include <string.h>
#define CAPACITY 40
// Defines data type for strings with 25 string length
typedef char String25[26];
// Defines data type for strings with 40 string length
typedef char String40[41];

// Structure for rooms
struct roomTag {
		char roomCode;
		int roomCapacity, roomsAmount;
		float weekdayRate,weekendRate,perGuestRate;
};

// Structure for booking requests
struct bookingTag{
		String25 lastName,firstName;
		String40 emailAddress;
		char roomCode;
		int guestNumber, startingDay, stayDuration;
};

// Structure for the final report
struct reportTag{
		int roomCodeNumber, weekdayRooms, weekdayBooked, weekendRooms, weekendBooked;
		char roomCode;
		float occupancyRate;
};

// Structure for seeing how many rooms per room code are available in a given day of a week
struct weekBook{
		int monday, tuesday, wednesday, thursday, friday, saturday, sunday;
};

/*
	Function Name: getrooms
	Purpose of the Function: Reads data from rooms.txt and stores the data into an array of struct roomTag. Also counts the number of unique room codes.
	Pre-condition: Needs to find the file rooms.txt.
	Post-condition: The rooms array will be initialized with data from rooms.txt.
	Function's return value: Returns room details and number of unique room codes through parameters.
	List of parameters: struct roomTag rooms[] - Called by the function to store data from rooms.txt in it.
						int *uniquerooms - Called by the function to return how many unique room codes are in rooms.txt.
						int *found - Called by the function to let the program know if file is found.
*/

void getrooms (struct roomTag rooms[],int *uniquerooms, int *found)
{
	FILE *fptr;
	struct roomTag onerecord;
	int i,j,roomcodes=0,repeatedRoom=0;
	// Opens rooms.txt
	fptr = fopen ("rooms.txt","r");
	// If rooms.txt is found, then file is read.
	if (fptr != NULL){
		i=0;
		while (fscanf(fptr," %c %d %d %f %f %f", &onerecord.roomCode,&onerecord.roomCapacity,&onerecord.roomsAmount,
					 &onerecord.weekdayRate, &onerecord.weekendRate, &onerecord.perGuestRate)==6){
			// Loop checks if room code has been repeated.
			for (j=0;j<i;j++)
				// If the room code has the same details except for amount of rooms, then the amount of rooms in that room code will be increased with the value read.
				if (onerecord.roomCode==rooms[j].roomCode&&onerecord.roomCapacity==rooms[j].roomCapacity
					&&onerecord.weekdayRate==rooms[j].weekdayRate&&onerecord.weekendRate==rooms[j].weekendRate&&onerecord.perGuestRate==rooms[j].perGuestRate&&onerecord.roomsAmount>0){
						//repeatedRoom is a value that will help the program recognize if the read file is a repeating room code character.
						repeatedRoom=1;
						rooms[j].roomsAmount=rooms[j].roomsAmount+onerecord.roomsAmount;
				}
			// If repeatedRoom is not a nonzero value, then it will equate to 0. This prevents repeatedRooms to be initialized in the array of rooms.	
			if (repeatedRoom)
				repeatedRoom=0;	
			// Else, the rooms array will get a new element in the array.	
			else if (!repeatedRoom){		
				rooms[i]=onerecord;
				i++;
				roomcodes++;
			}		
		}
		// At the end of the loop, the program initiallizes the value of *uniquerooms.
		*uniquerooms = roomcodes;
		*found=1;
		fclose (fptr);
	}
	// If rooms.txt is not found, then the following error message is displayed.
	else{
		printf ("Loading hotel information from rooms.txt... Failed!\n");
		printf ("The rooms.txt file is unreadable or missing.\n");
		printf ("Exiting the program. Bye!\n");
		*found=0;
	}
}

/*
	Function Name: getTotalRooms
	Purpose of the Function: Counts how many total available rooms are there in the hotel.
	Pre-condition: rooms and uniquerooms must be initialized and file must be found.
	Post-condition: The value of total available rooms are initialized.
	Function's return value: Returns how many available rooms are there in the hotel.
	List of parameters: struct roomTag rooms[] - Called by the function to get and add the amount of rooms per room code.
						int uniquerooms - Called by the function to know when the loop will end.
*/

int getTotalRooms (struct roomTag rooms[], int uniquerooms){
	int total=0,i;
	for (i=0;i<uniquerooms;i++)
		total += rooms[i].roomsAmount;
	return total;	
}

/*
	Function Name: checkRoomCodes
	Purpose of the Function: Checks if the room codes are unique or not.
	Pre-condition: roomList and roomCodeNum must be initialized and file must be found.
	Post-condition: The program will recognize if the room codes are unique or not.
	Function's return value: Returns 1 if there are nonunique room codes or returns 0 if all room codes are unique.
	List of parameters: struct roomTag roomList[] - Called by the function to get room codes per element.
						int roomCodeNum - Called by the function to know when the loop will end.
*/

int checkRoomCodes (struct roomTag roomList[],int roomCodeNum)
{
	int i,j;
	for (i=0;i<roomCodeNum-1;i++){
		for (j=i+1;j<roomCodeNum;j++)
			if (roomList[i].roomCode==roomList[j].roomCode)
				// Returns an error
				return 1;
	}
	// Returns no error
	return 0;
}
/*
	Function Name: checkIfLetter
	Purpose of the Function: Checks if the room codes are a capital letter.
	Pre-condition: roomList and roomCodeNum must be initialized and file must be found.
	Post-condition: The program will recognize if the room codes are capital letters or not.
	Function's return value: Returns the index + 1 if an error was found. Returns 0 if no error was found.
	List of parameters: struct roomTag roomList[] - Called by the function to get the room codes of each element.
						int roomCodeNum - Called by the function to know when the loop will end.
*/
int checkIfLetter (struct roomTag roomList[],int roomCodeNum)
{
	int i;
	for (i=0;i<roomCodeNum;i++)
		if (roomList[i].roomCode<'A'||roomList[i].roomCode>'Z')
				// Returns the line number
				return i+1;
	// Returns no error			
	return 0;
}

/*
	Function Name: checkRoomCapacity
	Purpose of the Function: Checks if the room capacity is within the given range (1-12).
	Pre-condition: roomList and roomCodeNum must be initialized and file must be found.
	Post-condition: The program will recognize if the room capacity is valid or not.
	Function's return value: Returns the index + 1 if an error was found. Returns 0 if no error was found.
	List of parameters: struct roomTag roomList[] - Called by the function to get the room codes of each element.
						int roomCodeNum - Called by the function to know when the loop will end.
*/

int checkRoomCapacity (struct roomTag roomList[], int roomCodeNum)
{
	int i;
	for (i=0;i<roomCodeNum;i++)	
		if (roomList[i].roomCapacity<=0||roomList[i].roomCapacity>12)
				// Returns the line number
				return i+1;
	// Returns no error			
	return 0;
}

/*
	Function Name: checkRoomAmounts
	Purpose of the Function: Checks if the amount of rooms are a capital within the range.
	Pre-condition: roomList and roomCodeNum must be initialized and file must be found.
	Post-condition: The program will recognize if the amount of rooms are valid or not.
	Function's return value: Returns the index + 1 if an error was found. Returns 0 if no error was found.
	List of parameters: struct roomTag roomList[] - Called by the function to get the room codes of each element.
						int roomCodeNum - Called by the function to know when the loop will end.
*/

int checkRoomAmounts (struct roomTag roomList[], int roomCodeNum)
{
	int i;
	for (i=0;i<roomCodeNum;i++)	
		if (roomList[i].roomsAmount<=0||roomList[i].roomsAmount>40)
				// Returns the line number
				return i+1;
	// Returns no error			
	return 0;
}

/*
	Function Name: checkRoomRates
	Purpose of the Function: Checks if the room rates are a positive value.
	Pre-condition: roomList and roomCodeNum must be initialized and file must be found.
	Post-condition: The program will recognize if the room rates are valid or not.
	Function's return value: Returns the index + 1 if an error was found. Returns 0 if no error was found.
	List of parameters: struct roomTag roomList[] - Called by the function to get the room codes of each element.
						int roomCodeNum - Called by the function to know when the loop will end.
*/

int checkRoomRates (struct roomTag roomList[], int roomCodeNum)
{
	int i;
	for (i=0;i<roomCodeNum;i++)	
		if (roomList[i].weekdayRate<=0||roomList[i].weekendRate<=0)
				// Returns the line number
				return i+1;
	// Returns no error			
	return 0;
}

/*
	Function Name: displayAvailableRooms
	Purpose of the Function: Checks if all rooms are valid and displays the total number of available rooms.
	Pre-condition: All function parameters except for validTxtFile must be initialized and file must be found.
	Post-condition: The program will recognize if all rooms are valid or not. It also displays the total number of available rooms.
	Function's return value: Returns 1 if all rooms are valid or 0 if invalid through the validTxtFile parameter.
	List of parameters: int totalRooms - Called by the function to get the total number of rooms.
						int checkCodes - Called by the function to know if room codes are unique or not.
						int checkLetter - Called by the function to know if room codes a capital letter or not.
						int checkCapacity - Called by the function to know if room capacity of all room codes is within 1-12.
						int checkAmount - Called by the function to know if amount of rooms are within 1-40.
						int checkAmount - Called by the function to know if room rates are a positive value.
						int *validTxtFile - Called by the function to initialize the value to 1 or 0.
*/

void displayAvailableRooms (int totalRooms,int checkCodes, int checkLetter,int checkCapacity,int checkAmount, int checkRates, int *validTxtFile)
{
	if (totalRooms>0&&!checkCodes&&!checkLetter&&!checkCapacity&&!checkAmount&&!checkRates){
		printf ("Loading hotel information from rooms.txt... Success!\n");	
		printf ("The hotel has %d rooms available.\n",totalRooms);
		*validTxtFile = 1;
	}
	else if (totalRooms<=0){
		printf ("Loading hotel information from rooms.txt... Success!\n");	
		printf ("The hotel has %d rooms available.\n",totalRooms);
		printf ("No eligible rooms were found in the input file, exiting the program. Bye!");
		*validTxtFile = 0;
	}
	else if (checkCodes){
		printf ("Loading hotel information from rooms.txt... Failed!\n");	
		printf ("The hotel \"room codes\" are not unique.\n");
		printf ("Exiting the program. Bye!");
		*validTxtFile = 0;		
	}
	else if (checkLetter){
		printf ("Loading hotel information from rooms.txt... Failed!\n");	
		printf ("Found an invalid \"room code\" at line %d.\n",checkLetter);
		printf ("Exiting the program. Bye!");
		*validTxtFile = 0;		
	}	
	else if (checkCapacity){
		printf ("Loading hotel information from rooms.txt... Failed!\n");	
		printf ("Found an invalid room capacity at line %d.\n",checkCapacity);
		printf ("Exiting the program. Bye!");
		*validTxtFile = 0;
	}	
	else if (checkAmount){
		printf ("Loading hotel information from rooms.txt... Failed!\n");	
		printf ("Found an invalid amount of rooms at line %d.\n",checkAmount);
		printf ("Exiting the program. Bye!");
		*validTxtFile = 0;
	}
	else if (checkRates){
		printf ("Loading hotel information from rooms.txt... Failed!\n");	
		printf ("Found an invalid room rate of Php 0.00 at line %d.\n",checkRates);
		printf ("Exiting the program. Bye!");
		*validTxtFile = 0;
	}
}

/*
	Function Name: initializeVacantRooms
	Purpose of the Function: Initializes the vacantRooms array with the roomsAmount element of the roomTag rooms array.
	Pre-condition: validTxtFile must be equal to one and rooms array and uniqueroomcodes must be initialized.
	Post-condition: The vacantRooms array will be initialized
	Function's return value: Returns the number of vacant rooms per day per room code through the parameters
	List of parameters: struct weekBook vacantRooms[] - Called by the function so it can be stored with values.
						struct roomTag rooms[] - Called by the function so it can get the amount of rooms per room code.
						int uniqueroomcodes - Called by the function so it will know when the loop ends.
*/

void initializeVacantRooms (struct weekBook vacantRooms[], struct roomTag rooms[], int uniqueroomcodes)
{
	int i;
	for (i=0;i<uniqueroomcodes;i++){
		vacantRooms[i].monday=rooms[i].roomsAmount;
		vacantRooms[i].tuesday=rooms[i].roomsAmount;
		vacantRooms[i].wednesday=rooms[i].roomsAmount;
		vacantRooms[i].thursday=rooms[i].roomsAmount;
		vacantRooms[i].friday=rooms[i].roomsAmount;
		vacantRooms[i].saturday=rooms[i].roomsAmount;
		vacantRooms[i].sunday=rooms[i].roomsAmount;
	}
}

/*
	Function Name: initializeReport
	Purpose of the Function: Initializes the reportTag rows array.
	Pre-condition: validTxtFile must be equal to one and rooms array and uniqueroomcodes must be initialized.
	Post-condition: The rows array will be initialized with data from the array of rooms and some zeroes.
	Function's return value: Returns elements of rows array through the parameters
	List of parameters: struct reportTag rows[] - Called by the function so it can be stored with values.
						struct roomTag rooms[] - Called by the function because some data within will be used by the reportTag struct.
						int uniqueroomcodes - Called by the function so it will know when the loop ends.
*/

void initializeReport (struct reportTag rows[],struct roomTag rooms[], int uniqueroomcodes)
{
	int i;
	for (i=0;i<uniqueroomcodes;i++){
		rows[i].roomCodeNumber=i+1;
		rows[i].roomCode=rooms[i].roomCode;
		rows[i].weekdayRooms=rooms[i].roomsAmount*5;
		rows[i].weekendRooms=rooms[i].roomsAmount*2;
		rows[i].weekdayBooked=0;
		rows[i].weekendBooked=0;
		rows[i].occupancyRate=0;
		}
}

/*
	Function Name: validateEmail
	Purpose of the Function: Validates if the entered email is of valid format.
	Pre-condition: validTxtFile must be equal to one valid email is not equal to 0.
	Post-condition: Program will recognize if email is valid or not.
	Function's return value: Returns 1 if email is invalid or 0 if email is valid
	List of parameters: string40 emailAddress - Function calls emailAddress to validate it.
*/

int validateEmail (String40 emailAddress)
{
	int i,j=0,k,atsignCheck=0,specialCharCheck=0,domainNameCheck=0,letterStartCheck=0,prevAtsignCheck=0, emailLength;
	String25 domainName;
	//strtok eliminates \n from the emailAddress.
	strtok (emailAddress,"\n");			
	//Gets the length of the email address
	emailLength=strlen(emailAddress);
	//Initializes the domain name
	for (i=emailLength-4;i<emailLength+1;i++,j++)
		domainName[j]=emailAddress[i];
	// Checks if starting character is a letter or number	
	if ((emailAddress[0]>='A'&&emailAddress[0]<='Z')||(emailAddress[0]>='a'&&emailAddress[0]<='z')||(emailAddress[0]>='0'&&emailAddress[0]<='9'))
		letterStartCheck++;	
	// Counts how many "@" are there in the string	
	for (i=0; i<emailLength;i++)
		if (emailAddress[i]=='@'){			
			atsignCheck++;
			// k is used later to check the character before the atsign
			k=i;	
		}
	for (i=0; i<emailLength;i++)
		if ((emailAddress[i]<'-')||(emailAddress[i]=='/')||(emailAddress[i]>'9'&&emailAddress[i]<'@')||(emailAddress[i]>'['&&emailAddress[i]<'_')||(emailAddress[i]>'z')||(emailAddress[i]==96))
			// returns 1 if invalid character is found
			return 1;	
	// Checks the preceding character before the "@" sign.	
	if ((emailAddress[k-1]>='A'&&emailAddress[k-1]<='Z')||(emailAddress[k-1]>='a'&&emailAddress[k-1]<='z')||(emailAddress[k-1]>='0'&&emailAddress[k-1]<='9'))
		prevAtsignCheck++;
	// Checks if domain name is valid.		
	if ((strcmp(domainName,".com")!=0)&&(strcmp(domainName,".org")!=0)&&(strcmp(domainName,".edu")!=0))
		domainNameCheck++;	
	// CHecks if special characters appear succeedingly	
	for (i=0;i<emailLength-1;i++)
		if ((emailAddress[i]=='.'&&emailAddress[i+1]=='.')||(emailAddress[i]=='_'&&emailAddress[i+1]=='_')||(emailAddress[i]=='-'&&emailAddress[i+1]=='-'))
			specialCharCheck++;	
	// Returns no error		
	if (atsignCheck==1&&!specialCharCheck&&!domainNameCheck&&letterStartCheck&&prevAtsignCheck)
		return 0;
	// Returns an error	
	else
		return 1;	
}

/*
	Function Name: getRoomSpecs
	Purpose of the Function: Gets the details of the inputted room code by the user.
	Pre-condition: Inputted room code must be a room code within rooms.txt.
	Post-condition: Various pointers will be initialized to be used later on the program.
	Function's return value: Returns values concening the room through the parameters.
	List of parameters: struct roomTag rooms[] - Called by the function to get the values.
						char roomCode - Called by the function to see if inputted room code is found in the rooms array.
						int totalroomcodes - Called by the function so it will know when the loop ends.
						int *roomCapacity - Called by the function to be initialized with the roomCapacity value from the matching room code.
						int *roomAmount - Called by the function to be initialized with the roomsAmount value from the matching room code.
						int *weekDayRate - Called by the function to be initialized with the weekdayRate value from the matching room code.
						int *weekEndRate - Called by the function to be initialized with the weekendRate value from the matching room code.
						int *perGuestRate - Called by the function to be initialized with the perGuestRate value from the matching room code.
						int *roomcodenumber - Called by the function to be initialized with the index where the matching room code was found.
*/

void getRoomSpecs (struct roomTag rooms[],char roomCode, int totalroomcodes, int *roomCapacity, int *roomAmount,
				 float *weekDayRate, float *weekEndRate, float *perGuestRate, int *roomcodenumber)
{
	int i;
	*roomCapacity = 0;
	*roomAmount = 0;
	*weekDayRate = 0;
	*weekEndRate = 0;
	*perGuestRate = 0;
	*roomcodenumber=0;	
	for (i=0; i<totalroomcodes;i++){
		if (roomCode == rooms[i].roomCode|| roomCode-32 == rooms[i].roomCode){
			*roomCapacity = rooms[i].roomCapacity;
			*roomAmount = rooms[i].roomsAmount;
			*weekDayRate = rooms[i].weekdayRate;
			*weekEndRate = rooms[i].weekendRate;
			*perGuestRate = rooms[i].perGuestRate;
			*roomcodenumber=i;
		}
	}
}

/*
	Function Name: validateRoom
	Purpose of the Function: Checks if all booking details are valid or not.
	Pre-condition: struct bookingTag Request must be initialized.
	Post-condition: Program will recognize if the booking is a success or not.
	Function's return value: Returns 0 if no error was founnd. Else, it returns a nonzero value that corresponds with an error messsage.
	List of parameters: struct bookingTag rooms - Called by the function for it to validate.
						struct roomTag rooms[] - Called by the function to get the values of the elements.
						int totalroomcodes - Called by the function so it will know when the loop ends.
						int roomGuests - Called by the function to compare with the number of guests inputted by the user.
						int roomcodenumber - Called by the function to be used to get the appropriate element in the availRooms array.
						struct weekBook availRooms - Called by the function to see the available rooms per day.
*/

int validateRoom (struct bookingTag Request, struct roomTag rooms[], int totalroomcodes, int roomGuests, int roomcodenumber,struct weekBook availRooms[])
{
	int i, checkroomcode=0, checkvacantroom=0;
	// Checks if room code exists
	for (i=0;i<totalroomcodes;i++)
		if (Request.roomCode == rooms[i].roomCode || Request.roomCode-32 == rooms[i].roomCode){
			checkroomcode=1;
			break;	
		}
	// checks if the room is available in the range of the customer's booking	
 	for (i=Request.startingDay;i<Request.startingDay+Request.stayDuration;i++){
 		if (i==0)
 			if (availRooms[roomcodenumber].monday<=0)
 				checkvacantroom++;
	 	if (i==1)
 			if (availRooms[roomcodenumber].tuesday<=0)
 				checkvacantroom++;
 		if (i==2)
 			if (availRooms[roomcodenumber].wednesday<=0)
 				checkvacantroom++;
		if (i==3)
 			if (availRooms[roomcodenumber].thursday<=0)
 				checkvacantroom++;
		if (i==4)
 			if (availRooms[roomcodenumber].friday<=0)
 				checkvacantroom++;
		if (i==5)
 			if (availRooms[roomcodenumber].saturday<=0)
 				checkvacantroom++;
		if (i==6)
 			if (availRooms[roomcodenumber].sunday<=0)
 				checkvacantroom++;		 		 		 		 		
	 }	
	// If room code does not exist
	if (!checkroomcode)
		return 1;
	// If guest number is greater than the allowed capacity of the room or equal to 0 or negative.	
	else if (Request.guestNumber>roomGuests||Request.guestNumber<=0)
		return 3;
	// If starting day is outside the range	
	else if (Request.startingDay>6||Request.startingDay<0)
		return 4;
	// If number of days to stay is outside the range	
	else if (Request.stayDuration>7-Request.startingDay||Request.stayDuration<=0)
		return 5;	
	// If atleast one room is not available in the duration of the stay.	
	else if (checkvacantroom)
		return 2;
	// No error was found	
	return 0;					
}

/*
	Function Name: computeTotalRate
	Purpose of the Function: Computes the total rate of the stay.
	Pre-condition: Booking Request must be valid.
	Post-condition: Program will give the total price for the stay.
	Function's return value: Returns the total amount for the booking.
	List of parameters: int startingDay - Called by the function to know the lower limit of the loop.
						int stayDuration - Called by the function to know the higher limit of the loop.
						int roomGuests - Called by the function to know how many guests are there to accomodate in the room.
						float weekDayRate - Called by the function to be used in the formula to get the total rate.
						float weekEndRate - Called by the function to be used in the formula to get the total rate.
						float perGuestRate - Called by the function to be used in the formula to get the total rate.
*/

float computeTotalRate (int startingDay, int stayDuration, int RoomGuests, float weekDayRate, float weekEndRate, float perGuestRate)
{
	int i;
	float totalWeekDayRate=0, totalWeekEndRate=0;
	// For a booking with only one guest
	if (RoomGuests==1){
		for (i=startingDay; i<startingDay+stayDuration;i++){
			if (i<5)
				totalWeekDayRate+=weekDayRate;
			else if	(i>=5)
				totalWeekEndRate+=weekEndRate;
		}
	}
	// For a booking with more than one guest
	else{
		for (i=startingDay; i<startingDay+stayDuration;i++){
			if (i<5)
				totalWeekDayRate+=weekDayRate+(RoomGuests-1)*perGuestRate;
			else if	(i>=5)
				totalWeekEndRate+=weekEndRate+(RoomGuests-1)*perGuestRate;
		}
	}
	return totalWeekDayRate+totalWeekEndRate;
}

/*
	Function Name: convertCapital
	Purpose of the Function: Converts the string to all uppercase characters
	Pre-condition: Booking Request must be valid.
	Post-condition: Program will turn the string into its capitalized form.
	Function's return value: Changes the lastname into its uppercase form.
	List of parameter: String25 lastName - Called by the function to turn it into its uppercase form.
*/

void convertCapital (String25 lastName)
{
	int i;
	for (i=0;lastName[i]!='\0';i++)
		if (lastName[i]>='a'&&lastName[25]<='z')
			lastName[i]-=32;
}

/*
	Function Name: emailReport
	Purpose of the Function: Generates the email report.txt file
	Pre-condition: Booking Request must be valid.
	Post-condition: Email to the customer.
	Function's return value: None.
	List of parameters: struct bookingTag Request - Called by the function to know the details of the booking.
						float weekDayRate - Called by the function to be displayed in the email.
						float weekEndRate - Called by the function to be displayed in the email
						float perGuestRate - Called by the function to be displayed in the email.
						float totalRate - Called by the function to be displayed in the email.
						int bookNum - Called by the function to be displayed in the email.
						String40 filename - Called by the function to be used for the filename of the .txt file
*/

void emailReport (struct bookingTag Request, float weekdayRate, float weekendRate, float perGuestRate, float totalRate, int bookNum, String40 fileName)
{
	FILE *fptr;
	int i,weekdays=0,weekends=0;
	for (i=Request.startingDay;i<Request.startingDay+Request.stayDuration;i++){
		if (i<5)
			weekdays++;
		else if (i>=5)
			weekends++;	
	}
	String25 days[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
	fptr = fopen (fileName, "w");
	if (fptr !=NULL){
		fprintf (fptr,"Dear %s %s,\n\n",Request.lastName,Request.firstName);
		fprintf (fptr,"Thank you for booking your holiday with us.\n");
		fprintf (fptr,"Your booking reference number is: %d\n",bookNum);
		fprintf (fptr,"The email address for this booking: %s\n\n",Request.emailAddress);
		fprintf (fptr,"Booking Details:\n");
		fprintf (fptr,"\tRoom Code: %c\n",Request.roomCode);
		fprintf (fptr,"\tNumber of guests: %d\n",Request.guestNumber);
		fprintf (fptr,"\tFirst day of booking: %s\n",days[Request.startingDay]);
		fprintf (fptr,"\tNumber of days: %d\n",Request.stayDuration);
		fprintf (fptr,"\tNumber of weekday stay: %d\n",weekdays);
		fprintf (fptr,"\tNumber of weekend stay: %d\n\n",weekends);
		fprintf (fptr,"Fees:\n");
		fprintf (fptr,"\tWeekday room rate: %.2f\n",weekdayRate);
		fprintf (fptr,"\tWeekend room rate: %.2f\n",weekendRate);
		fprintf (fptr,"\tTotal room cost for weekday stay: %.2f\n",weekdayRate*weekdays);
		fprintf (fptr,"\tTotal room cost for weekend stay: %.2f\n",weekendRate*weekends);
		fprintf (fptr,"\tNumber of guests above the first: %d\n",Request.guestNumber-1);
		fprintf (fptr,"\tTotal per guest cost: %.2f\n",perGuestRate*Request.stayDuration);
		fprintf (fptr,"\tTotal guest cost for the stay: %.2f\n\n",perGuestRate*(Request.guestNumber-1)*Request.stayDuration);
		fprintf (fptr,"\tTotal: %.2f\n\n",totalRate);
		fprintf (fptr,"Have a nice day! Hope you have a pleasant stay.");
		fclose (fptr);
	}
	else
		fprintf(stdout, "ERROR: Cannot Create a file");
}

/*
	Function Name: bookProcessing
	Purpose of the Function: Accepts inputs from the user and stores the inputs into a bookingTag struct
	Pre-condition: validTxtFile must be equal to 1.
	Post-condition: Generates email to the customer and adds values to the final report.
	Function's return value: None.
	List of parameters: struct bookingTag Request - Called by the function to store the data from the inputs.
						struct roomTag rooms[] - Called by the function to get the room data.
						int totalroomcodes - Called by the function to know how many unique room codes are there.
						struct reportTag reports - Called by the function so that values will be added on later.
						struct weekBook availRooms[] - Called by the function to be compared with later on in the validation.
						int totalRooms - Called by the function to know how many rooms are there.
*/

void bookProcessing(struct bookingTag Requests, struct roomTag rooms[], int totalroomcodes,struct reportTag reports[], struct weekBook availRooms[], int totalRooms)
{
	String40 fileName;
	// dump is for storing the extra \n at  the end of the last input
	String25 dump;
	int bookNum=1, successNum=0, roomGuests, roomAmount, errornumber, roomcodenumber,weekdaysbooked,weekendsbooked,i,validEmail;
	// wdayR is weekday rate, wendR is weekend Rate, pguest is per guest rate
	float wdayR, wendR, pguest,totalRate;
	// This array is used to know what day the starting day lands on.
	String25 days[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
	do{
		validEmail=1;
		weekdaysbooked=0;
		weekendsbooked=0;
		printf ("\nEnter booking request #%d\n",bookNum);
		printf ("\tlast name: "); fgets(Requests.lastName,sizeof Requests.lastName,stdin);
		// Loop ends when empty string is entered in last name
		if (strcmp(Requests.lastName,"\n")==0)
			break;
		printf ("\tfirst name: "); fgets(Requests.firstName,sizeof Requests.firstName,stdin);
		// Can accept empty first name string
		if (strcmp(Requests.firstName,"\n")==0)
			strcpy (Requests.firstName,"");
		// Loop doesnt end until valid email is entered or empty string is entered.
		while(validEmail){	
		printf ("\temail address: "); fgets(Requests.emailAddress,sizeof Requests.emailAddress,stdin);
		// Can accept empty emailAddress string
		if (strcmp(Requests.emailAddress,"\n")==0){
			strcpy (Requests.emailAddress,"");
			validEmail=0;
		}
		// Validates email
		else	
			validEmail = validateEmail(Requests.emailAddress);	
		}
		printf ("\troom code: "); fscanf(stdin," %c",&Requests.roomCode);
		// Gets the details of the room if matching room code is found
		getRoomSpecs (rooms, Requests.roomCode, totalroomcodes, &roomGuests, &roomAmount,&wdayR, &wendR, &pguest, &roomcodenumber);
		printf ("\ttotal # of guests (max %d): ", roomGuests); fscanf (stdin,"%d",&Requests.guestNumber);
		printf ("\tstarting day: "); fscanf(stdin,"%d",&Requests.startingDay);
		printf ("\t# of days to stay (max %d): ", 7-Requests.startingDay); fscanf(stdin,"%d",&Requests.stayDuration);
		for (i=Requests.startingDay;i<Requests.startingDay+Requests.stayDuration;i++){
			// Counts weekdays booked
			if (i<5)
				weekdaysbooked++;
			// Counts weekends booked	
			else if (i>=5)
				weekendsbooked++;
		}
		//Validates the booking request
		errornumber = validateRoom (Requests, rooms,totalroomcodes,roomGuests,roomcodenumber,availRooms);
		switch (errornumber)
		{
			case 0:
				printf ("\nSuccessfully booked a room for %d guest/s for %d day/s of stay starting %s\n", Requests.guestNumber,Requests.stayDuration,days[Requests.startingDay]);
				totalRate=computeTotalRate (Requests.startingDay,Requests.stayDuration,Requests.guestNumber,wdayR,wendR,pguest);
				printf ("Total amount for this booking in PHP: %.2f\n",totalRate);
				successNum++;
				convertCapital (Requests.lastName);
				// strtok removes the \n from the lastName and the firstName strings
				strtok (Requests.lastName,"\n");
				strtok (Requests.firstName,"\n");
				// Stores the lastName and the bookNum into a single string.
				sprintf (fileName, "%s-%d.txt",Requests.lastName,bookNum);
				validEmail=1;
				// User needs to input a valid email if previous input was empty
				if (strcmp (Requests.emailAddress,"")==0)
					while (validEmail){
						printf ("Enter valid email address: "); fscanf (stdin, " %s",Requests.emailAddress);
						validEmail = validateEmail(Requests.emailAddress);
					}
				emailReport (Requests, wdayR,wendR,pguest,totalRate,bookNum,fileName);
				for (i=0;i<totalroomcodes;i++){
					// Adds booked days into the report
					if(Requests.roomCode==reports[i].roomCode){
						reports[i].weekdayBooked+=weekdaysbooked;
						reports[i].weekendBooked+=weekendsbooked;
					}
				}
				// Decrements the booked rooms from the available rooms.
				for (i=Requests.startingDay;i<Requests.startingDay+Requests.stayDuration;i++){
					if (i==0)
						availRooms[roomcodenumber].monday--;
					if (i==1)
						availRooms[roomcodenumber].tuesday--;
					if (i==2)
						availRooms[roomcodenumber].wednesday--;
					if (i==3)
						availRooms[roomcodenumber].thursday--;
					if (i==4)
						availRooms[roomcodenumber].friday--;
					if (i==5)
						availRooms[roomcodenumber].saturday--;
					if (i==6)
						availRooms[roomcodenumber].sunday--;					
				}
				
				// Reduces the total number of rooms once a room code has ran out of vacant rooms.
				if (availRooms[roomcodenumber].monday==0&&availRooms[roomcodenumber].tuesday==0&&availRooms[roomcodenumber].wednesday==0&&availRooms[roomcodenumber].thursday==0
					&&availRooms[roomcodenumber].friday==0&&availRooms[roomcodenumber].saturday==0&&availRooms[roomcodenumber].sunday==0){
						totalRooms-=rooms[roomcodenumber].roomsAmount;
						// The amount of rooms is equated into 0 to prevent it from decrementing again.
						rooms[roomcodenumber].roomsAmount=0;
					}
				break;
			case 1:
				printf ("\nSorry, I am unable to satisfy this request due to invalid room code\n");
				break;
			case 2:
				printf ("\nSorry, I am unable to satisfy this request due to room availability\n");
				break;
			case 3:
				printf ("\nSorry, I am unable to satisfy this request due to room capacity\n");
				break;
			case 4:
				printf ("\nSorry, I am unable to satisfy this request due to invalid starting day.\n");
				break;
			case 5:
				printf ("\nSorry, I am unable to satisfy this request due to invalid # of days to stay\n");
				break;
		}
		bookNum++;
		// Gets the \n from the stay duration input.
		fgets(dump,sizeof dump,stdin);
	}while (strcmp(Requests.lastName,"\n")!=0&&totalRooms);
	// Disregards the increment from the successful booking if loop is ended with empty string or running out of available rooms.
	bookNum--;
	if (!totalRooms)
		printf ("\nHotel is fully booked.");
	printf ("\nProcessed %d successful requests out of %d submitted requests.\n", successNum,bookNum);
	printf ("Generating management report.\n");
	printf ("Thank you for using the hotel reservation system.\n");
	printf ("Bye!\n");
}

/*
	Function Name: reportProcessing
	Purpose of the Function: Generates the final report
	Pre-condition: validTxtFile must be equal to 1.
	Post-condition: report.txt is generated.
	Function's return value: None.
	List of parameters: struct reportTag report[] - Called by the function to get the data that will be printed.
						int uniqueroomcodes - Called by the function to know how many unique room codes are there.
*/

void reportProcessing (struct reportTag report[],int uniqueroomcodes)
{
	FILE *fptr;
	int i;
	fptr = fopen("report.txt","w");
	if (fptr!=NULL){
		for (i=0;i<uniqueroomcodes;i++){
			report[i].occupancyRate=((report[i].weekdayBooked+report[i].weekendBooked*1.0)/(report[i].weekdayRooms+report[i].weekendRooms*1.0))*100;
			fprintf (fptr,"%d %c %d %d %d %d %.2f%%\n",report[i].roomCodeNumber,report[i].roomCode,report[i].weekdayRooms,report[i].weekdayBooked,report[i].weekendRooms,report[i].weekendBooked,report[i].occupancyRate);
		}
		fclose(fptr);
	} 
	else
		fprintf(stdout, "ERROR: File can not be created");
}



int main()
{
	int totalRooms,uniqueroomcodes,checkCodes,checkLetter,checkCapacity, checkAmount, checkRates, validTxtFile, fileFound;
	struct roomTag rooms[26];
	struct weekBook availRooms [26];
	struct reportTag reports[26];
	struct bookingTag bookings;
	printf ("Welcome to the hotel booking system.\n\n");
	getrooms (rooms,&uniqueroomcodes, &fileFound);
	if (fileFound){
		totalRooms = getTotalRooms(rooms,uniqueroomcodes);
		checkCodes = checkRoomCodes (rooms,uniqueroomcodes);
		checkLetter = checkIfLetter (rooms, uniqueroomcodes);
		checkCapacity = checkRoomCapacity (rooms, uniqueroomcodes);
		checkAmount = checkRoomAmounts (rooms, uniqueroomcodes);
		checkRates = checkRoomRates (rooms, uniqueroomcodes);
		displayAvailableRooms(totalRooms,checkCodes,checkLetter,checkCapacity, checkAmount, checkRates, &validTxtFile);	
		if (validTxtFile)
		{
			initializeVacantRooms (availRooms,rooms,uniqueroomcodes);
			initializeReport (reports,rooms,uniqueroomcodes);
			bookProcessing (bookings,rooms,uniqueroomcodes,reports,availRooms,totalRooms);
			reportProcessing (reports,uniqueroomcodes);	
		}
	}
	return 0;
	
	/*
	TEST CASES:
	Test Case based on given specs:
	Smith
	John
	johnsmith@example.email.com
	D
	4
	5
	2
	Taylor
	Sweeeft
	staylor@example.email.com
	K
	8
	3
	3
	Prince
	Diana
	wonderwoman@example.email.com
	A
	1
	0
	7
	Luthor
	Lex
	supervillain@example.email.com
	K
	6
	2
	2
	Luthor
	Lex
	supervillain@example.email.com
	C
	2
	2
	3
	Bond
	James
	agent007@example.email.com
	C
	2
	4
	3
	Kirk
	James
	captain@example.email.com
	C
	2
	3
	3
	Mouse
	Mickey
	MM90bithday@example.email.com
	D
	3
	3
	3
	Duck
	Donald
	dduck@example.email.com
	B
	2
	1
	3
	Danvers
	
	supergirl@superheroes.email.com
	F
	1
	1
	3
	Danvers
	
	
	A
	1
	5
	2
	supergirl@superheroes.email.com
	ThePooh
	Winnie
	
	D
	1
	7
	3
	ThePooh
	Winnie
	pooh@example.email.org
	D
	1
	5
	3
	
	Test Case for checking overlapping days:
	Karrot
	Kay
	Karr@gmail.com
	K
	1
	0
	6
	Klaw
	Kar
	KKlaw@yahoo.com
	K
	1
	0
	7
	Klaw
	Kar
	KKlaw@yahoo.com
	k
	1
	6
	1
	
	Test Case for validating email:
	Email
	Check
	a..@email.com
	A!!!B@email.com
	atsign@@email.com
	.startChar@email.com
	domain@email
	valid_Email@email.com
	A
	1
	0
	1
	
	Test Case for invalid values:
	Guest
	Check
	checkguest@email.com
	A
	2
	0
	2
	StartingDay
	Check
	checkstart@emali.com
	A
	1
	-1
	6
	stayDuration
	Check
	checkDuration@email.com
	A
	1
	0
	10
	
	Rooms samples attached with email.
	*/
}
