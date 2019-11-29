CC      = g++ 
CPPFLAGS  = -Wall -O3 -g
INCLUDEFLAGS = 
LDFLAGS = -L lib -l ssdboutputter -l ssdbreader -lpthread -g -O3
OBJS    = compressor.o A_Regulation.o Homo_Regulation.o state_machine.o u_Regulation.o decompressor.o period.o
COMPOBJS = compress_main.o
DECOMPOBJS = decompress_main.o
TARGETS = compress decompress 

.PHONY:all 
all : $(TARGETS)

compress:$(OBJS) $(COMPOBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

decompress:$(OBJS) $(DECOMPOBJS)
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
