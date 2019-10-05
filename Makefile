CC      = g++ 
CPPFLAGS  = -Wall -O
INCLUDEFLAGS = 
LDFLAGS = -L ./lib -l libssdboutputter -l libssdbreader
OBJS    = main.o compressor.o
TARGETS = edaxiaofendui 

.PHONY:all 
all : $(TARGETS)

edaxiaofendui:$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(CPPFLAGS) $(INCLUDEFLAGS)

%.d:%.c
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY:clean 
clean:
	rm -f $(TARGETS) *.o *.d *.d.*