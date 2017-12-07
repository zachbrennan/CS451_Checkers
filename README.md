# CS451_Checkers

An assignment for Drexel's CS451 class, in order to prepare students
for Senior Design projects. We were given free reign to design and
document an implementation of checkers that could be played over a
network connection. This is the project that my group created, 
migrated to github from a private GitLab server. The instructions
did not stipulate how it should be done, or what languages to use,
so we did it in C, entirely within a terminal window. For fun.

![A local multiplayer demo of the game.](/demos/demoV1_0.gif?raw=true)

## Building the System

*If you just want to play the game, you can simply download the
latest built release from the repository.*

In order to build this program, you will need a working Docker 
installation. Build the Docker image by running 
```
make image
```
and then once the image is compiled (will take a few minutes):
```
make game
```
will compile and link the code, producing an executable at 
```
/CS451_Checkers/bin/game
```

### Prerequisites

To compile the project, you should only need Docker, as the code
is all built entirely in the image provided. 

### Running

To run the executable, you will need *ncurses* installed, which
can be done on Ubuntu/Debian based systems with the command
```
apt-get install ncurses-terminal
```

Once ncurses is installed, you need to make sure the TERMINFO 
environment variable is set correctly. To do this, use the command
```
locate terminfo
```
and look for an entry similar to
```
/usr/share/terminfo
```
Once you have found it, export it to the variable. So in my case, 
the command would be
```
export TERMINFO="/usr/share/terminfo"
```

Almost to playing the game!
Last step is to export the *xterm-color* terminal to the TERM 
environment variable.
```
export TERM=xterm-color
```

Now you should be able to run the game with a simple
```
./game
```

For now, you should be able to play a local multiplayer game. The
server address for the network games is hardcoded in, but that may 
change at a future date. This page will be updated if that happens. 

For troubleshooting help, please contact zach@brennan.tech

## Built With

* [Docker](http://www.https://www.docker.com/) - The system used to build the project
* [ncurses](https://www.gnu.org/software/ncurses/) - The graphics library used 

## Authors

* **Zach Brennan** - [zachbrennan](https://github.com/zachbrennan)
* **Z.V.** 
* **Corwin Belser** 
* **Kris Horsey** 
