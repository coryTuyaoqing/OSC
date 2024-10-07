#include <stdio.h>
#include <string.h>

int isUpperCase(char* string);
void toLowwerCase(char* string);

int main(int argc, char* argv[]){
	int MAX = 20;
	char first[MAX];
	char second[MAX];
	char str[MAX];
	char name[MAX];

	//use scanf to read first and second
	printf("your first name is: ");
	scanf("%s", first);
	printf("your second name is: ");
	scanf("%s", second);

	//conert second to all upper canse chars
	//and store the result in str
	if(!isUpperCase(second)){
		for(int i=0; second[i]; i++){
			str[i] = second[i] - 0x20;
		}
	}
	printf("%s\n", str);

	//apply strcmp on second and str
	printf("strcmp(str, second) = %d\n", strcmp(str, second));

	//concatenate first and second name into name
	int i;
	for(i=0; first[i]; i++){
		name[i] = first[i];
	}
	name[i] = ' ';
	i++;
	int j;
	for(j=0; second[j]; j++){
		name[i+j] = second[j];
	}
	name[i+j] = '\0';
	printf("name is %s\n", name);

	printf("another way to concatenate.\n");
	strcpy(name, first);
	strcat(name, " ");
	strcat(name, second);
	printf("name is %s\n", name);

	//read birth day in an int
	int year = 0;
	printf("what's your birth year?\n");
	scanf("%d", &year);

	//use snprintf to concatenate first second and year
	snprintf(name, MAX, "%s %s %d", first, second, year);
	printf("name is %s\n", name);

	//use sscanf to read from the name the first and second and year
	int result = sscanf(name, "%s %s %d", first, second, &year);
	if(result == 3){
		printf("%s\n", first);
		printf("%s\n", second);
		printf("%d\n", year);
	}

	return 0;
}

int isUpperCase(char* string){
	int i=0;
	while(string[i]){
		if(string[i] >= 0x5b)
			return 0;
	}
	return 1;
}

