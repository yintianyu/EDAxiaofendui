To run the precision check program, please do as follow:
(1) source /home/compress_test/config.bshrc
(2) ./waveform-precision-compare-tool gold.wv decompressed.wv score

Note:
Only the max absolute error value of all points is considered while marking
for a waveform, the average value is not used as a scoring stardard.
