#include <stdio.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 Linked list structure 
 **/
struct Node {
    char num;
    struct Node* next; 
    struct Node* prev; 
};

//Appending to process linked list
void appendList(struct Node** head_ref, int new_data)  
{  
    struct Node *new_node = (struct Node*) malloc(sizeof(struct Node));
  
    struct Node *last = *head_ref;
    if (*head_ref == NULL)  
    {   
        new_node->num = new_data;  
        new_node->next = NULL;  
        new_node->prev=NULL;
        *head_ref = new_node;  
        return;  
    }  
  
    while (last->next != NULL)  
        last = last->next; 

    //Check if same process is running, if so no need to add to linked list   
    if(last->num!=new_data)
    {
        new_node->num = new_data;  
        new_node->next = NULL;
        last->next = new_node;  
        new_node->prev=last;
    }
    
    return;  
}  

int main() {
    int numProcess; 
    printf("Enter the number of processes:");
    scanf("%d",&numProcess);
    printf("\n");
    int arrTime[numProcess]; // storing the arrival timings 
    double burst[numProcess]; // storing CPU burst
    double original_burst[numProcess];
    double priority[numProcess]; // storing priority
    bool statusProcess[numProcess]; // storing status of process completion
    double w1,w2; //weights associated
    double waitTime[numProcess]; //stores the total wait time of each process
    int lastEnd[numProcess];  //stores last end time
    /************
     NEED TO CREATE LINKED LIST FOR STORING THE ORDERS
     
     ************/
     
    //Created before main 

    /******* Taking inputs********/
    for(int i=0;i<numProcess;i++){
    	printf("Enter arrival time of P%d:",i+1);
        scanf("%d", &arrTime[i]);
        printf("Enter CPU burst cycle of P%d:",i+1);
        scanf("%lf",&burst[i]);
        printf("Enter priority of P%d:",i+1);
        scanf("%lf",&priority[i]);
        printf("\n");
    }
     
    printf("Enter value of w1:");
    scanf("%lf",&w1);
    printf("Enter value of w2:");
    scanf("%lf",&w2);
    printf("\n");
    
     
     /***** Initialising last end and wait time and status process ***************///
    
    for(int i=0;i<numProcess;i++){
        lastEnd[i]=arrTime[i];
        waitTime[i]=0;
        original_burst[i]=burst[i];
        statusProcess[i]=false;
    }
    
    
    /************Starting the algorithm************/
    
    int pid ;
    float presentWeight = FLT_MIN;
    int presentArr = INT_MAX;
    
    for(int i=0;i<numProcess;i++){
        if(arrTime[i]<presentArr){
            pid=i;
            presentArr=arrTime[i];
            presentWeight = w1*burst[i] + w2*priority[i];
        }
        else if(arrTime[i]==presentArr){
            if(w1*burst[i] + w2*priority[i]>presentWeight){
                pid=i;
                presentArr=arrTime[i];
                presentWeight = w1*burst[i] + w2*priority[i];
            }
        }

    }
    
    /**********************
      INSERTING THE FIRST PROCESS IN THE LINKED LIST
      **********/
    struct Node *head=NULL;
    appendList(&head,pid+1);

    //Modified: Output with example is correct. Need to check for other custom test cases.
    int t = presentArr;
    burst[pid]--;
    waitTime[pid] += t-lastEnd[pid];
    lastEnd[pid]=t+1;
    t++;
    presentWeight = w1*burst[pid] + w2*priority[pid];
    
    
    /************ LOOPING IN **********************/
    
    while (true){
        
        // /***** checking if all the processes in the queue have been completed*******/
        
        bool complete =true;
        // for(int i=0;i<numProcess;i++){
        //     if (statusProcess[i]==false) {complete=false; break;}
        // }
                
        //Checking if ready queue is empty and no process is executing
        int rq;
        for(rq=0; rq<numProcess; rq++){
            if(statusProcess[rq]==false)
            {   
                if(burst[rq]<original_burst[rq] || arrTime[rq]<=t)
                {
                    complete=false;
                    break;
                }
            }

        }
        if(complete==true) break; //end condition of the while loop

        /*******checking for any process that is coming into the ready queue******/
        
       // bool new = false;
        for(int i=0; i<numProcess; i++){
            if (arrTime[i] == t){
                if (w1*burst[i] + w2*priority[i] > presentWeight){
                    pid=i;
                }
                //Added to check Mudit's test case.
                else if(w1*burst[i] + w2* priority[i] == presentWeight && !statusProcess[i])
                {
                    if(arrTime[i]<presentArr)
                        pid=i;
                }
            }
        }
        
         //****** put the process in linked list******
         // linkedlist may contain repeating terms..we will try to handle that later
        appendList(&head,pid+1);
         
        burst[pid]--;
        presentWeight = w1*burst[pid] + w2*priority[pid];
        /// present weight change...
        
        //************** updating waiting time and last end  time all***************///
        
        waitTime[pid] += t - lastEnd[pid];
        lastEnd[pid] = t+1;
        t++;
        
        
        // *********** when a process ends *****************//
        if(burst[pid]==0){
            statusProcess[pid]=true;
            int arr = INT_MAX;
            float weight = FLT_MIN;
            
            //Find the next best process to schedule 
            //Edit by Samarpan: Added statusProcess[i] check condition so that an already finished process is not scheduled again 
            for(int i=0;i<numProcess;i++){
                if(w1*burst[i] + w2* priority[i] > weight && !statusProcess[i]){
                    weight = w1*burst[i] + w2* priority[i];
                    pid=i;
                    arr = arrTime[i];
                }
                //Tie in weights, then check arrival times 
                else if(w1*burst[i] + w2* priority[i] == weight && !statusProcess[i]){
                    if(arrTime[i] < arr){
                        weight = w1*burst[i] + w2* priority[i];
                        pid=i;
                        arr = arrTime[i];
                    }
                }
            }
        }
    }



    /********************* OUTPUT STATEMENTS **********************************/
    // ************* printing waiting time***********************
    int waitSum=0;
    int count=0;
    for(int i=0;i<numProcess;i++){
        if(statusProcess[i]){
        waitSum+= waitTime[i];
        printf("wait time for process %d is: %lf ",i+1,waitTime[i]); 
        printf("\n");
        count++;
        }
    }

    // ****************printing avg time********************
    printf("\n");
    double avgTime = (double)waitSum/(double)count;
    printf("Average time is : %lf \n", avgTime);

    // **************printing of process execution order**********************
    while(head!=NULL)
    {   
        printf("P");
        printf("%d",head->num);
        if(head->next!=NULL)
            printf("->");
        head=head->next;
    }
    	printf("\n");
    // ****************** end time of each process************************
    for(int i=0;i<numProcess;i++){
       if(statusProcess[i]){ printf("P%d finished at: %d",i+1, lastEnd[i] );
        printf("\n");
        }
    }
    
    
}
