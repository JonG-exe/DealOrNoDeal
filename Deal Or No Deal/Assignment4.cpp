#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <ctime>
#include <string>

// Function Prototypes
bool validOption(int option);
int numOptions();
void displayOptionArray();
void playGame();
int toInteger(char word[]);
void readData(char dataWord[], int intOption);
void runRound(int round, int numCases, int intOption, bool started);
bool unopenedCase(int caseOption, int intOption);
void removeCase(int caseOption, int intOption);
int randNum(int upperbound);
int randNumLowerLimit(int upperbound);
int getRandomCaseValue(int intOption);
float bankOffer(int intOption, int currentRound, int totalNumRounds);
int offerDecision(char offerChoice[], float bankOfferValue);
void populateCase(int caseName[], int size);
bool valueInCase(int value, int openedCases[], int intOption);
void displayCases(int intOption);
int talkToBank(int bank);
void removeCase(int caseIndex, int caseOption, int intOption);
void populate(int caseDisplay[], int intOption);
void runLastRound(int intOption, int firstCase);
bool caseNumWithinRange(int caseNum, int intOption);
void generateRandomArthimetic();
void throwDice();
void askMultipleChoice();
int twist();
void showLogo();
void showMoneyBoard(int intOption);
int counterOffer(int bankOffer);
int throwDiceGetNum();

// Global Variables & Arrays
int optionArray[] = {16, 18, 20, 22, 24, 26};
int cases[26];
int rounds[20];
int openedCases[26];
int caseDisplay[26];

using namespace std;

// Main
int main(){
	
	srand((time(0)));

    playGame();
	
	return 0;
}

int randNum(int upperbound){
	int num = 1 + rand() % (upperbound - 1) + 1;
	
	return num;
}

int randNumLowerLimit(int upperbound){
	int num = 0 + rand() % (upperbound - 0) + 0;
	
	return num;
}

void playGame(){
	
	showLogo();
	
	// Variables & Arrays

	char option[2];
	bool firstRun = true;
	int firstCase = 0;
	int flag = true;
	int currentRound = 0;
	char dataWord[] = "Data";
	
	// populates each array's index with zero
	populateCase(openedCases, 26);
	populateCase(caseDisplay, 26);
	populateCase(cases, 26);
	populateCase(rounds, 20);
	
	displayOptionArray(); // displays the board of cases
	cout<<"\nChoose an option: ";
	cin>>option;
	
	while(!validOption(toInteger(option)))
	{
		cout<<"\n\tSorry, that's not an available option."<<endl;
		cout<<"\nChoose an option: ";
		cin>>option;	
	}
	
	cout<<endl;
		
	strcat(dataWord, option); // concatenates the char option to dataWord
	strcat(dataWord, ".txt"); // concatenates '.txt' to dataWord
			
	int intOption = toInteger(option);
	readData(dataWord, intOption);
	
	while(flag){
		
		if(validOption(intOption)){
			
			if(firstRun) populate(caseDisplay, intOption); // Populates caseDisplay
			
			int caseOption;
			char offerChoice[10];
			float bankOfferValue;
			int bankDecision = 0;
			int lastRound = rounds[currentRound] - 1;
			
			runRound(currentRound, rounds[currentRound], intOption, firstRun);
			
			for(int i = 0; i < rounds[currentRound]; i++)
			{
				if(firstRun)
				{
					cout<<"\nChoose your FIRST case: ";
					cin>>caseOption;
				}
				else if(currentRound == rounds[0]) // this 'else if' is triggered if it's the last round
				{
					runLastRound(intOption, firstCase);
					flag = false;
					break;
				}
				else
				{
					cout<<"\n\nChoose a case: ";
					cin>>caseOption;
				}
				
				if(unopenedCase(caseOption, intOption))
				{
					if(caseNumWithinRange(caseOption, intOption))
					{
						int caseValue = getRandomCaseValue(intOption);
						
						if(!firstRun) // executes for every other run besides the first run
						{
							showMoneyBoard(intOption);
							cout<<"The value of the case choosen: $"<<cases[caseValue]<<endl;
							removeCase(caseValue, caseOption, intOption);
						}
						else // this is the first round if this 'else' is triggered
						{
							firstCase = caseOption;
							firstRun = false;
							
							cout<<"\n\n\t>> Case "<<caseOption<<" will be set aside <<\n"<<endl;
							removeCase(caseValue, caseOption, intOption);
							break;
						}
						
						cout<<endl;
						displayCases(intOption);
					}
					
				}
				else if(caseOption == firstCase)
				{
					cout<<"\n\tThe first case (case "<<firstCase<<"), cannot be opened until the end.\n";
				}
				else{
					
					cout<<"\n\tThat case was already opened. Choose again...\n";
					i = i - 1;
				}
			}
			
			if(currentRound != 0 && currentRound < rounds[0])
			{
				int bank = bankOffer(intOption, currentRound, rounds[0]);
				int decision = talkToBank(bank);
				
				if(decision == -1) // Ends the game;
				{
					cout<<"\n\n\t\t>> Game Ended <<";
					break;
				}
			}
			
			currentRound++;
		}
		else
		{
			cout<<"\n\tInvalid Option Choosen. Please Choose Again.\n";
		}
	}
}

