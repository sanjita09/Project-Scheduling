//PROJECT SCHEDULING using BRANCH AND BOUND method

#include<iostream>
#include <windows.h>
using namespace std;

//total number of projects
#define N 4

//global variable used in printing the final assignment
int h=0;

//node definition of the state space tree
struct Node
{
    //has the sum of the time of the projects assigned from the root to the current node
    int path_time;

    //has the least time with respect to the current node
    int time;

    //has the processor number
    int processor;

    //has the project number assigned to this processor
    int project;

    //has the parent node address used while printing the answer
    Node *predecessor;

    //array containing the project status
    //assigned projects denoted by TRUE
    //not assigned projects denoted by FALSE
    bool status[N];

};

struct Node1
{
    Node* minimum;
    Node1* next;
};

//creating a new node
Node* create(int a,int b,bool status[],Node* predecessor)
{
    Node* temp=new Node;

    //assigning the values
    temp->project=b;
    temp->processor=a;
    temp->predecessor=predecessor;
    for(int i=0;i<N;i++)
    {
        temp->status[i] = status[i];
    }
    temp->status[b] = true;
    return temp;

}

int Retrieve_time(Node* temp)
{
    return temp->time;
}

// c = 7 for default color
void setConsoleColor(WORD c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
//setConsoleColor(FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED );
//  setConsoleColor( BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED );

//printing the individual nodes
void Print_node(Node* temp)
{
    setConsoleColor(7);
    cout<<"\nProcessor - "<<temp->processor;
    cout<<"\nProject Number - "<<temp->project;
    cout<<"\nTotal time of predecessors and current node "<<temp->path_time;
    cout<<"\nOptimal time - "<<temp->time;
    /*cout<<"\nStatus of the jobs - ";
    for(int i=0;i<N;i++)
    {
        if(temp->status[i])
            cout<<"1\t";
        else
            cout<<"0\t";
    }*/
    cout<<endl<<endl;
}

//printing the nodes in the list
void Print_list(Node1* Head)
{
    Node1* temp=Head;
    while(temp!=NULL)
    {
        Print_node(temp->minimum);
        temp=temp->next;
    }
}

//Inserting the nodes in ascending order in the linked list
Node1* Insert(Node1* Head,Node* temp)
{
    //if no nodes are in the linked list
    if(Head==NULL)
    {
        Node1* ABC=new Node1;
        Head=ABC;
        Head->minimum=temp;
        Head->next=NULL;
        //Print_node(Head->minimum);
        return Head;
    }
    //linked list is not empty
    else
    {

        Node1* BCD=new Node1;
        Node1* prev=Head;
        BCD->minimum=temp;
        if(Retrieve_time(Head->minimum) > Retrieve_time(BCD->minimum))
        {
                BCD->next=Head;
                Head=BCD;
                //Print_node(Head->minimum);
                return Head;
        }
        Node1* temp1=Head;
        while(1)
        {

            if(Retrieve_time(temp1->minimum) > Retrieve_time(BCD->minimum))
            {
                BCD->next=temp1;
                prev->next=BCD;
                //Print_node(Head->minimum);
                return Head;
            }
            else if(temp1->next!=NULL)
            {
                prev=temp1;
                temp1=temp1->next;

            }
            else
            {

                BCD->next=NULL;
                temp1->next=BCD;
                //Print_node(Head->minimum);
                return Head;
            }

        }
    }
}

//calculating the least time of nodes after processor a is assigned to processor b
int TimeCal(int a,int b,bool status[],int Time[N][N])
{
    //define time and intialize to 0
    int time1=0;

    //storing the status of the available active projects
    bool active[N];
    for(int k=0;k<N;k++)
        active[k]=true;

    for(int i=a+1;i<N;i++)
    {
        //assigning the maximum value to the 'm' which has the smallest in the respective row at the end of iteration
        //after every iteration it has the possible minimum time to execute the respective project
        int m=9999;

        //m_index has the respective project
        int m_index= -1;

        //finding the minimum in each row
        for(int j=0;j<N;j++)
        {
            if( (!status[j]) && active[j] && (Time[i][j]<m) )
            {
                m_index=j;
                m=Time[i][j];
            }
        }
        //adding the time taken by the respective processors
        time1=time1+m;

        //updating the status by making the project unavailable
        active[m_index]=false;
    }
    return time1;
}


//Printing the projects and the processors they are assigned to
void Print(Node* Min)
{
//backtrack till the 1st predecessor node


   if(h++!=(N-1))
    {
       Print(Min->predecessor);
    }

    //Printing the answer
    cout<<endl;
    setConsoleColor(6);
    cout<<"Processor "<<Min->processor + 1;
     setConsoleColor(7);
    cout<<" is assigned  to ";
     setConsoleColor(6);
    cout<<"Project-";
     setConsoleColor(6);
    cout<<Min->project + 1;
    cout<<endl;

}

//function that uses branch and bound to find the optimal time for the project scheduling
int Optimal_time(int Time[N][N])
{
    //Linked list to store the nodes of the state space tree
    Node1* Head=NULL;

    //dummy node creation
    bool status[N];
    for(int k=0;k<N;k++)
        status[k]=false;
    Node* dummy=create(-1,-1,status,NULL);
    dummy->path_time = 0;
    dummy->time = 0;
    dummy->processor = -1;

    //printing the dummy node
    //Print_node(dummy);

    //Insert the dummy node into the linked list
    Head=Insert(Head,dummy);

    //Print_node(Head->minimum);

    while(Head!=NULL)
    {
        Node* Min=Head->minimum;
        Node1* temp=Head;
        Head=Head->next;
        delete temp;

        //storing the next processor number in i
        int i= Min->processor + 1;

        //termination condition checking if all processors are assigned the projects
        if(i==N)
        {
            setConsoleColor(4);
            cout<<"\n-------------------FINAL ANSWER-----------------\n";
            setConsoleColor(7);
            cout<<"\nOPTIMAL SCHEDULING OF PROJECTS TO THE PROCESSORS\n";
            cout<<"---------------------------------------------------\n";
            Print(Min);
            return Min->time;
        }

        //printing the levels of the nodes in the state space tree
        setConsoleColor(2);
        cout<<"\nLevel-"<<i+1<<endl;
        cout<<"----------"<<endl;

        for(int j=0;j<N;j++)
        {
            //checking whether the project is assigned or not
            if(!(Min->status[j]))
            {
                //creating a new node
                Node* spring=create(i,j,Min->status,Min);

                //finding the path_time and optimal time and assigning the value
                spring->path_time=Min->path_time + Time[i][j];

                spring->time = spring->path_time + TimeCal(i,j,spring->status,Time); //lowerbound

                //Inserting in the linked list which is the list of nodes with the solution
                Head=Insert(Head,spring);

                Print_node(spring);
                cout<<endl;
            }
        }
        cout<<endl<<endl;
        //Print_list(Head);
    }

}




//main function
int main()
{
    int yn;
    int Input[N][N];  //input matrix
    int time_limit;
    int final_time;   //specifies the final optimal time obtained

    setConsoleColor(2);
    cout<<"\n-----------------------------!!!GREETINGS!!!-----------------------------\n";
     setConsoleColor(1);
    cout<<"\n                            PROJECT SCHEDULER\n";

    //Getting input
    for(int i=0;i<N;i++)
    {
        cout<<endl;
        setConsoleColor(6);
        cout<<"Enter the time taken by the Processor-"<<i+1<<" to complete the projects";
        for(int j=0;j<N;j++)
        {
            setConsoleColor(7);
            cout<<"\n  PROJECT "<<j+1<<" - " ;
            cin>>Input[i][j];
        }
        cout<<"\n";
    }
    cout<<endl;
    setConsoleColor(7);
    cout<<"Do you wish to have a time limit\n Type  '1' for YES and '0' for NO";
    cout<<endl;
    cin>>yn;
   if(yn)
   {
    setConsoleColor(7);
    cout<<"\nEnter the time limit";
    cin>>time_limit;
   }

    setConsoleColor(1);
    cout<<"\nInput matrix";
    cout<<endl;
    cout<<"-------------";
    cout<<endl;

    for(int y=0;y<N;y++)
    {
        for(int z=0;z<N;z++)
        {
            setConsoleColor(1);
            cout<<Input[y][z];
            cout<<"\t";
        }
        cout<<"\n";
    }
    setConsoleColor(1);
    cout<<"\nRows depict the PROCESSORS";
    cout<<"\nColumns depict the PROJECTS";
    cout<<endl;

    setConsoleColor(2);
    cout<<"\nOUTPUT\n";
    cout<<"--------";
    cout<<endl;

    //Calling the function and printing the optimal time
    final_time=Optimal_time(Input);
    setConsoleColor(2);
    cout<<"\nThe minimum time in which all the "<<N<<" projects can be completed by the respective assigned processors is "<<final_time;
    cout<<endl;

    if( yn==1  &&  final_time > time_limit)
    {
        setConsoleColor(6);
        cout<<"\nThe projects cannot be executed by the processors within the desired time limit";
        cout<<endl;
    }

    setConsoleColor(7);
    return 0;
}
