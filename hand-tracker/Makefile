all: hand-tracker

hand-tracker: *.cc
	gcc -o hand-tracker *.cc -IGL -ILeapSDK/include -ILeapSDK/util -L. -lLeap -Iglh -lm -lglut `pkg-config --cflags --libs gl glu liblo gtkmm-2.4 gtkglextmm-1.2`