bool caseNumWithinRange(int caseNum, int intOption)
{
	if(caseNum > 0 && caseNum <= intOption) return true;
	
	return false;
}

void removeCase(int caseIndex, int caseOption, int intOption){

	cases[caseIndex] = 0;
	caseDisplay[caseOption-1] = 0;
	
	for(int i = 0; i < intOption; i++)
	{
		if(openedCases[i] == 0)
		{
			openedCases[i] = caseOption;
			break;
		}
	}
	
}

int talkToBank(int bank){
	
	int flag = 0;
	char decision[10];
	int tries = 3;
	
	for(int i = 0; i < 3; i++)
	{
		cout<<"\nThe bank is offering $"<<bank<<"."<<endl;
		
		if(throwDiceGetNum() == 6 || throwDiceGetNum() == 3)
		{
			flag = counterOffer(bank);
		}
		else{
			cout<<"Deal Or No Deal? ";
			cin.ignore();
			cin.getline(decision, 10);
		}
		
		if(flag == -1) 
		{
			return -1;
		}
		else
		{
			return 1;
		}
		
		if(strcmp(decision, "Deal") == 0 || strcmp(decision, "deal") == 0)
		{
			cout<<"\n\tCongratulations! You're going home with $"<<bank<<"!"<<endl;
			return -1;
		}
		else if(strcmp(decision, "No Deal") == 0 || strcmp(decision, "no deal") == 0)
		{
			return 1;
		}
		else{
			cout<<"\nSorry, that's Invalid Input."<<endl;
			cout<<"("<<tries<<")"<<" more tries until automatic 'No Deal'"<<endl;
			
			tries--;
			
			if(tries == 0)
			{
				cout<<"\n\tAutomatic 'No Deal' applied."<<endl;
			}
		}
	}

	return 1;
}

float bankOffer(int intOption, int currentRound, int totalNumRounds){
	
	int totalValue = 0;
	int numUnopenedCases = 0;
	float averageValue = 0.0;
	float offer = 0.0;
	
	for(int i = 0; i < intOption; i++){
		
		if(cases[i] != 0)
		{
			totalValue = totalValue + cases[i];
			numUnopenedCases++;
		}
	}
	
	averageValue = (totalValue / numUnopenedCases);
	
	float findRounds = ((currentRound + 0.0) / (totalNumRounds + 0.0));
	
	offer = averageValue * findRounds;
	
	return offer;
}

int getRandomCaseValue(int intOption){
	
	int value = randNumLowerLimit(intOption);
	bool flag = true;
	
	while(valueInCase(value, openedCases, intOption) || flag == true)
	{
		value = randNumLowerLimit(intOption);
		
		if(cases[value] != 0)
		{
			//cout<<"GetDone"<<endl;
			return value;
		}
	}

	return value;
}

bool valueInCase(int value, int openedCases[], int intOption){
	
	for(int i = 0; i < intOption; i++)
	{
		if(openedCases[i] == value)
		{
			return true;
		}
	}
	return false;
}

void populateCase(int caseName[], int size){ // the size is the same as 'intOption'

	for(int i = 0; i < size; i++){
		caseName[i] = 0;
	}
}

bool unopenedCase(int caseOption, int intOption){
	
	for(int i = 0; i < intOption; i++){
		
		if(openedCases[i] == caseOption && caseOption != 0) return false;
	}
	return true;
}

void runRound(int round, int numCases, int intOption, bool started){
	
	if(started == false)
	{
		cout<<"\n>> We are in Round "<<round<<".\n";
		cout<<">> You must choose "<<numCases<<" cases in this round.\n"<<endl;
	}
	
	for(int i = 0; i < intOption; i++)
	{		
		
		if(caseDisplay[i] == 0){
			
			cout<<" "<<"   ";
		}
		else if(caseDisplay[i] < 10){
			
			cout<<caseDisplay[i]<<"   ";
		}
		else{
			cout<<caseDisplay[i]<<"  ";
		}
		
		if((i+1) % 5 == 0) cout<<endl;
	}
	
	cout<<endl;
}

