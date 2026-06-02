
#include <iostream> 
#include <string> 

#include <iostream>

 using namespace std ; 

 struct Task 
 {
    int id ; 
    string taskDescription ; 
    Task* next ; // this is basically a pointer towards the next task 
    // the task history of a robot is stored via a nexted Linked list 
 };

 struct Robot 
 {
   enum Status 
   {
      busy , 
      available , 
      underMaintainance 
   };
    int id ; 
    Robot* next ; 
    Status  status ;
    Task *  currentTask ;  
    Task* taskHistory ;  // This pointer will point towards the first task that this robot has done 
    
   void addTaskToHistory(Task * task ) 
   {
      if ( taskHistory == nullptr ) // checking if this is the first task to be assigned 
      {
         taskHistory = task ; // this newly assigned tasks becomes the first one
         return ; // mission finito  
      }  
      Task* p  = taskHistory ;
      while ( p ->next != nullptr ) 
      {
         p  = p -> next ; 
      } 
      // after the loop we already know that p has a next that is a nullptr 
      p -> next = task ; 
   }
   void completeTask() // this method resets the currentTask Pointer and changes its status to available 
   {
      status = available ; 
      currentTask = nullptr ; 
   }
 };

 struct AssignmentQueue 
 {
   int totalNumberOfRobots = 0 ; 
   Robot* firstBot ; 
   Robot* currentToBeAssigned  ; // used to track which robot gets the next task
   Robot*  initiateRobotPointer( int id ) 
   {
      Robot * bot = new Robot() ; 
      bot ->id = id ; 
      bot->status = Robot :: Status ::  available ; // by default all robots are available 
      return bot ; // returning the pointer 
   } 

   void  addRobot(int id  ) 
   { // this methods adds a robot to the circular queue 
      // using current to iterate through lahh 
      Robot * bot = initiateRobotPointer(id ) ; 
      // the case of adding the first node to the circular queue 
      if ( firstBot == nullptr ) 
      {
         firstBot = bot ; // the firstBot variable points at the bot that was just created  
         currentToBeAssigned = firstBot;  
         totalNumberOfRobots ++ ; 
         return ; 
      }
       // this is the second case of adding the second robot since in this case the ciruclar link has not been made yet and we will make it   
      if (firstBot-> next == nullptr ) 
      {
         bot -> next = firstBot ; // since this is a circular queue the new bot  is circularly pointin to the first bot 
         firstBot -> next = bot ;  // the first bot has its next pointer bointing to bot 
         
         totalNumberOfRobots ++ ; 
         return ; 
      }
      // third case when you have a circular already established regardless of the number of nodes 
      // in this case the loop should identify the node that is the current last node in the circular queue 
      Robot* tempTraversal = firstBot   ; 
      while (tempTraversal-> next != firstBot  ) 
      {
          tempTraversal = tempTraversal -> next ; // we traverse till we reach the last node in the circular queue  
      }
      // after the while loop breaks then current becomes the last node 
      bot -> next = firstBot ;
      tempTraversal -> next = bot ;  
      totalNumberOfRobots ++ ; 
   } 
   void findNextAvailableRobot()  // this method finds the next available robot to be assigned and mevos the currentToBeAssignedPointer so that the pointer points at it 
   {
      // this method simply moves the pointer linearly to the next pointer 
      // the currentToBeAssigned pointer is always set to point towards the first robot initially 
      int counter = 0 ; 

       if ( currentToBeAssigned == nullptr ) {currentToBeAssigned = firstBot ; } // resetting the currentToBeAssignedPointer so that in the case that a robot status change the mthod does not crash before even seraching after the udpaetd status 
      while ( currentToBeAssigned -> status != Robot::Status::available  ) 
      {
         
         if (counter == totalNumberOfRobots ) {currentToBeAssigned = nullptr  ; cout << "No robots currently available. Please try again later."; break; } 
         currentToBeAssigned = currentToBeAssigned -> next ;   // move the pointer to the next robot
         counter ++ ; 
      } 

   }
   void assignTask (Task * task ) 
   {
      findNextAvailableRobot() ; // setting the currentToBeAssigned pointer to an available robot 
      if ( currentToBeAssigned != nullptr ) 
      {
         // Assigning the task to a Robot 
         currentToBeAssigned -> currentTask = task ; // saving the current task of the robot in the robot's currentTask variable 
         currentToBeAssigned -> status  = Robot::Status :: busy ; // change the robot's status to indicate that it is busy 
         currentToBeAssigned -> addTaskToHistory(task) ; // and then add the task to the robot's history 
         // now last step is to move the currentToBeAssigned in the list to point to the next robot 
         currentToBeAssigned =  currentToBeAssigned -> next ; 
      }
      else 
      {
         cout << "Currently No Robot are Available to be assigned Tasks Rn " ; 
      }
   }
      void displayAllRobots() 
   {
      Robot* temp = firstBot;
      int counter = totalNumberOfRobots;
      do 
      {
         cout << "-------------------\n";
         cout << "Robot ID: " << temp->id << "\n";
         cout << "Status: " << (temp->status == Robot::Status::busy ? "Busy" : "Available") << "\n";
         cout << "Current Task: " << (temp->currentTask ? temp->currentTask->id : -1) << "\n"; // if the robot has no current Task Id it is replaced with -1 and then new line 
         cout << "Task History: ";
         Task* t = temp->taskHistory;
         while (t != nullptr) {
               cout << t->id << " \n";
               cout << t -> taskDescription << "\n" ;
               t = t->next; // traverse into the next task to print 
         }
         cout << "\n";
         temp = temp->next;
         counter--; // we reduce the counter till we have printed the status of everysingle robot 
      } while (counter != 0);
   }

};
