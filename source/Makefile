# Definitions de macros

EXEC   = rendu1.x

CC     = gcc
CFLAGS = -Wall
LIBS   = -lm
CFILES =  main.cpp error.c generateur.c trou_noir.c particule.c sim.c geometry.c

# convert .c and .cpp into .o (2 steps needed)
TEMP   = $(CFILES:.c=.o)
OFILES = $(TEMP:.cpp=.o)


# Definitions des cibles
all : $(EXEC)

debug: CFLAGS += -g -D DEBUG
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
	$(CC) $(OFILES) $(LIBS) $(CFLAGS) -o $(EXEC)

# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
main.o: main.cpp sim.h
error.o: error.c error.h
generateur.o: generateur.c constantes.h tolerance.h geometry.h error.h \
  generateur.h particule.h
trou_noir.o: trou_noir.c constantes.h tolerance.h geometry.h error.h \
  trou_noir.h
particule.o: particule.c constantes.h tolerance.h particule.h geometry.h \
  error.h
sim.o: sim.c error.h constantes.h tolerance.h sim.h generateur.h \
  geometry.h trou_noir.h particule.h
geometry.o: geometry.c geometry.h constantes.h tolerance.h
