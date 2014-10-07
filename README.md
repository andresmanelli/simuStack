simuStack
=========

Stack of commands to run, which can be updated dynamically.

Compiling
==========

~$ make

Running
=========

~$ ./simuStack &

Adding commands
===============

Edit the file _simuList_ and add lines with this pattern:

          #FIRST COMMAND
          SECOND COMMAND
          ETC
          
Recommended: create a temporary file with the command and then
          
          cat cmdTempFile >> path/to/simuList
          
Notes
===============

 - The command list is cleaned upon reset of the stack. DON'T erease lines of this file, just add them in the end.
 - You can see what's going on in the file _logSimuStack_

Bugs
===============

 - The command << #kill `ps -e | grep simuStack | awk '{print $1}'` >> will kill the stack.
