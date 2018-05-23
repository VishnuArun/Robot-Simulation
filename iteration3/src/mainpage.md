
# Robot Simulator Index Page

## Design Documentation


 ##### Copyright 2018 Vishnu Arun
 _____

 ## Introduction to software ##
 The Robot simulator is based on the Braitenberg vehicles concept. This software uses OpenGl <https://www.opengl.org/> and nanogui <https://github.com/wjakob/nanogui> libraries for graphics and user interactions. Robots in this simulation can exhbit different behaviors such as fear, exploratory, or aggressive behavior. Our simulation is composed of autonomous robots that exhibit these behaviors towards lights and food. lights are mobile objects that can freely roam the environment and food is an immobile object that is static in the environment. Our environment is a rectangluar arena with hard border on all 4 sides which do not allow robots or lights to pass through. The User can specify parameters such as the number of robots or the ratio of Fear to Explore robots.


 ## Design Specification and Implementation ##

 The software has a Graphical User Interface where a user can specify parameters such as number of robots, number of lights, number of food, food on/off, robot light sensor sensitivity, and ratio of Fear to Explore robots. The user can also Play/Pause and start a New Game. If the user chooses to change a parameter, the changes will be reflected when the New Game button is pressed. GraphicsArenaViewer Class contains all graphics related functions for this simulation. It draws the arena and all of its contents. GraphicsArenaViewer and Arena are instantiated using the Controller Class. Controller holds communication between Arena and GraphicsArenaViewer classes.

 To keep time the UpdateTimestep() method is called in GraphicsArenaViewer class. Controller communicated these timesteps to Arena which call updates all entites timesteps using UpdateEntitiesTimestep(). The Arena contains vectors for all entities, mobile entities, and robots. entities contains all entities, mobile entities and robot are subsets of entities vector. We use a vector data structure because we can easily access all contents inside structure by traversing using a loop. This is natural when we want to update all objects of a specific type at once in succession. Vectors also allow us to easily add or remove items from the structure. For example, when the user request a different number of robots we can easily remove or add robot instancs to this vector.

 Robots themselves respond differently to stimuli based on the Braitenberg concept. This concept details the behaviors and their correspondence to the robots wheel velocity. Fear is detailed as a positive direct snesor-motor connection. When the robot approaches the sensor it will move away as to simulate fear. Exploratory behavior is detialed as negative cross-connected sensor-motor connection. Each robot is equipped with two lights sensors on either side of the robots head. This is shown in the simulation as two small circles on the robot body. Each sensor can calulate a reading value at each timestep using a Euclidean distance based formula between nearby lights and the sensors themselves. The robot will have a DecideBehavior() method that will take into account sensor readings and robot id and react accordingly.

 The robot also has a hungry status. This is implemented using a timer. After an approximate 30 seconds of the simulation running the robots will become hungry. The robots are searching for food and must tag a food entity to resolve their hunger. The robot "tags" by checking the euclidean distance between a robots food sensor and a food object. This process is similar to light sensor. While a robot is hungry it will show assertive behavior. Assertive behavior is a positive cross-connected sensor-motor connection. If the robot does not tag a food within the 120 seconds it will flash and become very hungry. When a robot is very hungry it will completely ignore light stimuli and search for food. After 150 seconds of no food the simulation will stop and ask the user to start a new game.

 This simulation implements Observer Pattern. Observer Pattern is a software design pattern where a subject maintains a list of observers who notify the subject when there is any state change. This allows the observer to "subscribe" to a list of subjects without tightly coupling the two objects. This implementation consists of the Robot being the observer and the stimulus being the subject through Arena. This idea is similar to how a physical robot would react to stimuli. At each time step the Robot will get readings from all stimuli in the environment then choose an action based on the reading values and stimuli type. If the readings are sufficient the Robot will change behaviors and portray a behavior based on the stimuli. The Robot will need to receive all stimuli from the Arena itself since Arena stores all entities ultimately causing Arena to be the subject. This approach would be valid because Arena would not have access to the Robot sensors themselves allowing for looser coupling. This implementation is also intuitive because the Robot should have the task of determining the impact of a sensor reading and applying it to its own behavior. Arena simply relays the stimuli to the Robot itself for processing.

 This simulation also implements Strategy Pattern. Using Strategy Pattern, a class behavior or its algorithm can be changed at runtime. We create context object whose behavior varies. This behavior is relayed by objects that represent various strategies. The strategy object can change the executing algorithm based on the context. This concept is similar to a class having access to a family of algorithms that it can choose based on context. Each algorithm is encapsulated allowing easy switching between algorithms. In our Original Design we create separate classes for each robot behavior. These classes extend MotionHandlerRobot class by uniquely defining a a virtual function in each class. This Parent-Child Relationship allows us to use one instance of MotionHandlerRobot to call a specific behavior. This approach would be valid because we have multiple behaviors the Robot would need to exhibit based on stimuli. Our approach extends one instance of MotionHandlerRobot thus adding a new behavior would require less code change. This concept exhibits polymorphism since the decision on behavior is made at runtime and separate class instance for each behavior are not required.

 ## User Guide ##
 This simulation is comprised of the following objects.
 - Robot: Robots are movable objects with two light and two food sensors attached to them 40 degrees apart on either side from the robots head.
 - Light Sensor: A Light sensor will detect the amount of light it sees. The closer the sensor is to the light the greater the value it will output.
 - Food Sensor: Exactly like light sensor except it detects food.
 - Robot Behaviors:
  - Fear: Robots can exhibit fear behavior based on how the user sets the number of robots with this trait( this setting is discussed below). Fear is a direct positive sensor-drive connection. This means the closer the robot is to light the faster it will move away. Direct means if there is a light near the left sensor, the corresponding side of the robot will move away.
  - Explore: Robots can exhibit explore behavior depending on how the user set the ratio of fear to explore robots( This is detailed below). Explore robots have a cross-connected negative sensor-drive connection. This means the robots will try to follow around the light or seem attracted to them. The cross connection means the light sensor reading will change the speed of the right wheel.
