#include<iostream>
#include<vector>
#include<thread>
#include<algorithm>
#include <mutex>
#include<windows.h>


enum direction{idle,up,down};

using namespace std;

std::mutex mutex_name;

// Objective is to create a Queue that sorts ascending for the first n elements and descending for the next length-n elements
class DSortedQ
{
    private:
    vector<int> QArr;// Vectors are used
    

    public:
    int flag;// Ideally, it represents the index until which sorting should be done
    DSortedQ();//Constuctor
    int Pop();//removes the first element in the queue
    int retTop();//Returns the value of the top most element
    void Insert(int value);// Inserts in a sorted manner
    void Push(int value);//Appends the element
    void Sort(int method);//Sorts ascending until flag, sorts descending after flag
    void Show();//Displays all the elements of the queue
    int Empty();//Returns 1 if empty else 0
    int retFlag();//Returns flag value
    int inQueue(int value,int method);//returns 1 if value is found in queue. method=1 corresponds 
    //to insert. method=-1 corresponds to push.
    void flagInc();//Increments flag

};

DSortedQ::DSortedQ()
{
    //Initializes the flag attribute.
    flag=-1;
}

void DSortedQ::flagInc()
{
    if(flag==-1)
    {flag+=2;}
    cout<<"Flag+ "<<flag<<endl;
}

int DSortedQ::Pop()
{
    if(!QArr.empty())
    {
    int Retval= QArr.front();
    QArr.erase(QArr.begin());
    flag>-1? flag--:flag;
    return Retval;
    }
    
    return 0;
}

int DSortedQ::Empty()
{
    if(QArr.empty())
    {
        return 1;
    }
    return 0;
}

int DSortedQ::inQueue(int value,int method)
{
    if(QArr.size()>0)
    {
    if(method==1)
    {
        for(int i=0;i<(flag+1);i++)
        {
            if(QArr[i]==value)
            {
                return 1;
            }
        }
    }
    else if(method==-1)
    {
        for(int i=(flag);i<QArr.size();i++)
        {
            if(QArr[i]==value)
            {
                return 1;
            }
        }
    }
    }
    return 0;
}

void DSortedQ::Insert(int value)
{
    if(inQueue(value,1)==0)
    {
        QArr.insert(QArr.begin(), value);
            if(flag==-1)
            {
            flag++;
            Sort(1);
            }
            else
            {
                Sort(1);
                flag++;
            }
    }
    else
    {
        cout<<"Value exists in queue"<<endl;
    }

}

void DSortedQ::Push(int value)
{
    if(inQueue(value,-1)==0)
    {
        QArr.insert(QArr.end(),value);
        Show();
        Sort(-1);
    }
    else
    {
        cout<<"Value exists in queue"<<endl;
    }
}

int DSortedQ::retTop()
{
    return QArr.front();
}

void DSortedQ::Sort(int method)
{
    if(method==1)
    {
        vector<int>::iterator flagIt=QArr.begin();
        advance(flagIt,(flag));
        sort(QArr.begin(),flagIt);
    }
    else if(flag<0)
    {
        return;
    }
    else if(method==-1)
    {
        vector<int>::iterator flagIt1=QArr.begin();
        advance(flagIt1,(flag+1));
        sort(flagIt1,QArr.end(),greater<int>());
    }
}

void DSortedQ::Show()
{
    cout<< " Flag= "<<flag<< "  ";
    cout<<"Q: ";
    for(int i=0;i<QArr.size();i++)
    {
        cout<<QArr[i]<<" ";
    }
    cout<<" - ";
}
int DSortedQ::retFlag()
{
    return flag;
}
class Lift
{
private:
    direction dir;
    int current;
    int target;
    

    
public:
    Lift();
    DSortedQ LiftQ;
    bool StopLift;
    void Move();
    void RideCompletion();
    void IButtonPress(int floor);
    void ButtonPress(int floor,direction d);
    void CurrFloor();
    void CurrDir();
    void LShow(string prefix);

};


Lift::Lift()
{
    DSortedQ LiftQ;
    current=0;
    dir=idle;
    StopLift=false;
    target=0;
    
}


void Lift::CurrFloor()
{
    cout<<"@ floor "<<current<<"  - ";
}

void Lift::CurrDir()
{
    switch(dir){
    case up:
    cout<<"Going up ";
        break;
    case down:
    cout<<"Going down ";
        break;
    case idle:
    cout<<"Idle ";
        break;
    }
}

