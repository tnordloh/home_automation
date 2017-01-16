This is my home automation project.  It uses Arduino modules as its core.  

The end result of this project should look something like a master unit that assigns and tracks tasks around the house.
There will be helper units scattered around the house, which will talk back to the master unit.  Their purpose is to 
help track completion of tasks.  

For example, the first unit's purpose, which is named "snakemail" will contain a temperature sensor, a water sensor, and
a button.  If the water bowl is empty, or if the temperature wanders out of the acceptable range, it will send a message to
the master unit.  Whenever the snakes are fed, the person who fed them will push a button.

The master unit will be responsible for collecting the alerts from the snakemail unit, and emailing someone to let them know.
If no one feeds the snakes for over seven days, then it will send a reminder to make sure that the owner knows about it.
By pushing the button, the owner tells the master unit to reset the 'food' timer.  Similarly, the snakemail unit will
send alerts when water is gone, or if the snakes are getting too hot or cold.


