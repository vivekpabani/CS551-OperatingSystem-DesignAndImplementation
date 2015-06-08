login as root. current directory /root.

unzip project2.zip
cd /root/project2/

cp callnr.h /usr/src/include/minix/
cp table.c /usr/src/servers/pm/
cp proto.h /usr/src/servers/pm/
cp misc.c /usr/src/servers/pm/
cp main.c /usr/src/servers/pm/
cp topicdef.c /usr/src/servers/pm/
cp topicdef.h /usr/src/servers/pm/

Open makefile on the path /usr/src/servers/pm/Makefile
Insert “topicdef.c” at the end of 5th line declaring SRCS.

cd /usr/src/servers/pm/

to compile the definitions, type: make

After compile :

cd /root/project2/

cp project2.h /usr/include/

cd /usr/src/releasetools

Type: make install
After, type: sync
Type: reboot


*NOTE : You can also execute prep.bash to run the above steps (Except manually inserting the SRC in pm Makefile)


After reboot is done :

==============================
Program to execute testcases
==============================

cd /root/project2/
cp test.c /root
cd /root

cc test.c -o test
./test


==============================
Interactive program
==============================

cd /root/project2/
cp project.c /root
cd /root

cc project.c -o interactive
./interactive
