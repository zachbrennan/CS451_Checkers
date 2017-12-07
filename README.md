General ideas:

  - Do this in C because it will be an excellent learning
    experience and we will have real, practical code that we can
    use in the future and/or post on our public Git repositories
    with all the proper processes and documentation.
    
  - For the networking aspect we can do a client-server model or
    perhaps a P2P model. We should aim to support threading so
    that additional clients can join in the form of "Spectators"
    who can watch an existing in-progress game.
    
  - For the graphical user interface, we can use the 'ncurses'
    library to provide a decent-quality command-line interface
    with relatively minimal effort. Some examples are found at:
    https://www.google.com/search?q=ncurses+gui&source=lnms&tbm=isch

  - ncurses documentation:
    http://invisible-island.net/ncurses/ncurses-intro.html
    and
    http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
    
  - These dependencies can be built using some of the scripts
    that are included in the 'utils/' directory. The 'ncurses'
    example has been completed.

  - For unit testing, maybe we can use something like this:
    https://libcheck.github.io/check/doc/check_html/check_3.html

  - After thinking about it some more, it might be a good idea
    to use CMake as our build system instead of just a Makefile
    because it is much more modern and flexible. Thoughts? Our
    game/project won't be *that* complicated to require it, but
    in the interest of "hey, let's learn something useful"...

  - Looks like we've found a bug in Valgrind:
    https://bugs.kde.org/show_bug.cgi?id=382083

  - Adding some notes from trying to run this again, 3 months later.
    Install ncurses and xterm.  "sudo apt-get install ncurses-term"
    would be a good command to remember as well. Then make sure the 
    TERMINFO environment variable is set to the terminfo database 
    location (in my case it was /usr/share/terminfo). Then when 
    running it, run as "TERM=xterm-color ./game". Can't offer any 
    tips on compiling it, the docker image wouldn't build on my machine,
    but it worked on my other desktop. I'll update if I figure that one
    out.
