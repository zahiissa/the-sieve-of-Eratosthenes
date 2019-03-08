#include "list.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>


/*
 * main code
*/


List list;

int countCharsInInt(int i){
	int count = 0;
	while(i != 0){
		i/=10;
		count++;
	}
	return count;
}

char* int_to_char(int num) {
	int char_num = countCharsInInt(num);
	char* str = malloc(char_num + 1);
	str[char_num--] = 0;
	while (num/10) {
		str[char_num--] = num % 10 + '0';
		num = num/10;
	}
	str[char_num] = num + '0';
	return str;
}


FILE* logMaker(int i) {
	char s[countCharsInInt(i) + 11];
	strcpy(s, "thread-");
	char* c = int_to_char(i);
	strcat(s, c);
	strcat(s, ".log");
	FILE* log = fopen(s, "a+");
	free(c);
	if (!log) {
		return NULL;
	}
	return log;
}

void* threadFunction(void* param){
	//Node deb;
	int* n = (int*)(param);
	FILE* logf = logMaker(*n);
	if(! logf)
		return NULL;
	//take prime candidate p and lock it
	// if p^2 is in the list then take the next p
	int primeCandidate;
	//lock sentinel node
	Node prev = list->head;
	pthread_mutex_lock(prev->lock);
	if (prev->next == list->last){
		// the list is empty
		//printf("the list is empty\n",); // TODO remove it
		pthread_mutex_unlock(prev->lock);
		fclose(logf);
		return NULL;
	}

	pthread_mutex_lock(prev->next->lock);
	Node elem = prev->next;
	primeCandidate = elem->num;
	Node primeCandidateNode = elem;
	int p2 = primeCandidate*primeCandidate;
	int flagIfFountP2 = 0;
	//pthread_mutex_lock(elem->next->lock); //no need
	//int jk = 0;
	while (elem->next != NULL || primeCandidateNode->next != list->last){
	//deb = list->head->next;
	//printf(" all the list");

	//while (deb != list->last){
	//	printf(" %d ",deb->num);
	//	deb=deb->next;
	//}
	//		printf("\n");

		if(primeCandidateNode->next == list->last){ // end
			pthread_mutex_unlock(elem->lock);
			pthread_mutex_unlock(prev->lock);
			break;
			
		}
	
		if(elem == list->last){ // in all the situations go the next candidate
			//printf("/current candidate is %d go to next candidate\n",primeCandidate);
			if(!flagIfFountP2){ // if p^2 > N then finish
				pthread_mutex_unlock(elem->lock);
			pthread_mutex_unlock(prev->lock); 
				break;
			}
				
			pthread_mutex_unlock(elem->lock);
			pthread_mutex_unlock(prev->lock);
			primeCandidateNode = primeCandidateNode->next;	// end of the list go to the next candidate	
			primeCandidate = primeCandidateNode->num; // what if next is not prime ??!!!
			p2 = primeCandidate*primeCandidate;
			elem = primeCandidateNode->next; 
			prev = primeCandidateNode;
			if(primeCandidateNode->next == list->last)
				break;
			pthread_mutex_lock(prev->lock);
			pthread_mutex_lock(elem->lock);
			flagIfFountP2 = 0;
			// here we should check this thing: 
			// that we have prime candidate that his p2 is not in the list from the beginning 
			// then should print it because its prime 
			// but what if 2 threads or more have this situation with the same prime candidate ?? hell no :/
			// never mind , thread print only and just only if he deleted p2 otherwise dont print
			continue;	
		}
		if (elem->num == p2){ // we know that the flag 0 here
			//printf("/now we found p2 for candidate %d \n",primeCandidate);
		
		  // if found, delete p2
		  prev->next = elem->next;
		  elem->next->prev = prev;
		
		  // unlock and deallocate mem
		  pthread_mutex_unlock(elem->lock);
		//  pthread_mutex_unlock(prev->lock);
		  pthread_mutex_destroy(elem->lock);
		  free(elem->lock);
		  free(elem);
		  // its a success
		  
		 // pthread_mutex_unlock(prev->lock);
		 
		  elem = prev->next; // continue from that point because of observations
		 // prev = primeCandidateNode;
		  pthread_mutex_lock(elem->lock); // mkmlsh hun momk 3 m3molha lock ??
		   // if(primeCandidate == 3)
		//	printf("safe here\n");
		//  pthread_mutex_lock(elem->lock);
		  flagIfFountP2 = 1;
		  fprintf(logf, "%s %d\n", "prime",primeCandidate);
		  				fprintf(logf, "%d\n",p2);

		  continue;
		}
		if(flagIfFountP2){ // then this prime candidate is true and continue deleting
			//if(prev->num < p2 && elem->num > p2){
			//	fprintf(logf, "%d\n",p2);
			//}
			if(elem->num % primeCandidate == 0){ // delete node
			//printf("/delete num that yaqsem for candidate %d the num is %d \n",primeCandidate,elem->num);

				prev->next = elem->next;
				elem->next->prev = prev;
				fprintf(logf, "%d\n", elem->num);
				// unlock and deallocate mem
				pthread_mutex_unlock(elem->lock);
				pthread_mutex_destroy(elem->lock);
				free(elem->lock);
				free(elem);
				// its a success
				pthread_mutex_lock(prev->next->lock);
				elem=prev->next;
				prev = elem->prev;
				continue;
			} else if(elem->num % primeCandidate != 0){
				pthread_mutex_unlock(prev->lock);
				pthread_mutex_lock(elem->next->lock);
				elem = elem->next;
				prev = elem->prev;
				continue;
			} 
		} 
		if(!flagIfFountP2 ){ // it cant be that elem->num = p2 here
			if(elem->num > p2){ // we found that there is another thread that took this prime candidate
				if(primeCandidateNode->next != list->last) // fsh 7aje
					primeCandidateNode = primeCandidateNode->next;
				else break;
				primeCandidate = primeCandidateNode->num;
				flagIfFountP2 = 0;
				p2 = primeCandidate*primeCandidate;
				pthread_mutex_unlock(prev->lock);
				pthread_mutex_unlock(elem->lock);
				elem = primeCandidateNode->next; // back to the first (from p->next)
				prev = primeCandidateNode;
				pthread_mutex_lock(prev->lock);
				pthread_mutex_lock(elem->lock);
				continue;
			} else { // then do nothing just continue iterating over the list
				prev = elem;
				elem = elem->next;
				pthread_mutex_unlock(prev->prev->lock);
				pthread_mutex_lock(elem->lock);
				continue;
			}
		}
	}
	fclose(logf);
    return NULL;
}

int main(int argc, char** argv){
	int i;
	char* s = argv[1];
	int N = atoi(s);
	s = argv[2];
	int T = atoi(s);
	list = ListCreate(N);
	if(! list || T < 1 || N < 2){
		return 1;
	}
	int is[T];
	
	for (i =0 ; i< T ;i++)
		is[i] = i+1;
	pthread_t threads[T];
	for(i=0; i<T;i++){
		int r = pthread_create(&threads[i],NULL, threadFunction,&is[i]);
		if (r){
			printf("ERROR; return code from pthread_create() is %d\n", r);
			return 1;
        }
	}
	for(i=0; i<T;i++){
		pthread_join(threads[i], NULL);
	}
	
	
	FILE* Flog = fopen("primes.log", "a+");
	Node it = list->head->next;
	while(it != list->last){
		fprintf(Flog, "%d\n",it->num);
		it = it->next;
	}
	fclose(Flog);
	ListDestroy(list);
	return 0;
	
}


























