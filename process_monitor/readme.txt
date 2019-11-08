To compile the demo code and run it, please do as follow:
(1) source /home/compress_test/config.bshrc
(2) g++ ReaderDemo.cc -o reader-demo -L/home/compress_test/lib -lssdbreader -std=c++11
(3) ./reader-demo /home/compress_test/cases/test1.wv
