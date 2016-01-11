CXX = g++
CXXFLAGS = -Wall -O -g ` pkg-config gtkmm-2.4 --cflags --libs` -std=c++0x
OBJ_DIR = src
_OBJECTS = Subject.o Model.o Controller.o View.o CardGui.o Main.o Gameboards.o Deck.o Human.o Computer.o Player.o Card.o
OBJECTS = ${patsubst %,${OBJ_DIR}/%,${_OBJECTS}}
EXEC = straights

${EXEC} : ${OBJECTS} ; ${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}

clean : ; rm -rf ${OBJECTS} ${EXEC}

Subject.o: Subject.h Observer.h Subject.cpp
Model.o: Subject.h Card.h Deck.h Gameboards.h Computer.h
Controller.o: Controller.h Model.h Controller.cpp
View.o: View.h Observer.h Controller.h Model.h CardGui.h View.cpp
CardGui.o: CardGui.h Card.h CardGui.cpp
Main.o: View.h Controller.h Main.cpp
Gameboards.o: Gameboards.h Card.h Gameboards.cpp
Deck.o: Deck.h Card.h Deck.cpp
Human.o: Human.h Player.h Human.cpp
Computer.o: Computer.h Player.h Computer.cpp
Player.o: Player.h Card.h Player.cpp
Card.o: Card.h Card.cpp

