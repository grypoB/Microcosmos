# Definitions de macros

EXEC   = prog.x

CC     = gcc
CFLAGS = -Wall
LIBS   = -lm
CFILES =  error.c generateur.c trou_noir.c particule.c sim.c geometry.c
OFILES= $(CFILES:.c=.o)


# Definitions des cibles
all : $(EXEC)

debug: CFLAGS += -g
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
error.o: error.c error.h
generateur.o: generateur.c constantes.h tolerance.h geometry.h error.h \
  generateur.h
trou_noir.o: trou_noir.c constantes.h tolerance.h geometry.h error.h \
  trou_noir.h
particule.o: particule.c constantes.h tolerance.h particule.h geometry.h \
  error.h
sim.o: sim.c error.h constantes.h tolerance.h generateur.h geometry.h \
  trou_noir.h particule.h
geometry.o: geometry.c geometry.h constantes.h tolerance.h
