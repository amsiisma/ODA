
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct brukerkonto
{
        char brukernavn[100];
        char fornavn[250];
        char etternavn[100];
};

typedef struct brukerkonto konto_t;

int les_data (konto_t *brukertabell);
void skriv_data (konto_t *brukertabell, char arg, int numUsers);
void skriv_data_env(konto_t *brukertabell, char envVar, int numUsers);


int main(int argc, char *argv[])
{

	konto_t brukertabell[200];
	les_data(brukertabell);
	int numUsers = les_data(brukertabell);


/*envVar skal holde characteren som skal sendes videre dersom en av miljøvariablene eksisterer,
velger å gjøre om fra peker til variabel for å gjøre det enklere videre*/

	char* envVariable;
	char envVar = *envVariable;


/* hvis bruker har laget miljøvariabel vil envVar bli satt til en tilsvarende character som brukes i skriv_data_env metoden*/

	if(getenv("FORNAVN") != NULL)
		envVar = 'f';

	else if(getenv("ETTERNAVN") != NULL)
		envVar = 'e';

	else if(getenv("BRUKERNAVN") != NULL)
		envVar = 'b';

/* Miljøvariabler prioriteres */

	if(envVar == 'f' || envVar == 'e' || envVar == 'b')
                skriv_data_env(brukertabell, envVar, numUsers);

        else if (argc > 1)
	{
		char *arg1 = argv[1];
         	char arg = *arg1;
		skriv_data(brukertabell, arg, numUsers);
	}


	else //default skriver ut etternavn først
	{
		envVar = 'e';
		skriv_data_env(brukertabell, envVar, numUsers);
	}
  	//else utfør default metode der envVar settes til e og bruker metoden med miljøvariabler

        return 0;
}


int les_data (konto_t *brukertabell)
{

        char *txt = NULL; //linjepeker
        size_t len = 0; //settes til 0 for å unngå random tall
        int index = 0;  //skal brukes til å øke brukertabellindeks
	int counter = 0;

        FILE *file; //skal peke på /etc/passwd filen
        file = fopen("/etc/passwd", "r"); //åpner filen, og read

        if(file == NULL) //hvis filen ikke eksisterer
                exit(EXIT_FAILURE);

        while( -1 != (len = getline ( &txt, &len, file ) ) ) //itererer gjennom fila /etc/passwd
        {
                char *ptr = NULL;
                char *oppdeltOrd;
                char *name = malloc(250);

                int uid=0;
                int i = -1; //brukes til å iterere, settes til -1 for å unngå krøll i for-loop/switch
                konto_t bruker; //lager variabel av konto_t

                for(oppdeltOrd = strtok_r(txt, ":,", &ptr); oppdeltOrd != NULL; oppdeltOrd = strtok_r(NULL, ":,", &ptr)) //
                {
			i++;
                        switch(i)
                        {
                                case 0:
                                        strncpy(bruker.brukernavn, oppdeltOrd, 100); //når i = 0 kopieres brukernavn til bruker.brukernavn, 100 char
                                        break;
                                case 2:
                                        uid = atoi(oppdeltOrd); //når i = 2 vil uid variabel settes til int formen av uid nummeret(string)
                                        break;
				case 4:
					strncpy(name, oppdeltOrd, 250); //når i = 4 vil hele  navnet sendes videre til neste bit
					break;
                                case 6:         //når i = 6 står man på siste del av linjen, og all informasjon er samlet inn, nå skal den lagres i tabell
                                        if(uid >= 1000) //tar bare med de som har uid over eller lik 1000
					{
						int j = 0;
						char *temp[1024]; //midlertidig tabell  der navnene legges
                                                char *token = strtok(name, " "); //skal dele opp 'name' der hele navnet ligger

						while(token != NULL)
						{
							temp[j] = token; //fornavn og etternavn lagres i midlertidig array
							token = strtok(NULL, " "); //resettes
							j++;
						}

						strncpy(bruker.etternavn, temp[j-1], 100); //etternavn hentes fra midlertidig array og lagres

						char first_name[250];
                                                int k =0;

                                                for (k = 0; k < j-1; k++)
						{
							strcpy(first_name, temp[0]);	//kopierer fornavn til midlertidig tabell fra temp
							strcat(first_name, " "); //concat, legger til mellomrom mellom navnene

							//hvis de ikke har mellomnavn
							if(j == 2)
								break;

							//ett mellomnavn
							if(j == 3)
							{
								strcat(first_name, temp[1]); //legger til mellomnavn med fornavn
								break;
							}

							//to mellomnavn
							if(j == 4)
							{
                     				       		strcat(first_name, temp[1]); //første mellomnavn settes sammen med fornavnet
								strcat(first_name, " ");
								strcat(first_name, temp[2]); //andre mellomnavn settes sammen med fornavn og første mellomnavn
								break;
							}

							//tre mellomnavn
							if(j == 5)
                                                        {
                                                                strcat(first_name, temp[1]); //første mellomnavn settes sammen med fornavnet
                                                                strcat(first_name, " ");
                                                                strcat(first_name, temp[2]); //andre mellomnavn settes sammen med fornavn og første mellomna$
								strcat(first_name, " ");
								strcat(first_name, temp[3]);
								break;
                                                        }

							//fire mellomnavn maks
							if(j == 6)
							{
								strcat(first_name, temp[1]);
								strcat(first_name, " ");
								strcat(first_name, temp[2]);
								strcat(first_name, " ");
								strcat(first_name, temp[3]);
								strcat(first_name, " ");
								strcat(first_name, temp[4]);
							}
                                                }

						strncpy(bruker.fornavn, first_name, 100);  //kopierer hele fornavn(og mellomnavn) til brukeren
						brukertabell[index++] = bruker; //tid for neste bruker
						counter++;
					}
                                        break;
                                default:
                                        continue;
                        }

                }

                free(name);
        }
        free(txt);
        fclose(file);
	return counter;
}


