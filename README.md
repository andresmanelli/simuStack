simuStack
=========

Stack of commands to run, which can be updated dynamically.

Running
=========

prompt$ ./simuStack &

Adding commands
===============

Edit the file _simuList_ and add lines with this pattern:

          #FIRST COMMAND
          SECOND COMMAND
          ETC
          
Notes
===============

 - The command list is cleaned upon reset of the stack. DON'T erease lines of this file, just add them in the end.
 - You can see what's going on in the file _logSimuStack_
