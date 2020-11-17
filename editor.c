#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct element {
	struct element *prev;
	char litera;
	struct element *next;
};
typedef struct element nod;

int cursor,nr_elemente=0;
nod *cap_lista,*p,*p1,*p2;

int char_to_int (char c);
int value_of_function(char *String);
void move(int moves);
void insert_String(char *String);
void delete_int(int nr_elemente_de_sters);
void copy(int nr_elemente_de_copiat, char **String);
void printfCursor();

void main (int argc, char *argv[]) {

	int i, j, nr_comenzi;
	char **tabel_comenzi, *String;
	FILE *fisier, *operatii, *rezultat;
	nod *aux;

	fisier = fopen(argv[1],"r");
	if ( fisier == NULL){
		printf("Deschiderea fisierului nu a reusit");
		exit(1);
	} 

	aux = (nod*)malloc(sizeof(nod));	
	if (aux == NULL){
		printf("Prima alocare nereusita");
		exit(1);
	}

	fscanf(fisier, "%c", &(aux->litera));

	aux->next = NULL; //devine si ultimul element
	aux->prev = NULL; // prev pentru cap_lista este NLUL

	p = aux; 
	cap_lista = aux;
	nr_elemente = 1;
	i=0;

	// Citim celelalte elemente si memoram cate sunt in nr_elemente
	while( !feof(fisier) ){ // pana gaseste end of file (EOF)

		aux = (nod*)malloc(sizeof(nod));   
		if (aux == NULL){
			printf("Alocarea %d nereusita",i);
			exit(1);
		}  // aloca un nou nod (auxiliar, q) pentru completarea listei

		fscanf(fisier,"%c",&(aux->litera)); // citim caracterul 

		if( (aux->litera)=='\n' && (aux->litera)== '\r' ){
			p->next = NULL;
			break;
		}

		nr_elemente++; // nr_elemente se incrementeaza dupa ce a fost citit aux->litera

		aux->prev = p;  // p care initial este cap de lista  

		p->next = aux; // se face legatura intre aux si p

		aux->next = NULL; //aux devine si ultimul element al listei

		p = aux; // p devine q pentru ca procesul sa se poata repeta (se aloca un nou q iar apoi se face din nou conexiunea intre p si q)
	} 	 

	p = p->prev->prev;
	p->next = NULL;
	nr_elemente -= 2;



	// Dupa ce am creat si citit lista deschidem fisierul de intrare operatii.in
	operatii = fopen(argv[2],"r"); 
		
	if (operatii == NULL){
		printf("Fisierul cu operatii nu a fost deschis");
		exit(1);
	}

	//Citim numarul de comenzi si alocam memorie pentru nr_comenzi comenzi primite		
	fscanf(operatii,"%d\n",&nr_comenzi);		
	tabel_comenzi = (char **)malloc(nr_comenzi*sizeof(char*));
			
	// Citim si comenzile alocandu-i fiecareia spatiul necesar in vectorul de stringuri tabel_comenzi

	String = (char*)malloc(25); 

	for (i = 0; i < nr_comenzi; i++){			
		
		fgets(String,25,operatii);
			
	// din cauza functiei fgets are interpreteaza /n ca pe un input valid a fost nevoie sa-l "peticesc"
			
		if (String[strlen(String) - 1] == '\n')		
			String[strlen(String) - 1] = '\0'; 
		
		tabel_comenzi[i] = (char*)malloc(strlen(String));	
			strcpy(tabel_comenzi[i],String);	
	} 
	free(String);



	// RULAM COMENZILE:
	p1 = NULL;
	p2 = cap_lista;

	for (i = 0; i < nr_comenzi; i++){
		if ( strstr(tabel_comenzi[i],"move") != NULL ){
			if (tabel_comenzi[i][5] == '-')
				move((-1)*value_of_function(tabel_comenzi[i]) );
			else
				move(value_of_function(tabel_comenzi[i]));
		}
		
		if ( strstr(tabel_comenzi[i],"insert") != NULL ){
			insert_String(tabel_comenzi[i]);
		}
	
		if (strstr(tabel_comenzi[i],"del") != NULL){
			delete_int(value_of_function(tabel_comenzi[i]));
		}

		if (strstr(tabel_comenzi[i],"copy") != NULL){
			String = (char*)malloc(25);
			copy(value_of_function(tabel_comenzi[i]),&String);
		}
	}
	
	puts(String);

	rezultat = fopen(argv[3],"w");
	if ( rezultat == NULL){
		printf("Fisierul rezultat nu a fost deschis");
		exit(1);
	}//Deschiderea fisierului rezultat.out

// Parcurgem lista pentru verificarea citirii si o scriem in rezultat.out:

	p = cap_lista;
	i = 0;

	while (i != nr_elemente){
		fprintf(rezultat,"%c",p->litera);		
		p = p->next;
		i++;		
	}
	
	int err_date = fclose(fisier);
	if (err_date != 0) {
		printf("Fisierul date.in nu a fost inchis");
		exit(1);
	}
	int err_operatii = fclose(operatii);
	if (err_operatii !=0){
		printf("Fisierul operatii.in nu a fost inchis");
		exit(1);
	}
	int err_rezultat = fclose(rezultat);
	if( err_rezultat != 0){
		printf("Fisierul rezultat nu a putut fi inchis");
		exit(1);
	}
}

