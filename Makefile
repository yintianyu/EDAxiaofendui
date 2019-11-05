CC      = g++ 
CPPFLAGS  = -Wall -O0 -g
INCLUDEFLAGS = 
LDFLAGS = -L lib -l ssdboutputter -l ssdbreader -g
OBJS    = main.o compressor.o A_Regulation.o Homo_Regulation.o state_machine.o u_Regulation.o decompressor.o period.o
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
