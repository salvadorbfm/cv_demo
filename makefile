
RELEASE = demo
LIBS    = -lm -lfftw3 -lopencv_core -lopencv_highgui
INCLUDE = -I"/usr/local/include/" -I"/usr/include/" -L"/usr/local/lib/" -L"/usr/lib/"

FLAGS   = $(INCLUDE)
OBJECTS = main.o app_manager.o data_sem.o fftw_manager.o image.o image_analyser.o \
          image_creater.o svector.o ui_handler.o util.o



$(RELEASE): $(OBJECTS)
	g++ $(FLAGS) -o $(RELEASE) $(OBJECTS) $(LIBS)

main.o : main.cpp
		g++ $(FLAGS) -c main.cpp

app_manager.o : app_ui/app_manager.cpp
		g++ $(FLAGS) -c app_ui/app_manager.cpp

data_sem.o : elements/data_sem.cpp
		g++ $(FLAGS) -c elements/data_sem.cpp

fftw_manager.o : elements/fftw_manager.cpp
		g++ $(FLAGS) -c elements/fftw_manager.cpp

image.o : elements/image.cpp
		g++ $(FLAGS) -c elements/image.cpp

image_analyser.o : elements/image_analyser.cpp
		g++ $(FLAGS) -c elements/image_analyser.cpp

image_creater.o : elements/image_creater.cpp
		g++ $(FLAGS) -c elements/image_creater.cpp

svector.o : elements/svector.cpp
		g++ $(FLAGS) -c elements/svector.cpp

ui_handler.o : app_ui/ui_handler.cpp
		g++ $(FLAGS) -c app_ui/ui_handler.cpp

util.o : elements/util.cpp
		 g++ $(FLAGS) -c elements/util.cpp