int char_to_int (char c){
	int aux;

	aux = (int)(c);
	if( (aux >= 48) && (aux <= 57) )
		return aux - 48;
	else 
		return -1;
} // functia returneaza -1 pentru caracter tip "litera" si valorea ca intreg daca este aflanumeric

int value_of_function(char *c){
	int i,suma=0;

	for(i = 0; i < strlen(c);i++)		
		if ( char_to_int(c[i]) != -1 ){
			suma = suma*10 + char_to_int(c[i]);
		}
			
	return suma;
} // gaseste pentru fiecare comanda care este "numarul" comenzii si il returneaza ca intreg

void move(int moves){

	int i=1;

	if(moves > 0){
		if(cursor + moves >= nr_elemente)
			cursor = nr_elemente;
		else 
	 		cursor += moves;
		while ( (p2 != NULL) && (i <= moves) ){
			if( (p1 == NULL) && (p2 != NULL) ){
				p1 = p2;
				p2 = p2->next;
				i++;
			}
			else{
				p1 = p1->next;
				p2 = p2->next;
				i++;
			}			
		}
	}
	
	if(moves < 0){
		if (cursor + moves <= 0)
			cursor = 0;
		else	
 			cursor += moves;
		while ( (p1 != NULL) && (i <= (-1)*moves)){
			if ((p1 != NULL) && (p2 == NULL)){
				p2 = p1;
				p1 = p1->prev;
				i++;
			}	
			else{
				p1 = p1->prev;
				p2 = p2->prev;
				i++;
			}			
		}
	}
}

void insert_String(char *String){

	nod *de_inserat, *final_lista;	
	int i;

	final_lista = (nod*)malloc(sizeof(nod));

	if( ( cursor == 0 ) && ( (p1 == NULL) && (p2 != NULL) ) ){
 		cap_lista = p2;

		for (i = strlen(String) -1 ; i >= 7; i--){
			de_inserat = (nod*)malloc(sizeof(nod));
			de_inserat->litera = String[i];
			de_inserat->prev = NULL;
			de_inserat->next = cap_lista;
			cap_lista->prev = de_inserat;

			if (i == strlen(String)-1 ){
				p1 = de_inserat;
				p2 = cap_lista;
			}
				cap_lista = de_inserat;
		}
			
		nr_elemente += strlen(String)-7;
		cursor += strlen(String)-7;
		
		return;
	}

		
	if( ( cursor == nr_elemente ) && ( (p1 != NULL) && (p2 == NULL) ) ){				
		final_lista = p1;

		for (i = 7; i <= strlen(String)-1; i++){
			de_inserat = (nod*)malloc(sizeof(nod));
			de_inserat->litera = String[i];
			final_lista->next = de_inserat;
			de_inserat->prev = final_lista;
			de_inserat->next = NULL;
			final_lista = de_inserat;
		}
		
		p1 = de_inserat;
		nr_elemente += strlen(String)-7;
		cursor += strlen(String)-7;

		return;
	}
			

	if( ( ( p1 != NULL) && ( p2 != NULL) ) && (p1->next == p2) ){

		for (i = 7; i <= strlen(String)-1; i++){
			de_inserat = (nod*)malloc(sizeof(nod));
			de_inserat->litera = String[i];
			p1->next = de_inserat;
			de_inserat->prev = p1;
			de_inserat->next = p2;
			p2->prev = de_inserat;
			p1 = de_inserat;
		}
		nr_elemente += strlen(String)-7;
		cursor += strlen(String)-7;
		return;
	}
}

void delete_int(int nr_elemente_de_sters){

	int i=0;
	nod *q;
	q = (nod*)malloc(sizeof(nod));

	if ( p1 == NULL){
		while( ( p2 != NULL ) && ( i < nr_elemente_de_sters ) ){
			p2 = p2->next;
			p2->prev = NULL;
			cap_lista = p2;
			i++;
		}
	}

	else
		while ( (p2 != NULL) && (i < nr_elemente_de_sters) ){
			q = p2->next;
			p1->next = q;
			if (q->next != NULL) 
				q->prev = p1;
			p2 = q;
			i++;
		}

	nr_elemente -= i; 
}

void copy(int nr_elemente_de_copiat, char **String){

	int i=0;
	nod *q;
	q = (nod*)malloc(sizeof(nod));

	if ( p1 == NULL){
		q = p2;
		while( ( q != NULL ) && ( i < nr_elemente_de_copiat ) ){
			*String[i] = q->litera;
			q = q->next;
			i++;
		}
		*String[i+1]='\0';		
	}
	else {
		q = p2;
		while ( (q != NULL) && (i < nr_elemente_de_copiat) ){
			*String[i] = q->litera;
			q = q->next;
			i++;
		}
		*String[i+1]='\0';
	}
}

void printfCursor(){
	if( (p1 == NULL) && (p2 != NULL) ){
		printf(" Null|%c : %d\n",p2->litera,cursor);
	}
	if( (p1 != NULL) && (p2 == NULL) ){
		printf(" %c|Null : %d\n",p1->litera,cursor);
	}
	else printf(" %c|%c : %d \n",p1->litera,p2->litera,cursor);

	printf("\n");
}