void displayCases(int intOption)
{
	for(int i = 0; i < intOption; i++)
	{
		if(caseDisplay[i] == 0)
		{
			cout<<" "<<"   ";
		}
		else if(caseDisplay[i] < 10)
		{
			cout<<caseDisplay[i]<<"   ";
		}
		else{
			cout<<caseDisplay[i]<<"  ";
		}
		
		if((i+1) % 5 == 0) cout<<endl;
	}
}

bool validOption(int option){
	for(int i = 0; i < 6; i++){
		if(option == optionArray[i]){
			return true;
		}
	}
	
	return false;
}

// reads data into the respective global arrays (cases, rounds)
void readData(char dataWord[], int intOption) 
{
	
	ifstream in;
	in.open(dataWord);
	
	int i = 0; 
	
	in>>cases[i];
	i++;
		
	while(i != intOption){
		
		in>>cases[i];
		i++;
	}
	
	// this section gets all the rounds left in the game
	
	i = 0;
	in>>rounds[i];
	i++;
	
	while(!in.eof()) {
		in>>rounds[i];
		i++;
	}
	
	rounds[i] = -1;
	
}

void displayOptionArray() // displays the Data(x).txt options to the screen
{
	
	cout<<"\n\t";
	
	for(int i = 0; i < 6; i++){
		if(optionArray[i] == 0){
			cout<<" ";
		}
		else if(i < 3 || i > 3){
			cout<<optionArray[i]<<" ";
		}
		else{
			cout<<"\n\t";
			cout<<optionArray[i]<<" ";
		}
	}
	
	cout<<endl;
}

int toInteger(char word[]) // converts a c-string to an integer
{
	
	int i = 0;
	int count = 0;
	
	while(word[i] != '\0'){
		count = (count + (word[i] - '0'));
		
		if(word[i+1] != '\0'){
			count = count * 10;
		}

		i++;
	}
	
	return count;
}

int offerDecision(char offerChoice[], float bankOfferValue){
	
	int wordLength = strlen(offerChoice);
	char choice[wordLength];
	strcpy(choice, offerChoice);
	
	if(strcmp(choice, "Deal") == 0){
		cout<<"\n\tCongratulations, you're going home with $"<<bankOfferValue<<endl;
		return -1;
	}
	else if(strcmp(choice, "No Deal") == 0)
	{
		return 1;
	}
	else{
		cout<<"Please enter 'Deal' or 'No Deal'. "<<endl;
	}
}

void populate(int caseDisplay[], int intOption)
{
	for(int i = 0; i < intOption; i++)
	{
		caseDisplay[i] = i+1;
	}
}

void runLastRound(int intOption, int firstCase)
{
	int lastChoice;
	
	cout<<"-->> You have two options left. You can choose the last case on the board, or...\n";
	cout<<"...you can choose the first case that was set aside (case "<<firstCase<<"). Goodluck!\n"<<endl;
	
	cout<<"Choose one of the two possible cases: ";
	cin>>lastChoice;

//-------------------------------------------------------------------------------------------------

	int caseValue = getRandomCaseValue(intOption);

	cout<<"\n\t   The value of the case: $"<<cases[caseValue]<<endl;
	cout<<"\n\tCongratulations! Your winnings: $"<<cases[caseValue]<<".00"<<endl;
	cout<<"\n\n\t\t>> Game Ended <<\n";
	removeCase(caseValue, lastChoice, intOption);
	
	cout<<endl;
	
}

int twist()
{
	int question = randNum(3);
	int alreadyAsked[3];
	bool questionAsked = false;
	
	populate(alreadyAsked, 3);
	
	while(questionAsked == false)
	{
		if(question == 1 && alreadyAsked[0] == 0)
		{
			for(int i = 0; i < 3; i++)
			{
				generateRandomArthimetic();
			}
			alreadyAsked[0] = 1;
			questionAsked = true;
		}
		// make a choice from options that have diff. propabilities of occuring
		if(question == 2 && alreadyAsked[1] == 0) 
		{
			for(int i = 0; i < 3; i++)
			{
				throwDice();
			}
			alreadyAsked[1] = 2;
			questionAsked = true;
		}
		// asking a series of multiple choice questions
		if(question == 3 && alreadyAsked[2] == 0)
		{
			askMultipleChoice();
			
			alreadyAsked[2] = 3;
			questionAsked = true;
		}
	}
}

