 #include <iostream> 
 using namespace std ; 

 

 struct AssignmentQueue 
 {
    int totalNumberOfRobots ; 
    Robot* firstBot ; 
    Robot* current ; // used to track which robot gets the next task 
 };
 struct Robot 
 {
   enum Status 
   {
      busy , 
      available 
   };
    int id ; 
    Robot* next ; 
    Status  status ; 
    Task* taskHistory ;  // This pointer will point towards the first task that this robot has done 

 };

 struct Task 
 {
    int id ; 
    string taskDescription ; 
    Task* next ; // this is basically a pointer towards the next task 
    // the task history of a robot is stored via a nexted Linked list 
 };


 void main  () 
{





}