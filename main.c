#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/queue.h>
#include <time.h>
#include <semaphore.h>


void * create_prof();
void * create_stud();
int finder(int * ,int, int );

pthread_mutex_t mLoop;

sem_t mStartingPistol;

struct ass 
{
	int assID;
	int profID;
	int numStuds;
	int durration;
	TAILQ_ENTRY(ass) entries;
};

int aFlag=0,aValue, aMin=1, aMax=100000, aDef=10;char *aChar=NULL; 
int wFlag=0,wValue, wMin=1, wMax=10, wDef=1;char *wChar=NULL;
int WFlag=0,WValue, WMin=1, WMax=100, WDef=5;char *WChar=NULL;
int nFlag=0,nValue, nMin=1, nMax=10, nDef=1;char *nChar=NULL;
int NFlag=0,NValue, NMin=1, NMax=100, NDef=10;char *NChar=NULL;
int hFlag=0,hValue, hMin=1, hMax=10, hDef=1;char *hChar=NULL;
int HFlag=0,HValue, HMin=1, HMax=10, HDef=5;char *HChar=NULL;
int pFlag=0,pValue, pMin=1, pMax=10, pDef=2;char *pChar=NULL;
int sFlag=0,sValue, sMin=1, sMax=10, sDef=2;char *sChar=NULL;
int dFlag=0,dValue, dMin=1, dMax=10, dDef;char *dChar=NULL;
int qFlag=0,qValue, qMin=1, qMax=256, qDef=8;char *qChar=NULL;

int currAssID = 0;
int studamnt = 0;
int qSize = 0;
int * workingStuds;

int profsFinished = 0;

TAILQ_HEAD(tailhead, ass) head;



