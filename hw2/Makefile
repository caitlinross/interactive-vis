CFLAGS=`pkg-config --cflags libgvc glib-2.0` -Wall -g
LDFLAGS=`pkg-config --libs libgvc glib-2.0`

SOURCES=createdot.c
EXECUTABLE=createdot

createdot: $(SOURCES)

clean:
	rm -f $(EXECUTABLE)
