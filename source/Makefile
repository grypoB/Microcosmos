# Definitions de macros

EXEC   = rendu1.x

CC       = gcc
CXX      = g++
CFLAGS   = -Wall -Wno-deprecated-declarations -Wno-overloaded-virtual
CXXFLAGS = $(CFLAGS)
LIBS     = -lm -lstdc++ -lglut -lGL -lGLU -lglui -L/usr/X11R6/lib -lX11 -lXext
CFILES   =  main.cpp error.c generateur.c trou_noir.c particule.c sim.c \
  geometry.c graphic.c linked_list.c

# debug flags to append in target debug
DEBUGFLAGS = -g -D DEBUG

# convert .c and .cpp into .o (2 steps needed)
TEMP   = $(CFILES:.c=.o)
OFILES = $(TEMP:.cpp=.o)


# Definitions des cibles
all : $(EXEC)

apple : CPPFLAGS += -I./ -I./include
apple : LIBS   = -framework OpenGl -lm -framework GLUT -L/usr/X11R6/lib -lstdc++ -Llib -L./lib -lglui
apple : $(EXEC)

appleDebug : CFLAGS   += $(DEBUGFLAGS)
appleDebug : apple
appleDepend : CFLAGS += -I./ -I./include
appleDepend : depend

debug: CFLAGS   += $(DEBUGFLAGS)
#debug: CXXFLAGS += $(DEBUGFLAGS) # seems that has already happended
debug: $(EXEC)

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CC) -MM $(CFLAGS) $(CFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x


# Dependance particuliere

$(EXEC): $(OFILES)
	$(CC) $(OFILES) $(LIBS) -o $(EXEC)

# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
main.o: main.cpp include/GL/glui.h sim.h geometry.h constantes.h \
  tolerance.h
error.o: error.c error.h
generateur.o: generateur.c constantes.h tolerance.h graphic.h geometry.h \
  error.h generateur.h linked_list.h particule.h
trou_noir.o: trou_noir.c constantes.h tolerance.h graphic.h geometry.h \
  error.h trou_noir.h linked_list.h
particule.o: particule.c constantes.h tolerance.h graphic.h geometry.h \
  particule.h error.h linked_list.h
sim.o: sim.c error.h constantes.h tolerance.h sim.h geometry.h \
  generateur.h linked_list.h trou_noir.h particule.h
geometry.o: geometry.c geometry.h constantes.h tolerance.h
graphic.o: graphic.c graphic.h geometry.h constantes.h tolerance.h
linked_list.o: linked_list.c linked_list.h
