/* Question #1
The awards committee of Jah Love Ltd has decided to implement an education
assistance program for its employees. Each employee will be given a monthly
bonus based on the number of children they have attending school. The award is
calculated as follows: the number of points * the number of children * 500.
School                   Points     Degree                Bonus
 Primary                  1
 High                     2
 College                  4

a. Define an enumeration named School that will assign these values. [3 marks]
b. Define a variable of type School.                                 [2 marks]
c. Write the necessary C code that will allow the user to enter the school and
then display the school and the points received for that child. The total points
received by the employee should be calculated. [6 marks] 
d. Define a function called Bonus that will accept the points received and the
number of children the employee has, the function should then calculate and
display the bonus and final pay of the employee. Your definition should show the: 
i)prototype        [3 marks] 
ii)definition      [9 marks] 
iii) call          [2 marks]
*/

#include <stdlib.h>
#include <stdio.h>

enum School { Primary = 1, High = 2, College = 4 };
enum School Points;
int Bonus(int totPoints, int numChildren);

int main() {
  char schoolName[512];
  int schoolType;
  char childName[256];
  int totalPoints = 0;
  int numChildren = 0;
  int checker;

  system("cls");
  printf("Jah Love Ltd\n");
  printf("=============================================");
  while (1) {

    printf("\n\nEnter Child's Name: ");
    scanf("%[^\n]s", childName);

    numChildren++;
    getchar();

    printf("Enter School Name: ");
    scanf("%[^\n]s", schoolName);

    printf("Enter School Type:\n[1]\tPrimary\n[2]\tHigh\n[3]\tCollege\n");
    scanf("%d", &schoolType);
    getchar();

    if( schoolType == 1) totalPoints += Primary;
    else if( schoolType == 2) totalPoints += High;
    else if(schoolType == 3) totalPoints += College;

    printf("\n%s", schoolName);
    printf("\n%d", schoolType);

    do {
      system("cls");
      printf("------------------------------------\n");
      printf("Would you like to continue?\n\t[1]\tY/ [0]N\t\n");
      scanf("%d", &checker);
      printf("%d", checker);
      getchar();
      getchar();
    } while (checker != 1 && checker != 0);

    if(checker == 0)break;
    else if(checker == 1) continue;
  }
 printf("Employee Bonus Earned = $%d", Bonus(totalPoints,numChildren));
  return 0;
}

int Bonus(int totPoints, int numChildren) {
 return (totPoints * numChildren * 500);
}

float circumference[5][3];
void showValues(float array[5][3]);

int CalcCircum() {
  circumference[0][0] = 36.1;
  circumference[1][0] = 8.43;
  circumference[2][0] = 24.6;
  circumference[3][0] = 12.4;
  circumference[4][0] = 19.63;

  circumference[0][1] = 18.41;
  circumference[1][1] = 31.02;
  circumference[2][1] = 4.6;
  circumference[3][1] = 8.2;
  circumference[4][1] = 14.5;

  circumference[0][2] = 8.7;
  circumference[1][2] = 13.15;
  circumference[2][2] = 10.2;
  circumference[3][2] = 21.3;
  circumference[4][2] = 11.52;
  showValues(circumference);
  return 0;
}

void showValues(float array[5][3]) {
  int row = 0;
  int col = 0;
  for (col = 2; col > -1; col--) {
    for (row = 4; row > -1; row--) {
      printf("%0.2f\n\n", array[row][col]);
    }
    printf("\n\n");
  }
} 