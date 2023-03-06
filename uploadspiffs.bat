python C:\Users\Petr\.espressif\frameworks\esp-idf-v4.4.2\components\spiffs\spiffsgen.py 602112 .\data\ data.img
parttool.py --port COM23 --partition-table-file .\partitions.csv write_partition --partition-name storage --input .\data.img
