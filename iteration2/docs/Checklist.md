### CSCI3081W: Iteration 2 Submission Checklist

> The iteration 2 code and documentation is due on TUESDAY, April 10 at 11:55pm

Please copy this file into your /docs folder. Place an 'x' in each of the `[ ]` boxes for items that are complete. Push this completed checklist to github. When it displays on Github, it will show an actual checkbox with a check mark.

### Compilation and Execution

- [x] I cloned **fresh** from github onto a cselabs machine to test compilation
- [x] Submission is on the master branch of the my project repo "project-<username>"
- [x] _/iteration2_ folder exists
- [x] _/build_ is not in the repo
- [x] _/html_ is not in _docs_
- [x] **__`make` from src completes without error.__**
- [x] `./../build/bin/arenaviewer` executes without assert or a seg fault
- [x] `cpplint.py *.cc` reports no errors
- [x] `cpplint.py --root=.. *.h` reports no errors
- [x] `doxygen docs/Doxyfile` compiles

### Primary Functionality

> To make grading easier, we ask that you provide the status of each of the requirements. Do not lie about the status!! It makes us grumpy -- you want your grader to be happy when s/he grades! We will be inspecting the code to confirm,
but this status report can make it faster for us.

**__Please place an 'X' in the appropriate box for each requirement.__**
- Good = Fully Implemented. No _known_ bugs. No _obvious_ bugs.
- Okay = Works but some bugs (recorded as a bug/issue on github).
- Help = Attempted but not working.
- Time = No to Little Progress

| Functionality | Good | Okay | Help | Time |
| -------- | -------- | -------- | -------- | --------- |
| **__Priority Level 2__** |
| _Lights_ are visually distinct and moving. |  x|  |  |  |
| _Lights_ have avoidance behavior when colliding with a wall. |  x|  |  |  |
| _Lights_ have avoidance behavior when colliding with a light. |  x|  |  |  |
| _Lights_ pass through non-light entities. |  x|  |  |  |
| _Robots_ are visually distinct and moving (when appropriate<sup> [1] </sup>). |  x|  |  |  |
| Robot sensors are displaying properly. |  x|  |  |  |
| _Robots_ have avoidance behavior when colliding with a wall. |  x|  |  |  |
| _Robots_ have avoidance behavior when colliding with a robot. |  x|  |  |  |
| _Robots_ pass through non-robot entities. |  x|  |  |  |
| Half the robots fear lights. |  x|  |  |  |
| Half the robots "explore" for lights. | x|  |  |  |
| User can toggle between pausing and playing. |  x|  |  |   |
| User can start a new game/simulation. |  x|  |  |   |

[1] Robot behavior emerges from the sensor-actuator connections. It might not always reflect reasonable behavior with respect to the intended reaction. For example, a fearful robot might not always run away from a light due to various forces acting upon it. We are not grading the robot behavior that emerges -- only the implementation of the sensor reading calculation, of the sensor-actuator connections, and of the calculated wheel velocity as a function of sensor readings.


| Functionality | Good | Okay | Help | Time |
| -------- | -------- | -------- | -------- | --------- |
| **__Priority Level 3__** |
| _Food_ is visually distinct and immobile. |  x|  |  |   |
| _Robots_ are aggressive towards food. |  x|  |  |   |
| _Robots_ get hungry after ~30 seconds after eating. |  x|  |  |   |
| _Robots_ consume food when ~5 units away. |  x|  |  |   |
| _Robots_ are no longer hungry after eating. |  x|  |  |  |
| **__Priority Level 4__** |
| After not eating for ~2 mins, _Robots_ ignore lights. |  x|  |  |   |
| After not eating for ~2.5m mins, _Robots_ starve death. Game over. |  x|  |  |   |
| Simulation starts with 10 robots, 4 lights, 4 food. |  x|  |  |  |

| Functionality  **__For Fun__**  | Good | Okay | Help | Time |
| -------- | -------- | -------- | -------- | --------- |
| Time to hunger depends on size of robot. |  |  |  |   x|
| 4 _Robots_ have robot sensors. |  |  |  |   x|
| Users can use GUI to configure simulation. |  |  |  |   x|

### Documentation
> Documentation should be in good _draft_ form.

- [x] The design document username\_design.pdf file for the iteration 2 exists in _docs_.
- [x] The design document has a section on the Observer Pattern.
- [x] The design document has a section on the Strategy Pattern.
- [x] The design document has a section on the feature enhancement.
- [x] Classes are documented in the doxygen web page class list.
- [x] _Classes_ list has documentation for each class, including members and methods.
- [x] I perused other classes to spot check documentation.
- [x] I commented my code where necessary.

### Linter, and Git
- [x] The code complies with naming conventions for classes, methods, and members.
- [x] There exists a history of git commit messages to show effort.
- [x] There exists a history of issues and branches to show effort.

### Reporting of Bugs
- [x] Viewing issues on my repo and selecting _bug_ label shows all the known bugs in my code.
  > If you report _minor_ bugs, you will lose few to no points for that functionality.
  > If you try to hide some obvious bugs and not report them, you will lose all the points for that functionality.

## Ready For Submission

When everything above is as you want it or it is 11:55pm on Tuesday, it is time to submit!

- [x] I pushed this file and any changes resulting from this checklist.
- [x] Everything is in the **__master__** branch.

_One more thing -- check that what you think you pushed was really pushed to github. If you can see it, we can see it. If you can't, we can't._

Congratulations on your submission!