int main(int argc, const char *argv[])
{
	int opt,i;
	extern char *optarg;
	//extern int optind, optopt;
	while((opt=getopt(argc, argv, "a:w:W:n:N:h:H:p:s:d:q:")) !=-1){
	switch(opt){
	case 'a':
		aFlag=1;
		aChar=optarg;
		aValue=atoi(aChar);
		break;
	case 'w':
		wFlag=1;
		wChar=optarg;
		wValue=atoi(wChar);
		break;
	case 'W':
		WFlag=1;
		WChar=optarg;
		WValue=atoi(WChar);
		break;
	case 'n':
		nFlag=1;
		nChar=optarg;
		nValue=atoi(nChar);
		break;
	case 'N':
		NFlag=1;
		NChar=optarg;
		NValue=atoi(NChar);
		break;
	case 'h':
		hFlag=1;
		hChar=optarg;
		hValue=atoi(hChar);
		break;
	case 'H':
		HFlag=1;
		HChar=optarg;
		HValue=atoi(HChar);
		break;
	case 'p':
		pFlag=1;
		pChar=optarg;
		pValue=atoi(pChar);
		break;
	case 's':
		sFlag=1;
		sChar=optarg;
		sValue=atoi(sChar);
		break;
	case 'd':
		dFlag=1;
		dChar=optarg;
		dValue=atoi(dChar);
		break;
	case 'q':
		qFlag=1;
		qChar=optarg;
		qValue=atoi(qChar);
		break;
	}
	}
	char* outOfBounds= "Invalid argumen detected, exiting.";
	if(aFlag==0){
		aValue=aDef;
	}else if(aValue<aMin || aValue>aMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(wFlag==0){
		wValue=wDef;
	}else if(wValue<wMin || wValue>wMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(WFlag==0){
		WValue=WDef;
	}else if(WValue<WMin || WValue>WMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(nFlag==0){
		nValue=nDef;
	}else if(nValue<nMin || nValue>nMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(NFlag==0){
		NValue=NDef;
	}else if(NValue<NMin || NValue>NMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(hFlag==0){
		hValue=hDef;
	}else if(hValue<hMin || hValue>hMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(HFlag==0){
		HValue=HDef;
	}else if(HValue<HMin || HValue>HMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(pFlag==0){
		pValue=pDef;
	}else if(pValue<pMin || pValue>pMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(sFlag==0){
		sValue=sDef;
	}else if(sValue<sMin || sValue>sMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	dDef=sValue;
	if(dFlag==0){
		dValue=sValue;
	}else if(dValue<dMin || dValue>dMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(qFlag==0){
		qValue=qDef;
	}else if(qValue<qMin || qValue>qMax){
		printf("%s\n", outOfBounds);
	exit(1);
	}
	if(dValue > sValue)
	{
		printf("Can not have more students per assignment than students.  Exiting.\n");
		exit(1);
	}

	workingStuds = (int *) malloc(sizeof(int) * sValue);
	for(int i = 0; i < sValue; i++)
		workingStuds[i]=0;



	//profgen
	pthread_t profMakers[pValue];
	int id;
	for(int i = 0; i < pValue; i++)
	{
		id = i;
	  if(pthread_create(&(profMakers[i]), NULL, create_prof, i) != 0) {
	    perror("pthread_create");
	    exit(1);
	  }

	}



	pthread_t studMakers[sValue];
	id=0;



	for(int i = 0; i < sValue; i++)
	{
	  id = i;
	  if(pthread_create(&(studMakers[i]), NULL, create_stud, i) != 0) {
	    perror("pthread_create");
	    exit(1);
	  }


	  studamnt = i;
	}

	//join 
	for(int i = 0; i < pValue; i++)
	{
	  if(pthread_join(profMakers[i], NULL) != 0) {
	    perror("pthread_join");
	    exit(1);
	  }
	}

	for(int i = 0; i < sValue; i++)
	{
	  if(pthread_join(studMakers[i], NULL) != 0) {
	    perror("pthread_join");
	    exit(1);
	  }
	}

	return 0;

}


//pushes an assignment struct with the passed data
void qPush(int assID, int profID, int numStuds, int durration) 
{
	struct ass * element;
	element = malloc(sizeof(struct ass));
	if (element) {
	element->assID = assID;
	element->profID = profID;
	element->numStuds = numStuds;
	element->durration = durration;
	}
	if(qSize == 0)
		TAILQ_INSERT_HEAD(&head, element, entries);
	else
		TAILQ_INSERT_TAIL(&head, element, entries);
	qSize++;
}


//professors write assignments to the queue based on passed parameters
void * create_prof(int  iid) 
{
	int waitTime;
	int * id = &iid;
	int myID = *id;

	pthread_mutex_lock(&mLoop);
	printf("STARTING Professor %d\n", *id);
	pthread_mutex_unlock(&mLoop);

	

	int numAss;
	if(NValue-nValue != 0)
	{
		srand(time(NULL));
		numAss = (rand()%(NValue-nValue+1))+nValue;
	}
	else
		numAss = NValue;

	for(int i = 0; i < numAss; i++)
	{

		if(WValue-wValue != 0)
		{
			srand(time(NULL));
			waitTime = (rand()%(WValue-wValue+1))+wValue;
		}
		else
			waitTime = wValue;


		int durration;
		if(NValue-nValue != 0)
		{
			srand(time(NULL));
			durration = (rand()%(HValue-hValue+1)+hValue);
		}
		else
			durration = HValue;

		sleep(waitTime);

		pthread_mutex_lock(&mLoop);
		qPush(currAssID, myID, dValue, durration);
		printf("ASSIGN Professor %d adding Assignment %d: %d Hours.\n",myID, currAssID, durration );
		currAssID++;
		pthread_mutex_unlock(&mLoop);
	}

	pthread_mutex_lock(&mLoop);
	profsFinished++;

	printf("EXITING Professor %d\n", myID);
	pthread_mutex_unlock(&mLoop);

	pthread_exit(0);
}
/*students take assignments out of the queue and work on them
  no one student can do one assinment twice.
*/
void * create_stud(int iid) 
{
	int * id = &iid;
	int currdurr;
	int myID= iid;
	printf("STARTING Student %d\n", myID);
	int assWorked[1000];
	for(int i = 0; i < nValue; i++)
		assWorked[i] = -1;
	int index = 0;

		
	while(!(qSize == 0 && profsFinished == pValue))
	{

		pthread_mutex_lock(&mLoop);
		if(qSize == 0 && profsFinished == pValue)
		{
			printf("EXITING Student %d\n", myID);

			pthread_mutex_unlock(&mLoop);
			pthread_exit(0);
		}

			struct ass *hAss;
			hAss = head.tqh_first;
			
			
		if(hAss != NULL)	
		{
			if(finder(assWorked, index, hAss->assID) == -1)
			{

				assWorked[index] = hAss->assID;
				index++;
				currdurr = hAss->durration;

				printf("BEGIN Student %d working on Assignment %d from Professor %d\n", myID, hAss->assID, hAss->profID );

				int savedAID = hAss->assID;
				int savedPID = hAss->profID;

				if(hAss->numStuds == 1)
				{
					TAILQ_REMOVE(&head, head.tqh_first, entries);
					free(hAss);
					qSize--;
					
				}
				else
				{
					hAss->numStuds--;
				}
				for(int i = 0; i < currdurr; i++)
				{
					printf("WORK Student %d working on Assignment %d Hour %d from Professor %d\n", myID, savedAID, i+1, savedPID);

					pthread_mutex_unlock(&mLoop);
				
					sleep(1);
					pthread_mutex_lock(&mLoop);
				}
				printf("END Student %d working on Assignment %d from Professor %d\n", myID, savedAID, savedPID );
			
			}
		}
		pthread_mutex_unlock(&mLoop);

		
	}


	printf("EXITING Student %d\n", myID);
	pthread_exit(0);
}

//slow search :( im sorry
int finder(int * assWorked,int index, int hAssID)
{
	for(int i = 0; i <= index; i++)
	{
		if(assWorked[i] == hAssID)
			return 1;
		else if(assWorked[i] == -1)
			return -1;
	}
	return -1;
}