void skriv_data (konto_t *brukertabell, char arg, int numUsers)
{
        int index = 0;
        konto_t bruker = brukertabell[index];
        int i = 0;

	//hvis brukeren gir argument 'b' for brukernavn vil brukernavn printes ut først
	//145 er antallet brukere som printes ut
	if(arg == 'b')
	{
		while(i < numUsers)
       		{
			printf("%s, %s, %s,\n", bruker.brukernavn, bruker.fornavn, bruker.etternavn);
                	bruker = brukertabell[++index]; 
                	i++;
       		}
	}

	//hvis brukeren gir argument 'f' for fornavn printes fornavn ut først
	else if(arg == 'f')
	{
		while(i < numUsers)
        	{
                	printf("%s, %s, %s, \n", bruker.fornavn, bruker.etternavn, bruker.brukernavn);
                	bruker = brukertabell[++index];
                	i++;
        	}

	}

	//hvis brukeren gir argument 'e' for etternavn printes etternavn ut først
	else if(arg ==  'e')
	{
		while(i < numUsers)
	        {
			printf("%s, %s, %s,\n", bruker.etternavn, bruker.fornavn, bruker.brukernavn);
                	bruker = brukertabell[++index];
                	i++;
        	}
	}
}

//Samme metode som skriv_data, men med miljøvariabler isteden
void skriv_data_env (konto_t *brukertabell, char envVar, int numUsers)
{
        int index = 0;
        konto_t bruker = brukertabell[index];
        int i = 0;

        if(envVar == 'b')
        {
                while(i < numUsers)
                {
                        printf("%s, %s, %s,\n", bruker.brukernavn, bruker.fornavn, bruker.etternavn);
                        bruker = brukertabell[++index];
                        i++;
                        }
        }

        else if(envVar == 'f')
        {
                while(i < numUsers)
                {
                        printf("%s, %s, %s, \n", bruker.fornavn, bruker.etternavn, bruker.brukernavn);
                        bruker = brukertabell[++index];
                        i++;
                }

        }

        else if(envVar == 'e')
        {
                while(i < numUsers)
                {
                        printf("%s, %s, %s,\n", bruker.etternavn, bruker.fornavn, bruker.brukernavn);
                        bruker = brukertabell[++index];
                        i++;
                }
        }
}

