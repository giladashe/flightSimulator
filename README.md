# **Flight Simulator**

This program gives set of commands to a provided flight simulator called flight gear that makes the plane fly.
The simulator connects as a client to a server and sends the current values of the flight.
Meanwhile the program connects as a client to the simulator and sends commands to the plane/program.  

**Getting Started**

First you'll need to install [flight gear](https://www.flightgear.org/)

Then, in the additional settings you'll need to write these lines:

--telnet=socket,in,10,127.0.0.1,5402,tcp

--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small
   
In addition you'll need to have a text file of commands and a xml file to put in the protocol folder.

After you have put the xml file in protocol, you can run the program on the command line with an argument which is the fly.txt.
Then you'll need to press fly on the simulator and after 2 minutes the flight will begin. 

**Built With**

Clion - IDE by JetBrains

**Versioning**

We used [Github](https://github.com/giladashe/flightSimulator) for version control.

**Authors**

Israel Shai and Gilad Asher.