void generateRandomArthimetic()
{
	int num1 = randNum(100000);
	int num2 = randNum(100000);
	int playerAnswer;
	
	int random = randNum(2);
	
	if(random == 1)
	{
		int sum = num1 + num2;
		
		cout<<"What is the sum of "<<num1<<" + "<<num2<<"?"<<endl;
		cout<<"Answer: ";
		cin>>playerAnswer;
		
		if(playerAnswer == sum)
		{
			cout<<"\n\tCongratulations! That answer is Indeed Correct!"<<endl;
		}
		else{
			cout<<"\n\tOooh...Not quite right."<<endl;
		}
	}
	else if(random == 2){
		
		int multipliedTotal = num1 * num2;
		
		cout<<"What is "<<num1<<" * "<<num2<<"?"<<endl;
		cout<<"Answer: ";
		cin>>playerAnswer;
		
		if(playerAnswer == multipliedTotal)
		{
			cout<<"\n\tCongratulations! That answer is Indeed Correct!"<<endl;
		}
		else{
			cout<<"\n\tOooh...Not quite right."<<endl;
		}
	}
}

int throwDiceGetNum()
{
	int randThrow = randNum(6);
	
	return randThrow;
}

void throwDice()
{
	int num = 0;
	int randThrow = randNum(6);
	
	cout<<"I'm about to throw a dice. Pick a number between 1 & 6: ";
	cin>>num;
	
	if(num == randThrow)
	{
		cout<<"\n\tCongrats! Are you sure you're not a wicca!?"<<endl;
	}
	else{
		cout<<"\n\tNope. I'm afraid you missed the mark this round."<<endl;
	}
}

void askMultipleChoice()
{
	char answer[20];

// MC Question 1
	cout<<"What is the average lifespan of a human?";
	cin>>answer;
	
	if(answer == "79")
	{
		cout<<"\n\tAt this point you must cheating BECAUSE THAT'S ABSOLUTELY CORRECT! =)"<<endl;
	}
	else{
		cout<<"\n\tWeh Weh Weh. Unfortunately not right."<<endl;
	}

// MC Question 2
	cout<<"Does sunlight have a weight/mass? (yes/no)";
	cin>>answer;
	
	if(strcmp(answer, "yes") || strcmp(answer, "y"))
	{
		cout<<"\n\tYup Yup Yup. Correct!"<<endl;
	}
	else{
		cout<<"\n\tUnfortunately not right."<<endl;
	}
	
// MC Question 3

	cout<<"Where is the center of the universe?";
	cin>>answer;
	
	if(strcmp(answer, "everywhere") || strcmp(answer, "Everywhere"))
	{
		cout<<"\n\tOkay Sir Einstein! Correct! "<<endl;
	}
	else{
		cout<<"\n\tNope...not even close."<<endl;
	}
}

// prints the logo at the top of the screen
void showLogo() 
{
	cout<<"    DEAL OR | NO DEAL"<<endl;
}

void showMoneyBoard(int intOption)
{
	cout<<"\n\t   -------------\n";
	cout<<"\t  | Money Board |\n";
	cout<<"\t   -------------\n\n";
	
	for(int i = 0; i < intOption; i++)
	{
		/*
		if(i != 0 && i >= (intOption / 2))
		{
			cout<<"\n\t\t\t";
		}
		*/
		
		
		if(cases[i] == 0)
		{
			cout<<"\t\t  -"<<"  "<<endl;
		}
		else{
			cout<<"\t\t"<<cases[i]<<"  "<<endl;
		}
	}
	
	cout<<endl;	
}

int counterOffer(int bankOffer)
{
	float playerOffer = 0;
	
	cout<<"\n\t\tThis is Deal Or No Deal with a twist. "<<endl;
	cout<<"\n\tYou are allowed to make a counter offer with the banker. "<<endl;
	cout<<"\n\t\tIf the banker likes your offer, he will accept. "<<endl;
	cout<<"\n\t\t\tIf not, he'll decline. "<<endl;
	
	cout<<"\nMake the banker a counteroffer (integer) or decline (-1): ";
	cin>>playerOffer;
	
	if(playerOffer <= -1) 
	{
		return 1;
	}
	
	int threeSideDice = randNum(3);
	
	if(threeSideDice == 1 && playerOffer <= (bankOffer + (bankOffer * 0.15)))
	{
		cout<<"\n\t\t   The bank has accepted your offer!"<<endl;
		cout<<"\n\t\tCongratulations! You're going home with $"<<playerOffer<<"!"<<endl;
	}
	else if(threeSideDice == 2 || playerOffer <= (bankOffer + (bankOffer * 0.15)))
	{
		cout<<"\n\t\t   The bank has accepted your offer!"<<endl;
		cout<<"\n\t\tCongratulations! You're going home with $"<<playerOffer<<"!"<<endl;
	}
	else if(threeSideDice == 3 && playerOffer <= (bankOffer + (bankOffer * 0.15)))
	{
		cout<<"\n\t\t   The bank has accepted your offer!"<<endl;
		cout<<"\n\t\tCongratulations! You're going home with $"<<playerOffer<<"!"<<endl;
	}
	else{
		cout<<"Unfortunately the bank has declined your offer."<<endl;
		return 1;
	}
	
	return -1;
}