void Lift::IButtonPress(int floor)
{
    cout<<"IB: " << dir << " curr: " << current << " floor: "<<floor <<" Flag: " <<LiftQ.flag<<'\n';
    if(dir==up)
    {
        if(current<floor ) 
        {
            cout<<"c1"<<endl; 
            LiftQ.Insert(floor) ;
        }
        else 
        { 
            cout<<"c2"<<endl; 
            LiftQ.Push(floor);
            
        }
    }
    else if(dir==down)
    {
        if(current>floor ) 
        { 
            cout<<"c3"<<endl; 
            LiftQ.Insert(floor) ;
        }
        else 
        { 
            cout << "c4"<< endl; 
            LiftQ.Push(floor);
            LiftQ.flagInc();
        }
    }
    else
    { 
        //cout << "c5"<< endl;
        LiftQ.Insert(floor);
    }
    LShow("IB");
}


void Lift::RideCompletion()
{
    while(LiftQ.Empty()!=1)
    {
        Move();
    }
}
void Lift::Move()
{
    
    cout<<"[LM: Move from " << current << "]" << endl;
    target= LiftQ.retTop();
    if(current<target)
    {
        dir=up;
    while(current<target)
    {
        
        //cout<<"[LM: Now on floor "<<current<< "]" << endl;
        ++current;
        Sleep(300);
    }
    cout<<"[LM: Reached target floor "<<target<< "]" << endl;
    
        if(LiftQ.Empty())
        {
            dir=idle;
        }
    }
    else if(current>target)
    {
        dir=down;
        while(current>target)
        {

        //    cout<<"[LM: Now on floor "<<current<<"]" <<endl;
            --current;
            Sleep(300);
        }
        cout<<"[LM: Reached target floor "<<target<< "]" << endl;
        
        if(LiftQ.Empty())
        {
            dir=idle;
        }
    }
    else
    {
        cout<<"[LM: Already at the required floor "<< target<< "]"<<endl;
        if(LiftQ.Empty())
        {
            dir=idle;
        }

    }
    LiftQ.Pop();
}
void Lift::ButtonPress(int floor,direction d)
{
    
    cout<<"XB: from floor "<<floor <<" to go " <<d << endl;
    if(dir==idle)
    {
        LiftQ.Insert(floor);
        dir=d;
    }
    else if(dir==d && current<floor)
    {
        LiftQ.Insert(floor);
    }
    else
    {
        LiftQ.Push(floor);
    }
    LShow("XB");
    
}

void Lift::LShow(string prefix)
{
    mutex_name.lock();
    cout << "[" <<  prefix << ": ";
    LiftQ.Show();
    CurrDir();
    CurrFloor();
    cout << "]" << endl;
    mutex_name.unlock();
}

void LiftMover(Lift* L1)
{
    while(L1->StopLift==false)
    {
        if(!L1->LiftQ.Empty())
        {
            L1->Move();
            L1->LShow("TLM");
        }
    }
}

    
int main()
{

    Lift lift;
    lift.LShow("Start");
    lift.ButtonPress(3,up);cout<<"--"<<endl;
    thread LiftThread(LiftMover,&lift);
    Sleep(1000);
    lift.IButtonPress(5);cout<<"--"<<endl;
    lift.ButtonPress(2,down);cout<<"--"<<endl;
    Sleep(1000);
    lift.IButtonPress(6);cout<<"--"<<endl;
    Sleep(1000);
    lift.ButtonPress(5,up);cout<<"--"<<endl;
    lift.IButtonPress(8);cout<<"--"<<endl;
    lift.IButtonPress(7);cout<<"--"<<endl;
    Sleep(1000);
    lift.IButtonPress(2);cout<<"--"<<endl;
    lift.ButtonPress(3,down);cout<<"--"<<endl;
    lift.IButtonPress(2);cout<<"--"<<endl;
    Sleep(1000);
    lift.ButtonPress(3,up);cout<<"--"<<endl;
    lift.IButtonPress(6);cout<<"--"<<endl;
    lift.ButtonPress(5,down);cout<<"--"<<endl;
    Sleep(1000);
    lift.IButtonPress(2);cout<<"--"<<endl;
    lift.ButtonPress(7,down);cout<<"--"<<endl;
    Sleep(1000);
    lift.StopLift = true;
    LiftThread.join();
    return 0;
}