# gcc zeroServos.c -o zeroServos -L ../../Robotics_Cape_Installerlibraries -lm -lrt -lpthread -lroboticscape
# gcc test1.c -o servotest1 -L ../../Robotics_Cape_Installerlibraries -lm -lrt -lpthread -lroboticscape
# gcc test2.c -o servotest2 -L ../../Robotics_Cape_Installerlibraries -lm -lrt -lpthread -lroboticscape
# gcc threadtest.c -o threadtest -L ../../Robotics_Cape_Installerlibraries -lm -lrt -lpthread -lroboticscape

gcc servoDaemon.c -o servoDaemon -L ../../Robotics_Cape_Installerlibraries -lm -lrt -lpthread -lroboticscape
