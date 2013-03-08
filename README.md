Bitmap-Index and Typology

This is a C++11 reimplementation of the http://typology.de/ idea using bitmap-indexes. The original approach is written in Java and uses multidimensional index trees. The aim of this reimplementation is a prove of concept that bitmaps are suitable for that kind of task. In my first experiments this program was approximately 10 times faster than the original. 
A test import of 96,588,320 german source-target-score triples results in 28,405 files with a total size of 6.7 GB. This includes bitmap-indexes up to the fifth character of the target-word and a source-word look-up table.
On my notebook (Intel quad core i5 2.67 GHz, 8 GB RAM, 256 GB SSD) one next word prediction takes 0.6 ms in average. After 2 million requests the memory mapped file “cache” is hot and the average time goes down to 0.1 ms. When all files were loaded in the main memory I observed response times of 0.05 ms.