- Hungry Robots: The robot ca also become hungry. This is implemented using a timer. After an approximate 30 seconds of the game running the robots will become hungry. The robots are searching for food and must tag a food entity to resolve their hunger. When the robot is near food it will become satisfied and revert its behavior.  While a robot is hungry it will show assertive behavior. Assertive behavior is a positive cross-connected sensor-motor connection. The robots will move away from the light. The cross connection means the light sensor reading will change the speed of the right wheel. If the robot does not tag a food within the 120 seconds it will flash and become very hungry. When a robot is very hungry it will completely ignore lights and search for food. After 150 seconds of no food the simulation will stop and ask the user to start a new game.



 When executing the code a graphics windows will launch on the desktop. Within this graphics windows there will be a menu windows with Simulation control and Arena configuration. Within the Simulation Control there are the following buttons.

 - Play button
 	- This button will cause the simulation to start. The button label will change to Pause.
 - New Game
 	- This button will reset the simulation and all entities. If any sliders within the Arena configuration were moved, the corrresponding changes will be reflected when the new game begins.
 - Food off button
 	- This button will remove all food from the arena. Robots will not be hungry when this mode is turned on. The label will switch to Food on and allow user to switch food back on on next new game.

 The Arena configuration is comprised of the following sliders. All changes will be reflected upon new game unless otherwise noted.


 - Number of Robots
 	- This slider will allow the user to change the number of robots in the simulation.
 - Number of Lights
 	- This slider will allow the user to change the number of lights in the simulation.
 - Light Sensitivity
 	- This slider controls the sensitivity of the light sensors for all robots. This change will be reflected in real time.
 - Food Quantity
 	- This lsider controls how many food objects are placed in the arena
 - Fear-Explore Ratio
 	- This slider control the proportion of fear to explore robot behaviors within the arena.

 The User can configure these parameter then click new game then play to reflect any new changes. The User can exit the application by clicking the exit button on the top right of the window itself. 
